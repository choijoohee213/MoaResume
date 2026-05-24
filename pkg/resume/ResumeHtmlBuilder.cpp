#include "ResumeHtmlBuilder.h"

static const QString CSS = R"(
<style>
body {
    font-family: 'Apple SD Gothic Neo', 'Malgun Gothic', sans-serif;
    font-size: 10pt;
    color: #374151;
    margin: 0; padding: 0;
    line-height: 1.6;
}
#header {
    padding-bottom: 12px;
    border-bottom: 2px solid #3B82F6;
    margin-bottom: 4px;
}
h1 {
    font-size: 22pt;
    font-weight: bold;
    color: #1E3A5F;
    margin: 0 0 4px 0;
}
.contact {
    font-size: 9pt;
    color: #6B7280;
}
.contact span { margin-right: 14px; }
h2 {
    font-size: 11pt;
    font-weight: bold;
    color: #1E3A5F;
    border-bottom: 1px solid #BFDBFE;
    padding-bottom: 3px;
    margin: 18px 0 8px 0;
}
.item { margin-bottom: 10px; }
.item-header {
    font-weight: bold;
    font-size: 10pt;
    color: #111827;
}
.item-meta {
    font-size: 9pt;
    color: #6B7280;
    margin-top: 1px;
}
.item-desc {
    font-size: 9pt;
    color: #374151;
    margin-top: 3px;
    white-space: pre-wrap;
}
.tag {
    font-size: 8pt;
    color: #3B82F6;
    background: #EFF6FF;
    padding: 1px 6px;
    border-radius: 4px;
    margin-right: 4px;
}
.intro-q {
    font-weight: bold;
    font-size: 10pt;
    color: #1E3A5F;
    margin-bottom: 4px;
}
.intro-a {
    font-size: 9pt;
    color: #374151;
    white-space: pre-wrap;
    margin-bottom: 12px;
}
</style>
)";

QString ResumeHtmlBuilder::esc(const QString &s) {
    QString r = s;
    r.replace("&", "&amp;").replace("<", "&lt;").replace(">", "&gt;");
    return r;
}

QString ResumeHtmlBuilder::field(const QMap<QString, QString> &f, const QString &key) {
    return f.value(key).trimmed();
}

QString ResumeHtmlBuilder::dateRange(const QString &start, const QString &end) {
    if (start.isEmpty() && end.isEmpty()) return "";
    if (start.isEmpty()) return end;
    if (end.isEmpty())   return start + " ~";
    return start + " ~ " + end;
}

QString ResumeHtmlBuilder::build(ResumeService &service) {
    QString html = "<!DOCTYPE HTML><html><head><meta charset=\"UTF-8\">" + CSS + "</head><body>";

    for (const ResumeCategory &cat : service.getCategories()) {
        if (cat.getItems().isEmpty()) continue;

        if (cat.getType() == CategoryType::BasicInfo) {
            html += buildBasicInfo(cat.getItems().first().getFields());
            continue;
        }

        html += buildSection(cat.getName(), cat.getItems(), cat.getType());
    }

    html += "</body></html>";
    return html;
}

QString ResumeHtmlBuilder::buildBasicInfo(const QMap<QString, QString> &f) {
    QString name = esc(field(f, "name"));
    if (name.isEmpty()) name = "(이름 없음)";

    QString contact;
    auto addContact = [&](const QString &key, const QString &prefix = "") {
        QString v = field(f, key);
        if (!v.isEmpty())
            contact += "<span>" + esc(prefix.isEmpty() ? v : prefix + " " + v) + "</span>";
    };
    addContact("birthDate");
    addContact("phone");
    addContact("email");
    addContact("address");
    addContact("github",    "GitHub:");
    addContact("portfolio", "포트폴리오:");

    return QString(
        "<div id='header'>"
        "<h1>%1</h1>"
        "<div class='contact'>%2</div>"
        "</div>"
    ).arg(name, contact);
}

QString ResumeHtmlBuilder::buildSection(const QString &title,
                                        const QList<ResumeItem> &items,
                                        CategoryType type) {
    QString body;
    for (const ResumeItem &item : items) {
        const QMap<QString, QString> &f = item.getFields();
        switch (type) {
        case CategoryType::Education:    body += buildEducationItem(f);    break;
        case CategoryType::Career:       body += buildCareerItem(f);       break;
        case CategoryType::Skills:       body += buildSkillsItem(f);       break;
        case CategoryType::Certificate:  body += buildCertificateItem(f);  break;
        case CategoryType::Award:        body += buildAwardItem(f);        break;
        case CategoryType::Project:      body += buildProjectItem(f);      break;
        case CategoryType::Activity:     body += buildActivityItem(f);     break;
        case CategoryType::Language:     body += buildLanguageItem(f);     break;
        case CategoryType::Introduction: body += buildIntroductionItem(f); break;
        default: break;
        }
    }
    if (body.isEmpty()) return "";
    return "<h2>" + esc(title) + "</h2>" + body;
}

QString ResumeHtmlBuilder::buildEducationItem(const QMap<QString, QString> &f) {
    QString school   = esc(field(f, "school"));
    QString eduType  = esc(field(f, "educationType"));
    QString major    = esc(field(f, "major"));
    QString range    = esc(dateRange(field(f, "startDate"), field(f, "endDate")));
    QString gradStat = esc(field(f, "graduationStatus"));
    QString gpa      = field(f, "gpa");
    QString gpaMax   = field(f, "gpaMax");

    QString header = school;
    if (!eduType.isEmpty()) header += " <span style='color:#6B7280;font-weight:normal;'>(" + eduType + ")</span>";

    QString meta;
    if (!major.isEmpty())    meta += major + "  ";
    if (!range.isEmpty())    meta += range + "  ";
    if (!gradStat.isEmpty()) meta += "[" + gradStat + "]";

    QString desc;
    if (!gpa.isEmpty()) {
        desc += "학점: " + esc(gpa);
        if (!gpaMax.isEmpty()) desc += " / " + esc(gpaMax);
    }
    QString minorType = field(f, "minorType");
    QString minorMajor = field(f, "minorMajor");
    if (!minorType.isEmpty() && minorType != "없음" && !minorMajor.isEmpty())
        desc += (desc.isEmpty() ? "" : "  |  ") + esc(minorType) + ": " + esc(minorMajor);

    return "<div class='item'>"
           "<div class='item-header'>" + header + "</div>"
           + (meta.isEmpty()  ? "" : "<div class='item-meta'>"  + meta  + "</div>")
           + (desc.isEmpty()  ? "" : "<div class='item-desc'>"  + desc  + "</div>")
           + "</div>";
}

QString ResumeHtmlBuilder::buildCareerItem(const QMap<QString, QString> &f) {
    QString company  = esc(field(f, "company"));
    QString position = esc(field(f, "position"));
    QString range    = esc(dateRange(field(f, "startDate"), field(f, "endDate")));
    QString desc     = esc(field(f, "description"));
    QString achieve  = esc(field(f, "achievement"));

    QString header = company;
    if (!position.isEmpty()) header += "  <span style='color:#6B7280;font-weight:normal;'>| " + position + "</span>";
    if (!range.isEmpty())    header += "  <span style='color:#9CA3AF;font-size:9pt;'>" + range + "</span>";

    return "<div class='item'>"
           "<div class='item-header'>" + header + "</div>"
           + (desc.isEmpty()    ? "" : "<div class='item-desc'>담당업무: "  + desc    + "</div>")
           + (achieve.isEmpty() ? "" : "<div class='item-desc'>성과: "      + achieve + "</div>")
           + "</div>";
}

QString ResumeHtmlBuilder::buildSkillsItem(const QMap<QString, QString> &f) {
    QString name     = esc(field(f, "skillName"));
    QString level    = esc(field(f, "level"));
    QString duration = esc(field(f, "duration"));

    QString header = name;
    if (!level.isEmpty())    header += "  <span class='tag'>" + level + "</span>";
    if (!duration.isEmpty()) header += "  <span style='color:#9CA3AF;font-size:9pt;'>" + duration + "</span>";

    return "<div class='item'><div class='item-header'>" + header + "</div></div>";
}

QString ResumeHtmlBuilder::buildCertificateItem(const QMap<QString, QString> &f) {
    QString name   = esc(field(f, "name"));
    QString grade  = esc(field(f, "grade"));
    QString number = esc(field(f, "number"));
    QString issuer = esc(field(f, "issuer"));
    QString date   = esc(field(f, "date"));

    QString header = name;
    if (!grade.isEmpty()) header += "  <span class='tag'>" + grade + "</span>";

    QString meta;
    if (!issuer.isEmpty()) meta += issuer + "  ";
    if (!date.isEmpty())   meta += date;
    if (!number.isEmpty()) meta += (meta.isEmpty() ? QString() : QString("  |  ")) + "자격증번호: " + number;

    return "<div class='item'>"
           "<div class='item-header'>" + header + "</div>"
           + (meta.isEmpty() ? "" : "<div class='item-meta'>" + meta + "</div>")
           + "</div>";
}

QString ResumeHtmlBuilder::buildAwardItem(const QMap<QString, QString> &f) {
    QString name      = esc(field(f, "name"));
    QString organizer = esc(field(f, "organizer"));
    QString date      = esc(field(f, "date"));
    QString desc      = esc(field(f, "description"));

    QString meta;
    if (!organizer.isEmpty()) meta += organizer + "  ";
    if (!date.isEmpty())      meta += date;

    return "<div class='item'>"
           "<div class='item-header'>" + name + "</div>"
           + (meta.isEmpty() ? "" : "<div class='item-meta'>" + meta + "</div>")
           + (desc.isEmpty() ? "" : "<div class='item-desc'>" + desc + "</div>")
           + "</div>";
}

QString ResumeHtmlBuilder::buildProjectItem(const QMap<QString, QString> &f) {
    QString name      = esc(field(f, "name"));
    QString range     = esc(dateRange(field(f, "startDate"), field(f, "endDate")));
    QString techStack = esc(field(f, "techStack"));
    QString role      = esc(field(f, "role"));
    QString achieve   = esc(field(f, "achievement"));
    QString link      = esc(field(f, "link"));

    QString header = name;
    if (!range.isEmpty()) header += "  <span style='color:#9CA3AF;font-size:9pt;'>" + range + "</span>";

    QString meta;
    if (!techStack.isEmpty()) meta += "사용기술: " + techStack + "  ";
    if (!role.isEmpty())      meta += "역할: " + role;

    return "<div class='item'>"
           "<div class='item-header'>" + header + "</div>"
           + (meta.isEmpty()    ? "" : "<div class='item-meta'>"  + meta    + "</div>")
           + (achieve.isEmpty() ? "" : "<div class='item-desc'>성과: " + achieve + "</div>")
           + (link.isEmpty()    ? "" : "<div class='item-meta'>링크: "  + link    + "</div>")
           + "</div>";
}

QString ResumeHtmlBuilder::buildActivityItem(const QMap<QString, QString> &f) {
    QString name  = esc(field(f, "name"));
    QString org   = esc(field(f, "organization"));
    QString range = esc(dateRange(field(f, "startDate"), field(f, "endDate")));
    QString desc  = esc(field(f, "description"));

    QString header = name;
    if (!org.isEmpty())   header += "  <span style='color:#6B7280;font-weight:normal;'>| " + org + "</span>";
    if (!range.isEmpty()) header += "  <span style='color:#9CA3AF;font-size:9pt;'>" + range + "</span>";

    return "<div class='item'>"
           "<div class='item-header'>" + header + "</div>"
           + (desc.isEmpty() ? "" : "<div class='item-desc'>" + desc + "</div>")
           + "</div>";
}

QString ResumeHtmlBuilder::buildLanguageItem(const QMap<QString, QString> &f) {
    QString lang     = esc(field(f, "language"));
    QString testName = esc(field(f, "testName"));
    QString score    = esc(field(f, "score"));
    QString date     = esc(field(f, "date"));

    QString header = lang;
    if (!testName.isEmpty()) header += "  <span style='color:#6B7280;font-weight:normal;'>" + testName + "</span>";
    if (!score.isEmpty())    header += "  <span class='tag'>" + score + "</span>";

    QString meta = date;

    return "<div class='item'>"
           "<div class='item-header'>" + header + "</div>"
           + (meta.isEmpty() ? "" : "<div class='item-meta'>" + meta + "</div>")
           + "</div>";
}

QString ResumeHtmlBuilder::buildIntroductionItem(const QMap<QString, QString> &f) {
    QString title   = esc(field(f, "title"));
    QString content = esc(field(f, "content"));

    return "<div class='item'>"
           "<div class='intro-q'>Q. " + title + "</div>"
           "<div class='intro-a'>" + content + "</div>"
           "</div>";
}
