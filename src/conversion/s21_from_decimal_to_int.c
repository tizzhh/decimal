#include "../s21_decimal.h"

int s21_from_decimal_to_int(s21_decimal src, int *dst) {
  if (dst == NULL) return ERR;
  *dst = 0;
  int sign = s21_get_sign(&src);
  s21_truncate(src, &src);
  int status = 0;
  if (src.bits[2] || src.bits[1] ||
      (sign == 1 ? (unsigned int)src.bits[0] > (unsigned int)INT_MAX + 1
                 : (unsigned int)src.bits[0] > INT_MAX)) {
    status = 1;
  } else {
    int tmp = src.bits[0];
    if (sign) tmp = -tmp;
    *dst = tmp;
  }
  return status;
}