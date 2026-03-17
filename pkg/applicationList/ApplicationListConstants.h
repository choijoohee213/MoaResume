#ifndef APPLICATIONLISTCONSTANTS_H
#define APPLICATIONLISTCONSTANTS_H

namespace ApplicationListConstants {
    constexpr const char *APPLICATION_LIST_JSON_FILE_NAME = "applicationList.json";

    constexpr const char *JSON_KEY_ID = "id";
    constexpr const char *JSON_KEY_COMPANY_NAME = "companyName";
    constexpr const char *JSON_KEY_POSITION = "position";
    constexpr const char *JSON_KEY_STATUS = "status";
    constexpr const char *JSON_KEY_DEADLINE = "deadline";

    constexpr const char *JSON_KEY_APPLICATIONS = "applications";
    constexpr const char *JSON_KEY_NEXT_ID = "nextId";
    constexpr const char *JSON_KEY_NOTES = "notes";

    constexpr const char *QSS_PATH = ":/applicationList/styles/applicationList.qss";

    constexpr const char *BTN_TEXT_ADD = "추가";
    constexpr const char *BTN_TEXT_EDIT = "수정";
    constexpr const char *BTN_TEXT_DELETE = "삭제";
    constexpr const char *BTN_TEXT_SAVE = "저장";
    constexpr const char *BTN_TEXT_CANCEL = "취소";

    constexpr const char *BTN_NAME_ADD = "addButton";
    constexpr const char *BTN_NAME_EDIT = "editButton";
    constexpr const char *BTN_NAME_DELETE = "deleteButton";

    constexpr const char *DIALOG_TITLE_ADD = "지원서 추가";
    constexpr const char *DIALOG_TITLE_EDIT = "지원서 수정";
    constexpr const char *DIALOG_TITLE_DETAIL = "지원서 상세";

    constexpr const char *LABEL_COMPANY_NAME = "기업 이름:";
    constexpr const char *LABEL_POSITION = "직무:";
    constexpr const char *LABEL_STATUS = "상태:";
    constexpr const char *LABEL_DEADLINE = "마감일:";
    constexpr const char *LABEL_NOTES = "메모:";

    constexpr const char *STATUS_APPLIED = "지원 완료";
    constexpr const char *STATUS_DOCUMENT_PASSED = "서류 통과";
    constexpr const char *STATUS_CODING_TEST_PASSED = "인적성 검사(코딩테스트) 통과";
    constexpr const char *STATUS_FIRST_INTERVIEW_PASSED = "1차 면접 통과";
    constexpr const char *STATUS_FINAL_INTERVIEW_PASSED = "최종 면접 통과";
    constexpr const char *STATUS_FINAL_PASSED = "최종 합격";
    constexpr const char *STATUS_REJECTED = "불합격";

    constexpr const char *MSG_TITLE_ERROR = "오류";
    constexpr const char *MSG_TITLE_CONFIRM = "확인";

    constexpr const char *MSG_DELETE_CONFIRM = "선택한 지원서를 삭제하시겠습니까?";
    constexpr const char *MSG_ADD_FAILED = "지원서 추가에 실패했습니다.";
    constexpr const char *MSG_UPDATE_FAILED = "지원서 수정에 실패했습니다.";
    constexpr const char *MSG_DELETE_FAILED = "지원서 삭제에 실패했습니다.";
    constexpr const char *MSG_VALIDATION_EMPTY = "모든 필드를 입력해주세요.";
};

#endif
