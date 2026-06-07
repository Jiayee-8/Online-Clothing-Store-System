#include <iostream>
#include "Database.h"
#include "UserManagement.h"
#include "ProductManagement.h"

using namespace std;

int main() {
    MYSQL* conn = Database::Connect();
    if (!conn) {
        cout << "[ERROR] Unable to connect to the database. Exiting..." << endl;
        return 1;
    }

    int choice;
    do {
        system("cls");
        UserManagement::DisplayHeader("Online Clothing Store");
        cout << "1. Register" << endl;
        cout << "2. Login" << endl;
        cout << "3. Exit" << endl;
        cout << "Enter your choice: ";
        cin >> choice;
        cin.ignore(); // To ignore leftover '\n' from cin

        switch (choice) {
        case 1:
            UserManagement::Register(conn);
            break;

        case 2: {
            string userType = UserManagement::Login(conn); // Modified Login to return user type
            if (userType == "admin") {
                ProductManagement::AdminPage(conn); // Admin functionality
            }
            else if (userType == "customer") {
                ProductManagement::CustomerPage(conn); // Customer functionality
            }
            else {
                cout << "[ERROR] Login failed or invalid user type." << endl;
            }
            break;
        }

        case 3:
            cout << "Exiting... Goodbye!" << endl;
            break;

        default:
            cout << "[ERROR] Invalid choice. Try again." << endl;
            system("pause");
        }
    } while (choice != 3);

    mysql_close(conn); // Close the database connection
    return 0;
}
