#ifndef RESUMECONSTANTS_H
#define RESUMECONSTANTS_H

namespace ResumeConstants {
    constexpr const char *RESUME_JSON_FILE_NAME = "resume.json";

    // JSON keys
    constexpr const char *JSON_KEY_CATEGORIES = "categories";
    constexpr const char *JSON_KEY_CATEGORY_ID = "id";
    constexpr const char *JSON_KEY_CATEGORY_NAME = "name";
    constexpr const char *JSON_KEY_CATEGORY_TYPE = "type";
    constexpr const char *JSON_KEY_CATEGORY_ITEMS = "items";
    constexpr const char *JSON_KEY_ITEM_ID = "id";
    constexpr const char *JSON_KEY_ITEM_FIELDS = "fields";
    constexpr const char *JSON_KEY_NEXT_ITEM_ID = "nextItemId";

    // Category names
    constexpr const char *CATEGORY_BASIC_INFO = "기본 정보";
    constexpr const char *CATEGORY_EDUCATION = "학력";
    constexpr const char *CATEGORY_CAREER = "경력";
    constexpr const char *CATEGORY_SKILLS = "기술 스택";
    constexpr const char *CATEGORY_CERTIFICATE = "자격증";
    constexpr const char *CATEGORY_AWARD = "수상";
    constexpr const char *CATEGORY_PROJECT = "프로젝트";
    constexpr const char *CATEGORY_ACTIVITY = "대외활동";
    constexpr const char *CATEGORY_LANGUAGE = "어학";
    constexpr const char *CATEGORY_INTRODUCTION = "자기소개";

    // Buttons
    constexpr const char *BTN_TEXT_ADD = "추가";
    constexpr const char *BTN_TEXT_DELETE = "삭제";
    constexpr const char *BTN_TEXT_SAVE = "저장";
    constexpr const char *BTN_TEXT_CANCEL = "취소";
    constexpr const char *BTN_TEXT_COPY = "복사";

    // Dialog titles
    constexpr const char *DIALOG_TITLE_ADD = "항목 추가";
    constexpr const char *DIALOG_TITLE_EDIT = "항목 수정";

    // Messages
    constexpr const char *MSG_TITLE_ERROR = "오류";
    constexpr const char *MSG_TITLE_CONFIRM = "확인";
    constexpr const char *MSG_DELETE_CONFIRM = "선택한 항목을 삭제하시겠습니까?";
    constexpr const char *MSG_VALIDATION_EMPTY = "필수 항목을 입력해주세요.";
    constexpr const char *MSG_COPY_SUCCESS = "클립보드에 복사되었습니다.";

    // Style
    constexpr const char *QSS_PATH = ":/resume/styles/resume.qss";
};

enum class CategoryType {
    BasicInfo = 0,
    Education,
    Career,
    Skills,
    Certificate,
    Award,
    Project,
    Activity,
    Language,
    Introduction
};

#endif // RESUMECONSTANTS_H
