#include "ApplicationListService.h"
#include "../../../common/util/JsonFileUtils.h"
#include "../ApplicationListConstants.h"
#include <QJsonDocument>
#include <QJsonArray>
#include <QJsonObject>
#include <QDebug>

ApplicationListService::ApplicationListService(QObject *parent)
    : QObject(parent)
      , mNextId(0) {
    load();
}

ApplicationListService &ApplicationListService::getInstance() {
    static ApplicationListService instance;
    return instance;
}

QList<Application> ApplicationListService::getAllApplications() const {
    return mApplications;
}

Application ApplicationListService::getApplicationById(int id) const {
    for (const Application &app: mApplications) {
        if (app.getId() == id) {
            return app;
        }
    }
    return Application();
}

bool ApplicationListService::addApplication(const Application &application) {
    Application newApplication = application;
    newApplication.setId(mNextId++);
    mApplications.append(newApplication);

    bool result = save();
    if (result) {
        emit dataChanged();
    }
    return result;
}

bool ApplicationListService::updateApplication(const Application &application) {
    for (Application &app: mApplications) {
        if (app.getId() == application.getId()) {
            app = application;

            bool result = save();
            if (result) {
                emit dataChanged();
            }
            return result;
        }
    }
    return false;
}

bool ApplicationListService::deleteApplication(int id) {
    for (int i = 0; i < mApplications.length(); i++) {
        if (mApplications[i].getId() == id) {
            mApplications.removeAt(i);

            bool result = save();
            if (result) {
                emit dataChanged();
            }
            return result;
        }
    }
    return false;
}

bool ApplicationListService::save() {
    QJsonArray jsonArray;
    for (const Application &app: mApplications) {
        jsonArray.append(app.toJson());
    }

    QJsonObject rootObject;
    rootObject["applications"] = jsonArray;
    rootObject["nextId"] = mNextId;

    QJsonDocument doc(rootObject);
    QString filePath = JsonFileUtils::getDataFilePath(ApplicationListConstants::APPLICATION_LIST_JSON_FILE_NAME);

    return JsonFileUtils::saveJsonToFile(filePath, doc);
}

bool ApplicationListService::load() {
    QString filePath = JsonFileUtils::getDataFilePath(ApplicationListConstants::APPLICATION_LIST_JSON_FILE_NAME);

    if (!JsonFileUtils::fileExists(filePath)) {
        mApplications.clear();
        mNextId = 0;
        return true;
    }

    QJsonDocument doc = JsonFileUtils::loadJsonFromFile(filePath);
    if (doc.isNull() || !doc.isObject()) {
        qWarning() << "Invalid JSON format";
        return false;
    }

    QJsonObject rootObject = doc.object();
    QJsonArray jsonArray = rootObject["applications"].toArray();
    mNextId = rootObject["nextId"].toInt(0);

    mApplications.clear();
    for (const QJsonValue &value: jsonArray) {
        if (value.isObject()) {
            mApplications.append(Application::fromJson(value.toObject()));
        }
    }

    return true;
}


