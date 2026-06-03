#ifndef RESUMESERVICE_H
#define RESUMESERVICE_H

#include <QObject>
#include <QList>
#include "../model/ResumeCategory.h"

class ResumeService : public QObject {
    Q_OBJECT

public:
    static ResumeService &getInstance();

    QList<ResumeCategory> getCategories() const;
    ResumeCategory getCategoryById(int categoryId) const;

    bool addItem(int categoryId, const ResumeItem &item);
    bool updateItem(int categoryId, const ResumeItem &item);
    bool deleteItem(int categoryId, int itemId);
    bool reorderItems(int categoryId, const QList<int> &orderedItemIds);

    bool save();
    bool load();

signals:
    void dataChanged(int categoryId);

private:
    ResumeService(QObject *parent = nullptr);
    ~ResumeService() = default;
    ResumeService(const ResumeService &) = delete;
    ResumeService &operator=(const ResumeService &) = delete;

    void initDefaultCategories();

private:
    QList<ResumeCategory> mCategories;
};

#endif // RESUMESERVICE_H
