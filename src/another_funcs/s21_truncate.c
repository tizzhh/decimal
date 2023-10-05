#include "../s21_decimal.h"

int s21_truncate(s21_decimal value, s21_decimal *result) {
  if (result == NULL) return ERR;
  s21_set_decimal_to_zero(result);
  unsigned int sign = s21_get_sign(&value);
  unsigned int exp = s21_get_exp(&value);
  while (exp-- > 0) s21_div_by_10(&value);
  result->bits[0] = value.bits[0];
  result->bits[1] = value.bits[1];
  result->bits[2] = value.bits[2];

  s21_set_bit(result, 127, sign);
  return OK;
}