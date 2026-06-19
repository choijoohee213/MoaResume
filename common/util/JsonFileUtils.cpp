#include "JsonFileUtils.h"
#include <QFile>
#include <QDir>
#include <QStandardPaths>
#include <QDebug>

QString JsonFileUtils::getDataDirectory() {
    QString appDataPath = QStandardPaths::writableLocation(QStandardPaths::AppDataLocation);
    QDir dir(appDataPath);
    if (!dir.exists()) {
        dir.mkpath(appDataPath);
    }
    return appDataPath;
}

QString JsonFileUtils::getDataFilePath(const QString& filename) {
    return getDataDirectory() + "/" + filename;
}

bool JsonFileUtils::saveJsonToFile(const QString& filePath, const QJsonDocument& doc) {
    QFile file(filePath);
    if (!file.open(QIODevice::WriteOnly)) {
        qWarning() << "Failed to open file for writing:" << filePath;
        return false;
    }

    file.write(doc.toJson());
    file.close();
    return true;
}

QJsonDocument JsonFileUtils::loadJsonFromFile(const QString& filePath) {
    QFile file(filePath);
    if (!file.open(QIODevice::ReadOnly)) {
        qWarning() << "Failed to open file for reading:" << filePath;
        return QJsonDocument();
    }

    QByteArray data = file.readAll();
    file.close();

    QJsonDocument doc = QJsonDocument::fromJson(data);
    if (doc.isNull()) {
        qWarning() << "Invalid JSON format in file:" << filePath;
    }

    return doc;
}

bool JsonFileUtils::fileExists(const QString& filePath) {
    return QFile::exists(filePath);
}
