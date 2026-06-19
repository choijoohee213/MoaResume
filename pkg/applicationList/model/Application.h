#ifndef APPLICATION_H
#define APPLICATION_H

#include <QString>
#include <QDate>
#include <QJsonObject>

class Application {
public:
    Application();
    Application(const QString &companyName, const QString &position,
                const QString &status, const QDate &deadline);

    // Getters
    int getId() const { return mId; }
    QString getCompanyName() const { return mCompanyName; }
    QString getPosition() const { return mPosition; }
    QString getStatus() const { return mStatus; }
    QDate getDeadline() const { return mDeadline; }
    QString getNotes() const { return mNotes; }
    QString getLogoPath() const { return mLogoPath; }

    // Setters
    void setId(int id) { mId = id; }
    void setCompanyName(const QString &name) { mCompanyName = name; }
    void setPosition(const QString &position) { mPosition = position; }
    void setStatus(const QString &status) { mStatus = status; }
    void setDeadline(const QDate &deadline) { mDeadline = deadline; }
    void setNotes(const QString &notes) { mNotes = notes; }
    void setLogoPath(const QString &logoPath) { mLogoPath = logoPath; }

    // JSON serialization
    QJsonObject toJson() const;
    static Application fromJson(const QJsonObject &json);

private:
    int mId;
    QString mCompanyName;
    QString mPosition;
    QString mStatus;
    QDate mDeadline;
    QString mNotes;
    QString mLogoPath;
};

#endif // APPLICATION_H