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
    string q;
    if (filter.empty()) {
        q = "SELECT*FROM orders ORDER BY issued_at DESC"
        res = w.exec(q)
    }
    else {
        q = "SELECT*FROM orders WHERE $1 ORDER BY issued_at DESC"
        res = w.exec_params(q, filter);
    }
    for (int i = 0; i < res.size(); i++) {
        Order ord;
        ord.id = res[i]["id"];
        ord.article = res[i]["parcel_article"];
        ord.cell = res[i]["cell"];
        ord.code = res[i]["order_code"];
        ord.created_at = res[i]["created_at"];
        ord.phone = res[i]["phone"];
        ord.status = res[i]["status"];
        orders.push_back(ord);
    }
    return orders;
}

std::string Database::addOrder(const std::string& contact,
                               const std::string& cell) {
    pqxx::work w(conn_);
    string q = "INSERT INTO orders (order_code, phone, parcel_article, cell) VALUES ($1, $2, $3, $4)";
    string code = "";
    if (looksLikePhone(contact)) {
        w.exec_params(code, contact, "", cell);
    }
    else {
        w.exec_params(code, "", contact, cell);
    }
    q = "SELECT id FROM orders WHERE phone = $1 OR parcel_article = $1";
    pqxx::result res = w.exec_params(q, contact);
    string pad = "";
    int num = res[0]["id"];
    int temp = num;
    while (temp % 10 != 0) {
        pad += "0";
        temp /= 10;
    }
    code = "PVZ-" + pad + num;
    return code;
}

bool Database::issueOrder(const std::string& code) {
    pqxx::work w(conn_);
    string q = "SELECT*FROM orders WHERE order_code = $1";
    pqxx::result search = w.exec_params(q, code);
    if (search.empty() || search[0]["status"] != "ready") {
        return false;
    }
    q = "UPDATE orders SET status = $1, issued_at = NOW() WHERE order_code = $2"
    w.exec_params(q, "issued", code);
    w.commit();
    return true;
}

bool Database::cancelOrder(const std::string& code) {
    pqxx::work w(conn_);
    string q = "SELECT*FROM orders WHERE order_code = $1";
    pqxx::result search = w.exec_params(q, code);
    if (search.empty() || search[0]["status"] != "ready") {
        return false;
    }
    q = "UPDATE orders SET status = $1 WHERE order_code = $2"
    w.exec_params(q, "cancelled", code);
    w.commit();
    return true;
}

Report Database::buildReport() {
    int ready, issued, cancelled;
    pqxx::work w(conn_);
    string q = "SELECT*FROM orders WHERE status = $1";
    pqxx::result res; 
    res = w.exec_params(q, "ready");
    ready = res.size();
    res = w.exec_params(q, "issued");
    issued = res.size();
    res = w.exec_params(q, "cancelled");
    cancelled = res.size();
    return Report(ready, issued, cancelled);
}

std::vector<Order> Database::listByStatus(const std::string& status) {
    std::vector<Order> orders;
    pqxx::work w(conn_);
    string q;
    if (status.empty()) {
        q = "SELECT*FROM orders ORDER BY issued_at DESC"
            res = w.exec(q)
    }
    else {
        q = "SELECT*FROM orders WHERE status = $1 ORDER BY issued_at DESC"
            res = w.exec_params(q, status);
    }
    for (int i = 0; i < res.size(); i++) {
        Order ord;
        ord.id = res[i]["id"];
        ord.article = res[i]["parcel_article"];
        ord.cell = res[i]["cell"];
        ord.code = res[i]["order_code"];
        ord.created_at = res[i]["created_at"];
        ord.phone = res[i]["phone"];
        ord.status = res[i]["status"];
        orders.push_back(ord);
    }
    return orders;
}

int Database::countClosedOrders() {
    pqxx::work w(conn_);
    string q = "SELECT*FROM orders WHERE status = $1 OR status = $2";
    pqxx::result res = w.exec_params(q, "cancelled", "issued");
    return res.size();
}

int Database::deleteClosedOrders() {
    int count = countClosedOrders();
    pqxx::work w(conn_);
    string q = "DELETE FROM orders WHERE status = $1 OR status = $2";
    w.exec_params(q, "cancelled", "issued");
    w.commit();
    return count;
}