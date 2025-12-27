#include "Application.h"
#include "../ApplicationListConstants.h"

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
    json[ApplicationListConstants::JSON_KEY_ID] = mId;
    json[ApplicationListConstants::JSON_KEY_COMPANY_NAME] = mCompanyName;
    json[ApplicationListConstants::JSON_KEY_POSITION] = mPosition;
    json[ApplicationListConstants::JSON_KEY_STATUS] = mStatus;
    json[ApplicationListConstants::JSON_KEY_DEADLINE] = mDeadline.toString(Qt::ISODate);
    return json;
}

Application Application::fromJson(const QJsonObject &json) {
    Application app;
    app.setId(json[ApplicationListConstants::JSON_KEY_ID].toInt());
    app.setCompanyName(json[ApplicationListConstants::JSON_KEY_COMPANY_NAME].toString());
    app.setPosition(json[ApplicationListConstants::JSON_KEY_POSITION].toString());
    app.setStatus(json[ApplicationListConstants::JSON_KEY_STATUS].toString());
    app.setDeadline(QDate::fromString(json[ApplicationListConstants::JSON_KEY_DEADLINE].toString(), Qt::ISODate));
    return app;
}