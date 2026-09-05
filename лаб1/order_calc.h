#pragma once
#include <string>
#include <vector>
#include <cmath>

double applyDiscount(double total, bool isPremium);
double calcShipping(double total);
double finalPrice(double total, double discount, double shipping);
int countExpensiveItems(const std::vector<double>& prices, double threshold);

double maxPriceEvenValueEvenIndex(const std::vector<int>& prices);
int sumEvenAboveSecondaryDiagonal(const std::vector< std::vector<int> >& A);

