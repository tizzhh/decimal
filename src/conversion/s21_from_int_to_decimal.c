#include "../s21_decimal.h"

int s21_from_int_to_decimal(int src, s21_decimal *dst) {
  if (dst == NULL) return ERR;
  s21_set_decimal_to_zero(dst);
  if (src < 0) {
    s21_set_bit(dst, 127, 1);
    src = -src;
  }
  dst->bits[0] = (unsigned int)src;
  return OK;
}