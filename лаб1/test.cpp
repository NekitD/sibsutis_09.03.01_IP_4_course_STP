#include <iostream>
#include "order_calc.h"

using namespace std;

int main()
{
    //C0 C1 C2
    cout << "applyDiscount(-1, true) = " << applyDiscount(-1, true) << endl;
    cout << "applyDiscount(0, true) = " << applyDiscount(0, true) << endl; 
    cout << "applyDiscount(10001, true) = " << applyDiscount(10001, true) << endl;
    cout << "applyDiscount(10001, false) = " << applyDiscount(10001, false) << endl;
    cout << "applyDiscount(6000, true) = " << applyDiscount(6000, true) << endl; 
    cout << "applyDiscount(6000, false) = " << applyDiscount(6000, false) << endl; 
    //C0 C1 C2
    cout << "calcShipping(-1) = " << calcShipping(-1) << endl;
    cout << "calcShipping(1) = " << calcShipping(1) << endl;
    cout << "calcShipping(6000) = " << calcShipping(6000) << endl;
    //C0 С1 C2
    cout << "finalPrice(-1, 1, 1) = " << finalPrice(-1, 1, 1) << endl;
    cout << "finalPrice(1, -1, 1) = " << finalPrice(1, -1, 1) << endl;
    cout << "finalPrice(1, 1, -1) = " << finalPrice(1, 1, -1) << endl;
    cout << "finalPrice(1, 1000, 1) = " << finalPrice(1000, 0, 0) << endl;
    cout << "finalPrice(1, 1000, 1) = " << finalPrice(1, 1000, 1) << endl;
    //C0
    cout << "countExpensiveItems({}, 10.0) = " << countExpensiveItems({}, 10.0);
    cout << "countExpensiveItems({15.0, 20.0}, 10.0) = " << countExpensiveItems({15.0, 20.0}, 10.0);
    cout << "countExpensiveItems({5.0, 3.0}, 10.0) = " << countExpensiveItems({5.0, 3.0}, 10.0);

    //C1
    cout << "countExpensiveItems({5.0, 15.0}, 10.0) = " << countExpensiveItems({5.0, 15.0}, 10.0);

    //C2
    cout << "countExpensiveItems({5.0, 10.0, 15.0}, 15.0) = " << countExpensiveItems({5.0, 10.0, 15.0}, 15.0);
    cout << "countExpensiveItems({5.0, 10.0, 15.0}, 10.0) = " << countExpensiveItems({5.0, 10.0, 15.0}, 10.0);

    //=====Вариант 8==============================
    // function 8: C0 C1 C2
    cout << "maxPriceEvenValueEvenIndex({}) = " << maxPriceEvenValueEvenIndex({}) << endl;
    cout << "maxPriceEvenValueEvenIndex({2, 4, 5, 6, NULL, 7, 2}) = " << maxPriceEvenValueEvenIndex({2, 4, 5, 6, NULL, 7, 2}) << endl;

    // function 3: C0 C1 C2
    cout << "sumEvenAboveSecondaryDiagonal({}) = " << sumEvenAboveSecondaryDiagonal({}) << endl;
    cout << "sumEvenAboveSecondaryDiagonal({{1, 2, 3}, {4, 5, 6}, {7, 8, 9}}) = " << sumEvenAboveSecondaryDiagonal({{1, 2, 3}, {4, 5, 6}, {7, 8, 9}}) << endl;
}


