#include "ResumeHtmlBuilder.h"
#include <QFileInfo>

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
    font-size: 13pt;
    font-weight: bold;
    color: #1E3A5F;
    background-color: #EFF6FF;
    border-left: 4px solid #3B82F6;
    padding: 5px 10px;
    margin: 22px 0 10px 0;
}
.item { margin-bottom: 0; }
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

QString ResumeHtmlBuilder::build(ResumeService &service, const QSet<int> &includedIds) {
    auto included = [&](const ResumeCategory &cat) {
        return includedIds.isEmpty() || includedIds.contains(cat.getId());
    };

    QString html = "<!DOCTYPE HTML><html><head><meta charset=\"UTF-8\">" + CSS + "</head><body>";

    // 기본정보 항상 맨 먼저
    for (const ResumeCategory &cat : service.getCategories()) {
        if (cat.getType() == CategoryType::BasicInfo) {
            if (included(cat)) {
                QMap<QString, QString> f;
                if (!cat.getItems().isEmpty())
                    f = cat.getItems().first().getFields();
                html += buildBasicInfo(f);
            }
            break;
        }
    }
    // 나머지 카테고리
    for (const ResumeCategory &cat : service.getCategories()) {
        if (cat.getType() == CategoryType::BasicInfo || cat.getItems().isEmpty()) continue;
        if (!included(cat)) continue;
        html += buildSection(cat.getName(), cat.getItems(), cat.getType());
    }

    html += "</body></html>";
    return html;
}

QString ResumeHtmlBuilder::buildBasicInfo(const QMap<QString, QString> &f) {
    QString name = esc(field(f, "name"));
    if (name.isEmpty()) name = "(이름 없음)";

    auto row = [&](const QString &label, const QString &key) -> QString {
        QString v = esc(field(f, key));
        if (v.isEmpty()) return "";
        return "<div class='item-meta'><b>" + label + "</b>  " + v + "</div>";
    };

    auto linkRow = [&](const QString &label, const QString &url) -> QString {
        if (url.isEmpty()) return "";
        QString escaped = esc(url);
        QString href = url.startsWith("http") ? escaped : "https://" + escaped;
        return "<div class='item-meta'><b>" + label + "</b>  "
               "<a href='" + href + "' style='color:#3B82F6;'>" + escaped + "</a></div>";
    };

    QString contact;
    contact += row("생년월일", "birthDate");
    contact += row("연락처",   "phone");
    contact += row("이메일",   "email");
    contact += row("주소",     "address");
    contact += linkRow("GitHub", field(f, "github"));

    // 포트폴리오: 링크 또는 파일
    QString portfolioType = field(f, "portfolioType");
    if (portfolioType == "파일") {
        QString filePath = field(f, "portfolioFile");
        if (!filePath.isEmpty()) {
            QString fileName = esc(QFileInfo(filePath).fileName());
            contact += "<div class='item-meta'><b>포트폴리오</b>  " + fileName + " (첨부파일)</div>";
        }
    } else {
        contact += linkRow("포트폴리오", field(f, "portfolio"));
    }

    QString photoPath = field(f, "photoPath");
    QString photoHtml;
    if (!photoPath.isEmpty()) {
        QString src = "file:///" + photoPath;
        photoHtml = "<img src='" + src + "' width='90' height='90' "
                    "style='border-radius:45px; vertical-align:middle;'>&nbsp;&nbsp;&nbsp;";
    }

    return "<h2>기본정보</h2>"
           "<div class='item'>"
           "<table><tr><td>" + photoHtml + "</td>"
           "<td valign='middle'>"
           "<div class='item-header' style='font-size:14pt;'>" + name + "</div>"
           + contact +
           "</td></tr></table>"
           "</div>";
}

static const QString ITEM_SEP = "<p style='margin:4px 0;'></p>";

QString ResumeHtmlBuilder::buildSection(const QString &title,
                                        const QList<ResumeItem> &items,
                                        CategoryType type) {
    QStringList parts;
    for (const ResumeItem &item : items) {
        const QMap<QString, QString> &f = item.getFields();
        QString part;
        switch (type) {
        case CategoryType::Education:    part = buildEducationItem(f);    break;
        case CategoryType::Career:       part = buildCareerItem(f);       break;
        case CategoryType::Skills:       part = buildSkillsItem(f);       break;
        case CategoryType::Certificate:  part = buildCertificateItem(f);  break;
        case CategoryType::Award:        part = buildAwardItem(f);        break;
        case CategoryType::Project:      part = buildProjectItem(f);      break;
        case CategoryType::Activity:     part = buildActivityItem(f);     break;
        case CategoryType::Language:     part = buildLanguageItem(f);     break;
        case CategoryType::Introduction: part = buildIntroductionItem(f); break;
        default: break;
        }
        if (!part.isEmpty()) parts.append(part);
    }
    if (parts.isEmpty()) return "";
    return "<h2>" + esc(title) + "</h2>" + parts.join(ITEM_SEP);
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

    return "<div class='item'>"
           "<div class='item-header'>" + header + "</div>"
           + (techStack.isEmpty() ? "" : "<div class='item-meta'>사용기술: " + techStack + "</div>")
           + (role.isEmpty()      ? "" : "<div class='item-meta'>역할: "     + role      + "</div>")
           + (achieve.isEmpty()   ? "" : "<div class='item-desc'>성과: "     + achieve   + "</div>")
           + (link.isEmpty()      ? "" : "<div class='item-meta'>링크: "     + link      + "</div>")
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
