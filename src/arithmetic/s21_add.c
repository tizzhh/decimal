#include "../s21_decimal.h"

int s21_add(s21_decimal value_1, s21_decimal value_2, s21_decimal *result) {
  // exit = 0 - продолжаем работатьб, 1 - успешное завершение, -1 - ошибка
  s21_init_zero(result);
  int exp_1 = exp_decimal_bin2dec(value_1);
  int exp_2 = exp_decimal_bin2dec(value_2);
  int error = add_main(value_1, exp_1, value_2, exp_2, result);
  return error;
}

void s21_init_zero(s21_decimal *n) {
  for (int i = 0; i < 4; i++) {
    n->bits[i] = 0;
  }
}

int exp_decimal_bin2dec(s21_decimal dst) {
  int exp_int = 0;
  int index = 23;
  int exp_2 = 7;
  while (index >= 16) {
    if (get_bit(dst.bits[3], index)) {
      exp_int += pow(2, exp_2);
    }
    index--;
    exp_2--;
  }
  return exp_int;
}

int get_bit(unsigned int value, int BitNumber) {
  return value & (1 << BitNumber);
}

int add_main(s21_decimal value_1, int exp_1, s21_decimal value_2, int exp_2,
             s21_decimal *result) {
  int error = 0;
  int index = 0;
  variable_sign sign_indx = check_sign_values(&value_1, &value_2);
  int greater = comparing_numbers(sign_indx, &value_1, &value_2);
  int value_1_arr[NUM_255];
  int value_2_arr[NUM_255];
  int result_arr[NUM_255];
  init(result_arr);
  fill_arr(value_1_arr, value_1, value_2_arr, value_2);
  error = add_mul_1010(value_1_arr, value_2_arr, &exp_1, &exp_2);
  if (error == 123 && sign_indx == plus_plus) {
    return 1;
  } else if (error == 123 && sign_indx == minus_minus) {
    return 2;
  }
  int count_bit = s21_ret_max_count_bit_and_fill(value_1_arr, value_2_arr);
  if (exp_1 == exp_2 && (sign_indx == plus_plus || sign_indx == minus_minus)) {
    add_similar_sign(result_arr, value_1_arr, value_2_arr, &index, &count_bit);
  } else if (exp_1 == exp_2 &&
             (sign_indx == plus_minus || sign_indx == minus_plus)) {
    add_difference_sign(result_arr, value_1_arr, value_2_arr, &index,
                        &count_bit, greater);
  }
  int copy_exp_1 = exp_1;
  if (count_bit > 96) {
    while (count_bit > 96) {
      if (!copy_exp_1) {
        count_bit = s21_count_filled_bit(result_arr);
        if (count_bit > 96) {
          error = 123;
          break;
        }
      }
      add_div_10(result_arr, &exp_1, &count_bit);
      count_bit = s21_count_filled_bit(result_arr);
      copy_exp_1--;
    }
  }
  if (error == 123 && sign_indx == plus_plus) {
    return 1;
  } else if (error == 123 && sign_indx == minus_minus)
    return 2;
  write_float_decimal_exp_more(result_arr, result, index, 2);
  int count_10_bit[8] = {0};
  count10_to_bin(&exp_1, count_10_bit);
  for (int i = 0, j = 16; i < 8; i++, j++) {
    if (count_10_bit[i])
      set_1_bit(&(result->bits[3]), j);
    else
      set_0_bit(&(result->bits[3]), j);
  }
  s21_set_sign_add(sign_indx, result, greater, result_arr);
  return 0;
}

int check_sign_values(s21_decimal *value_1, s21_decimal *value_2) {
  variable_sign sign_indx = unknown;
  int negative_value_1 = check_negative_bit(*value_1);
  int negative_value_2 = check_negative_bit(*value_2);

  if (!negative_value_1 && !negative_value_2) {
    sign_indx = plus_plus;
  } else if (negative_value_1 && negative_value_2) {
    sign_indx = minus_minus;
  } else if (!negative_value_1 && negative_value_2) {
    sign_indx = plus_minus;
  } else if (negative_value_1 && !negative_value_2) {
    sign_indx = minus_plus;
  }
  return sign_indx;
}

int check_negative_bit(s21_decimal value) { return value.bits[3] & (1 << 31); }

int comparing_numbers(int sign_indx, s21_decimal *value_1,
                      s21_decimal *value_2) {
  int greater = -1;
  if (sign_indx == plus_minus || sign_indx == minus_plus) {
    if (sign_indx == plus_minus) {
      s21_toggle_bit(value_2, 127);
      greater = s21_nikita_is_greater(
          *value_1, *value_2);  // если value_1 > value_2 то 1, иначе 0
      s21_toggle_bit(value_2, 127);
    } else {
      s21_toggle_bit(value_1, 127);
      greater = s21_nikita_is_greater(*value_1, *value_2);
      s21_toggle_bit(value_1, 127);
    }
  }
  return greater;
}

void s21_toggle_bit(s21_decimal *value, const int num) {
  value->bits[num / 32] ^= 1u << num % 32;
}

int s21_nikita_is_greater(s21_decimal value_1, s21_decimal value_2) {
  s21_decimal new_num1, new_num2;
  normalization(&value_1, &value_2, &new_num1, &new_num2);
  int res = nikita_compare(&new_num1, &new_num2);
  return res == GREATER ? 1 : 0;
}

char nikita_compare(const s21_decimal *dec1, const s21_decimal *dec2) {
  int status = EQUAL;
  if (s21_is_zeroo(dec1) != 1 || s21_is_zeroo(dec2) != 1) {
    int is_negative1 = s21_get_sign(dec1);
    int is_negative2 = s21_get_sign(dec2);
    if (is_negative1 && !is_negative2) {
      status = LESS;
    } else if (is_negative2 && !is_negative1) {
      status = GREATER;
    }
    for (int i = 95; i >= 0 && status == 2; --i) {
      int first_bit = s21_get_bit(dec1, i);
      int second_bit = s21_get_bit(dec2, i);
      if (first_bit > second_bit) {
        status = is_negative1 ? LESS : GREATER;
      } else if (second_bit > first_bit) {
        status = is_negative1 ? GREATER : LESS;
      }
    }
  }
  return status;
}

char s21_is_zeroo(const s21_decimal *num1) {
  return !num1->bits[0] && !num1->bits[1] && !num1->bits[2];
}

int normalization(const s21_decimal *number1, const s21_decimal *number2,
                  s21_decimal *new_num1, s21_decimal *new_num2) {
  another_code code = OK;
  int exp1 = number1->bits[3] << 1 >> 17, exp2 = number2->bits[3] << 1 >> 17;
  decimal_copy(number1, new_num1);
  decimal_copy(number2, new_num2);
  if (exp1 != exp2) {
    if (exp1 < exp2) {
      while (exp1++ < exp2) {
        if (s21_nikita_mult_by_10(new_num1) == ERR) {
          code = ERR;
          break;
        }
      }
    } else if (exp2 < exp1) {
      while (exp2++ < exp1) {
        if (s21_nikita_mult_by_10(new_num2) == ERR) {
          code = ERR;
          break;
        }
      }
    }
  }
  return code;
}

void decimal_copy(const s21_decimal *src, s21_decimal *dst) {
  for (size_t i = 0; i != 4; ++i) dst->bits[i] = src->bits[i];
}

int s21_nikita_mult_by_10(s21_decimal *dest) {
  unsigned long int carry = 0;
  s21_decimal copy;
  copy.bits[3] = dest->bits[3];
  another_code code = ERR;
  for (int i = 0; i < 3; ++i) {
    unsigned long temp = (unsigned long)(unsigned)dest->bits[i] * 10;
    copy.bits[i] = dest->bits[i] * 10;
    copy.bits[i] += carry;
    carry = (temp) >> 32;
  }
  if (carry == 0) {
    code = OK;
    decimal_copy(&copy, dest);
  }
  return code;
}

void init(int *result) {
  for (int i = 0; i < NUM_255; i++) {
    result[i] = 2;
  }
}

void fill_arr(int *value_1_arr, s21_decimal value_1, int *value_2_arr,
              s21_decimal value_2) {
  init(value_1_arr);
  init(value_2_arr);
  for (int i = 0; i < 96; i++) {
    if (get_bit(value_1.bits[i / 32], i % 32))
      value_1_arr[i] = 1;
    else
      value_1_arr[i] = 0;
    if (get_bit(value_2.bits[i / 32], i % 32))
      value_2_arr[i] = 1;
    else
      value_2_arr[i] = 0;
  }
}

int add_mul_1010(int *value_1_arr, int *value_2_arr, int *exp_1, int *exp_2) {
  int error = 0;
  int flag = 0;
  //определяем разностьстепеней умножаем на 10 пока экспоненты не станут равными
  int exp_delta = abs(*exp_1 - *exp_2);
  int value_min_exp[NUM_255];
  if (*exp_1 < *exp_2) {
    for (int i = 0; i < NUM_255; i++) {
      value_min_exp[i] = value_1_arr[i];
    }
    flag = 1;
  } else {
    for (int i = 0; i < NUM_255; i++) {
      value_min_exp[i] = value_2_arr[i];
    }
    flag = 2;
  }
  int result[NUM_255];
  int memory = 0;
  while (exp_delta) {
    int index = 0;
    int j = 2;
    init(result);
    int count_bit = 0;
    int count_1_bits = 0;
    for (int i = 254; i >= 0; i--) {
      if (value_min_exp[i] == 1) {
        index = 1;
        count_1_bits++;
      }
      if (index) count_bit++;
    }
    index = 0;
    if (count_1_bits >= 96) {
      error = 123;
    }
    result[index] = 0;
    index++;
    result[index] = value_min_exp[index - 1];
    index++;
    result[index] = value_min_exp[index - 1];
    index++;
    while (value_min_exp[j] != 2) {
      if (value_min_exp[j] == 1 && value_min_exp[j - 2] == 1) {
        if (memory == 0) {
          result[index] = 0;
          memory = 1;
        } else {
          result[index] = 1;
        }
      } else if ((value_min_exp[j] == 0 && value_min_exp[j - 2] == 1) ||
                 (value_min_exp[j] == 1 && value_min_exp[j - 2] == 0)) {
        if (memory == 0) {
          result[index] = 1;
        } else {
          result[index] = 0;
        }
      } else if (value_min_exp[j] == 0 && value_min_exp[j - 2] == 0) {
        if (memory == 0) {
          result[index] = 0;
        } else {
          result[index] = 1;
          memory = 0;
        }
      }
      index++;
      j++;
    }
    //сложение первых двух разрядов
    for (int k = 0; k <= 1; k++, j++, index++) {
      if (value_min_exp[j - 2] == 1 && memory == 1) {
        result[index] = 0;
      } else if ((value_min_exp[j - 2] == 0 && memory == 1) ||
                 (value_min_exp[j - 2] == 1 && memory == 0)) {
        result[index] = 1;
        memory = 0;
      } else if (value_min_exp[j - 2] == 0 && memory == 0) {
        result[index] = 0;
      }
    }
    if (memory == 1) {
      result[index] = 1;
      memory = 0;
    }
    //запись в правильном порядке в новый массив
    for (int k = 0; k < NUM_255; k++) {
      value_min_exp[k] = result[k];
    }
    exp_delta--;
    if (flag == 1) {
      *exp_1 = *exp_1 + 1;
    } else if (flag == 2) {
      *exp_2 = *exp_2 + 1;
    }
  }
  if (flag == 1) {
    for (int k = 0; k < NUM_255; k++) {
      value_1_arr[k] = value_min_exp[k];
    }
  } else if (flag == 2) {
    for (int k = 0; k < NUM_255; k++) {
      value_2_arr[k] = value_min_exp[k];
    }
  }
  return error;
}

int s21_ret_max_count_bit_and_fill(int *value_1_arr, int *value_2_arr) {
  int count_bit_1 = s21_count_filled_bit(value_1_arr);
  int count_bit_2 = s21_count_filled_bit(value_2_arr);
  for (int i = 0; i < NUM_255; i++) {
    if (value_2_arr[i] == 2) value_2_arr[i] = 0;
    if (value_1_arr[i] == 2) value_1_arr[i] = 0;
  }
  int count_bit = count_bit_1 < count_bit_2 ? count_bit_2 : count_bit_1;
  return count_bit;
}

int s21_count_filled_bit(int *value) {
  int index = 0;
  int count_bit = 0;
  for (int i = 254; i >= 0; i--) {
    if (value[i] == 1) index = 1;
    if (index) {
      count_bit++;
    }
  }
  return count_bit;
}

void add_similar_sign(int *result_arr, int *value_1_arr, int *value_2_arr,
                      int *index, int *count_bit) {
  int memory = 0;
  for (; *index < *count_bit; ++(*index)) {
    if (value_1_arr[*index] == 1 && value_2_arr[*index] == 1) {
      //нет в памяти доп разряда
      if (memory == 0) {
        result_arr[*index] = 0;
        memory = 1;
      } else {
        result_arr[*index] = 1;
      }
    } else if ((value_1_arr[*index] == 0 && value_2_arr[*index] == 1) ||
               (value_1_arr[*index] == 1 && value_2_arr[*index] == 0)) {
      if (memory == 0) {
        result_arr[*index] = 1;
      } else {
        result_arr[*index] = 0;
      }
    } else if (value_1_arr[*index] == 0 && value_2_arr[*index] == 0) {
      if (memory == 0) {
        result_arr[*index] = 0;
      } else {
        result_arr[*index] = 1;
        memory = 0;
      }
    }
  }
  if (memory) {
    result_arr[*index] = 1;
    (*count_bit)++;
    memory = 0;
  } else
    (*index)--;
}

void add_difference_sign(int *result_arr, int *value_1_arr, int *value_2_arr,
                         int *index, int *count_bit, int greater) {
  if (greater == 1) {
    int buf = 0;
    for (*index = 0; *index < *count_bit; ++(*index)) {
      int tmp = value_2_arr[*index] - buf - value_1_arr[*index];
      buf = (tmp < 0) ? 1 : 0;
      if (tmp < 0) tmp = 1;
      result_arr[*index] = tmp;
    }
  } else if (greater == 0) {
    for (; *index < *count_bit; ++(*index)) {
      if (value_2_arr[*index] == 0 && value_1_arr[*index] == 0) {
        result_arr[*index] = 0;
      } else if (value_2_arr[*index] == 1 && value_1_arr[*index] == 0) {
        result_arr[*index] = 1;
      } else if (value_2_arr[*index] == 0 && value_1_arr[*index] == 1) {
        //цикл для реализации ситуации когда (10000 - 1010) и 1 нужно брать не
        //из соседнего разряда
        for (int j = *index + 1; j >= 0; j++) {
          if (value_2_arr[j] == 0) {
            value_2_arr[j] = 1;
          } else if (value_2_arr[j] == 1) {
            value_2_arr[j] = 0;
            break;
          }
        }
        result_arr[*index] = 1;
      } else if (value_2_arr[*index] == 1 && value_1_arr[*index] == 1) {
        result_arr[*index] = 0;
      }
    }
  }
}

void add_div_10(int *result_arr, int *exp_1, int *count_bit) {
  int result_arr_copy[NUM_255];
  init(result_arr_copy);
  int arr_10[5] = {0, 1, 0, 1, 0};
  int arr_help[5];
  int arr_help_copy[5];
  for (int i = 0; i < 5; i++) {
    arr_help[i] = 0;
    arr_help_copy[i] = 0;
  }
  int index = *count_bit - 1;
  int index_result = 0;
  int start = 1;
  for (int i = 1; i < 5; i++) {
    arr_help[i] = result_arr[index];
    index--;
  }
  while (index >= 0) {
    while (add_less_1010(arr_help, arr_10) && index >= 0) {
      for (int i = 0; i < 4; i++) {
        arr_help[i] = arr_help[i + 1];
      }
      arr_help[4] = result_arr[index];
      index--;
      if (!start && add_less_1010(arr_help, arr_10)) {
        result_arr_copy[index_result] = 0;
        index_result++;
      }
    }
    start = 0;
    if (index < 0 && add_less_1010(arr_help, arr_10)) break;
    //процесс вычитания 1010 из arr_help запись в arr_help_copy полученного
    //результата
    s21_mul_1010(arr_help, arr_help_copy, arr_10);
    result_arr_copy[index_result] = 1;
    index_result++;
    for (int i = 0; i < 5; i++) {
      arr_help[i] = arr_help_copy[i];
    }
  }
  for (int i = 254; i >= 0; i--) {
    if (result_arr_copy[i] != 2) {
      index = i;
      break;
    }
  }
  start = 0;
  init(result_arr);
  while (index >= 0) {
    result_arr[start] = result_arr_copy[index];
    index--;
    start++;
  }
  *exp_1 = *exp_1 - 1;
}

int add_less_1010(int *arr_help, int *arr_10) {
  //если число >= 1010 то вернется 0
  int answer = 0;
  for (int i = 0; i < 5; i++) {
    if (arr_help[i] != arr_10[i]) {
      if (arr_help[i] == 1) {
        // arr_help больше или равно 1010
        answer = 0;
        break;
      } else {
        // arr_help меньше 1010
        answer = 1;
        break;
      }
    }
  }
  return answer;
}

void s21_mul_1010(int *arr_help, int *arr_help_copy, int *arr_10) {
  for (int i = 4; i >= 0; i--) {
    if (arr_help[i] == 0 && arr_10[i] == 0) {
      arr_help_copy[i] = 0;
    } else if (arr_help[i] == 1 && arr_10[i] == 0) {
      arr_help_copy[i] = 1;
    } else if (arr_help[i] == 0 && arr_10[i] == 1) {
      for (int j = i - 1; j >= 0; j--) {
        if (arr_help[j] == 0) {
          arr_help[j] = 1;
        } else if (arr_help[j] == 1) {
          arr_help[j] = 0;
          break;
        }
      }
      arr_help_copy[i] = 1;
    } else if (arr_help[i] == 1 && arr_10[i] == 1) {
      arr_help_copy[i] = 0;
    }
  }
}

void write_float_decimal_exp_more(int *result, s21_decimal *dst, int index,
                                  int bit) {
  switch (bit) {
    case 0:
      for (; index >= 0; index--) {
        if (result[index] == 1) set_1_bit(&(dst->bits[0]), index);
      }
    case 1:
      for (; index >= 32; index--) {
        if (result[index] == 1) set_1_bit(&(dst->bits[1]), index);
      }
      for (; index >= 0; index--) {
        if (result[index] == 1) set_1_bit(&(dst->bits[0]), index);
      }
    case 2:
      for (; index >= 64; index--) {
        if (result[index] == 1) set_1_bit(&(dst->bits[2]), index);
      }
      for (; index >= 32; index--) {
        if (result[index] == 1) set_1_bit(&(dst->bits[1]), index);
      }
      for (; index >= 0; index--) {
        if (result[index] == 1) set_1_bit(&(dst->bits[0]), index);
      }
  }
}

void set_1_bit(int *value, int BitNumber) { *value |= (1 << BitNumber); }

void count10_to_bin(int *count_10, int *count_10_bit) {
  int mod_2 = 0;
  int index = 0;
  while (*count_10) {
    mod_2 = *count_10 % 2;
    count_10_bit[index] = mod_2;
    *count_10 /= 2;
    index++;
  }
}

void set_0_bit(int *value, int BitNumber) { *value &= ~(1 << BitNumber); }

void s21_set_sign_add(int sign, s21_decimal *result, int greater,
                      int *result_arr) {
  if (sign == 2) set_1_bit(&(result->bits[3]), 31);
  int check_equal = 0;
  if (sign == 3 && greater == 0) {
    for (int i = 0; i < NUM_255; i++) {
      if (result_arr[i] == 1) {
        check_equal = 1;
        break;
      }
    }
    if (check_equal) set_1_bit(&(result->bits[3]), 31);
  } else if ((sign == plus_minus && greater == 1) ||
             (sign == minus_plus && greater == 1)) {
    set_1_bit(&(result->bits[3]), 31);
  }
}

int s21_set_exp(s21_decimal *value, int result) {
  int error = 0;
  int max_exp = 28;
  int power = 16;
  if ((-max_exp * 2 < result) && (result < max_exp * 2)) {
    for (int i = 20; i > 15; i--, power /= 2) {
      if (result >= power) {
        result -= power;
        set_1_bit(&value->bits[3], i);
      } else {
        set_0_bit(&value->bits[3], i);
      }
    }
  } else
    error = 1;
  return error;
}