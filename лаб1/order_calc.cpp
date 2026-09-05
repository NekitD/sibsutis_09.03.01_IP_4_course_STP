#include "order_calc.h"

// 1. Расчёт скидки
// Описание: возвращает сумму со скидкой в зависимости от стоимости заказа и статуса клиента.
// - Если total < 0, возвращает -1.0 (ошибка).
// - Если total >= 10000: скидка 20% для премиум-клиентов, 15% для обычных.
// - Если total >= 5000: скидка 10% для премиум-клиентов, 5% для обычных.
// - В остальных случаях скидка 0%.
double applyDiscount(double total, bool isPremium) 
{
  if (total < 0)
    return -1.0;
  if (total >= 10000) 
  {
    if (isPremium) 
    {
      return total * 0.80;
    } else 
    {
      return total * 0.85;
    }
  } else if (total >= 5000) 
  {
    if (isPremium) 
    {
      return total * 0.90;
    } else 
    {
      return total * 0.95;
    }
  }
  return total;
}

// 2. Расчёт стоимости доставки
// Описание: возвращает стоимость доставки в зависимости от суммы заказа.
// - Если total < 0, возвращает -1.0 (ошибка).
// - Если total >= 5000, доставка бесплатная (0.0).
// - Иначе доставка стоит 300.0.
double calcShipping(double total) {
  if (total < 0)
    return -1.0;
  if (total <= 5000)
    return 0.0;
  return 300.0;
}

// 3. Итоговая цена
// Описание: вычисляет финальную стоимость заказа с учётом скидки и доставки.
// - Если любой аргумент отрицательный, возвращает -1.0 (ошибка).
// - Если итоговая цена получается отрицательной, она обнуляется (0.0).
// - Результат округляется до двух знаков после запятой.
double finalPrice(double total, double discount, double shipping) 
{
  if (total < 0 || discount < 0 || shipping < 0)
    return -1.0;
  double result = total - discount + shipping;
  if (result < 0)
    result = 0.0;
  return std::round(result * 100) / 100;
}

// 4. Подсчёт количества дорогих товаров в корзине
// Описание: возвращает количество товаров, цена которых превышает заданный порог.
// - Если вектор пустой, возвращает 0.
// - В противном случае подсчитывает количество элементов > threshold.
int countExpensiveItems(const std::vector<double>& prices, double threshold) {
  int count = 0;
  for (size_t i = 0; i < prices.size(); ++i) {
    if (prices[i] > threshold) {
      count++;
    }
  }
  return count;
}

// Вариант 7
void rotatePrices(std::vector<double>& prices, int shift, int direction)
{
    if (shift <= 0 || prices.empty())
    {
        return;
    }

    shift = shift % prices.size();
    if (shift == 0)
    {
        return;
    }
    std::vector<double> newprices(prices.size());
    if (direction == 0)
    {
        for (int i = 0; i < prices.size(); i++)
        {
            newprices[(i + shift) % prices.size()] = prices[i];
        }
    }
    else if (direction == 1)
    {
        for (int i = 0; i < prices.size(); i++)
        {
            newprices[(i - shift + prices.size()) % prices.size()] = prices[i];
        }
    }
    prices = std::move(newprices);
}

int sumOddBelowMainDiagonal(const std::vector<std::vector<int>>& A)
{
    if (A.empty() || A[0].empty())
    {
        return 0;
    }
    int sum = 0;
    int rows = A.size();
    int cols = A[0].size();

    for (int i = 1; i < rows; i++)
    {
        for (int j = 0; j < i; j++)
        {
            if (A[i][j] % 2 != 0)
            {
                sum += A[i][j];
            }
        }
    }

    return sum;
}

// Вариант 8
double maxPriceEvenValueEvenIndex(const std::vector<int>& prices){
  std::vector<int>::const_iterator price;
  int max = 0;
  int id = 0;
  for(price = prices.begin(); price != prices.end(); price++){
    if((*price % 2 == 0) && (id % 2 == 0) && (*price > max)){
      max = *price;
    }
    id++;
  } 
  return max;
}

int sumEvenAboveSecondaryDiagonal(const std::vector<std::vector<int>>& A){
  std::vector< std::vector<int> >::const_iterator row;
  std::vector<int>::const_iterator col;
  int row_id = 0;
  int col_id = 0;
  int sum = 0;
  int size = A.size();
  for(row = A.begin(); row != A.end(); row++){
    col_id = 0;
    for(col = (*row).begin(); col != (*row).end(); col++){
      if((*col % 2 == 0) && (row_id + col_id < size - 1)){
        sum += *col;
      }
      col_id++;
    }
    row_id++;
  }
  return sum;
}
