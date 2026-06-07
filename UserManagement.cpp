#include <iostream>
#include <string>
#include <conio.h>  // For _getch()
#include <iomanip>  // For formatting output
#include <stdlib.h> // For system("cls") and system("pause")
#include "UserManagement.h"

using namespace std;

string UserManagement::currentUserId;  // Static member variable for storing user ID

// Function to display header
void UserManagement::DisplayHeader(const string& title) {
    system("cls");
    cout << "               =============================================================" << endl;
    cout << "               |                                                           |" << endl;
    cout << "               |               Welcome to the Online Clothing Store        |" << endl;
    cout << "               |                                                           |" << endl;
    cout << "               |                       ~Have a Nice Day~                   |" << endl;
    cout << "               |                                                           |" << endl;
    cout << "               =============================================================" << endl;
    cout << setw(55) << title << endl;
    cout << "               -------------------------------------------------------------" << endl;
    cout << endl;
}

// Function to register users
void UserManagement::Register(MYSQL* conn) {
    DisplayHeader("User Registration");
    string username, password, userType;

    cout << "Enter a Username: ";
    getline(cin, username);

    // Validate username input
    if (username.empty()) {
        cout << "[ERROR] Username cannot be empty." << endl;
        system("pause");
        return;
    }

    cout << "Enter a Password: ";
    char ch;
    while ((ch = _getch()) != 13) {  // Password masking
        if (ch == '\b' && password.length() > 0) {
            // Handle backspace: remove the last character from password string
            password.pop_back();
            cout << "\b \b";  // Move the cursor back, print a space, and move it back again
        }
        else if (ch != '\b') {
            // If it's not a backspace, add the character to the password string
            password += ch;
            cout << '*';  // Mask the character
        }
    }
    cout << endl;

    // Validate password input
    if (password.empty()) {
        cout << "[ERROR] Password cannot be empty." << endl;
        system("pause");
        return;
    }

    cout << "\nRegister as: 1. Customer  2. Admin" << endl;
    cout << "Enter your choice (1 or 2): ";
    getline(cin, userType);

    if (userType != "1" && userType != "2") {
        cout << "[ERROR] Invalid choice. Please choose either 1 (Customer) or 2 (Admin)." << endl;
        system("pause");
        return;
    }

    string insert_query;
    if (userType == "1") {
        insert_query = "INSERT INTO customer (username, password) VALUES ('" + username + "', '" + password + "')";
    }
    else {
        insert_query = "INSERT INTO admin (username, password) VALUES ('" + username + "', '" + password + "')";
    }

    if (mysql_query(conn, insert_query.c_str()) != 0) {
        cout << "[ERROR] Registration failed. Error: " << mysql_error(conn) << endl;
        system("pause");
        return;
    }

    cout << "\n[INFO] Registration successful!" << endl;

    // Retrieve the user ID after successful registration
    string getUserIdQuery;
    if (userType == "1") {
        getUserIdQuery = "SELECT customerid FROM customer WHERE username = '" + username + "' AND password = '" + password + "'";
    }
    else {
        getUserIdQuery = "SELECT adminid FROM admin WHERE username = '" + username + "' AND password = '" + password + "'";
    }

    if (mysql_query(conn, getUserIdQuery.c_str()) == 0) {
        MYSQL_RES* res = mysql_store_result(conn);
        if (res && mysql_num_rows(res) > 0) {
            MYSQL_ROW row = mysql_fetch_row(res);
            string userId = row[0];
            setCurrentUserId(userId);  // Store the user ID in static variable
            cout << "Your user ID is: " << userId << endl;
        }
        else {
            cout << "[ERROR] Failed to retrieve user ID. Error: " << mysql_error(conn) << endl;
        }
        mysql_free_result(res);
    }
    else {
        cout << "[ERROR] Failed to execute query for retrieving user ID. Error: " << mysql_error(conn) << endl;
    }

    system("pause");
}

// Function to login users
string UserManagement::Login(MYSQL* conn) {
    UserManagement::DisplayHeader("User Login");

    string username, password, userType;
    cout << "Enter Username: ";
    getline(cin, username);

    // Validate username input
    if (username.empty()) {
        cout << "[ERROR] Username cannot be empty." << endl;
        system("pause");
        return "invalid";
    }

    cout << "Enter Password: ";
    char ch;
    while ((ch = _getch()) != 13) { // Mask password input
        if (ch == '\b' && password.length() > 0) {
            // Handle backspace: remove the last character from password string
            password.pop_back();
            cout << "\b \b";  // Move the cursor back, print a space, and move it back again
        }
        else if (ch != '\b') {
            // If it's not a backspace, add the character to the password string
            password += ch;
            cout << '*';  // Mask the character
        }
    }
    cout << endl;

    // Validate password input
    if (password.empty()) {
        cout << "[ERROR] Password cannot be empty." << endl;
        system("pause");
        return "invalid";
    }

    cout << "Login as: 1. Customer  2. Admin" << endl;
    cout << "Enter your choice (1 or 2): ";
    getline(cin, userType);

    if (userType != "1" && userType != "2") {
        cout << "[ERROR] Invalid choice. Please choose either 1 (Customer) or 2 (Admin)." << endl;
        system("pause");
        return "invalid";
    }

    string query;
    if (userType == "1") {
        query = "SELECT customerid FROM customer WHERE username = '" + username + "' AND password = '" + password + "'";
        userType = "customer";
    }
    else {
        query = "SELECT adminid FROM admin WHERE username = '" + username + "' AND password = '" + password + "'";
        userType = "admin";
    }

    if (mysql_query(conn, query.c_str()) == 0) {
        MYSQL_RES* res = mysql_store_result(conn);
        if (res && mysql_num_rows(res) > 0) {
            MYSQL_ROW row = mysql_fetch_row(res);
            string userId = row[0];
            setCurrentUserId(userId);  // Store the user ID in static variable
            cout << "\n[INFO] Login successful! Welcome, " << username << "!" << endl;
            cout << "Your user ID is: " << userId << endl;  // Display user ID
            mysql_free_result(res);
            system("pause");
            return userType;
        }
        else {
            cout << "\n[ERROR] Invalid Username or Password. Please try again." << endl;
        }
        mysql_free_result(res);
    }
    else {
        cout << "\n[ERROR] Failed to execute query. Error: " << mysql_error(conn) << endl;
    }

    system("pause");
    return "invalid";
}
