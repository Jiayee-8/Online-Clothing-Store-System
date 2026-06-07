#include "Database.h"

using namespace std;

MYSQL* Database::Connect() {
    MYSQL* conn = mysql_init(0);
    if (!conn) {
        cout << "MySQL initialization failed!" << endl;
        return nullptr;
    }

    conn = mysql_real_connect(conn, "localhost", "root", "", "online clothing store", 3306, NULL, 0);
    if (conn) {
        cout << "Connected to database!" << endl;
    }
    else {
        cout << "Failed to connect to database!" << endl;
    }
    return conn;
}
