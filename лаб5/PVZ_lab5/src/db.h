#pragma once
#include <pqxx/pqxx>
#include <string>
#include <vector>

struct Order {
    int         id;
    std::string code;
    std::string phone;
    std::string article;
    std::string cell;
    std::string status;
    std::string created_at;
};

struct Report {
    int ready_count;
    int issued_count;
    int cancelled_count;
};

class Database {
public:
    explicit Database(const std::string& conn_str);

    std::vector<Order> listOrders(const std::string& filter = "");
    std::string addOrder(const std::string& contact,
                         const std::string& cell);
    bool issueOrder(const std::string& code);
    bool cancelOrder(const std::string& code);
    Report buildReport();

    std::vector<Order> listByStatus(const std::string& status);
    int  countClosedOrders();
    int  deleteClosedOrders();

private:
    pqxx::connection conn_;
};