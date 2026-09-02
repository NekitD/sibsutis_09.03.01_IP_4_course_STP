#include <iostream>
#include "order_calc.h"

using namespace std;

int main()
{
    // C0 C1
    // applyDiscount
    cout << "applyDiscount(-1, true) = " << applyDiscount(-1, true) << endl;
    cout << "applyDiscount(0, true) = " << applyDiscount(0, true) << endl; 
    cout << "applyDiscount(10001, true) = " << applyDiscount(10001, true) << endl;
    cout << "applyDiscount(10001, false) = " << applyDiscount(10001, false) << endl;
    cout << "applyDiscount(6000, true) = " << applyDiscount(6000, true) << endl; 
    cout << "applyDiscount(6000, false) = " << applyDiscount(6000, false) << endl; 

    // calcShipping
    cout << "calcShipping(-1) = " << calcShipping(-1) << endl;
    cout << "calcShipping(1) = " << calcShipping(1) << endl;
    cout << "calcShipping(6000) = " << calcShipping(6000) << endl;

    // finalPrice
    cout << "finalPrice(-1, 1, 1) = " << finalPrice(-1, 1, 1) << endl;
    cout << "finalPrice(1, -1, 1) = " << finalPrice(1, -1, 1) << endl;
    cout << "finalPrice(1, 1, -1) = " << finalPrice(1, 1, -1) << endl;
    cout << "finalPrice(1, 1000, 1) = " << finalPrice(1000, 0, 0) << endl;
    cout << "finalPrice(1, 1000, 1) = " << finalPrice(1, 1000, 1) << endl;
}