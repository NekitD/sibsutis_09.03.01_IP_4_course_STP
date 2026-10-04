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
    // TODO
    return {};
}

std::string Database::addOrder(const std::string& contact,
                               const std::string& cell) {
    // TODO
    return "";
}

bool Database::issueOrder(const std::string& code) {
    pqxx::work w(conn_);
    string q = "SELECT orders WHERE order_code = $1";
    pqxx::result search = w.exec_params(q, code);
    if (search.empty() || search[0]["status"] != "ready") {
        return false;
    }
    q = "UPDATE orders SET status = $1 WHERE order_code = $2"
    pqxx::result res = w.exec_params(q, "issued", code);
    w.commit();
    if(res.empty()) return false;
    return true;
}

bool Database::cancelOrder(const std::string& code) {
    pqxx::work w(conn_);
    string q = "SELECT orders WHERE order_code = $1";
    pqxx::result search = w.exec_params(q, code);
    if (search.empty() || search[0]["status"] != "ready") {
        return false;
    }
    q = "UPDATE orders SET status = $1 WHERE order_code = $2"
    pqxx::result res = w.exec_params(q, "canceled", code);
    w.commit();
    if (res.empty()) return false;
    return true;
}

Report Database::buildReport() {
    int ready, issued, cancelled;
    pqxx::work w(conn_);
    string q = "SELECT orders WHERE status = $1";
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
    // TODO
    return {};
}

int Database::countClosedOrders() {
    // TODO
    return 0;
}

int Database::deleteClosedOrders() {
    // TODO
    return 0;
}