#ifndef RESUMEITEM_H
#define RESUMEITEM_H

#include <QString>
#include <QMap>
#include <QJsonObject>

class ResumeItem {
public:
    ResumeItem();

    int getId() const { return mId; }
    QMap<QString, QString> getFields() const { return mFields; }
    QString getField(const QString &key) const { return mFields.value(key); }

    void setId(int id) { mId = id; }
    void setField(const QString &key, const QString &value) { mFields[key] = value; }
    void setFields(const QMap<QString, QString> &fields) { mFields = fields; }

    QJsonObject toJson() const;
    static ResumeItem fromJson(const QJsonObject &json);

private:
    int mId;
    QMap<QString, QString> mFields;
};

#endif // RESUMEITEM_H
