#include "ResumeService.h"
#include "../ResumeConstants.h"
#include "../../../common/util/JsonFileUtils.h"
#include <QJsonArray>
#include <QJsonObject>
#include <QJsonDocument>

ResumeService &ResumeService::getInstance() {
    static ResumeService instance;
    return instance;
}

ResumeService::ResumeService(QObject *parent) : QObject(parent) {
    if (!load()) {
        initDefaultCategories();
    }
}

void ResumeService::initDefaultCategories() {
    QList<QPair<const char*, CategoryType>> defaults = {
        {ResumeConstants::CATEGORY_BASIC_INFO,   CategoryType::BasicInfo},
        {ResumeConstants::CATEGORY_EDUCATION,    CategoryType::Education},
        {ResumeConstants::CATEGORY_CAREER,       CategoryType::Career},
        {ResumeConstants::CATEGORY_SKILLS,       CategoryType::Skills},
        {ResumeConstants::CATEGORY_CERTIFICATE,  CategoryType::Certificate},
        {ResumeConstants::CATEGORY_AWARD,        CategoryType::Award},
        {ResumeConstants::CATEGORY_PROJECT,      CategoryType::Project},
        {ResumeConstants::CATEGORY_ACTIVITY,     CategoryType::Activity},
        {ResumeConstants::CATEGORY_LANGUAGE,     CategoryType::Language},
        {ResumeConstants::CATEGORY_INTRODUCTION, CategoryType::Introduction},
    };

    int id = 1;
    for (const auto &pair : defaults) {
        ResumeCategory category(pair.first, pair.second);
        category.setId(id++);
        mCategories.append(category);
    }
}

QList<ResumeCategory> ResumeService::getCategories() const {
    return mCategories;
}

ResumeCategory ResumeService::getCategoryById(int categoryId) const {
    for (const ResumeCategory &category : mCategories) {
        if (category.getId() == categoryId) return category;
    }
    return ResumeCategory();
}

bool ResumeService::addItem(int categoryId, const ResumeItem &item) {
    for (int i = 0; i < mCategories.size(); ++i) {
        if (mCategories[i].getId() == categoryId) {
            ResumeItem newItem = item;
            newItem.setId(mCategories[i].takeNextItemId());
            mCategories[i].addItem(newItem);
            bool saved = save();
            if (saved) emit dataChanged(categoryId);
            return saved;
        }
    }
    return false;
}

bool ResumeService::updateItem(int categoryId, const ResumeItem &item) {
    for (int i = 0; i < mCategories.size(); ++i) {
        if (mCategories[i].getId() == categoryId) {
            mCategories[i].updateItem(item);
            bool saved = save();
            if (saved) emit dataChanged(categoryId);
            return saved;
        }
    }
    return false;
}

bool ResumeService::reorderItems(int categoryId, const QList<int> &orderedItemIds) {
    for (int i = 0; i < mCategories.size(); ++i) {
        if (mCategories[i].getId() == categoryId) {
            mCategories[i].reorderItems(orderedItemIds);
            bool saved = save();
            if (saved) emit dataChanged(categoryId);
            return saved;
        }
    }
    return false;
}

bool ResumeService::deleteItem(int categoryId, int itemId) {
    for (int i = 0; i < mCategories.size(); ++i) {
        if (mCategories[i].getId() == categoryId) {
            mCategories[i].removeItem(itemId);
            bool saved = save();
            if (saved) emit dataChanged(categoryId);
            return saved;
        }
    }
    return false;
}

bool ResumeService::save() {
    QJsonObject root;
    QJsonArray categoriesArray;
    for (const ResumeCategory &category : mCategories) {
        categoriesArray.append(category.toJson());
    }
    root[ResumeConstants::JSON_KEY_CATEGORIES] = categoriesArray;

    QJsonDocument doc(root);
    QString filePath = JsonFileUtils::getDataFilePath(ResumeConstants::RESUME_JSON_FILE_NAME);
    return JsonFileUtils::saveJsonToFile(filePath, doc);
}

bool ResumeService::load() {
    QString filePath = JsonFileUtils::getDataFilePath(ResumeConstants::RESUME_JSON_FILE_NAME);
    if (!JsonFileUtils::fileExists(filePath)) return false;

    QJsonDocument doc = JsonFileUtils::loadJsonFromFile(filePath);
    if (doc.isNull() || !doc.isObject()) return false;

    QJsonArray categoriesArray = doc.object()[ResumeConstants::JSON_KEY_CATEGORIES].toArray();
    if (categoriesArray.isEmpty()) return false;

    mCategories.clear();
    for (const QJsonValue &val : categoriesArray) {
        mCategories.append(ResumeCategory::fromJson(val.toObject()));
    }
    return true;
}
