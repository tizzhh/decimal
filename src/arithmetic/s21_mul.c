#include "s21_big_decimal.h"

int s21_mul(s21_decimal value_1, s21_decimal value_2, s21_decimal *result) {
  int error = 0;
  s21_init_zero(result);
  int exp1 = s21_get_exp(&value_1);
  int exp2 = s21_get_exp(&value_2);
  s21_big_decimal value1_big = from_decimal_to_big_decimal(value_1);
  s21_big_decimal value2_big = from_decimal_to_big_decimal(value_2);
  s21_big_decimal result_big = {0};

  s21_bin_mul_bdcml(value1_big, value2_big, &result_big);
  if (s21_get_sign_bdcml(value1_big) != s21_get_sign_bdcml(value2_big))
    s21_set_sign_bdcml(&result_big, 1);
  else
    s21_set_sign_bdcml(&result_big, 0);
  s21_set_exp_bdcml(&result_big, exp1 + exp2);
  s21_set_sign_and_exp_bdcml(&result_big);
  s21_normal_bdcml(&result_big);

  error = from_big_decimal_to_decimal(result_big, result, 0);
  return error;
}