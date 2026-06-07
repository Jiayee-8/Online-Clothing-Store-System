#include "Report.h"
#include <iostream>
#include <iomanip>
#include <map>
#include <string>
#include <mysql.h>

// Generate Monthly Sales Report
void Report::GenerateMonthlySalesReport(MYSQL* conn) {
    std::string query =
        "SELECT DATE_FORMAT(payment_date, '%Y-%m') AS month, "
        "SUM(amount) AS total_sales "
        "FROM payment "
        "GROUP BY DATE_FORMAT(payment_date, '%Y-%m') "
        "ORDER BY month;";

    std::cout << "Executing query: " << query << std::endl;

    if (mysql_query(conn, query.c_str())) {
        std::cerr << "[ERROR] Failed to retrieve sales report: " << mysql_error(conn) << std::endl;
        return;
    }

    MYSQL_RES* res = mysql_store_result(conn);
    if (!res || mysql_num_rows(res) == 0) {
        std::cout << "[INFO] No sales data available for the selected period." << std::endl;
        mysql_free_result(res);
        return;
    }

    std::map<std::string, double> salesData;
    MYSQL_ROW row;
    while ((row = mysql_fetch_row(res)) != nullptr) {
        std::string month = row[0];
        double sales = row[1] ? std::stod(row[1]) : 0.0;
        salesData[month] = sales;
    }
    mysql_free_result(res);

    DisplayLineGraph(salesData, "Monthly Sales Report (After Discounts)");
}

// Generate Gross Profit Report
void Report::GenerateGrossProfitReport(MYSQL* conn) {
    // SQL query to calculate gross profit per month dynamically
    std::string query =
        "SELECT DATE_FORMAT(o.order_date, '%Y-%m') AS month, "
        "SUM(o.total_amount) AS total_sales, "
        "COALESCE(SUM(oi.quantity * p.cost_price), 0) AS total_cost, "
        "(SUM(o.total_amount) - COALESCE(SUM(oi.quantity * p.cost_price), 0)) AS gross_profit "
        "FROM orders o "
        "LEFT JOIN order_items oi ON o.orderid = oi.orderid "
        "LEFT JOIN product_size ps ON oi.sizeid = ps.sizeid "
        "LEFT JOIN product p ON ps.productid = p.productid "
        "WHERE o.status = 'Paid' "
        "GROUP BY DATE_FORMAT(o.order_date, '%Y-%m') "
        "ORDER BY month;";

    std::cout << "[INFO] Executing query: " << query << "\n";

    // Execute the query
    if (mysql_query(conn, query.c_str())) {
        std::cerr << "[ERROR] Failed to retrieve gross profit report: " << mysql_error(conn) << std::endl;
        return;
    }

    // Store the result set
    MYSQL_RES* res = mysql_store_result(conn);
    if (!res) {
        std::cerr << "[ERROR] Failed to retrieve result set: " << mysql_error(conn) << std::endl;
        return;
    }

    if (mysql_num_rows(res) == 0) {
        std::cout << "[INFO] No gross profit data available for the selected period.\n";
        mysql_free_result(res);
        return;
    }

    // Map to store gross profit data by month
    std::map<std::string, double> profitData;

    // Display column headers
    std::cout << "\nMonth       | Total Sales (RM) | Total Cost (RM) | Gross Profit (RM)\n";
    std::cout << "------------------------------------------------------------------\n";

    // Fetch and display each row
    MYSQL_ROW row;
    while ((row = mysql_fetch_row(res)) != nullptr) {
        std::string month = row[0] ? row[0] : "NULL";
        double total_sales = row[1] ? std::stod(row[1]) : 0.0;
        double total_cost = row[2] ? std::stod(row[2]) : 0.0;
        double gross_profit = row[3] ? std::stod(row[3]) : 0.0;

        // Format the row for better display
        std::cout << std::setw(10) << month << " | "
            << std::setw(16) << std::fixed << std::setprecision(2) << total_sales << " | "
            << std::setw(14) << total_cost << " | "
            << std::setw(16) << gross_profit << "\n";

        // Store the gross profit data for later graph display
        profitData[month] = gross_profit;
    }
    std::cout << "------------------------------------------------------------------\n";

    // Free the result set
    mysql_free_result(res);

    // Display the gross profit line graph
    DisplayLineGraph(profitData, "Gross Profit Report");
}



// Display Line Graph
void Report::DisplayLineGraph(const std::map<std::string, double>& data, const std::string& title) {
    if (data.empty()) {
        std::cout << "[INFO] No data to display." << std::endl;
        return;
    }

    double maxValue = 0;

    // Determine maximum value for scaling
    for (const auto& pair : data) {
        if (pair.second > maxValue) {
            maxValue = pair.second;
        }
    }

    // Header
    std::cout << "\n" << title << "\n";
    std::cout << std::string(60, '=') << "\n";
    std::cout << std::left << std::setw(10) << "Month" << "| Sales (RM)\n";
    std::cout << std::string(60, '-') << "\n";

    // Generate graph
    for (const auto& pair : data) {
        const std::string& month = pair.first;
        double value = pair.second;
        int barLength = static_cast<int>((value / maxValue) * 50); // Scale bar to 50 characters

        std::cout << std::setw(10) << month << "| " << std::string(barLength, '*') << " " << std::fixed << std::setprecision(2) << value << "\n";
    }

    std::cout << std::string(60, '=') << "\n";

    system("pause");
}
