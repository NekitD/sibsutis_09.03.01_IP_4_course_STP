#include <iostream>
#include "order_calc.h"
#include <gtest/gtest.h>

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
    cout << endl;

    cout << "========================================" << endl;
    cout << "C1 check for countExpensiveItems:" << endl;
    cout << "========================================" << endl;
    cout << "countExpensiveItems({5.0, 15.0}, 10.0) = " << countExpensiveItems({5.0, 15.0}, 10.0) << endl;
    cout << endl;

    cout << "========================================" << endl;
    cout << "C2 check for countExpensiveItems:" << endl;
    cout << "========================================" << endl;
    cout << "countExpensiveItems({5.0, 10.0, 15.0}, 15.0) = " << countExpensiveItems({5.0, 10.0, 15.0}, 15.0) << endl;
    cout << "countExpensiveItems({5.0, 10.0, 15.0}, 10.0) = " << countExpensiveItems({5.0, 10.0, 15.0}, 10.0) << endl;
    cout << endl;
    //=====Вариант 8==============================
    // function 8: C0 C1 C2
    cout << "========================================" << endl;
    cout << "C0 C1 C2 check for maxPriceEvenValueEvenIndex:" << endl;
    cout << "========================================" << endl;
    cout << "maxPriceEvenValueEvenIndex({}) = " << maxPriceEvenValueEvenIndex({}) << endl;
    cout << "maxPriceEvenValueEvenIndex({2, 4, 5, 6, 8, 9, NULL, 7, 2}) = " << maxPriceEvenValueEvenIndex({2, 4, 5, 6, 8, 9, NULL, 7, 2}) << endl;
    cout << endl;
    // function 3: C0 C1 C2
    cout << "========================================" << endl;
    cout << "C0 C1 C2 check for sumEvenAboveSecondaryDiagonal:" << endl;
    cout << "========================================" << endl;
    cout << "sumEvenAboveSecondaryDiagonal({}) = " << sumEvenAboveSecondaryDiagonal({}) << endl;
    cout << "sumEvenAboveSecondaryDiagonal({{1, 2, 3}, {4, 5, 6}, {7, 8, 9}}) = " << sumEvenAboveSecondaryDiagonal({{1, 2, 3}, {4, 5, 6}, {7, 8, 9}}) << endl;
    cout << endl;
}


TEST(ApplyDiscountTest, AllC) 
{
    EXPECT_DOUBLE_EQ(-1.0, applyDiscount(-1, true));
    EXPECT_DOUBLE_EQ(0.0, applyDiscount(0, true));
    EXPECT_DOUBLE_EQ(10001 * 0.80, applyDiscount(10001, true));
    EXPECT_DOUBLE_EQ(10001 * 0.85, applyDiscount(10001, false));
    EXPECT_DOUBLE_EQ(6000 * 0.90, applyDiscount(6000, true));
    EXPECT_DOUBLE_EQ(6000 * 0.95, applyDiscount(6000, false));
}

TEST(CalcShippingTest, AllC)
{
    EXPECT_DOUBLE_EQ(-1.0, calcShipping(-1));
    EXPECT_DOUBLE_EQ(300.0, calcShipping(1));
    EXPECT_DOUBLE_EQ(0.0, calcShipping(6000));
}

TEST(FinalPriceTest, AllC)
{
    EXPECT_DOUBLE_EQ(-1.0, finalPrice(-1, 1, 1));
    EXPECT_DOUBLE_EQ(-1.0, finalPrice(1, -1, 1));
    EXPECT_DOUBLE_EQ(-1.0, finalPrice(1, 1, -1));
    EXPECT_DOUBLE_EQ(1000.0, finalPrice(1000, 0, 0));
    EXPECT_DOUBLE_EQ(0.0, finalPrice(1, 1000, 1));
}

TEST(CountExpensiveItemsTest, C0) 
{
    EXPECT_EQ(0, countExpensiveItems({}, 10.0));
    EXPECT_EQ(2, countExpensiveItems({ 15.0, 20.0 }, 10.0));
    EXPECT_EQ(0, countExpensiveItems({ 5.0, 3.0 }, 10.0));
}

TEST(CountExpensiveItemsTest, C1)
{
    EXPECT_EQ(1, countExpensiveItems({ 5.0, 15.0 }, 10.0));
}

TEST(CountExpensiveItemsTest, C2)
{
    EXPECT_EQ(1, countExpensiveItems({ 5.0, 10.0, 15.0 }, 10.0));
    EXPECT_EQ(0, countExpensiveItems({ 5.0, 10.0, 15.0 }, 15.0));
}


//-----------------------------------------------------------


//C0
TEST(RotatePricesTest, C0)
{
    std::vector<double> empty;
    rotatePrices(empty, 2, 0);
    EXPECT_TRUE(empty.empty());

    //shift <= 0
    std::vector<double> prices1 = { 1.0, 2.0, 3.0 };
    rotatePrices(prices1, 0, 0);
    EXPECT_DOUBLE_EQ(1.0, prices1[0]);
    EXPECT_DOUBLE_EQ(2.0, prices1[1]);
    EXPECT_DOUBLE_EQ(3.0, prices1[2]);

    //if (shift == 0)
    std::vector<double> prices2 = { 1.0, 2.0, 3.0 };
    rotatePrices(prices2, 3, 0);
    EXPECT_DOUBLE_EQ(1.0, prices2[0]);
    EXPECT_DOUBLE_EQ(2.0, prices2[1]);
    EXPECT_DOUBLE_EQ(3.0, prices2[2]);

    //if (direction = 0)
    std::vector<double> prices3 = { 1.0, 2.0, 3.0, 4.0, 5.0 };
    rotatePrices(prices3, 2, 0);
    EXPECT_DOUBLE_EQ(4.0, prices3[0]);
    EXPECT_DOUBLE_EQ(5.0, prices3[1]);
    EXPECT_DOUBLE_EQ(1.0, prices3[2]);
    EXPECT_DOUBLE_EQ(2.0, prices3[3]);
    EXPECT_DOUBLE_EQ(3.0, prices3[4]);

    //if (direction = 1)
    std::vector<double> prices4 = { 1.0, 2.0, 3.0, 4.0, 5.0 };
    rotatePrices(prices4, 2, 1);
    EXPECT_DOUBLE_EQ(3.0, prices4[0]);
    EXPECT_DOUBLE_EQ(4.0, prices4[1]);
    EXPECT_DOUBLE_EQ(5.0, prices4[2]);
    EXPECT_DOUBLE_EQ(1.0, prices4[3]);
    EXPECT_DOUBLE_EQ(2.0, prices4[4]);
}

TEST(RotatePricesTest, C1)
{
    std::vector<double> prices = { 1.0, 2.0, 3.0 };
    rotatePrices(prices, 1, 2); 
    EXPECT_DOUBLE_EQ(0.0, prices[0]);
    EXPECT_DOUBLE_EQ(0.0, prices[1]);
    EXPECT_DOUBLE_EQ(0.0, prices[2]);
}

//C2
TEST(RotatePricesTest, C2)
{
    std::vector<double> prices = { 37.0 };
    rotatePrices(prices, 5, 0);
    EXPECT_DOUBLE_EQ(37.0, prices[0]);

    rotatePrices(prices, 5, 1);
    EXPECT_DOUBLE_EQ(37.0, prices[0]);
}


// C0
TEST(SumOddBelowMainDiagonalTests, C0)
{
    std::vector<std::vector<int>> empty;
    EXPECT_EQ(0, sumOddBelowMainDiagonal(empty));

    std::vector<std::vector<int>> emptyRow = { {} };
    EXPECT_EQ(0, sumOddBelowMainDiagonal(emptyRow));

    std::vector<std::vector<int>> A = {{1, 2, 3}, {4, 5, 6},{7, 1, 9}};
    EXPECT_EQ(8, sumOddBelowMainDiagonal(A));

    std::vector<std::vector<int>> B = {{1, 2, 3},{2, 5, 6},{4, 6, 9}};
    EXPECT_EQ(0, sumOddBelowMainDiagonal(B));
}

//C1
TEST(SumOddBelowMainDiagonalTest, C1)
{
    std::vector<std::vector<int>> A = {{5}};
    EXPECT_EQ(0, sumOddBelowMainDiagonal(A));

    std::vector<std::vector<int>> B = {{1, 2},{3, 4}};
    EXPECT_EQ(3, sumOddBelowMainDiagonal(B));
}

//C2
TEST(SumOddBelowMainDiagonalTest, C2)
{
    std::vector<std::vector<int>> A = {{1, 2, 3},{4, 5, 6}};
    EXPECT_EQ(0, sumOddBelowMainDiagonal(A));

    std::vector<std::vector<int>> B = {{1, 2},{3, 4},{5, 6}};
    EXPECT_EQ(8, sumOddBelowMainDiagonal(B));
}


TEST(Var8Func1Test, emptyTest){
    EXPECT_EQ(maxPriceEvenValueEvenIndex({}), 0);
}

TEST(Var8Func1Test, arrayTest){
    EXPECT_EQ(maxPriceEvenValueEvenIndex({2, 4, 5, 6, 8, 9, NULL, 7, 2}), 8);
    // 2 - первое, второе и третье условие выполняется / Первая ветка
    // 4 - первое, третье условие выполняется, второе - нет. / Вторая ветка
    // 5 - второе, третье условие выполняется, первое - нет / Вторая ветка
    // 6- первое, третье выполняются, второе - нет. / Вторая ветка
    // 8 - первое, второе и третье условие выполняется / Первая ветка
    // 9 - третье выполняется, третье, второе - нет / Вторая ветка
    // NULL - третье не выполняется / Вторая ветка
    // 7 - третье выполняется, первое, второе - нет / Вторая ветка
    // 2 - первое, второе выполняется, третье - нет / Вторая ветка
}

TEST(Var8Func2Test, emptyTest){
    EXPECT_EQ(sumEvenAboveSecondaryDiagonal({}), 0);
}

TEST(Var8Func2Test, arrayTest){
    EXPECT_EQ(sumEvenAboveSecondaryDiagonal({{1, 2, 3}, {4, 5, 6}, {7, 8, 9}}), 6);
    // 1 - Второе условие выполняется, первое - нет / Вторая ветка
    // 2 - Первое и второе условия выполняются / Первая ветка
    // 3 - Второе условие выполняется, первое - нет / Вторая ветка
    // 4 - Первое условие выполняется, второе нет / Вторая ветка
    // 5 - Первое и второе условия не выполняются / Вторая ветка
    // 6 - Первое и второе условие выполняются / Первая ветка
    // 7 - Первое и второе условия не выполняются / Вторая ветка
    // 8 - Первое условие выполняется, второе - нет / Вторая ветка
    // 9 - Первое и второе условие не выполняются. / Вторая ветка
}


