#include "ApplicationListService.h"

ApplicationListService::ApplicationListService(QObject *parent)
    : QObject(parent)
      , mNextId(0) {
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
    return true;
}

bool ApplicationListService::load() {
    return true;
}


