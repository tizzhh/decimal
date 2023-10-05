#include "../s21_decimal.h"

int s21_floor(s21_decimal value, s21_decimal *result) {
  if (result == NULL) return ERR;
  s21_set_decimal_to_zero(result);
  unsigned int sign = s21_get_sign(&value);
  s21_truncate(value, &value);
  result->bits[0] = value.bits[0];
  result->bits[1] = value.bits[1];
  result->bits[2] = value.bits[2];
  if (sign == 1) {
    s21_set_bit(result, 127, sign);
    s21_decimal temp = {{1, 0, 0, 0}};
    s21_sub(*result, temp, result);
  }
  return OK;
}