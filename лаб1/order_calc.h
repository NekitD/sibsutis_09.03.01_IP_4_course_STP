#pragma once
#include <string>
#include <vector>

double applyDiscount(double total, bool isPremium);
double calcShipping(double total);
double finalPrice(double total, double discount, double shipping);
int countExpensiveItems(const std::vector<double>& prices, double threshold);

