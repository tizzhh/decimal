#include "../s21_decimal.h"

int s21_sub(s21_decimal value_1, s21_decimal value_2, s21_decimal *result) {
  int error = 0;
  s21_decimal value_2_inv = value_2;
  s21_toggle_bit(&value_2_inv, 127);

  error = s21_add(value_1, value_2_inv, result);
  return error;
}