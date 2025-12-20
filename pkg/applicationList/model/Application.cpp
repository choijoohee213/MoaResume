#include "Application.h"

Application::Application()
    : mId(-1), mCompanyName(""), mPosition(""), mStatus(""), mDeadline(QDate()) {
}

Application::Application(const QString &companyName, const QString &position,
                         const QString &status, const QDate &deadline)
    : mId(-1), mCompanyName(companyName), mPosition(position),
      mStatus(status), mDeadline(deadline) {
}

QJsonObject Application::toJson() const {
    QJsonObject json;
    json["id"] = mId;
    json["companyName"] = mCompanyName;
    json["position"] = mPosition;
    json["status"] = mStatus;
    json["deadline"] = mDeadline.toString(Qt::ISODate);
    return json;
}

Application Application::fromJson(const QJsonObject &json) {
    Application app;
    app.setId(json["id"].toInt());
    app.setCompanyName(json["companyName"].toString());
    app.setPosition(json["position"].toString());
    app.setStatus(json["status"].toString());
    app.setDeadline(QDate::fromString(json["deadline"].toString(), Qt::ISODate));
    return app;
}