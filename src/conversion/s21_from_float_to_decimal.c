#include <math.h>
#include <string.h>

#include "../s21_decimal.h"

int s21_from_float_to_decimal(float src, s21_decimal *dst) {
  if (dst == NULL || isnan(src) || isinf(src) || fabs(src) >= MAX_DECIMAL)
    return ERR;
  s21_set_decimal_to_zero(dst);
  if (fabs(src) > 0 && fabs(src) < 1e-28) return ERR;
  char str[100] = {0};
  sprintf(str, "%e", src);
  int exp = s21_get_exp_str(str);
  int sign = 0, mult_num = 0;
  int mantissa = s21_get_mantissa_str(str, &sign, &mult_num);
  exp += (mult_num - 1);
  dst->bits[0] = mantissa;
  while (exp > 28) {
    --exp;
    s21_div_by_10(dst);
  }
  while (exp < 0) {
    ++exp;
    s21_mult_by_10(dst);
  }
  dst->bits[3] |= exp << 16;
  s21_set_bit(dst, 127, sign);
  return OK;
}