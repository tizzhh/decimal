#include <math.h>
#include <stdio.h>
#include <stdlib.h>

#define MAX_DECIMAL 7.9228162514264337593543950335e+28
#define MAX_FLOAT 3.402823466e+38
#define UINT_MAX 4294967295
#define INT_MAX 2147483647

#define FALSE 0
#define TRUE 1

typedef struct {
  int bits[4];
} s21_decimal;

typedef enum errors {
  err_OK,
  err_INFINITY,
  err_NEG_INFINITY,
  err_ZERO_DIV
} error_code;

typedef enum status { EQUAL, LESS, GREATER } status_code;

typedef enum {
  unknown,
  plus_plus,
  minus_minus,
  plus_minus,
  minus_plus
} variable_sign;

typedef enum { OK, ERR } another_code;

#define NUM_255 255

//АРИФМЕТИКА
int s21_add(s21_decimal value_1, s21_decimal value_2, s21_decimal *result);
int s21_sub(s21_decimal value_1, s21_decimal value_2, s21_decimal *result);
int s21_mul(s21_decimal value_1, s21_decimal value_2, s21_decimal *result);
int s21_div(s21_decimal value_1, s21_decimal value_2, s21_decimal *result);

//ОПЕРАТОРЫ СРАВНЕНИЯ
int s21_is_less(s21_decimal dec1, s21_decimal dec2);
int s21_is_less_or_equal(s21_decimal dec1, s21_decimal dec2);
int s21_is_greater(s21_decimal dec1, s21_decimal dec2);
int s21_is_greater_or_equal(s21_decimal dec1, s21_decimal dec2);
int s21_is_equal(s21_decimal dec1, s21_decimal dec2);
int s21_is_not_equal(s21_decimal dec1, s21_decimal dec2);

//ПРЕОБРАЗОВАТЕЛИ
int s21_from_int_to_decimal(int src, s21_decimal *dst);
int s21_from_float_to_decimal(float src, s21_decimal *dst);
int s21_from_decimal_to_int(s21_decimal src, int *dst);
int s21_from_decimal_to_float(s21_decimal src, float *dst);

//ДРУГИЕ ФУНКЦИИ
int s21_floor(s21_decimal value, s21_decimal *result);
int s21_round(s21_decimal value, s21_decimal *result);
int s21_truncate(s21_decimal value, s21_decimal *result);
int s21_negate(s21_decimal value, s21_decimal *result);

void s21_set_bit(s21_decimal *dec, int bit, int value);
unsigned int s21_get_bit(const s21_decimal *dec, int bit);
unsigned int s21_get_sign(const s21_decimal *dec);
unsigned int s21_get_exp(const s21_decimal *dec);
int s21_div_by_10(s21_decimal *dest);
void s21_set_decimal_to_zero(s21_decimal *dec);
int s21_compare(s21_decimal dec1, s21_decimal dec2);
int s21_is_zero(const s21_decimal *dec);
void s21_mult_by_10(s21_decimal *dest);
int s21_get_exp_str(const char *str);
int s21_get_mantissa_str(const char *str, int *sign, int *mult_num);
int s21_isdigit(const char *ch);
int s21_get_highest_bit(const s21_decimal *dec);

void s21_init_zero(s21_decimal *n);
int exp_decimal_bin2dec(s21_decimal dst);
int get_bit(unsigned int value, int BitNumber);
int add_main(s21_decimal value_1, int exp_1, s21_decimal value_2, int exp_2,
             s21_decimal *result);
int check_sign_values(s21_decimal *value_1, s21_decimal *value_2);
int check_negative_bit(s21_decimal value);
int comparing_numbers(int sign_indx, s21_decimal *value_1,
                      s21_decimal *value_2);
void s21_toggle_bit(s21_decimal *value, const int num);
int s21_nikita_is_greater(s21_decimal value_1, s21_decimal value_2);
int normalization(const s21_decimal *number1, const s21_decimal *number2,
                  s21_decimal *new_num1, s21_decimal *new_num2);
void decimal_copy(const s21_decimal *src, s21_decimal *dst);
char nikita_compare(const s21_decimal *dec1, const s21_decimal *dec2);
char s21_is_zeroo(const s21_decimal *num1);
void init(int *result);
void fill_arr(int *value_1_arr, s21_decimal value_1, int *value_2_arr,
              s21_decimal value_2);
int add_mul_1010(int *value_1_arr, int *value_2_arr, int *exp_1, int *exp_2);
int s21_ret_max_count_bit_and_fill(int *value_1_arr, int *value_2_arr);
int s21_count_filled_bit(int *value);
void add_similar_sign(int *result_arr, int *value_1_arr, int *value_2_arr,
                      int *index, int *count_bit);
void add_difference_sign(int *result_arr, int *value_1_arr, int *value_2_arr,
                         int *index, int *count_bit, int greater);
void add_div_10(int *result_arr, int *exp_1, int *count_bit);
int add_less_1010(int *arr_help, int *arr_10);
void s21_mul_1010(int *arr_help, int *arr_help_copy, int *arr_10);

void write_float_decimal_exp_more(int *result, s21_decimal *dst, int index,
                                  int bit);
void set_1_bit(int *value, int BitNumber);
void count10_to_bin(int *count_10, int *count_10_bit);
void set_0_bit(int *value, int BitNumber);
void s21_set_sign_add(int sign, s21_decimal *result, int greater,
                      int *result_arr);
int s21_nikita_mult_by_10(s21_decimal *dest);
int s21_set_exp(s21_decimal *value, int result);
