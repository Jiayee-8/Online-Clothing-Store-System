#include "ProductManagement.h"
#include "UserManagement.h"
#include "Payment.h"
#include "Discount.h"
#include "Report.h"
#include <iomanip> // For formatting output
#include <vector>  // Required for vector
#include <utility> // Required for pair
#include <tuple>
#include <algorithm>  
#include <set>

using namespace std;

void ProductManagement::DisplayHeader(const string& title) {
    // Retrieve the current user ID (optional: you can handle if no user is logged in)
    string userId = UserManagement::getCurrentUserId();
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

    // Calculate dynamic padding for the title
    int total_width = 72; // Total width of the line (excluding the margin)
    int padding = (total_width - title.length()) / 2;

    cout << "               "; // Indent to match the margin
    cout << string(padding, ' ') << title << string(padding, ' ') << endl;

    cout << "               -------------------------------------------------------------" << endl;
    cout << endl;
    cout << flush;  // Forces output to be shown immediately
}


// Display the Admin Menu choices
void ProductManagement::DisplayAdminMenu() {
    cout << "1. Manage Product" << endl;
    cout << "2. Manage Stock" << endl;
    cout << "3. Manage Order" << endl;
    cout << "4. Generate Report" << endl;
    cout << "5. Return to Main Menu" << endl;
}

// Display the Customer Menu choices
void ProductManagement::DisplayCustomerMenu() {
    cout << "1. View Products" << endl;
    cout << "2. Add to Cart" << endl;
    cout << "3. View Cart" << endl;
    cout << "4. Return to Main Menu" << endl;
}

// Admin Page
void ProductManagement::AdminPage(MYSQL* conn) {
    DisplayHeader("Admin Page");
    int choice;
    do {
        DisplayAdminMenu();
        cout << "Enter your choice: ";
        cin >> choice;

        switch (choice) {
        case 1:
            ManageProduct(conn); // Function to manage products (add, edit, delete products)
            break;
        case 2:
            ManageStock(conn); // Function to manage stock levels (update stock)
            break;
        case 3:
            ManageOrder(conn); // Function to manage orders (view, update orders)
            break;
        case 4:
            GenerateReportMenu(conn); // Function to generate reports
            break;
        case 5:
            cout << "[INFO] Exiting admin menu...\n";
            return;
        default:
            cout << "[ERROR] Invalid choice. Please try again.\n";
            system("pause");
        }
    } while (true);
}


// Customer Page
void ProductManagement::CustomerPage(MYSQL* conn) {
    DisplayHeader("Customer Page");
    int choice;
    do {
        DisplayCustomerMenu();
        cout << "Enter your choice: ";
        cin >> choice;

        switch (choice) {
        case 1:
            ViewProducts(conn);
            break;
        case 2:
            AddToCart(conn);
            break;
        case 3:
            ViewCart(conn);
            break;
        case 4:
            cout << "[INFO] Exiting customer menu...\n";
            return;
        default:
            cout << "[ERROR] Invalid choice. Try again." << endl;
            system("pause");
        }
    } while (true);
}

// Admin Functions
void ProductManagement::ManageProduct(MYSQL* conn) {
    int choice;

    while (true) {
        system("cls");
        DisplayHeader("Manage Product Menu");
        cout << "1. Add Product\n";
        cout << "2. Edit Product\n";
        cout << "3. Delete Product\n";
        cout << "4. Back to Admin Menu\n";
        cout << "Choose an option: ";
        cin >> choice;

        switch (choice) {
        case 1:
            AddProduct(conn); // Function to add a product
            break;
        case 2:
            EditProduct(conn); // Function to edit an existing product
            break;
        case 3:
            DeleteProduct(conn); // Function to delete a product
            break;
        case 4:
            system("cls");
            DisplayHeader("Admin Page");
            return; // Back to Admin Menu
        default:
            cout << "[ERROR] Invalid choice. Please try again.\n";
            system("pause");
        }
    }
}


void ProductManagement::AddProduct(MYSQL* conn) {
    string product_name, description, categoryid, size;
    double price, cost_price;
    int stock;

    system("cls");
    DisplayHeader("Add Products");

    // Display available categories
    cout << "Available Categories:\n";
    string category_query = "SELECT categoryid, category_name, gender FROM category";
    if (mysql_query(conn, category_query.c_str())) {
        cerr << "[ERROR] Failed to retrieve categories: " << mysql_error(conn) << endl;
        system("pause");
        return;
    }

    MYSQL_RES* res = mysql_store_result(conn);
    if (mysql_num_rows(res) == 0) {
        cout << "[ERROR] No categories available.\n";
        mysql_free_result(res);
        system("pause");
        return;
    }

    cout << left << setw(15) << "Category ID"
        << setw(30) << "Category Name"
        << setw(10) << "Gender" << endl;
    cout << string(55, '-') << endl;

    MYSQL_ROW row;
    while ((row = mysql_fetch_row(res)) != nullptr) {
        cout << left << setw(15) << row[0]
            << setw(30) << row[1]
            << setw(10) << row[2] << endl;
    }
    mysql_free_result(res);

    // Get category ID from admin
    cout << "\nEnter the Category ID of the product: ";
    cin >> categoryid;

    // Validate category ID
    string validate_category_query = "SELECT COUNT(*) FROM category WHERE categoryid = '" + categoryid + "'";
    if (mysql_query(conn, validate_category_query.c_str())) {
        cerr << "[ERROR] Failed to validate category ID: " << mysql_error(conn) << endl;
        system("pause");
        return;
    }

    res = mysql_store_result(conn);
    row = mysql_fetch_row(res);
    int category_exists = atoi(row[0]);
    mysql_free_result(res);

    if (category_exists == 0) {
        cout << "[ERROR] Invalid Category ID. Please try again.\n";
        system("pause");
        return;
    }

    // Get product details
    cin.ignore();
    cout << "Enter product name: ";
    getline(cin, product_name);

    cout << "Enter product description: ";
    getline(cin, description);

    cout << "Enter product price: ";
    cin >> price;

    cout << "Enter product cost price: ";
    cin >> cost_price;  // New line to enter the cost price

    cout << "Enter product size (e.g., S, M, L): ";
    cin.ignore();
    getline(cin, size);

    cout << "Enter stock quantity for this size: ";
    cin >> stock;

    // Insert product into 'product' table
    string insert_product_query = "INSERT INTO product (product_name, categoryid, description, price, cost_price) "
        "VALUES ('" + product_name + "', '" + categoryid + "', '" + description + "', " + to_string(price) + ", " + to_string(cost_price) + ")";

    if (mysql_query(conn, insert_product_query.c_str())) {
        cerr << "[ERROR] Failed to add product: " << mysql_error(conn) << endl;
        system("pause");
        return;
    }

    // Retrieve the newly generated productid using the product name and categoryid
    string get_productid_query = "SELECT productid FROM product WHERE product_name = '" + product_name + "' AND categoryid = '" + categoryid + "'";
    if (mysql_query(conn, get_productid_query.c_str())) {
        cerr << "[ERROR] Failed to retrieve product ID: " << mysql_error(conn) << endl;
        system("pause");
        return;
    }

    res = mysql_store_result(conn);
    row = mysql_fetch_row(res);
    string productid = row[0];  // Retrieve the productid as a string
    mysql_free_result(res);

    if (productid.empty()) {
        cerr << "[ERROR] Could not retrieve the product ID. Please check the database.\n";
        system("pause");
        return;
    }

    // Insert product size and stock into 'product_size' table
    string insert_size_query = "INSERT INTO product_size (productid, size, stock) "
        "VALUES ('" + productid + "', '" + size + "', " + to_string(stock) + ")";

    if (mysql_query(conn, insert_size_query.c_str())) {
        cerr << "[ERROR] Failed to add product size: " << mysql_error(conn) << endl;
        system("pause");
        return;
    }

    cout << "[INFO] Product added successfully!\n";
    system("pause");
}

void ProductManagement::EditProduct(MYSQL* conn) {
    system("cls");
    DisplayHeader("Edit Products");

    // Display all products with details excluding size information
    cout << "Available Products:\n";
    string query = "SELECT p.productid, p.categoryid, c.category_name, p.product_name, p.price, p.cost_price, p.description "
        "FROM product p "
        "INNER JOIN category c ON p.categoryid = c.categoryid";

    if (mysql_query(conn, query.c_str())) {
        cerr << "[ERROR] Failed to retrieve products: " << mysql_error(conn) << endl;
        system("pause");
        return;
    }

    MYSQL_RES* res = mysql_store_result(conn);
    if (mysql_num_rows(res) == 0) {
        cout << "[INFO] No products available to edit.\n";
        mysql_free_result(res);
        system("pause");
        return;
    }

    // Display the products in a formatted table
    cout << left << setw(10) << "ProductID"
        << setw(10) << "CatID"
        << setw(20) << "Category Name"
        << setw(25) << "Product Name"
        << setw(10) << "Price"
        << setw(10) << "Cost Price"
        << setw(30) << "Description" << endl;
    cout << string(115, '-') << endl;

    MYSQL_ROW row;
    while ((row = mysql_fetch_row(res)) != nullptr) {
        cout << left << setw(10) << row[0] // productid
            << setw(10) << row[1] // categoryid
            << setw(20) << row[2] // category_name
            << setw(25) << row[3] // product_name
            << setw(10) << row[4] // price
            << setw(10) << row[5] // cost_price
            << setw(30) << row[6] << endl; // description
    }
    mysql_free_result(res);

    // Prompt admin to choose a product to edit
    string productid;
    cout << "\nEnter the Product ID of the product to edit: ";
    cin >> productid;

    // Validate productid
    string validate_productid_query = "SELECT COUNT(*) FROM product WHERE productid = '" + productid + "'";
    if (mysql_query(conn, validate_productid_query.c_str())) {
        cerr << "[ERROR] Failed to validate Product ID: " << mysql_error(conn) << endl;
        system("pause");
        return;
    }

    res = mysql_store_result(conn);
    row = mysql_fetch_row(res);
    int productid_exists = atoi(row[0]);
    mysql_free_result(res);

    if (productid_exists == 0) {
        cout << "[ERROR] Invalid Product ID. Please try again.\n";
        system("pause");
        return;
    }

    // Get current product details (including cost price)
    string current_categoryid, current_product_name, current_description;
    double current_price, current_cost_price;
    query = "SELECT p.categoryid, p.product_name, p.description, p.price, p.cost_price "
        "FROM product p "
        "WHERE p.productid = '" + productid + "'";

    if (mysql_query(conn, query.c_str())) {
        cerr << "[ERROR] Failed to retrieve product details: " << mysql_error(conn) << endl;
        system("pause");
        return;
    }

    res = mysql_store_result(conn);
    row = mysql_fetch_row(res);

    current_categoryid = row[0];
    current_product_name = row[1];
    current_description = row[2];
    current_price = atof(row[3]);
    current_cost_price = atof(row[4]);

    mysql_free_result(res);

    // Get new details from the admin (including cost price)
    string categoryid, product_name, description;
    double price, cost_price;

    cout << "Enter new category ID (leave empty to keep current): ";
    cin.ignore();
    getline(cin, categoryid);

    cout << "Enter new product name (leave empty to keep current): ";
    getline(cin, product_name);

    cout << "Enter new description (leave empty to keep current): ";
    getline(cin, description);

    cout << "Enter new price (enter -1 to keep current): ";
    cin >> price;

    cout << "Enter new cost price (enter -1 to keep current): ";
    cin >> cost_price;

    // Update query with conditional checks (including cost price)
    string update_query = "UPDATE product p SET ";

    if (!categoryid.empty()) {
        update_query += "p.categoryid = '" + categoryid + "', ";
    }
    if (!product_name.empty()) {
        update_query += "p.product_name = '" + product_name + "', ";
    }
    if (!description.empty()) {
        update_query += "p.description = '" + description + "', ";
    }
    if (price != -1) {
        update_query += "p.price = " + to_string(price) + ", ";
    }
    if (cost_price != -1) {
        update_query += "p.cost_price = " + to_string(cost_price) + ", ";
    }

    // Remove the last comma and space
    update_query = update_query.substr(0, update_query.length() - 2);

    // Add WHERE clause
    update_query += " WHERE p.productid = '" + productid + "'";

    // Execute the update query
    if (mysql_query(conn, update_query.c_str())) {
        cerr << "[ERROR] Failed to update product: " << mysql_error(conn) << endl;
        system("pause");
        return;
    }

    cout << "[INFO] Product updated successfully!\n";

    // Display the updated product list (including cost price)
    cout << "\nUpdated Product List:\n";
    query = "SELECT p.productid, p.categoryid, c.category_name, p.product_name, p.price, p.cost_price, p.description "
        "FROM product p "
        "INNER JOIN category c ON p.categoryid = c.categoryid";

    if (mysql_query(conn, query.c_str())) {
        cerr << "[ERROR] Failed to retrieve updated products: " << mysql_error(conn) << endl;
        system("pause");
        return;
    }

    res = mysql_store_result(conn);
    if (mysql_num_rows(res) == 0) {
        cout << "[INFO] No products available to display.\n";
        mysql_free_result(res);
    }
    else {
        system("cls");
        DisplayHeader("Edit Product");
        // Display the updated products in a formatted table
        cout << left << setw(10) << "ProductID"
            << setw(10) << "CatID"
            << setw(20) << "Category Name"
            << setw(25) << "Product Name"
            << setw(10) << "Price"
            << setw(10) << "Cost Price"
            << setw(30) << "Description" << endl;
        cout << string(115, '-') << endl;

        while ((row = mysql_fetch_row(res)) != nullptr) {
            cout << left << setw(10) << row[0] // productid
                << setw(10) << row[1] // categoryid
                << setw(20) << row[2] // category_name
                << setw(25) << row[3] // product_name
                << setw(10) << row[4] // price
                << setw(10) << row[5] // cost_price
                << setw(30) << row[6] << endl; // description
        }
        mysql_free_result(res);
    }

    system("pause");
}

void ProductManagement::DeleteProduct(MYSQL* conn) {
    system("cls");
    DisplayHeader("Delete Products");

    // Display all products with details (including cost price)
    cout << "Available Products:\n";
    string query = "SELECT p.productid, p.categoryid, c.category_name, p.product_name, p.price, p.cost_price, p.description "
        "FROM product p "
        "INNER JOIN category c ON p.categoryid = c.categoryid";

    if (mysql_query(conn, query.c_str())) {
        cerr << "[ERROR] Failed to retrieve products: " << mysql_error(conn) << endl;
        system("pause");
        return;
    }

    MYSQL_RES* res = mysql_store_result(conn);
    if (mysql_num_rows(res) == 0) {
        cout << "[INFO] No products available to delete.\n";
        mysql_free_result(res);
        system("pause");
        return;
    }

    // Display the products in a formatted table, including cost price
    cout << left << setw(10) << "ProductID"
        << setw(10) << "CatID"
        << setw(20) << "Category Name"
        << setw(25) << "Product Name"
        << setw(10) << "Price"
        << setw(10) << "Cost Price"
        << setw(30) << "Description" << endl;
    cout << string(115, '-') << endl;

    MYSQL_ROW row;
    while ((row = mysql_fetch_row(res)) != nullptr) {
        cout << left << setw(10) << row[0] // productid
            << setw(10) << row[1] // categoryid
            << setw(20) << row[2] // category_name
            << setw(25) << row[3] // product_name
            << setw(10) << row[4] // price
            << setw(10) << row[5] // cost_price
            << setw(30) << row[6] << endl; // description
    }
    mysql_free_result(res);

    // Prompt admin to choose a product to delete
    string productid;
    cout << "\nEnter the Product ID of the product to delete: ";
    cin >> productid;

    // Validate productid
    string validate_productid_query = "SELECT COUNT(*) FROM product WHERE productid = '" + productid + "'";
    if (mysql_query(conn, validate_productid_query.c_str())) {
        cerr << "[ERROR] Failed to validate Product ID: " << mysql_error(conn) << endl;
        system("pause");
        return;
    }

    res = mysql_store_result(conn);
    row = mysql_fetch_row(res);
    int productid_exists = atoi(row[0]);
    mysql_free_result(res);

    if (productid_exists == 0) {
        cout << "[ERROR] Invalid Product ID. Please try again.\n";
        system("pause");
        return;
    }

    // Ask for confirmation
    char confirmation;
    cout << "Are you sure you want to delete product with ID " << productid << "? (y/n): ";
    cin >> confirmation;

    if (confirmation != 'y' && confirmation != 'Y') {
        cout << "[INFO] Product deletion canceled.\n";
        system("pause");
        return;
    }

    // Delete product from product_size table first (cascade if necessary)
    string delete_product_size_query = "DELETE FROM product_size WHERE productid = '" + productid + "'";
    if (mysql_query(conn, delete_product_size_query.c_str())) {
        cerr << "[ERROR] Failed to delete product size: " << mysql_error(conn) << endl;
        system("pause");
        return;
    }

    // Now delete product from the product table
    string delete_product_query = "DELETE FROM product WHERE productid = '" + productid + "'";
    if (mysql_query(conn, delete_product_query.c_str())) {
        cerr << "[ERROR] Failed to delete product: " << mysql_error(conn) << endl;
        system("pause");
        return;
    }

    cout << "[INFO] Product deleted successfully!\n";

    system("cls");
    DisplayHeader("Delete Products");
    // Display the updated product list (including cost price)
    cout << "\nUpdated Product List:\n";
    query = "SELECT p.productid, p.categoryid, c.category_name, p.product_name, p.price, p.cost_price, p.description "
        "FROM product p "
        "INNER JOIN category c ON p.categoryid = c.categoryid";

    if (mysql_query(conn, query.c_str())) {
        cerr << "[ERROR] Failed to retrieve updated products: " << mysql_error(conn) << endl;
        system("pause");
        return;
    }

    res = mysql_store_result(conn);
    if (mysql_num_rows(res) == 0) {
        cout << "[INFO] No products available to display.\n";
        mysql_free_result(res);
    }
    else {
        // Display the updated products in a formatted table, including cost price
        cout << left << setw(10) << "ProductID"
            << setw(10) << "CatID"
            << setw(20) << "Category Name"
            << setw(25) << "Product Name"
            << setw(10) << "Price"
            << setw(10) << "Cost Price"
            << setw(30) << "Description" << endl;
        cout << string(115, '-') << endl;

        while ((row = mysql_fetch_row(res)) != nullptr) {
            cout << left << setw(10) << row[0] // productid
                << setw(10) << row[1] // categoryid
                << setw(20) << row[2] // category_name
                << setw(25) << row[3] // product_name
                << setw(10) << row[4] // price
                << setw(10) << row[5] // cost_price
                << setw(30) << row[6] << endl; // description
        }
        mysql_free_result(res);
    }
    system("pause");
}

void ProductManagement::ManageStock(MYSQL* conn) {
    int choice;
    while (true) {
        system("cls");
        DisplayHeader("Manage Stock");
        cout << "1. View Stock\n";
        cout << "2. Update Stock\n";
        cout << "3. Return to Admin Menu\n";
        cout << "Choose an option: ";
        cin >> choice;

        switch (choice) {
        case 1:
            ViewStock(conn);
            break;
        case 2:
            UpdateStock(conn);
            break;
        case 3:
            system("cls");
            DisplayHeader("Admin Page");
            return;  // Return to Admin Menu
        default:
            cout << "[ERROR] Invalid choice. Please try again.\n";
            system("pause");
            break;
        }
    }
}

void ProductManagement::ViewStock(MYSQL* conn) {
    system("cls");
    DisplayHeader("View Stock");

    // Query to retrieve stock details
    string query = "SELECT ps.productid, p.product_name, ps.size, ps.stock "
        "FROM product_size ps "
        "INNER JOIN product p ON ps.productid = p.productid";

    if (mysql_query(conn, query.c_str())) {
        cerr << "[ERROR] Failed to retrieve stock details: " << mysql_error(conn) << endl;
        system("pause");
        return;
    }

    MYSQL_RES* res = mysql_store_result(conn);
    if (mysql_num_rows(res) == 0) {
        cout << "[INFO] No stock data available to display.\n";
        mysql_free_result(res);
        system("pause");
        return;
    }

    // Display the stock in a formatted table
    cout << left << setw(10) << "ProductID"
        << setw(25) << "Product Name"
        << setw(10) << "Size"
        << setw(10) << "Stock" << endl;
    cout << string(60, '-') << endl;

    MYSQL_ROW row;
    while ((row = mysql_fetch_row(res)) != nullptr) {
        cout << left << setw(10) << row[0] // productid
            << setw(25) << row[1] // product_name
            << setw(10) << row[2] // size
            << setw(10) << row[3] << endl; // stock
    }
    mysql_free_result(res);

    system("pause");
}

void ProductManagement::UpdateStock(MYSQL* conn) {
    system("cls");
    DisplayHeader("Update Stock");

    // Display current stock
    cout << "Available Stock:\n";
    string query = "SELECT ps.productid, p.product_name, ps.size, ps.stock "
        "FROM product_size ps "
        "INNER JOIN product p ON ps.productid = p.productid";

    if (mysql_query(conn, query.c_str())) {
        cerr << "[ERROR] Failed to retrieve stock details: " << mysql_error(conn) << endl;
        system("pause");
        return;
    }

    MYSQL_RES* res = mysql_store_result(conn);
    if (mysql_num_rows(res) == 0) {
        cout << "[INFO] No stock data available to update.\n";
        mysql_free_result(res);
        system("pause");
        return;
    }

    // Display the stock in a formatted table
    cout << left << setw(10) << "ProductID"
        << setw(25) << "Product Name"
        << setw(10) << "Size"
        << setw(10) << "Stock" << endl;
    cout << string(60, '-') << endl;

    MYSQL_ROW row;
    while ((row = mysql_fetch_row(res)) != nullptr) {
        cout << left << setw(10) << row[0] // productid
            << setw(25) << row[1] // product_name
            << setw(10) << row[2] // size
            << setw(10) << row[3] << endl; // stock
    }
    mysql_free_result(res);

    // Prompt admin to choose a product to update
    string productid, size;
    int new_stock;

    cout << "\nEnter the Product ID of the product to update stock: ";
    cin >> productid;

    cout << "Enter the Size of the product to update stock: ";
    cin >> size;

    // Validate productid and size combination
    string validate_query = "SELECT COUNT(*) FROM product_size "
        "WHERE productid = '" + productid + "' AND size = '" + size + "'";
    if (mysql_query(conn, validate_query.c_str())) {
        cerr << "[ERROR] Failed to validate Product ID and Size: " << mysql_error(conn) << endl;
        system("pause");
        return;
    }

    res = mysql_store_result(conn);
    row = mysql_fetch_row(res);
    int record_exists = atoi(row[0]);
    mysql_free_result(res);

    if (record_exists == 0) {
        cout << "[ERROR] Invalid Product ID or Size. Please try again.\n";
        system("pause");
        return;
    }

    // Get new stock value
    cout << "Enter the new stock quantity: ";
    cin >> new_stock;

    // Update stock in the database
    string update_query = "UPDATE product_size SET stock = " + to_string(new_stock) +
        " WHERE productid = '" + productid + "' AND size = '" + size + "'";
    if (mysql_query(conn, update_query.c_str())) {
        cerr << "[ERROR] Failed to update stock: " << mysql_error(conn) << endl;
        system("pause");
        return;
    }

    cout << "[INFO] Stock updated successfully!\n";

    system("cls");
    DisplayHeader("Update Stock");
    // Display the updated stock list
    cout << "\nUpdated Stock List:\n";
    query = "SELECT ps.productid, p.product_name, ps.size, ps.stock "
        "FROM product_size ps "
        "INNER JOIN product p ON ps.productid = p.productid";

    if (mysql_query(conn, query.c_str())) {
        cerr << "[ERROR] Failed to retrieve updated stock details: " << mysql_error(conn) << endl;
        system("pause");
        return;
    }

    res = mysql_store_result(conn);
    if (mysql_num_rows(res) == 0) {
        cout << "[INFO] No stock data available to display.\n";
        mysql_free_result(res);
    }
    else {
        // Display the updated stock in a formatted table
        cout << left << setw(10) << "ProductID"
            << setw(25) << "Product Name"
            << setw(10) << "Size"
            << setw(10) << "Stock" << endl;
        cout << string(60, '-') << endl;

        while ((row = mysql_fetch_row(res)) != nullptr) {
            cout << left << setw(10) << row[0] // productid
                << setw(25) << row[1] // product_name
                << setw(10) << row[2] // size
                << setw(10) << row[3] << endl; // stock
        }
        mysql_free_result(res);
    }

    system("pause");
}


void ProductManagement::ManageOrder(MYSQL* conn) {
    int choice;

    while (true) {
        system("cls");
        DisplayHeader("Manage Order Menu");
        cout << "1. View Orders\n";
        cout << "2. Update Order Status\n";
        cout << "3. Back to Admin Menu\n";
        cout << "Choose an option: ";
        cin >> choice;

        switch (choice) {
        case 1:
            ViewOrder(conn); // Function to view all orders
            break;
        case 2:
            UpdateOrderStatus(conn); // Function to update order status (e.g., shipped, canceled)
            break;
        case 3:
            system("cls");
            DisplayHeader("Admin Page");
            return; // Back to Admin Menu
        default:
            cout << "[ERROR] Invalid choice. Please try again.\n";
            system("pause");
        }
    }
}

void ProductManagement::ViewOrder(MYSQL* conn) {
    system("cls");
    DisplayHeader("View Orders");

    // Query to calculate the total price for each order
    string query =
        "SELECT o.orderid, c.username, o.order_date, o.status, "
        "SUM(oi.quantity * oi.unit_price) AS total_price "
        "FROM orders o "
        "INNER JOIN order_items oi ON o.orderid = oi.orderid "
        "INNER JOIN customer c ON o.customerid = c.customerid "
        "GROUP BY o.orderid, c.username, o.order_date, o.status";

    if (mysql_query(conn, query.c_str())) {
        cerr << "[ERROR] Failed to retrieve orders: " << mysql_error(conn) << endl;
        system("pause");
        return;
    }

    MYSQL_RES* res = mysql_store_result(conn);
    if (mysql_num_rows(res) == 0) {
        cout << "[INFO] No orders available to view.\n";
        mysql_free_result(res);
        system("pause");
        return;
    }

    // Display the orders in a formatted table
    cout << left << setw(10) << "OrderID"
        << setw(20) << "Customer Name"
        << setw(20) << "Order Date"
        << setw(15) << "Status"
        << setw(15) << "Total Price" << endl;
    cout << string(75, '-') << endl;

    MYSQL_ROW row;
    while ((row = mysql_fetch_row(res)) != nullptr) {
        cout << left << setw(10) << row[0]  // orderid
            << setw(20) << row[1]  // customer_name
            << setw(20) << row[2]  // order_date
            << setw(15) << row[3]  // order_status
            << setw(15) << row[4] << endl;  // total_price
    }
    mysql_free_result(res);

    system("pause");
}


void ProductManagement::UpdateOrderStatus(MYSQL* conn) {
    system("cls");
    DisplayHeader("Update Order Status");

    // Query to retrieve orders
    string query = "SELECT o.orderid, c.username, o.order_date, o.status "
        "FROM orders o "
        "INNER JOIN customer c ON o.customerid = c.customerid";

    if (mysql_query(conn, query.c_str())) {
        cerr << "[ERROR] Failed to retrieve orders: " << mysql_error(conn) << endl;
        system("pause");
        return;
    }

    MYSQL_RES* res = mysql_store_result(conn);
    if (mysql_num_rows(res) == 0) {
        cout << "[INFO] No orders available to update.\n";
        mysql_free_result(res);
        system("pause");
        return;
    }

    // Display the orders in a formatted table
    cout << left << setw(10) << "OrderID"
        << setw(15) << "Customer Name"
        << setw(20) << "Order Date"
        << setw(15) << "Status" << endl;
    cout << string(60, '-') << endl;

    MYSQL_ROW row;
    while ((row = mysql_fetch_row(res)) != nullptr) {
        cout << left << setw(10) << row[0]  // orderid
            << setw(15) << row[1]  // customer_name
            << setw(20) << row[2]  // order_date
            << setw(15) << row[3] << endl;  // order_status
    }
    mysql_free_result(res);

    // Prompt admin to choose an order to update
    string orderid;
    cout << "\nEnter the Order ID to update: ";
    cin >> orderid;

    // Validate orderid exists
    string validate_orderid_query = "SELECT COUNT(*) FROM orders WHERE orderid = '" + orderid + "'";
    if (mysql_query(conn, validate_orderid_query.c_str())) {
        cerr << "[ERROR] Failed to validate Order ID: " << mysql_error(conn) << endl;
        system("pause");
        return;
    }

    res = mysql_store_result(conn);
    row = mysql_fetch_row(res);
    int order_exists = atoi(row[0]);
    mysql_free_result(res);

    if (order_exists == 0) {
        cout << "[ERROR] Invalid Order ID. Please try again.\n";
        system("pause");
        return;
    }

    // Get current order status
    string current_status;
    query = "SELECT status FROM orders WHERE orderid = '" + orderid + "'";

    if (mysql_query(conn, query.c_str())) {
        cerr << "[ERROR] Failed to retrieve order status: " << mysql_error(conn) << endl;
        system("pause");
        return;
    }

    res = mysql_store_result(conn);
    row = mysql_fetch_row(res);
    current_status = row[0];
    mysql_free_result(res);

    // Display current order status
    cout << "Current Order Status: " << current_status << endl;

    string new_status;
    bool validStatus = false; // Flag to check for valid input

    // Define a set of valid order statuses
    set<string> validStatuses = { "shipped", "in transit", "delivered", "pending", "paid" };

    while (!validStatus) {
        cout << "Enter new order status (e.g., 'Shipped', 'In Transit', 'Delivered', 'Pending', 'Paid'): ";
        cin.ignore();
        getline(cin, new_status);

        // Convert to lowercase for case-insensitive comparison
        string lowerStatus = new_status;
        transform(lowerStatus.begin(), lowerStatus.end(), lowerStatus.begin(), ::tolower);

        // Check if the input is a valid status
        if (validStatuses.find(lowerStatus) != validStatuses.end()) {
            validStatus = true; // Input is valid
        }
        else {
            cout << "[ERROR] Invalid order status! Please choose from 'Shipped', 'In Transit', 'Delivered', 'Pending', or 'Paid'.\n";
        }
    }

    // Proceed with the valid order status
    cout << "\n[INFO] You selected: " << new_status << endl;


    // Update the order status
    string update_query = "UPDATE orders SET status = '" + new_status + "' WHERE orderid = '" + orderid + "'";

    if (mysql_query(conn, update_query.c_str())) {
        cerr << "[ERROR] Failed to update order status: " << mysql_error(conn) << endl;
        system("pause");
        return;
    }

    cout << "[INFO] Order status updated successfully!\n";

    system("cls");
    DisplayHeader("Update Order Status");
    // Display updated order status
    query = "SELECT o.orderid, c.username, o.order_date, o.status "
        "FROM orders o "
        "INNER JOIN customer c ON o.customerid = c.customerid";

    if (mysql_query(conn, query.c_str())) {
        cerr << "[ERROR] Failed to retrieve updated orders: " << mysql_error(conn) << endl;
        system("pause");
        return;
    }

    res = mysql_store_result(conn);
    if (mysql_num_rows(res) == 0) {
        cout << "[INFO] No orders available to display.\n";
        mysql_free_result(res);
    }
    else {
        cout << left << setw(10) << "OrderID"
            << setw(15) << "Customer Name"
            << setw(20) << "Order Date"
            << setw(15) << "Status" << endl;
        cout << string(60, '-') << endl;

        while ((row = mysql_fetch_row(res)) != nullptr) {
            cout << left << setw(10) << row[0]  // orderid
                << setw(15) << row[1]  // customer_name
                << setw(20) << row[2]  // order_date
                << setw(15) << row[3] << endl;  // order_status
        }
        mysql_free_result(res);
    }

    system("pause");
}

void ProductManagement::GenerateReportMenu(MYSQL* conn) {
    Report report; // Create a Report object
    int reportChoice;

    do {
        system("cls");
        DisplayHeader("Generate Reports");
        cout << "1. Monthly Sales Report\n";
        cout << "2. Gross Profit Report\n";
        cout << "3. Back to Admin Menu\n";
        cout << "Enter your choice: ";
        cin >> reportChoice;

        switch (reportChoice) {
        case 1:
            report.GenerateMonthlySalesReport(conn); // Generate Monthly Sales Report
            break;
        case 2:
            report.GenerateGrossProfitReport(conn);
            break;
        case 3:
            system("cls");
            DisplayHeader("Admin Page");
            system("pause");
            return;
        default:
            cout << "[ERROR] Invalid choice. Please try again.\n";
            system("pause");
        }
    } while (true);
}


// Customer Functions
void ProductManagement::ViewProducts(MYSQL* conn) {
    DisplayHeader("View Products");

    MYSQL_RES* res;
    MYSQL_ROW row;
    int gender_choice, category_choice;
    string selected_gender;

    // Define a maximum number of genders (adjust this based on your needs)
    const int MAX_GENDERS = 10;
    string genders[MAX_GENDERS];
    int gender_count = 0;

    while (true) { // Outer loop for gender selection
        // Step 1: Gender Selection
        string gender_query = "SELECT DISTINCT gender FROM category";
        if (mysql_query(conn, gender_query.c_str())) {
            cerr << "[ERROR] Failed to retrieve genders: " << mysql_error(conn) << endl;
            system("pause");
            return;
        }

        res = mysql_store_result(conn);

        if (!res || mysql_num_rows(res) == 0) {
            cerr << "[INFO] No gender options found in the database.\n";
            mysql_free_result(res);
            system("pause");
            return;
        }

        // Reset gender count and array for each new query result
        gender_count = 0;

        system("cls");
        DisplayHeader("Select Gender");
        cout << "Choose a gender:\n";

        // Loop through the query results and store them in the array
        while ((row = mysql_fetch_row(res)) != nullptr && gender_count < MAX_GENDERS) {
            genders[gender_count] = row[0]; // Store gender in the array
            cout << gender_count + 1 << ". " << genders[gender_count] << endl;
            gender_count++;
        }
        cout << gender_count + 1 << ". Return to Main Menu\n";
        cout << "Enter your choice: ";
        cin >> gender_choice;

        if (gender_choice == gender_count + 1) {
            mysql_free_result(res);
            system("cls"); // Clear the screen before returning to the main menu
            DisplayHeader("Customer Page");
            return; // Exit to the main menu
        }

        if (gender_choice < 1 || gender_choice > gender_count) {
            cout << "[ERROR] Invalid choice. Try again.\n";
            system("pause");
            continue;
        }

        selected_gender = genders[gender_choice - 1];
        mysql_free_result(res);

        while (true) { // Inner loop for category selection
            // Step 2: Category Selection Based on Gender
            string category_query = "SELECT categoryid, category_name, description FROM category WHERE gender = '" + selected_gender + "'";
            if (mysql_query(conn, category_query.c_str())) {
                cerr << "[ERROR] Failed to retrieve categories: " << mysql_error(conn) << endl;
                system("pause");
                return;
            }

            res = mysql_store_result(conn);

            if (!res || mysql_num_rows(res) == 0) {
                cerr << "[INFO] No categories found for the selected gender.\n";
                mysql_free_result(res);
                system("pause");
                break; // Go back to the gender menu
            }

            vector<pair<string, string>> categories; // To store categoryid and categoryname
            int i = 1;

            system("cls");
            DisplayHeader("Select Category");
            cout << "Choose a category:\n";
            while ((row = mysql_fetch_row(res)) != nullptr) {
                cout << i << ". " << row[1] << " :\n " << row[2] << endl << endl;
                categories.emplace_back(row[0], row[1]);
                i++;
            }
            cout << i << ". Back to Gender Selection\n";
            cout << "Enter your choice: ";
            cin >> category_choice;

            if (category_choice == i) {
                mysql_free_result(res);
                break; // Go back to gender selection
            }

            if (category_choice < 1 || category_choice > i) {
                cout << "[ERROR] Invalid choice. Try again.\n";
                system("pause");
                continue;
            }

            string selected_category_id = categories[category_choice - 1].first;
            string selected_category_name = categories[category_choice - 1].second;
            mysql_free_result(res);

            // Step 3: Display Products in Selected Category
            string product_query = "SELECT ps.sizeid, p.product_name, p.description, ps.size, ps.stock, p.price "
                "FROM product p "
                "JOIN product_size ps ON p.productid = ps.productid "
                "WHERE p.categoryid = '" + selected_category_id + "' "
                "ORDER BY p.product_name, ps.size";

            if (mysql_query(conn, product_query.c_str())) {
                cerr << "[ERROR] Failed to retrieve products: " << mysql_error(conn) << endl;
                system("pause");
                return;
            }

            MYSQL_RES* product_res = mysql_store_result(conn);

            if (!product_res || mysql_num_rows(product_res) == 0) {
                cout << "[INFO] No products found in this category.\n";
                mysql_free_result(product_res);
                system("pause");
                continue;
            }

            system("cls");
            DisplayHeader("Products in " + selected_category_name);

            cout << left << setw(15) << "Product ID"
                << setw(30) << "Name"
                << setw(10) << "Size"
                << setw(10) << "Stock"
                << setw(10) << "Price"
                << setw(40) << "Description" << endl;
            cout << string(90, '-') << endl;

            while ((row = mysql_fetch_row(product_res)) != nullptr) {
                cout << left << setw(15) << row[0]        // Size ID but show as Product ID to become visible
                    << setw(30) << row[1]               // Product Name
                    << setw(10) << row[3]               // Size
                    << setw(10) << row[4]               // Stock
                    << setw(10) << row[5]               // Price
                    << setw(40) << row[2] << endl;      // Description
            }

            mysql_free_result(product_res);
            // Ask the customer if they want to add an item to the cart
            char add_choice;
            cout << "\nWould you like to add an item to your cart? (Y/N): ";
            cin >> add_choice;

            if (tolower(add_choice) == 'y') {
                AddToCart(conn); // Call the AddToCart function
            }
            else {
                cout << "[INFO] Returning to Category Selection...\n";
                system("pause");
            }
        }
    }
}



// AddToCart
void ProductManagement::AddToCart(MYSQL* conn) {
    string userId = UserManagement::getCurrentUserId(); // Retrieve the current user ID
    DisplayHeader("Add to Cart");

    // Step 1: Display all products with size information for customer reference
    string product_query = "SELECT ps.sizeid, p.product_name, p.description, ps.size, ps.stock, p.price "
        "FROM product_size ps "
        "JOIN product p ON ps.productid = p.productid "
        "ORDER BY p.product_name, ps.size";

    if (mysql_query(conn, product_query.c_str())) {
        cerr << "[ERROR] Failed to retrieve product details: " << mysql_error(conn) << endl;
        system("pause");
        return;
    }

    MYSQL_RES* res = mysql_store_result(conn);
    if (!res || mysql_num_rows(res) == 0) {
        cout << "[INFO] No products found in the database.\n";
        mysql_free_result(res);
        system("pause");
        return;
    }

    // Display product table
    system("cls");
    DisplayHeader("Available Products");

    cout << left << setw(15) << "Product ID" //Actually is size id
        << setw(30) << "Product Name"
        << setw(10) << "Size"
        << setw(10) << "Stock"
        << setw(10) << "Price"
        << setw(40) << "Description" << endl;
    cout << string(100, '-') << endl;

    MYSQL_ROW row;
    while ((row = mysql_fetch_row(res)) != nullptr) {
        cout << left << setw(15) << row[0]        // Size ID but show as Product ID to become visible
            << setw(30) << row[1]               // Product Name
            << setw(10) << row[3]               // Size
            << setw(10) << row[4]               // Stock
            << setw(10) << row[5]               // Price
            << setw(40) << row[2] << endl;      // Description
    }

    mysql_free_result(res);

    // Step 2: Prompt the customer to input the Size ID and quantity
    string sizeid;
    int quantity;

    cout << "\nEnter the Product ID of the product you wish to add to the cart (e.g., S001): ";
    cin >> sizeid;

    // Validate if the entered Size ID exists in the database
    string sizeid_query = "SELECT ps.sizeid, p.product_name, ps.size, ps.stock, p.price "
        "FROM product_size ps "
        "JOIN product p ON ps.productid = p.productid "
        "WHERE ps.sizeid = '" + sizeid + "'";
    if (mysql_query(conn, sizeid_query.c_str())) {
        cerr << "[ERROR] Failed to validate Product ID: " << mysql_error(conn) << endl;
        system("pause");
        return;
    }

    res = mysql_store_result(conn);
    if (!res || mysql_num_rows(res) == 0) {
        cout << "[INFO] Invalid Product ID. Please check and try again.\n";
        mysql_free_result(res);
        system("pause");
        system("cls");
        DisplayHeader("Available Products");
        return;
    }

    // Display product details for confirmation
    row = mysql_fetch_row(res);
    cout << "\nProduct Details:\n";
    cout << "Product ID: " << row[0] << endl;
    cout << "Product Name: " << row[1] << endl;
    cout << "Size: " << row[2] << endl;
    cout << "Stock Available: " << row[3] << endl;
    cout << "Price: " << row[4] << endl;

    int available_stock = stoi(row[3]); // Stock available for the selected size
    mysql_free_result(res);

    // Ask customer to input quantity
    cout << "Enter the quantity you wish to add to the cart: ";
    cin >> quantity;

    if (quantity <= 0) {
        cout << "[ERROR] Quantity must be greater than zero.\n";
        system("pause");
        return;
    }

    if (quantity > available_stock) {
        cout << "[ERROR] Insufficient stock. Available stock: " << available_stock << endl;
        system("pause");
        return;
    }

    // Step 3: Add the selected product to the cart table
    string add_cart_query = "INSERT INTO cart (customerid, sizeid, quantity) "
        "VALUES ('" + userId + "', '" + sizeid + "', " + to_string(quantity) + ")";

    if (mysql_query(conn, add_cart_query.c_str())) {
        cerr << "[ERROR] Failed to add item to cart: " << mysql_error(conn) << endl;
    }
    else {
        cout << "[INFO] Item added to cart successfully." << endl;
    }
    system("pause");
    system("cls");
    DisplayHeader("Available Products");
    return;
}

// ViewCart
void ProductManagement::ViewCart(MYSQL* conn) {

    string userId = UserManagement::getCurrentUserId(); // Retrieve the current user ID
    DisplayHeader("Your Cart");

    string cart_query = "SELECT c.sizeid, p.product_name, ps.size, c.quantity, p.price, (c.quantity * p.price) AS total_price "
        "FROM cart c "
        "JOIN product_size ps ON c.sizeid = ps.sizeid "
        "JOIN product p ON ps.productid = p.productid "
        "WHERE c.customerid = '" + userId + "'";


    if (mysql_query(conn, cart_query.c_str())) {
        cerr << "[ERROR] Failed to retrieve cart: " << mysql_error(conn) << endl;
        system("pause");
        return;
    }

    MYSQL_RES* res = mysql_store_result(conn);
    if (!res || mysql_num_rows(res) == 0) {
        cout << "[INFO] Your cart is empty.\n";
        mysql_free_result(res);
        system("pause");
        system("cls");
        DisplayHeader("Your Cart");
        return;
    }

    MYSQL_ROW row;
    double grand_total = 0.0;

    cout << left << setw(15) << "Product ID"
        << setw(40) << "Product Name"
        << setw(10) << "Size"
        << setw(10) << "Quantity"
        << setw(10) << "Price"
        << setw(15) << "Total Price" << endl;
    cout << string(100, '-') << endl;

    while ((row = mysql_fetch_row(res)) != nullptr) {
        cout << left << setw(15) << row[0]        // Size ID
            << setw(40) << row[1]               // Product Name
            << setw(10) << row[2]               // Size
            << setw(10) << row[3]               // Quantity
            << setw(10) << row[4]               // Price
            << setw(15) << row[5] << endl;      // Total Price

        grand_total += atof(row[5]);
    }

    cout << "\nGrand Total: RM" << fixed << setprecision(2) << grand_total << endl;

    mysql_free_result(res);
    // Now display the options for the next actions (Update, Delete, Checkout)
    char action_choice;
    cout << "\nSelect an action:\n";
    cout << "1. Update Cart\n";
    cout << "2. Delete Item from Cart\n";
    cout << "3. Checkout\n";
    cout << "4. Return to Main Menu\n";
    cout << "Enter your choice (1/2/3/4): ";
    cin >> action_choice;

    // Handle the user's choice
    switch (action_choice) {
    case '1':
        UpdateCart(conn); // Call updateCart function
        break;
    case '2':
        DeleteFromCart(conn); // Call deleteFromCart function
        break;
    case '3':
        Checkout(conn); // Call checkout function
        break;
    case '4':
        system("cls");  // Clear the screen (optional)
        DisplayHeader("Customer Page");
        break;
    default:
        cout << "[ERROR] Invalid choice. Returning to Main Menu.\n";
        system("pause");
        system("cls");
        DisplayHeader("Customer Page");
        break;
    }
}


// UpdateCart
void ProductManagement::UpdateCart(MYSQL* conn) {
    string userId = UserManagement::getCurrentUserId(); // Retrieve the current user ID
    string cart_query = "SELECT c.sizeid, p.product_name, ps.size, c.quantity, p.price, (c.quantity * p.price) AS total_price "
        "FROM cart c "
        "JOIN product_size ps ON c.sizeid = ps.sizeid "
        "JOIN product p ON ps.productid = p.productid "
        "WHERE c.customerid = '" + userId + "'";

    if (mysql_query(conn, cart_query.c_str())) {
        cerr << "[ERROR] Failed to retrieve cart: " << mysql_error(conn) << endl;
        system("pause");
        return;
    }

    MYSQL_RES* res = mysql_store_result(conn);
    if (!res || mysql_num_rows(res) == 0) {
        cout << "[INFO] Your cart is empty.\n";
        mysql_free_result(res);
        system("pause");
        return;
    }

    system("cls");
    DisplayHeader("Update Cart");

    // Store valid sizeid values
    vector<string> validSizeIds;

    // Display the cart table
    MYSQL_ROW row;
    cout << left << setw(15) << "Product ID"
        << setw(40) << "Product Name"
        << setw(10) << "Size"
        << setw(10) << "Quantity"
        << setw(10) << "Price"
        << setw(15) << "Total Price" << endl;
    cout << string(100, '-') << endl;

    while ((row = mysql_fetch_row(res)) != nullptr) {
        // Store the sizeid to validate input later
        validSizeIds.push_back(row[0]);

        cout << left << setw(15) << row[0]
            << setw(40) << row[1]
            << setw(10) << row[2]
            << setw(10) << row[3]
            << setw(10) << row[4]
            << setw(15) << row[5] << endl;
    }

    mysql_free_result(res);

    // Prompt the user to update the cart with input validation
    string sizeid;
    int new_quantity;

    bool valid_input = false;
    while (!valid_input) {
        cout << "\nEnter the Product ID to update the quantity: ";
        cin >> sizeid;

        // Check if the entered sizeid is valid
        if (find(validSizeIds.begin(), validSizeIds.end(), sizeid) != validSizeIds.end()) {
            valid_input = true;
        }
        else {
            cout << "[ERROR] Invalid Product ID entered. Please choose a valid ID from the cart.\n";
        }
    }

    cout << "Enter the new quantity: ";
    cin >> new_quantity;

    // Update query with the sizeid as a string (wrap it in single quotes for SQL)
    string update_query = "UPDATE cart SET quantity = " + to_string(new_quantity) +
        " WHERE sizeid = '" + sizeid + "' AND customerid = '" + userId + "'";

    if (mysql_query(conn, update_query.c_str())) {
        cerr << "[ERROR] Failed to update cart: " << mysql_error(conn) << endl;
        system("pause");
        return;
    }

    cout << "[INFO] Cart updated successfully.\n";

    // After updating, refresh the cart display
    system("pause");
    ViewCart(conn); // Re-display the cart with updated information
}


// DeleteFromCart
void ProductManagement::DeleteFromCart(MYSQL* conn) {
    string userId = UserManagement::getCurrentUserId(); // Retrieve the current user ID
    string cart_query = "SELECT c.sizeid, p.product_name, ps.size, c.quantity, p.price, (c.quantity * p.price) AS total_price "
        "FROM cart c "
        "JOIN product_size ps ON c.sizeid = ps.sizeid "
        "JOIN product p ON ps.productid = p.productid "
        "WHERE c.customerid = '" + userId + "'";

    if (mysql_query(conn, cart_query.c_str())) {
        cerr << "[ERROR] Failed to retrieve cart: " << mysql_error(conn) << endl;
        system("pause");
        return;
    }

    MYSQL_RES* res = mysql_store_result(conn);
    if (!res || mysql_num_rows(res) == 0) {
        cout << "[INFO] Your cart is empty.\n";
        mysql_free_result(res);
        system("pause");
        return;
    }

    // Store valid sizeid values
    vector<string> validSizeIds;

    system("cls");
    DisplayHeader("Delete Cart");
    // Display the cart table
    MYSQL_ROW row;
    cout << left << setw(15) << "Product ID"
        << setw(40) << "Product Name"
        << setw(10) << "Size"
        << setw(10) << "Quantity"
        << setw(10) << "Price"
        << setw(15) << "Total Price" << endl;
    cout << string(100, '-') << endl;

    while ((row = mysql_fetch_row(res)) != nullptr) {
        // Store the sizeid to validate input later
        validSizeIds.push_back(row[0]);

        cout << left << setw(15) << row[0]
            << setw(40) << row[1]
            << setw(10) << row[2]
            << setw(10) << row[3]
            << setw(10) << row[4]
            << setw(15) << row[5] << endl;
    }

    mysql_free_result(res);

    // Prompt the user to delete an item from the cart with input validation
    string sizeid;
    bool valid_input = false;
    while (!valid_input) {
        cout << "\nEnter the Product ID to delete from the cart: ";
        cin >> sizeid;

        // Check if the entered sizeid is valid
        if (find(validSizeIds.begin(), validSizeIds.end(), sizeid) != validSizeIds.end()) {
            valid_input = true;
        }
        else {
            cout << "[ERROR] Invalid Product ID entered. Please choose a valid ID from the cart.\n";
        }
    }

    string delete_query = "DELETE FROM cart WHERE sizeid = '" + sizeid + "' AND customerid = '" + userId + "'";
    if (mysql_query(conn, delete_query.c_str())) {
        cerr << "[ERROR] Failed to delete item from cart: " << mysql_error(conn) << endl;
        system("pause");
        return;
    }

    cout << "[INFO] Item deleted successfully.\n";

    // After deleting, refresh the cart display
    system("pause");
    ViewCart(conn); // Re-display the cart with updated information
}



// Checkout
void ProductManagement::Checkout(MYSQL* conn) {
    string userId = UserManagement::getCurrentUserId();
    string cart_query = "SELECT c.sizeid, p.product_name, ps.size, c.quantity, p.price, (c.quantity * p.price) AS total_price, ps.stock "
        "FROM cart c "
        "JOIN product_size ps ON c.sizeid = ps.sizeid "
        "JOIN product p ON ps.productid = p.productid "
        "WHERE c.customerid = '" + userId + "'";

    if (mysql_query(conn, cart_query.c_str())) {
        cerr << "[ERROR] Failed to retrieve cart: " << mysql_error(conn) << endl;
        system("pause");
        return;
    }

    MYSQL_RES* res = mysql_store_result(conn);
    if (!res || mysql_num_rows(res) == 0) {
        cout << "[INFO] Your cart is empty. Cannot proceed to checkout.\n";
        mysql_free_result(res);
        system("pause");
        return;
    }

    system("cls");
    DisplayHeader("Checkout");

    MYSQL_ROW row;
    double grand_total = 0.0;
    vector<pair<string, int>> stock_updates; // To track stock updates (sizeid, new stock)
    vector<tuple<string, string, int, double>> order_items; // To track order items (sizeid, product_name, quantity, unit_price)

    cout << left << setw(15) << "Product ID"
        << setw(40) << "Product Name"
        << setw(10) << "Size"
        << setw(10) << "Quantity"
        << setw(10) << "Unit Price"
        << setw(15) << "Total Price" << endl;
    cout << string(100, '-') << endl;

    while ((row = mysql_fetch_row(res)) != nullptr) {
        string sizeid = row[0];
        string product_name = row[1];
        string size = row[2];
        int quantity = stoi(row[3]);
        double unit_price = stod(row[4]);
        double total_price = stod(row[5]);
        int stock = stoi(row[6]);

        cout << left << setw(15) << sizeid
            << setw(40) << product_name
            << setw(10) << size
            << setw(10) << quantity
            << setw(10) << unit_price
            << setw(15) << total_price << endl;

        grand_total += total_price;

        if (quantity > stock) {
            cout << "[ERROR] Insufficient stock for Product ID: " << sizeid << ". Available stock: " << stock << endl;
            mysql_free_result(res);
            system("pause");
            return;
        }

        stock_updates.emplace_back(sizeid, stock - quantity);
        order_items.emplace_back(sizeid, product_name, quantity, unit_price);
    }

    mysql_free_result(res);

    cout << "\nGrand Total Before Discounts: RM" << fixed << setprecision(2) << grand_total << endl;

    // Apply fixed discount (but don't update the grand total yet)
    double fixedDiscount = Discount::ApplyFixedDiscount(grand_total);

    // Apply tiered discount (but don't update the grand total yet)
    double tierDiscount = Discount::ApplyTieredDiscount(grand_total);

    Discount discount;  // Create an instance of the Discount class

    // Check for coupon code
    string couponCode;
    double couponDiscount = 0.0;

    cout << "Do you have a coupon code? (Y/N): ";
    char hasCoupon;
    cin >> hasCoupon;

    if (tolower(hasCoupon) == 'y') {
        bool validCoupon = false;

        while (!validCoupon) {
            cout << "Please enter your coupon code: ";
            cin >> couponCode;

            // Apply the coupon code and check if it is valid
            couponDiscount = discount.ApplyCouponCode(couponCode);  // Use the discount instance here

            if (couponDiscount > 0.0) {
                // If valid coupon, break out of the loop
                validCoupon = true;
            }
            else {
                // If invalid coupon, ask again
                cout << "Would you like to try another coupon code? (Y/N): ";
                char retry;
                cin >> retry;
                if (tolower(retry) != 'y') {
                    // If the user does not want to try again, skip coupon code
                    validCoupon = true;
                }
            }
        }
    }

    // Now calculate the final grand total after all discounts
    double finalTotalAfterDiscounts = grand_total - fixedDiscount - tierDiscount - couponDiscount;

    // Display final total and discounts
    cout << string(70, '-') << endl;
    cout << "\nSummary of Discounts:\n";
    cout << "  Fixed Discount: RM" << fixed << setprecision(2) << fixedDiscount << endl;
    cout << "  Tier Discount: RM" << fixed << setprecision(2) << tierDiscount << endl;
    cout << "  Coupon Discount: RM" << fixed << setprecision(2) << couponDiscount << endl;

    cout << "\nGrand Total After Discounts: RM" << fixed << setprecision(2) << finalTotalAfterDiscounts << endl;

    // Confirm checkout
    cout << "\nWould you like to confirm checkout? (Y/N): ";
    char confirm;
    cin >> confirm;
    confirm = tolower(confirm);

    if (confirm != 'y') {
        cout << "[INFO] Checkout cancelled.\n";
        system("pause");
        system("cls");
        DisplayHeader("Checkout");
        return;
    }

    // Insert the order before payment (grand_total before discounts)
    string insert_order_query = "INSERT INTO orders (customerid, order_date, total_amount, status) "
        "VALUES ('" + userId + "', NOW(), " + to_string(grand_total) + ", 'Pending')";
    if (mysql_query(conn, insert_order_query.c_str())) {
        cerr << "[ERROR] Failed to create order: " << mysql_error(conn) << endl;
        system("pause");
        return;
    }

    // Retrieve the generated order ID
    MYSQL_RES* order_res = mysql_store_result(conn);
    string orderid = "0";

    string order_query = "SELECT orderid FROM orders WHERE customerid = '" + userId + "' "
        "ORDER BY order_date DESC LIMIT 1";
    if (mysql_query(conn, order_query.c_str())) {
        cerr << "[ERROR] Unable to retrieve order ID: " << mysql_error(conn) << endl;
        system("pause");
        return;
    }

    order_res = mysql_store_result(conn);
    MYSQL_ROW order_row = mysql_fetch_row(order_res);
    if (order_row != nullptr) {
        orderid = order_row[0];
    }
    mysql_free_result(order_res);

    // Insert order items and update stock
    for (const auto& item : order_items) {
        string sizeid = get<0>(item);
        int quantity = get<2>(item);
        double unit_price = get<3>(item);

        string item_query = "INSERT INTO order_items (orderid, sizeid, quantity, unit_price) "
            "VALUES ('" + orderid + "', '" + sizeid + "', " + to_string(quantity) + ", " + to_string(unit_price) + ")";
        if (mysql_query(conn, item_query.c_str())) {
            cerr << "[ERROR] Failed to add order item: " << mysql_error(conn) << endl;
            system("pause");
            return;
        }

        string update_stock_query = "UPDATE product_size SET stock = " + to_string(stock_updates.back().second) +
            " WHERE sizeid = '" + sizeid + "'";
        if (mysql_query(conn, update_stock_query.c_str())) {
            cerr << "[ERROR] Failed to update stock: " << mysql_error(conn) << endl;
            system("pause");
            return;
        }
    }

    // Insert payment record with grand total after discounts
    string insert_payment_query = "INSERT INTO payment (orderid, customerid, amount, payment_date, payment_method) "
        "VALUES ('" + orderid + "', '" + userId + "', " + to_string(finalTotalAfterDiscounts) + ", NOW(), 'Credit Card')";
    if (mysql_query(conn, insert_payment_query.c_str())) {
        cerr << "[ERROR] Failed to create payment record: " << mysql_error(conn) << endl;
        system("pause");
        return;
    }

    // Clear the cart
    string clear_cart_query = "DELETE FROM cart WHERE customerid = '" + userId + "'";
    if (mysql_query(conn, clear_cart_query.c_str())) {
        cerr << "[ERROR] Failed to clear cart: " << mysql_error(conn) << endl;
        system("pause");
        return;
    }

    cout << "\n[INFO] Checkout completed successfully! Proceeding to payment...\n";
    system("pause");

    // Redirect to the Payment class
    Payment payment;
    payment.ProcessPayment(conn, userId, finalTotalAfterDiscounts, orderid);

    // Redirect back to the customer page after payment
    system("cls");
    DisplayHeader("Customer Page");
}



