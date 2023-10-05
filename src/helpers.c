#include <math.h>
#include <string.h>

#include "s21_decimal.h"

void s21_set_bit(s21_decimal *dec, int bit, int value) {
  unsigned int index = bit / 32;
  unsigned int pos = bit % 32;
  if (!value && index < 4) {
    dec->bits[index] &= ~(1 << pos);
  } else if (index < 4) {
    dec->bits[index] |= (1 << pos);
  }
}

unsigned int s21_get_bit(const s21_decimal *dec, int bit) {
  unsigned int index = bit / 32;
  unsigned int pos = bit % 32;

  unsigned int res = 0;
  if (index < 4) {
    res = (dec->bits[index] >> pos) & 1;
  }
  return res;
}

unsigned int s21_get_sign(const s21_decimal *dec) {
  return (unsigned)dec->bits[3] >> 31;
}

unsigned int s21_get_exp(const s21_decimal *dec) {
  return (unsigned)dec->bits[3] << 1 >> 17;
}

int s21_div_by_10(s21_decimal *dest) {
  unsigned long int temp = (dest->bits[2] % 10) * (UINT_MAX + 1);
  unsigned long int temp2 = ((dest->bits[1] + temp) % 10) * (UINT_MAX + 1);
  dest->bits[2] = (unsigned)dest->bits[2] / 10;
  dest->bits[1] = ((unsigned)dest->bits[1] + temp) / 10;
  int ost = ((unsigned)dest->bits[0] + temp2) % 10;
  dest->bits[0] = ((unsigned)dest->bits[0] + temp2) / 10;

  return ost;
}

void s21_set_decimal_to_zero(s21_decimal *dec) {
  memset(dec->bits, 0x00, sizeof(dec->bits));
}

int s21_is_zero(const s21_decimal *dec) {
  return !dec->bits[0] && !dec->bits[1] && !dec->bits[2];
}

void s21_mult_by_10(s21_decimal *dest) {
  unsigned long int carry = 0;
  s21_decimal copy;
  for (int i = 0; i < 3; ++i) {
    unsigned long temp = (unsigned long)(unsigned)dest->bits[i] * 10;
    copy.bits[i] = dest->bits[i] * 10;
    copy.bits[i] += carry;
    carry = (temp) >> 32;
  }

  if (!carry) {
    dest->bits[0] = copy.bits[0];
    dest->bits[1] = copy.bits[1];
    dest->bits[2] = copy.bits[2];
  }
}

int s21_get_exp_str(const char *str) {
  int last_pos = strlen(str) - 1;
  return str[last_pos - 2] == '+'
             ? (-1 * (str[last_pos] - '0' + (str[last_pos - 1] - '0') * 10))
             : (str[last_pos] - '0' + (str[last_pos - 1] - '0') * 10);
}

int s21_get_mantissa_str(const char *str, int *sign, int *mult_num) {
  int exp_skip = 5;
  int no_trailing_zeroes = 1;
  int mantissa = 0;
  for (int pos = strlen(str) - exp_skip, exp = 0; pos >= 0; --pos) {
    if (str[pos] != '0') no_trailing_zeroes = 0;
    if (!no_trailing_zeroes && s21_isdigit(&str[pos])) {
      mantissa += (str[pos] - '0') * pow(10, exp++);
      ++*mult_num;
    }
  }
  if (str[0] == '-') *sign = 1;
  return mantissa;
}

int s21_isdigit(const char *ch) { return *ch >= '0' && *ch <= '9'; }

int s21_get_highest_bit(const s21_decimal *dec) {
  int bit = 95;
  for (; bit >= 0; --bit) {
    if (s21_get_bit(dec, bit)) break;
  }
  return bit;
}