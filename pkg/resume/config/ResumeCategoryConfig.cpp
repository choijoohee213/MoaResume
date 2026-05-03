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
        // onCopyClicked() 등에서 사용하는 전체 가능 필드 목록
        return {
            {"educationType",    "학력 구분",          FieldDefinition::Combo, true,
                {"고등학교", "대학교", "대학원"}},
            {"school",          "학교명",              FieldDefinition::Text, true},
            {"startDate",       "입학일",              FieldDefinition::Date},
            {"admissionStatus", "입학 상태",           FieldDefinition::Combo, false,
                {"일반입학", "편입", "3학년 편입"}},
            {"endDate",         "졸업(예정)일",        FieldDefinition::Date},
            {"graduationStatus","졸업 상태",           FieldDefinition::Combo, false,
                {"졸업", "재학중", "졸업예정", "수료", "중퇴", "휴학"}},
            {"major",           "전공명",              FieldDefinition::Text},
            {"minorType",       "부전공/복수전공",     FieldDefinition::Combo, false,
                {"없음", "부전공", "복수전공"}},
            {"minorMajor",      "부/복수전공 전공명",  FieldDefinition::Text},
            {"gpa",             "총학점 평균",         FieldDefinition::Text},
            {"gpaMax",          "학점 만점",           FieldDefinition::Combo, false,
                {"4.0", "4.3", "4.5", "100점"}},
            {"attachmentPath",  "성적증명서",          FieldDefinition::FileAttach},
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
        QString school  = fields.value("school");
        QString eduType = fields.value("educationType");
        QString major   = fields.value("major");
        if (!major.isEmpty())   return QString("%1 · %2").arg(school, major);
        if (!eduType.isEmpty()) return QString("%1 (%2)").arg(school, eduType);
        return school;
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

QList<FieldDefinition> ResumeCategoryConfig::getEducationFields(const QString &eduType) {
    QList<FieldDefinition> fields = {
        {"school",          "학교명",       FieldDefinition::Text, true},
        {"startDate",       "입학일",       FieldDefinition::Date},
        {"admissionStatus", "입학 상태",    FieldDefinition::Combo, false,
            eduType == "고등학교"
                ? QStringList{"일반입학", "편입"}
                : QStringList{"일반입학", "편입", "3학년 편입"}},
        {"endDate",         "졸업(예정)일", FieldDefinition::Date},
        {"graduationStatus","졸업 상태",    FieldDefinition::Combo, false,
            eduType == "고등학교"
                ? QStringList{"졸업", "재학중", "졸업예정", "중퇴", "휴학"}
                : QStringList{"졸업", "재학중", "졸업예정", "수료", "중퇴", "휴학"}},
    };

    if (eduType != "고등학교") {
        fields.append({"major",    "전공명",          FieldDefinition::Text});
        fields.append({"minorType","부전공/복수전공", FieldDefinition::Combo, false,
                        QStringList{"없음", "부전공", "복수전공"}});
        // minorMajor 는 ResumeItemDialog 에서 minorType 선택에 따라 동적으로 처리
        fields.append({"gpa",      "총학점 평균",     FieldDefinition::Text});
        fields.append({"gpaMax",   "학점 만점",       FieldDefinition::Combo, false,
                        QStringList{"4.0", "4.3", "4.5", "100점"}});
    }

    fields.append({"attachmentPath", "성적증명서", FieldDefinition::FileAttach});
    return fields;
}
