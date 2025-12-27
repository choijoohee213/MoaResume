#ifndef JSONFILEUTILS_H
#define JSONFILEUTILS_H

#include <QString>
#include <QJsonDocument>

class JsonFileUtils {
public:
    static QString getDataDirectory();

    static QString getDataFilePath(const QString &filename);

    static bool saveJsonToFile(const QString &filePath, const QJsonDocument &doc);

    static QJsonDocument loadJsonFromFile(const QString &filePath);

    static bool fileExists(const QString &filePath);
};

#endif // JSONFILEUTILS_H
