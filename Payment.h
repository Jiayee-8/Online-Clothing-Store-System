#ifndef PAYMENT_H
#define PAYMENT_H

#include <iostream>
#include <iomanip>
#include <string>
#include <mysql.h>

using namespace std;

class Payment {
public:
    static void DisplayHeader(const string& title);
    void ProcessPayment(MYSQL* conn, const string& userId, double grandTotal, const string& orderId);
    void GenerateReceipt(MYSQL* conn, const std::string& paymentId);
};

#endif
