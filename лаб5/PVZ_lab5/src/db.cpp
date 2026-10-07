#include "db.h"
#include <sstream>
#include <iomanip>
#include <stdexcept>
#include <cctype>

namespace {

bool looksLikePhone(const std::string& s) {
    if (s.empty()) return false;
    int digits = 0;
    for (char c : s) {
        unsigned char uc = static_cast<unsigned char>(c);
        if (std::isdigit(uc)) { digits++; continue; }
        if (c == '+' || c == '-' || c == ' ' || c == '(' || c == ')') continue;
        return false;
    }
    return digits >= 10;
}

Order rowToOrder(const pqxx::row_ref& row)
{
    Order ord;
    ord.id = row["id"].as<int>();

    if(row["parcel_article"].is_null())
    {
        ord.article = "";
    }
    else
    {
        ord.article = row["parcel_article"].as<string>();
    }

    if(row["cell"].is_null())
    {
        ord.cell = "";
    }
    else
    {
        ord.cell = row["cell"].as<string>();
    }

    if(row["phone"].is_null())
    {
        ord.phone = "";
    }
    else
    {
        ord.phone = row["phone"].as<string>();
    }

    ord.code = row["order_code"].as<string>();
    ord.created_at = row["created_at"].as<string>();
    ord.status = row["status"].as<string>();
    return ord;
}


} 

Database::Database(const std::string& conn_str)
    : conn_(conn_str)
{
    if (!conn_.is_open())
        throw std::runtime_error("Не удалось подключиться к PostgreSQL");
}

std::vector<Order> Database::listOrders(const std::string& filter) {
    std::vector<Order> orders;
    pqxx::work w(conn_);
    pqxx::result res;
    string q;
    if (filter.empty()) {
        q = "SELECT*FROM orders ORDER BY created_at DESC";
        res = w.exec(q);
    }
    else {
        q = "SELECT*FROM orders WHERE (POSITION($1 IN order_code) != 0 OR POSITION($1 IN phone) != 0 OR POSITION($1 IN parcel_article) != 0) ORDER BY created_at DESC";
        res = w.exec_params(q, filter);
    }
    for(int i = 0; i < res.size(); i++)
    {
        orders.push_back(rowToOrder(res[i]));
    }
    return orders;
}

std::string Database::addOrder(const std::string& contact, const std::string& cell) {
    pqxx::work w(conn_);
    string q = "INSERT INTO orders (order_code, phone, parcel_article, cell) VALUES ($1, $2, $3, $4)";
    string code = "";
    if (looksLikePhone(contact)) {
        w.exec_params(q, code, contact, "", cell);
    }
    else {
        w.exec_params(q, code, "", contact, cell);
    }
    q = "SELECT MAX(id) FROM orders";
    pqxx::result res = w.exec(q);
    string pad = "";
    int num = res[0]["max"].as<int>();
    int temp = num, dig = 0;
    while (temp % 10 != 0) {
        dig++;
        temp /= 10;
    }
    for (int i = 0; i < (6 - dig); i++) {
        pad += "0";
    }
    code = "PVZ-" + pad;
    code += to_string(num);
    q = "UPDATE orders SET order_code = $1 WHERE id = $2";
    w.exec_params(q,code, num);
    w.commit();
    return code;
}

bool Database::issueOrder(const std::string& code) {
    pqxx::work w(conn_);
    string q = "SELECT*FROM orders WHERE order_code = $1";
    pqxx::result search = w.exec_params(q, code);
    if (search.empty() || search[0]["status"].as<string>() != "ready") {
        return false;
    }
    q = "UPDATE orders SET status = $1, issued_at = NOW() WHERE order_code = $2";
    w.exec_params(q, "issued", code);
    w.commit();
    return true;
}

bool Database::cancelOrder(const std::string& code) {
    pqxx::work w(conn_);
    string q = "SELECT*FROM orders WHERE order_code = $1";
    pqxx::result search = w.exec_params(q, code);
    if (search.empty() || search[0]["status"].as<string>() != "ready") {
        return false;
    }
    q = "UPDATE orders SET status = $1 WHERE order_code = $2";
    w.exec_params(q, "cancelled", code);
    w.commit();
    return true;
}

Report Database::buildReport() {
    int ready, issued, cancelled;
    pqxx::work w(conn_);
    string q = "SELECT COUNT(*) FROM orders WHERE status = $1";
    pqxx::result res; 
    res = w.exec_params(q, "ready");
    ready = res[0][0].as<int>();
    res = w.exec_params(q, "issued");
    issued = res[0][0].as<int>();
    res = w.exec_params(q, "cancelled");
    cancelled = res[0][0].as<int>();
    return Report(ready, issued, cancelled);
}

std::vector<Order> Database::listByStatus(const std::string& status) {
    std::vector<Order> orders;
    pqxx::work w(conn_);
    pqxx::result res;
    string q;
    if (status.empty()) {
        q = "SELECT*FROM orders ORDER BY issued_at DESC";
        res = w.exec(q);
    }
    else {
        q = "SELECT*FROM orders WHERE status = $1 ORDER BY issued_at DESC";
            res = w.exec_params(q, status);
    }
    for (int i = 0; i < res.size(); i++)
    {
        orders.push_back(rowToOrder(res[i]));
    }
    return orders;
}

int Database::countClosedOrders() {
    pqxx::work w(conn_);
    string q = "SELECT COUNT(*) FROM orders WHERE status = $1 OR status = $2";
    pqxx::result res = w.exec_params(q, "cancelled", "issued");
    return res[0][0].as<int>();
}

int Database::deleteClosedOrders() {
    int count = countClosedOrders();
    pqxx::work w(conn_);
    string q = "DELETE FROM orders WHERE status = $1 OR status = $2";
    w.exec_params(q, "cancelled", "issued");
    w.commit();
    return count;
}
