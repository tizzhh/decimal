#include "../s21_decimal.h"

int s21_round(s21_decimal value, s21_decimal *result) {
  if (result == NULL) return ERR;
  s21_set_decimal_to_zero(result);
  double remainder = 0.0;
  unsigned int exp = s21_get_exp(&value);
  unsigned int sign = s21_get_sign(&value);
  while (exp-- > 0) {
    remainder = s21_div_by_10(&value);
  }
  value.bits[3] = 0;
  remainder = remainder >= 5 ? 1 : 0;
  if (remainder) {
    s21_decimal temp = {{1, 0, 0, 0}};
    s21_add(value, temp, result);
  } else {
    result->bits[0] = value.bits[0];
    result->bits[1] = value.bits[1];
    result->bits[2] = value.bits[2];
  }
  if (sign) s21_set_bit(result, 127, sign);
  return OK;
}