#include <math.h>

#include "../s21_decimal.h"

int s21_from_decimal_to_float(s21_decimal src, float *dst) {
  if (dst == NULL) return ERR;
  *dst = 0.0;
  int exp = s21_get_exp(&src);
  int sign = s21_get_sign(&src);
  long double temp = 0.0;
  for (int i = 0; i < 96; ++i) {
    if (s21_get_bit(&src, i)) temp += pow(2, i);
  }

  temp = temp / pow(10, exp);
  if (sign) temp = -temp;
  *dst = temp;
  return OK;
}
