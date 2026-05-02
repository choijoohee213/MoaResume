#include "ResumeCategoryConfig.h"
#include <QMap>

QList<FieldDefinition> ResumeCategoryConfig::getFields(CategoryType type) {
    switch (type) {
    case CategoryType::BasicInfo:
        return {
            {"name",      "이름",           FieldDefinition::Text, true},
            {"birthDate", "생년월일",        FieldDefinition::Date},
            {"phone",     "연락처",          FieldDefinition::Text},
            {"email",     "이메일",          FieldDefinition::Text},
            {"address",   "주소",            FieldDefinition::Text},
            {"github",    "GitHub",          FieldDefinition::Text},
            {"portfolio", "포트폴리오 링크", FieldDefinition::Text},
        };
    case CategoryType::Education:
        return {
            {"school",    "학교명", FieldDefinition::Text, true},
            {"major",     "전공",   FieldDefinition::Text},
            {"degree",    "학위",   FieldDefinition::Combo, false,
                {"고졸", "학사", "석사", "박사", "수료"}},
            {"startDate", "입학년월", FieldDefinition::Date},
            {"endDate",   "졸업년월", FieldDefinition::Date},
            {"gpa",       "학점",     FieldDefinition::Text},
        };
    case CategoryType::Career:
        return {
            {"company",     "회사명",   FieldDefinition::Text, true},
            {"position",    "직무",     FieldDefinition::Text},
            {"startDate",   "입사년월", FieldDefinition::Date},
            {"endDate",     "퇴사년월", FieldDefinition::Date},
            {"description", "담당업무", FieldDefinition::Multiline},
            {"achievement", "성과",     FieldDefinition::Multiline},
        };
    case CategoryType::Skills:
        return {
            {"skillName", "기술명",   FieldDefinition::Text, true},
            {"level",     "숙련도",   FieldDefinition::Combo, false,
                {"상", "중", "하"}},
            {"duration",  "경험기간", FieldDefinition::Text},
        };
    case CategoryType::Certificate:
        return {
            {"name",   "자격증명",  FieldDefinition::Text, true},
            {"issuer", "발급기관",  FieldDefinition::Text},
            {"date",   "취득일",    FieldDefinition::Date},
        };
    case CategoryType::Award:
        return {
            {"name",        "수상명",   FieldDefinition::Text, true},
            {"organizer",   "주최기관", FieldDefinition::Text},
            {"date",        "수상일",   FieldDefinition::Date},
            {"description", "수상내용", FieldDefinition::Multiline},
        };
    case CategoryType::Project:
        return {
            {"name",        "프로젝트명", FieldDefinition::Text, true},
            {"startDate",   "시작일",     FieldDefinition::Date},
            {"endDate",     "종료일",     FieldDefinition::Date},
            {"techStack",   "사용기술",   FieldDefinition::Text},
            {"role",        "역할",       FieldDefinition::Text},
            {"achievement", "성과",       FieldDefinition::Multiline},
            {"link",        "링크",       FieldDefinition::Text},
        };
    case CategoryType::Activity:
        return {
            {"name",         "활동명",   FieldDefinition::Text, true},
            {"organization", "기관",     FieldDefinition::Text},
            {"startDate",    "시작일",   FieldDefinition::Date},
            {"endDate",      "종료일",   FieldDefinition::Date},
            {"description",  "활동내용", FieldDefinition::Multiline},
        };
    case CategoryType::Language:
        return {
            {"language", "언어",     FieldDefinition::Text, true},
            {"testName", "시험명",   FieldDefinition::Text},
            {"score",    "점수/등급", FieldDefinition::Text},
            {"date",     "취득일",   FieldDefinition::Date},
        };
    case CategoryType::Introduction:
        return {
            {"title",   "질문", FieldDefinition::Text, true},
            {"content", "내용", FieldDefinition::Multiline},
        };
    }
    return {};
}

QString ResumeCategoryConfig::getSummary(CategoryType type, const QMap<QString, QString> &fields) {
    switch (type) {
    case CategoryType::BasicInfo:
        return fields.value("name");
    case CategoryType::Education: {
        QString school = fields.value("school");
        QString major  = fields.value("major");
        return major.isEmpty() ? school : QString("%1 · %2").arg(school, major);
    }
    case CategoryType::Career: {
        QString company  = fields.value("company");
        QString position = fields.value("position");
        return position.isEmpty() ? company : QString("%1 · %2").arg(company, position);
    }
    case CategoryType::Skills: {
        QString name  = fields.value("skillName");
        QString level = fields.value("level");
        return level.isEmpty() ? name : QString("%1 [%2]").arg(name, level);
    }
    case CategoryType::Certificate:
        return fields.value("name");
    case CategoryType::Award:
        return fields.value("name");
    case CategoryType::Project: {
        QString name  = fields.value("name");
        QString stack = fields.value("techStack");
        return stack.isEmpty() ? name : QString("%1 · %2").arg(name, stack);
    }
    case CategoryType::Activity: {
        QString name = fields.value("name");
        QString org  = fields.value("organization");
        return org.isEmpty() ? name : QString("%1 · %2").arg(name, org);
    }
    case CategoryType::Language: {
        QString lang  = fields.value("language");
        QString score = fields.value("score");
        return score.isEmpty() ? lang : QString("%1 · %2").arg(lang, score);
    }
    case CategoryType::Introduction:
        return fields.value("title");
    }
    return {};
}
