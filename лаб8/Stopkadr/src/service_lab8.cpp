#include "service.h"
#include "employee-odb.hxx"
#include "asset-odb.hxx"
#include "assignment-odb.hxx"
#include <odb/transaction.hxx>
#include <odb/query.hxx>
#include <algorithm>
#include <ctime>

using QAst = odb::query<Asset>;
using QEmp = odb::query<Employee>;
using QAsg = odb::query<Assignment>;

// Лабораторная 8: 8 методов через ODB.
// Готово: searchAssets, listAssetsPaged, assetHistory, employeeAssets.
// Реализуйте: duplicateAsset, issueMultiple, reassignAsset, deleteAssetCascade.

// ------------------------------------------------------------
// Копирование техники. newInv должен быть уникален.
// Ошибка: "Инвентарный номер <newInv> уже существует".
// ------------------------------------------------------------
void Service::duplicateAsset(long sourceId, const std::string& newInv) {
    odb::transaction t(db_.begin());


    if (!db_.query<Asset>(QAst::inventoryNumber == newInv).empty())
        throw DomainError("Инвентарный номер " + newInv + " уже существует");

    Asset a;
    odb::result<Asset> r = db_.query<Asset>(QAst::id == sourceId);
    if (r.begin() != r.end()){
        a = *r.begin();
    }
    Asset na;
    na.setInventoryNumber(newInv);
    na.setModel(a.model());
    na.setCategory(a.category());
    na.setAssigned(false);
    db_.persist(na);
    t.commit();
}

// ------------------------------------------------------------
// Выдать N единиц одному фотографу одной транзакцией.
// Ошибки: "Не выбрано ни одной единицы техники",
//         "Нельзя выдать уволенному фотографу",
//         "Техника <inv> уже выдана".
// При ошибке — ни одна не выдана.
// ------------------------------------------------------------
void Service::issueMultiple(const std::vector<long>& assetIds, long employeeId) {
    std::vector<long>::const_iterator asset;
    for(asset = assetIds.begin(); asset != assetIds.end(); asset++){
        issue(*asset, employeeId);
    }
}

std::vector<Asset> Service::searchAssets(const std::string& category,
                                          bool onlyFree,
                                          const std::string& text) {
    odb::transaction t(db_.begin());

    odb::query<Asset> q = odb::query<Asset>::true_expr;

    if (!category.empty())
        q = q && (QAst::category == category);

    if (onlyFree)
        q = q && (QAst::assigned == false);

    if (!text.empty()) {
        std::string pattern = "%" + text + "%";
        q = q && (QAst::model.like(pattern) ||
                  QAst::inventoryNumber.like(pattern));
    }

    std::vector<Asset> out;
    odb::result<Asset> r = db_.query<Asset>(q);
    for (auto it = r.begin(); it != r.end(); ++it)
        out.push_back(*it);

    std::sort(out.begin(), out.end(), [](const Asset& a, const Asset& b) {
        return a.id() > b.id();
    });

    t.commit();
    return out;
}

PagedAssets Service::listAssetsPaged(int page, int pageSize,
                                       const std::string& statusFilter) {
    odb::transaction t(db_.begin());
    PagedAssets result;

    odb::query<Asset> q = odb::query<Asset>::true_expr;
    if (statusFilter == "В стойке")
        q = q && (QAst::assigned == false);
    else if (statusFilter == "На съёмке")
        q = q && (QAst::assigned == true);

    std::vector<Asset> all;
    odb::result<Asset> r = db_.query<Asset>(q);
    for (auto it = r.begin(); it != r.end(); ++it)
        all.push_back(*it);

    std::sort(all.begin(), all.end(), [](const Asset& a, const Asset& b) {
        return a.id() > b.id();
    });

    int offset = page * pageSize;
    int end = std::min(offset + pageSize, (int)all.size());

    for (int i = offset; i < end; ++i) {
        AssetRow row;
        row.asset = all[i];

        if (row.asset.isAssigned()) {
            auto aq = db_.query<Assignment>(
                QAsg::assetId == row.asset.id() &&
                QAsg::returnedAt.is_null());

            if (!aq.empty()) {
                Assignment asg = *aq.begin();
                row.holder = db_.load<Employee>(asg.employeeId())->fullName();
            }
        }

        result.items.push_back(row);
    }

    result.hasNext = (offset + pageSize < (int)all.size());

    t.commit();
    return result;
}

std::vector<HistoryEntry> Service::assetHistory(long assetId) {
    odb::transaction t(db_.begin());
    std::vector<HistoryEntry> out;

    odb::result<Assignment> r = db_.query<Assignment>(QAsg::assetId == assetId);
    for (auto it = r.begin(); it != r.end(); ++it) {
        HistoryEntry e;
        e.assignment = *it;
        e.employeeName = db_.load<Employee>(e.assignment.employeeId())->fullName();
        out.push_back(e);
    }

    std::sort(out.begin(), out.end(),
              [](const HistoryEntry& a, const HistoryEntry& b) {
                  return a.assignment.issuedAt() > b.assignment.issuedAt();
              });

    t.commit();
    return out;
}

std::vector<Asset> Service::employeeAssets(long employeeId) {
    odb::transaction t(db_.begin());
    std::vector<Asset> out;

    odb::result<Assignment> r = db_.query<Assignment>(
        QAsg::employeeId == employeeId && QAsg::returnedAt.is_null());
    for (auto it = r.begin(); it != r.end(); ++it)
        out.push_back(*db_.load<Asset>(it->assetId()));

    std::sort(out.begin(), out.end(), [](const Asset& a, const Asset& b) {
        return a.id() > b.id();
    });

    t.commit();
    return out;
}

// ------------------------------------------------------------
// Передать выданную технику другому фотографу.
// Ошибки: "Техника не выдана — используйте «Выдать»",
//         "Нельзя переназначить уволенному фотографу",
//         "Нет активной выдачи",
//         "Техника уже у этого фотографа".
// ------------------------------------------------------------
void Service::reassignAsset(long assetId, long newEmployeeId) {
    giveBack(assetId);
    issue(assetId, newEmployeeId);
}

// ------------------------------------------------------------
// Удалить технику и все её выдачи. Только если не выдана.
// Ошибка: "Нельзя удалить выданную технику — сначала примите возврат".
// ------------------------------------------------------------
void Service::deleteAssetCascade(long assetId) {
    odb::transaction t(db_.begin());

    odb::result<Asset> r = db_.query<Asset>(QAst::id == assetId);
    if (r.begin() == r.end())
        throw DomainError("Такой техники не существует");

    if (!db_.query<Assignment>(QAsg::assetId == assetId && QAsg::returnedAt.is_null()).empty()) {
        throw DomainError("Нельзя удалить выданную технику — сначала примите возврат");
    }

    db_.erase_query<Assignment>(QAsg::assetId == assetId);
    db_.erase_query<Asset>(QAst::id == assetId);

    t.commit();
}