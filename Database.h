#ifndef DATABASE_H
#define DATABASE_H

#include <mysql.h>
#include <iostream>
#include <string>

class Database {
public:
    static MYSQL* Connect();
};

#endif // DATABASE_H

