#ifndef RESUMECATEGORYCONFIG_H
#define RESUMECATEGORYCONFIG_H

#include <QString>
#include <QStringList>
#include <QList>
#include "../ResumeConstants.h"

struct FieldDefinition {
    enum Type { Text, Date, Combo, Multiline, Markdown, FileAttach };

    QString key;
    QString label;
    Type type;
    QStringList options; // Combo 전용
    bool required;

    FieldDefinition(const QString &key, const QString &label, Type type,
                    bool required = false, const QStringList &options = QStringList())
        : key(key), label(label), type(type), options(options), required(required) {}
};

class ResumeCategoryConfig {
public:
    static QList<FieldDefinition> getFields(CategoryType type);
    static QList<FieldDefinition> getEducationFields(const QString &eduType);
    static QString getSummary(CategoryType type, const QMap<QString, QString> &fields);
};

#endif // RESUMECATEGORYCONFIG_H
