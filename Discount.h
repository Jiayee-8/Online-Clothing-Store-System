#ifndef DISCOUNT_H
#define DISCOUNT_H

#include <string>

using namespace std;

class Discount {
public:
    // Fixed discount amount
    static const double fixedDiscountAmount;

    // Tiered discount thresholds and percentages
    static const double tier1Threshold;
    static const double tier2Threshold;
    static const double tier1Discount;
    static const double tier2Discount;

    // Validate and calculate the coupon discount
    static double ApplyCouponCode(const string& couponCode);

    // Apply the fixed discount
    static double ApplyFixedDiscount(double& total);

    // Apply tiered discount
    static double ApplyTieredDiscount(double& total);
};

#endif // DISCOUNT_H
