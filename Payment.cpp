#include "payment.h"         // Payment class header file
#include "usermanagement.h"  // UserManagement class header file (for getting the current user ID)
#include <iostream>
#include <iomanip>
#include <string>
#include <mysql.h>           // MySQL C++ connector header for database operations
#include <algorithm>  // Include for std::transform

using namespace std;

// Display the header with a title
void Payment::DisplayHeader(const string& title) {
    string userId = UserManagement::getCurrentUserId(); // Retrieve the current user ID
    system("cls");
    cout << "               =============================================================" << endl;
    cout << "               |                                                           |" << endl;
    cout << "               |               Welcome to the Online Clothing Store        |" << endl;
    cout << "               |                                                           |" << endl;
    cout << "               |                       ~Have a Nice Day~                   |" << endl;
    cout << "               |                                                           |" << endl;
    cout << "               =============================================================" << endl;

    // Display the logged-in user ID
    cout << "               Logged-in User ID: " << userId << endl;
    cout << "               -------------------------------------------------------------" << endl;

    int total_width = 72; // Total width of the line
    int padding = (total_width - title.length()) / 2;

    cout << "               "; // Indent to match the margin
    cout << string(padding, ' ') << title << string(padding, ' ') << endl;

    cout << "               -------------------------------------------------------------" << endl;
    cout << endl;
    cout << flush; // Force immediate display
}

// Process the payment
void Payment::ProcessPayment(MYSQL* conn, const string& userId, double amount, const string& orderId) {
    DisplayHeader("Process Payment");

    cout << "Order ID: " << orderId << endl;
    cout << "Amount Due: RM" << fixed << setprecision(2) << amount << endl;

    // Dynamically allocate memory for the payment method
    string* paymentMethod = new string;
    bool validMethod = false; // Flag to check for valid input

    while (!validMethod) {
        cout << "\nChoose a payment method (e.g., Credit Card, PayPal, COD): ";
        cin.ignore();
        getline(cin, *paymentMethod);  // Use the pointer dereferencing to read input

        // Convert to lowercase for case-insensitive comparison
        string lowerMethod = *paymentMethod;
        transform(lowerMethod.begin(), lowerMethod.end(), lowerMethod.begin(), ::tolower);

        // Check for valid payment methods
        if (lowerMethod == "credit card" || lowerMethod == "paypal" || lowerMethod == "cod") {
            validMethod = true; // Input is valid
        }
        else {
            cout << "[ERROR] Invalid payment method! Please choose from Credit Card, PayPal, or COD.\n";
        }
    }

    // Proceed with the valid payment method
    cout << "\n[INFO] You selected: " << *paymentMethod << endl;

    // Insert payment into the database
    string insertPaymentQuery =
        "INSERT INTO payment (orderid, customerid, payment_method, payment_date, amount) "
        "VALUES ('" + orderId + "', '" + userId + "', '" + *paymentMethod + "', NOW(), " + to_string(amount) + ")";

    if (mysql_query(conn, insertPaymentQuery.c_str())) {
        cerr << "[ERROR] Failed to process payment: " << mysql_error(conn) << endl;
        delete paymentMethod; // Free allocated memory before returning
        system("pause");
        return;
    }

    // Get the latest payment ID
    string paymentIdQuery = "SELECT paymentid FROM payment WHERE customerid = '" + userId + "' "
        "ORDER BY payment_date DESC LIMIT 1";
    if (mysql_query(conn, paymentIdQuery.c_str())) {
        cerr << "[ERROR] Failed to retrieve Payment ID: " << mysql_error(conn) << endl;
        delete paymentMethod; // Free allocated memory before returning
        system("pause");
        return;
    }

    MYSQL_RES* res = mysql_store_result(conn);
    if (!res || mysql_num_rows(res) == 0) {
        cerr << "[ERROR] Payment ID retrieval failed!" << endl;
        mysql_free_result(res);
        delete paymentMethod; // Free allocated memory before returning
        system("pause");
        return;
    }

    MYSQL_ROW row = mysql_fetch_row(res);
    string paymentId = row[0]; // Retrieve the latest payment ID
    mysql_free_result(res);

    // Update the order status to "Paid"
    string updateOrderQuery = "UPDATE orders SET status = 'Paid' WHERE orderid = '" + orderId + "'";
    if (mysql_query(conn, updateOrderQuery.c_str())) {
        cerr << "[ERROR] Failed to update order status: " << mysql_error(conn) << endl;
        delete paymentMethod; // Free allocated memory before returning
        system("pause");
        return;
    }

    cout << "\n[INFO] Payment successful! Generating receipt...\n";
    system("pause");

    // Generate and display the receipt
    GenerateReceipt(conn, paymentId);

    // Free dynamically allocated memory for payment method
    delete paymentMethod;
}


void Payment::GenerateReceipt(MYSQL* conn, const std::string& paymentId) {
    DisplayHeader("Payment Receipt");

    // Query to retrieve payment and order details, including total discount calculation
    string query =
        "SELECT p.paymentid, p.orderid, p.customerid, p.payment_method, "
        "DATE_FORMAT(p.payment_date, '%Y-%m-%d %H:%i:%s') AS payment_date, "
        "IFNULL(p.amount, 0.0) AS amount, "
        "IFNULL(o.total_amount, 0.0) AS total_amount, "
        "(IFNULL(o.total_amount, 0.0) - IFNULL(p.amount, 0.0)) AS total_discount "
        "FROM payment p "
        "JOIN orders o ON p.orderid = o.orderid "
        "WHERE p.paymentid = '" + paymentId + "'";

    if (mysql_query(conn, query.c_str())) {
        cerr << "[ERROR] Failed to retrieve payment details: " << mysql_error(conn) << endl;
        system("pause");
        return;
    }

    MYSQL_RES* res = mysql_store_result(conn);
    if (!res || mysql_num_rows(res) == 0) {
        cerr << "[ERROR] No payment record found for Payment ID: " << paymentId << endl;
        mysql_free_result(res);
        system("pause");
        return;
    }

    MYSQL_ROW row = mysql_fetch_row(res);

    // Retrieve payment details from the query result
    string receiptPaymentId = row[0] ? row[0] : "N/A";
    string receiptOrderId = row[1] ? row[1] : "N/A";
    string receiptCustomerId = row[2] ? row[2] : "N/A";
    string receiptPaymentMethod = row[3] ? row[3] : "N/A";
    string receiptPaymentDate = row[4] ? row[4] : "N/A";

    double totalAmount = row[6] ? stod(row[6]) : 0.0;  // Original total amount
    double amountPaid = row[5] ? stod(row[5]) : 0.0;  // Amount paid by customer
    double totalDiscount = row[7] ? stod(row[7]) : 0.0;  // Total discount applied


    mysql_free_result(res);

    // Display the receipt details
    cout << "=============================================================" << endl;
    cout << "|                      PAYMENT RECEIPT                      |" << endl;
    cout << "=============================================================" << endl;
    cout << left << setw(20) << "Payment ID: " << receiptPaymentId << endl;
    cout << left << setw(20) << "Order ID: " << receiptOrderId << endl;
    cout << left << setw(20) << "Customer ID: " << receiptCustomerId << endl;
    cout << left << setw(20) << "Payment Method: " << receiptPaymentMethod << endl;
    cout << left << setw(20) << "Payment Date: " << receiptPaymentDate << endl;
    cout << "-------------------------------------------------------------" << endl;
    cout << left << setw(20) << "Original Total: " << "RM" << fixed << setprecision(2) << totalAmount << endl;
    cout << left << setw(20) << "Discount Applied: " << "RM" << fixed << setprecision(2) << totalDiscount << endl;
    cout << left << setw(20) << "Amount Paid: " << "RM" << fixed << setprecision(2) << amountPaid << endl;
    cout << "=============================================================" << endl;
    cout << "|          Thank you for shopping with us!                  |" << endl;
    cout << "|           We look forward to serving you again!           |" << endl;
    cout << "=============================================================" << endl;

    system("pause");
}
