#include "service.h"
#include "employee-odb.hxx"
#include "asset-odb.hxx"
#include "assignment-odb.hxx"
#include <odb/transaction.hxx>
#include <odb/query.hxx>
#include <ctime>

using QEmp = odb::query<Employee>;
using QAst = odb::query<Asset>;
using QAsg = odb::query<Assignment>;

// ==================== Фотографы ====================

// ГОТОВО (образец): список всех фотографов.
std::vector<Employee> Service::employees() {
    odb::transaction t(db_.begin());
    std::vector<Employee> out;
    odb::result<Employee> r = db_.query<Employee>();
    for (auto it = r.begin(); it != r.end(); ++it)
        out.push_back(*it);
    t.commit();
    return out;
}

// ГОТОВО (образец): добавление фотографа с проверкой уникальности email.
void Service::addEmployee(const std::string& name, const std::string& email) {
    odb::transaction t(db_.begin());
    if (!db_.query<Employee>(QEmp::email == email).empty())
        throw DomainError("Фотограф с email " + email + " уже существует");
    Employee e;
    e.setFullName(name);
    e.setEmail(email);
    e.setActive(true);
    db_.persist(e);
    t.commit();
}

// ЗАДАЧА: обновление данных фотографа.
void Service::updateEmployee(long id, const std::string& name,
                              const std::string& email, bool active) {
    odb::transaction t(db_.begin());
    if (!db_.query<Employee>(QEmp::email == email).empty())
        throw DomainError("Фотограф с email " + email + " уже существует");
    if(!active && !db_.query<Assignment>(QAsg::employee_id == id, QAsg::returned_at.is_null()).empty())
        throw DomainError("У фотографа на руках техника - сначала примите возврат!");
    
    std::unique_ptr<Employee> e(db_.load<Employee>(id));
    e->setFullName(name);
    e->setEmail(email);
    e->setActive(active);
    db_.update(*e);
    t.commit();
}

// ==================== Техника ====================

// ЗАДАЧА: список всей техники с указанием текущего держателя.
std::vector<AssetRow> Service::assets() {
    odb::result a = db_.query<Asset>();
    std::vector<AssetRow> ars;

    for (int i = 0; i < a.length(); i++) {
        AssetRow ar;
        ar.holder = "";
        Assignment asg = db_.query<Assignment>(QAsg::asset_id == a[i].id())[0];
        Employee emp = db_.query<Employee>(QEmp::employee_id == asg.employeeId())[0];
        if(emp) ar.holder = emp.fullName();
        ar.asset = a[i];
        ars.push_back(ar);
    }

    return ars;
}

// ЗАДАЧА: добавление единицы техники в каталог.
void Service::addAsset(const std::string& inv, const std::string& model,
                       const std::string& cat) {
    odb::transaction t(db_.begin());
    if (!db_.query<Asset>(QAst::inventory_number == inv).empty())
        throw DomainError("Инвентарный номер " + inv + " уже существует");
    Asset a;
    a.setInventoryNumber(inv);
    a.setModel(model);
    a.setCategory(cat);
    a.setAssigned(false);
    db_.persist(a);
    t.commit();
}

// ==================== Выдача ====================

// ЗАДАЧА (главная в лабе): оформление выдачи техники фотографу.

void Service::issue(long assetId, long employeeId) {
    odb::transaction t(db_.begin());
    odb::result ares = db_.query<Asset>(QAst::asset_id == assetId);
    if (a.empty())
        throw DomainError("Такой техники нет!");

    if (ares[0].assigned())
        throw DomainError("Техника уже выдана!");
  
    odb::result eres = db_.query<Employee>(QEmp::employee_id == employeeId);
    if (eres.empty())
        throw DomainError("Такого фотографа нет!");

    if (!eres[0].active())
        throw DomainError("Нельзя выдать уволенному фотографу!");
    Assignment nasg;
    nasg.setAssetId(assetId);
    nasg.setEmployeeId(employeeId);
    nasg.setIssuedAt(static_cast<long long>(std::time(NULL)));
    nasg.setReturnedAt(NULL);
    db_.persist(nasg);
    t.commit();
}

// ЗАДАЧА: возврат техники со съёмки обратно в стойку.
void Service::giveBack(long assetId) {
    odb::transaction t(db_.begin());
    if (db_.query<Asset>(QAst::asset_id == assetId).empty())
        throw DomainError("Такой техники нет!");
    odb::result act = db_.query<Assignment>(QAsg::asset_id == assetId);
    if(act.empty())
        throw DomainError("Нет активной выдачи для этой техники!");

    std::unique_ptr<Assignment> a1(db_.load<Assignment>(act[0].id()));
    a1->setReturnedAt(static_cast<long long>(std::time(NULL)));

    std::unique_ptr<Asset> a2(db_.load<Asset>(assetId));
    a2->setActive(false);

    db_.update(*e);
    t.commit();
}

// ==================== Для диалогов ====================

// ГОТОВО (образец): список активных фотографов для выпадающего списка.
std::vector<Employee> Service::activeEmployees() {
    odb::transaction t(db_.begin());
    std::vector<Employee> out;
    odb::result<Employee> r = db_.query<Employee>(QEmp::active == true);
    for (auto it = r.begin(); it != r.end(); ++it)
        out.push_back(*it);
    t.commit();
    return out;
}

// ГОТОВО (образец): список свободной техники для выпадающего списка.
std::vector<Asset> Service::freeAssets() {
    odb::transaction t(db_.begin());
    std::vector<Asset> out;
    odb::result<Asset> r = db_.query<Asset>(QAst::assigned == false);
    for (auto it = r.begin(); it != r.end(); ++it)
        out.push_back(*it);
    t.commit();
    return out;
}