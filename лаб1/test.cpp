#include <iostream>
#include "order_calc.h"

using namespace std;

int main()
{
    cout << "========================================" << endl;
    cout << "C0 C1 C2 check for applyDiscount:" << endl;
    cout << "========================================" << endl;
    cout << "applyDiscount(-1, true) = " << applyDiscount(-1, true) << endl;
    cout << "applyDiscount(0, true) = " << applyDiscount(0, true) << endl; 
    cout << "applyDiscount(10001, true) = " << applyDiscount(10001, true) << endl;
    cout << "applyDiscount(10001, false) = " << applyDiscount(10001, false) << endl;
    cout << "applyDiscount(6000, true) = " << applyDiscount(6000, true) << endl; 
    cout << "applyDiscount(6000, false) = " << applyDiscount(6000, false) << endl; 
    cout << endl;
    
    cout << "========================================" << endl;
    cout << "C0 C1 C2 check for calcShipping:" << endl;
    cout << "========================================" << endl;
    cout << "calcShipping(-1) = " << calcShipping(-1) << endl;
    cout << "calcShipping(1) = " << calcShipping(1) << endl;
    cout << "calcShipping(6000) = " << calcShipping(6000) << endl;
    cout << endl;

    cout << "========================================" << endl;
    cout << "C0 C1 C2 check for finalPrice:" << endl;
    cout << "========================================" << endl;
    cout << "finalPrice(-1, 1, 1) = " << finalPrice(-1, 1, 1) << endl;
    cout << "finalPrice(1, -1, 1) = " << finalPrice(1, -1, 1) << endl;
    cout << "finalPrice(1, 1, -1) = " << finalPrice(1, 1, -1) << endl;
    cout << "finalPrice(1, 1000, 1) = " << finalPrice(1000, 0, 0) << endl;
    cout << "finalPrice(1, 1000, 1) = " << finalPrice(1, 1000, 1) << endl;
    cout << endl;

    cout << "========================================" << endl;
    cout << "C0 check for countExpensiveItems:" << endl;
    cout << "========================================" << endl;
    cout << "countExpensiveItems({}, 10.0) = " << countExpensiveItems({}, 10.0) << endl;
    cout << "countExpensiveItems({15.0, 20.0}, 10.0) = " << countExpensiveItems({15.0, 20.0}, 10.0) << endl;
    cout << "countExpensiveItems({5.0, 3.0}, 10.0) = " << countExpensiveItems({5.0, 3.0}, 10.0) << endl;

    cout << "========================================" << endl;
    cout << "C1 check for countExpensiveItems:" << endl;
    cout << "========================================" << endl;
    cout << "countExpensiveItems({5.0, 15.0}, 10.0) = " << countExpensiveItems({5.0, 15.0}, 10.0) << endl;

    cout << "========================================" << endl;
    cout << "C2 check for countExpensiveItems:" << endl;
    cout << "========================================" << endl;
    cout << "countExpensiveItems({5.0, 10.0, 15.0}, 15.0) = " << countExpensiveItems({5.0, 10.0, 15.0}, 15.0) << endl;
    cout << "countExpensiveItems({5.0, 10.0, 15.0}, 10.0) = " << countExpensiveItems({5.0, 10.0, 15.0}, 10.0) << endl;

    //=====Вариант 8==============================
    // function 8: C0 C1 C2
    cout << "========================================" << endl;
    cout << "C0 C1 C2 check for maxPriceEvenValueEvenIndex:" << endl;
    cout << "========================================" << endl;
    cout << "maxPriceEvenValueEvenIndex({}) = " << maxPriceEvenValueEvenIndex({}) << endl;
    cout << "maxPriceEvenValueEvenIndex({2, 4, 5, 6, NULL, 7, 2}) = " << maxPriceEvenValueEvenIndex({2, 4, 5, 6, NULL, 7, 2}) << endl;

    // function 3: C0 C1 C2
    cout << "========================================" << endl;
    cout << "C0 C1 C2 check for sumEvenAboveSecondaryDiagonal:" << endl;
    cout << "========================================" << endl;
    cout << "sumEvenAboveSecondaryDiagonal({}) = " << sumEvenAboveSecondaryDiagonal({}) << endl;
    cout << "sumEvenAboveSecondaryDiagonal({{1, 2, 3}, {4, 5, 6}, {7, 8, 9}}) = " << sumEvenAboveSecondaryDiagonal({{1, 2, 3}, {4, 5, 6}, {7, 8, 9}}) << endl;
}


