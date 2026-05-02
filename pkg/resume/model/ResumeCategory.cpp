#include "ResumeCategory.h"
#include "../ResumeConstants.h"
#include <QJsonArray>

ResumeCategory::ResumeCategory()
    : mId(-1), mName(""), mType(CategoryType::BasicInfo), mNextItemId(1) {}

ResumeCategory::ResumeCategory(const QString &name, CategoryType type)
    : mId(-1), mName(name), mType(type), mNextItemId(1) {}

void ResumeCategory::updateItem(const ResumeItem &item) {
    for (int i = 0; i < mItems.size(); ++i) {
        if (mItems[i].getId() == item.getId()) {
            mItems[i] = item;
            return;
        }
    }
}

void ResumeCategory::removeItem(int itemId) {
    for (int i = 0; i < mItems.size(); ++i) {
        if (mItems[i].getId() == itemId) {
            mItems.removeAt(i);
            return;
        }
    }
}

ResumeItem ResumeCategory::getItemById(int itemId) const {
    for (const ResumeItem &item : mItems) {
        if (item.getId() == itemId) return item;
    }
    return ResumeItem();
}

QJsonObject ResumeCategory::toJson() const {
    QJsonObject json;
    json[ResumeConstants::JSON_KEY_CATEGORY_ID]   = mId;
    json[ResumeConstants::JSON_KEY_CATEGORY_NAME] = mName;
    json[ResumeConstants::JSON_KEY_CATEGORY_TYPE] = static_cast<int>(mType);
    json[ResumeConstants::JSON_KEY_NEXT_ITEM_ID]  = mNextItemId;

    QJsonArray itemsArray;
    for (const ResumeItem &item : mItems) {
        itemsArray.append(item.toJson());
    }
    json[ResumeConstants::JSON_KEY_CATEGORY_ITEMS] = itemsArray;

    return json;
}

ResumeCategory ResumeCategory::fromJson(const QJsonObject &json) {
    ResumeCategory category;
    category.setId(json[ResumeConstants::JSON_KEY_CATEGORY_ID].toInt());
    category.setName(json[ResumeConstants::JSON_KEY_CATEGORY_NAME].toString());
    category.setType(static_cast<CategoryType>(
        json[ResumeConstants::JSON_KEY_CATEGORY_TYPE].toInt()));
    category.mNextItemId = json[ResumeConstants::JSON_KEY_NEXT_ITEM_ID].toInt(1);

    QJsonArray itemsArray = json[ResumeConstants::JSON_KEY_CATEGORY_ITEMS].toArray();
    QList<ResumeItem> items;
    for (const QJsonValue &val : itemsArray) {
        items.append(ResumeItem::fromJson(val.toObject()));
    }
    category.setItems(items);

    return category;
}
