#ifndef RESUMECATEGORY_H
#define RESUMECATEGORY_H

#include <QString>
#include <QList>
#include <QJsonObject>
#include "ResumeItem.h"
#include "../ResumeConstants.h"

class ResumeCategory {
public:
    ResumeCategory();
    ResumeCategory(const QString &name, CategoryType type);

    int getId() const { return mId; }
    QString getName() const { return mName; }
    CategoryType getType() const { return mType; }
    QList<ResumeItem> getItems() const { return mItems; }

    void setId(int id) { mId = id; }
    void setName(const QString &name) { mName = name; }
    void setType(CategoryType type) { mType = type; }
    void setItems(const QList<ResumeItem> &items) { mItems = items; }

    void addItem(const ResumeItem &item) { mItems.append(item); }
    void updateItem(const ResumeItem &item);
    void removeItem(int itemId);
    void reorderItems(const QList<int> &orderedItemIds);
    ResumeItem getItemById(int itemId) const;

    int takeNextItemId() { return mNextItemId++; }

    QJsonObject toJson() const;
    static ResumeCategory fromJson(const QJsonObject &json);

private:
    int mId;
    QString mName;
    CategoryType mType;
    QList<ResumeItem> mItems;
    int mNextItemId;
};

#endif // RESUMECATEGORY_H
