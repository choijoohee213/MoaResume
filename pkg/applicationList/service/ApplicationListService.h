#ifndef APPLICATIONLISTSERVICE_H
#define APPLICATIONLISTSERVICE_H

#include <QObject>
#include <QList>
#include "../model/Application.h"

class ApplicationListService : public QObject {
    Q_OBJECT

public:
    // Singleton instance
    static ApplicationListService &getInstance();

    // CRUD operations
    QList<Application> getAllApplications() const;

    Application getApplicationById(int id) const;

    bool addApplication(const Application &application);

    bool updateApplication(const Application &application);

    bool deleteApplication(int id);

    // File operations
    bool save();

    bool load();

signals:
    void dataChanged();

private:
    ApplicationListService(QObject *parent = nullptr);

    ~ApplicationListService() = default;

    // Prevent copying
    ApplicationListService(const ApplicationListService &) = delete;

    ApplicationListService &operator=(const ApplicationListService &) = delete;

private:
    QList<Application> mApplications;
    int mNextId;
};


#endif //APPLICATIONLISTSERVICE_H
