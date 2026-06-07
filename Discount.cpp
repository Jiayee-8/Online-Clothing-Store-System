#include "Discount.h"
#include <iostream>
#include <iomanip>
#include <algorithm> // For transform

using namespace std;

// Initialize static constants
const double Discount::fixedDiscountAmount = 10.0;
const double Discount::tier1Threshold = 100.0;
const double Discount::tier2Threshold = 200.0;
const double Discount::tier1Discount = 0.05;
const double Discount::tier2Discount = 0.10;

// Calculate and return the fixed discount without modifying the total
double Discount::ApplyFixedDiscount(double& total) {
    if (total > 0.0) { // Apply only if the total is greater than 0
        cout << "[INFO] A fixed discount of RM" << fixedDiscountAmount << " is available.\n";
        return fixedDiscountAmount;
    }
    return 0.0;
}

// Calculate and return the tiered discount without modifying the total
double Discount::ApplyTieredDiscount(double& total) {
    double discount = 0.0;

    // Determine the applicable tiered discount
    if (total > tier2Threshold) {
        discount = total * tier2Discount; // 10% discount for orders above RM200
        cout << "[INFO] A tiered discount of 10% (RM" << fixed << setprecision(2) << discount
            << ") is available for spending over RM200.00.\n";
    }
    else if (total > tier1Threshold) {
        discount = total * tier1Discount; // 5% discount for orders above RM100
        cout << "[INFO] A tiered discount of 5% (RM" << fixed << setprecision(2) << discount
            << ") is available for spending over RM100.00.\n";
    }

    return discount;
}

// Validate coupon code and return the discount amount
double Discount::ApplyCouponCode(const string& couponCode) {
    const double validCouponDiscount = 15.0; // Discount for valid coupon
    string validCoupon = "SAVE15"; // Example valid coupon code
    string code = couponCode;

    // Convert the input coupon code to uppercase for case-insensitive comparison
    transform(code.begin(), code.end(), code.begin(), ::toupper);

    if (code == validCoupon) {
        cout << "[INFO] Coupon code applied! You saved RM" << validCouponDiscount << ".\n";
        return validCouponDiscount;
    }
    else {
        cout << "[INFO] Invalid coupon code. No discount applied.\n";
        return 0.0;
    }
}
