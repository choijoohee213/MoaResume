#include "ResumeItem.h"
#include "../ResumeConstants.h"

ResumeItem::ResumeItem() : mId(-1) {}

QJsonObject ResumeItem::toJson() const {
    QJsonObject json;
    json[ResumeConstants::JSON_KEY_ITEM_ID] = mId;

    QJsonObject fieldsJson;
    for (auto it = mFields.constBegin(); it != mFields.constEnd(); ++it) {
        fieldsJson[it.key()] = it.value();
    }
    json[ResumeConstants::JSON_KEY_ITEM_FIELDS] = fieldsJson;

    return json;
}

ResumeItem ResumeItem::fromJson(const QJsonObject &json) {
    ResumeItem item;
    item.setId(json[ResumeConstants::JSON_KEY_ITEM_ID].toInt());

    QJsonObject fieldsJson = json[ResumeConstants::JSON_KEY_ITEM_FIELDS].toObject();
    QMap<QString, QString> fields;
    for (auto it = fieldsJson.constBegin(); it != fieldsJson.constEnd(); ++it) {
        fields[it.key()] = it.value().toString();
    }
    item.setFields(fields);

    return item;
}
