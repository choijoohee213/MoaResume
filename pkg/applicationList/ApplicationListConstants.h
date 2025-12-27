#ifndef APPLICATIONLISTCONSTANTS_H
#define APPLICATIONLISTCONSTANTS_H

namespace ApplicationListConstants {
    // File names
    constexpr const char *APPLICATION_LIST_JSON_FILE_NAME = "applicationList.json";

    // JSON keys for Application model
    constexpr const char *JSON_KEY_ID = "id";
    constexpr const char *JSON_KEY_COMPANY_NAME = "companyName";
    constexpr const char *JSON_KEY_POSITION = "position";
    constexpr const char *JSON_KEY_STATUS = "status";
    constexpr const char *JSON_KEY_DEADLINE = "deadline";

    // JSON keys for ApplicationList data
    constexpr const char *JSON_KEY_APPLICATIONS = "applications";
    constexpr const char *JSON_KEY_NEXT_ID = "nextId";
};

#endif //APPLICATIONLISTCONSTANTS_H
