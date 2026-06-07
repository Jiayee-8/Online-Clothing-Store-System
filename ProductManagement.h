#ifndef PRODUCT_MANAGEMENT_H
#define PRODUCT_MANAGEMENT_H

#include <mysql.h>
#include <string>
#include <iostream>

using namespace std;

class ProductManagement {
public:
    static void DisplayHeader(const string& title);
    static void DisplayAdminMenu();
    static void DisplayCustomerMenu();
    static void AdminPage(MYSQL* conn);
    static void CustomerPage(MYSQL* conn);

private:
    // Admin Functions
    static void ManageProduct(MYSQL* conn);
    static void AddProduct(MYSQL* conn);
    static void EditProduct(MYSQL* conn);
    static void DeleteProduct(MYSQL* conn);
    static void ManageStock(MYSQL* conn);
    static void ViewStock(MYSQL* conn);
    static void UpdateStock(MYSQL* conn);
    static void ManageOrder(MYSQL* conn);
    static void ViewOrder(MYSQL* conn);
    static void UpdateOrderStatus(MYSQL* conn);
    static void GenerateReportMenu(MYSQL* conn);

    // Customer Functions
    static void ViewProducts(MYSQL* conn);
    static void AddToCart(MYSQL* conn);
    static void ViewCart(MYSQL* conn);
    static void UpdateCart(MYSQL* conn);
    static void DeleteFromCart(MYSQL* conn);
    static void Checkout(MYSQL* conn);
};

#endif // PRODUCT_MANAGEMENT_H
