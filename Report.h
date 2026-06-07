#ifndef REPORT_H
#define REPORT_H

#include <string>
#include <vector>
#include <map>
#include <mysql.h>

class Report {
public:
    void GenerateMonthlySalesReport(MYSQL* conn);
    void GenerateGrossProfitReport(MYSQL* conn);
private:
    void DisplayLineGraph(const std::map<std::string, double>& data, const std::string& title);
};

#endif

