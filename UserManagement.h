#ifndef USER_MANAGEMENT_H
#define USER_MANAGEMENT_H

#include <mysql.h>
#include <string>
#include <iostream>

using namespace std;

class UserManagement {
private:
    static string currentUserId;  // This will store the user_id of the logged-in user

public:
    static void DisplayHeader(const string& title);
    static string Login(MYSQL* conn);
    static void Register(MYSQL* conn);
    static string getCurrentUserId() { return currentUserId; }
    static void setCurrentUserId(const string& userId) { currentUserId = userId; }
};

#endif // USER_MANAGEMENT_H


