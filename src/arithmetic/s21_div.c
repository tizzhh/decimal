#include "../s21_decimal.h"

int s21_div(s21_decimal value_1, s21_decimal value_2, s21_decimal *result) {
  int error = 0;
  if (value_2.bits[0] == 0 && value_2.bits[1] == 0 && value_2.bits[2] == 0 &&
      value_2.bits[3] == 0) {
    error = 3;
  } else if (s21_is_equal(value_1, value_2)) {
    s21_init_zero(result);
    error = 2;
  } else {
    float num1, num2, res;
    s21_from_decimal_to_float(value_1, &num1);
    s21_from_decimal_to_float(value_2, &num2);
    res = num1 / num2;
    if (res == (float)(1 / 0.0) || res == (float)(-1 / 0.0)) error = 1;
    if (!error) s21_from_float_to_decimal(res, result);
  }
  return error;
}
