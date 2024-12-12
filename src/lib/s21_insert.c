#include "../s21_string.h"

void *s21_insert(const char *src, const char *str, size_t start_index) {
  s21_size_t i = 0, j = 0, k = start_index;
  if (src == S21_NULL || str == S21_NULL || s21_strlen(src) < start_index)
    return S21_NULL;
  char *result =
      (char *)calloc(s21_strlen(src) + s21_strlen(str) + 1, sizeof(char));
  if (result) {
    for (; i < s21_strlen(src) && i < start_index; i++) {
      result[i] = src[i];
    }
    for (; j < s21_strlen(str); j++, i++) {
      result[i] = str[j];
    }
    for (; k < s21_strlen(src); i++, k++) {
      result[i] = src[k];
    }
  }

  return result;
}