#include <stdlib.h>

#include "../s21_decimal.h"

int s21_compare(s21_decimal dec1, s21_decimal dec2) {
  int exp1 = s21_get_exp(&dec1);
  int exp2 = s21_get_exp(&dec2);

  int highest_bit1 = s21_get_highest_bit(&dec1);
  int highest_bit2 = s21_get_highest_bit(&dec2);
  int diff = abs(exp1 - exp2);
  if (exp1 < exp2) {
    if (highest_bit1 + diff > 95) {
      while (exp2-- > exp1) s21_div_by_10(&dec2);
    } else {
      while (exp1++ < exp2) s21_mult_by_10(&dec1);
    }
  } else if (exp2 < exp1) {
    if (highest_bit2 + diff > 95) {
      while (exp1-- > exp2) s21_div_by_10(&dec1);
    } else {
      while (exp2++ < exp1) s21_mult_by_10(&dec2);
    }
  }

  status_code status = EQUAL;
  if (s21_is_zero(&dec1) != 1 || s21_is_zero(&dec2) != 1) {
    int is_negative1 = s21_get_sign(&dec1);
    int is_negative2 = s21_get_sign(&dec2);
    if (is_negative1 && !is_negative2) {
      status = LESS;
    } else if (is_negative2 && !is_negative1) {
      status = GREATER;
    }
    for (int i = 95; i >= 0 && status == EQUAL; --i) {
      int first_bit = s21_get_bit(&dec1, i);
      int second_bit = s21_get_bit(&dec2, i);
      if (first_bit > second_bit) {
        status = is_negative1 ? LESS : GREATER;
      } else if (second_bit > first_bit) {
        status = is_negative1 ? GREATER : LESS;
      }
    }
  }

  return status;
}

int s21_is_less(s21_decimal dec1, s21_decimal dec2) {
  int status = s21_compare(dec1, dec2);
  return status == LESS ? TRUE : FALSE;
}

int s21_is_less_or_equal(s21_decimal dec1, s21_decimal dec2) {
  int status = s21_compare(dec1, dec2);
  return status == LESS || status == EQUAL ? TRUE : FALSE;
}

int s21_is_greater(s21_decimal dec1, s21_decimal dec2) {
  int status = s21_compare(dec1, dec2);
  return status == GREATER ? TRUE : FALSE;
}

int s21_is_greater_or_equal(s21_decimal dec1, s21_decimal dec2) {
  int status = s21_compare(dec1, dec2);
  return status == GREATER || status == EQUAL ? TRUE : FALSE;
}

int s21_is_equal(s21_decimal dec1, s21_decimal dec2) {
  int status = s21_compare(dec1, dec2);
  return status == EQUAL ? TRUE : FALSE;
}

int s21_is_not_equal(s21_decimal dec1, s21_decimal dec2) {
  int status = s21_compare(dec1, dec2);
  return status != EQUAL ? TRUE : FALSE;
}