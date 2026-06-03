#ifndef RESUMEHTMLBUILDER_H
#define RESUMEHTMLBUILDER_H

#include <QString>
#include <QMap>
#include <QSet>
#include "service/ResumeService.h"

class ResumeHtmlBuilder {
public:
    static QString build(ResumeService &service,
                         const QSet<int> &includedIds = QSet<int>());

private:
    static QString buildBasicInfo(const QMap<QString, QString> &f);
    static QString buildSection(const QString &title,
                                const QList<ResumeItem> &items,
                                CategoryType type);
    static QString buildEducationItem(const QMap<QString, QString> &f);
    static QString buildCareerItem(const QMap<QString, QString> &f);
    static QString buildSkillsItem(const QMap<QString, QString> &f);
    static QString buildCertificateItem(const QMap<QString, QString> &f);
    static QString buildAwardItem(const QMap<QString, QString> &f);
    static QString buildProjectItem(const QMap<QString, QString> &f);
    static QString buildActivityItem(const QMap<QString, QString> &f);
    static QString buildLanguageItem(const QMap<QString, QString> &f);
    static QString buildIntroductionItem(const QMap<QString, QString> &f);

    static QString esc(const QString &s);
    static QString dateRange(const QString &start, const QString &end);
    static QString field(const QMap<QString, QString> &f, const QString &key);
};

#endif // RESUMEHTMLBUILDER_H
