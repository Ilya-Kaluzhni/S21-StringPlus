#include "../s21_string.h"

char *s21_strchr(const char *str, int c) {
  if (str == S21_NULL) {
    return S21_NULL;
  }
  if (c == '\0') {
    return ((char *)str + s21_strlen(str));
  }

  char *result_c = S21_NULL;

  for (s21_size_t i = 0; i < s21_strlen(str); i++) {
    if (str[i] == c) {
      result_c = (char *)str + i;
      break;
    }
  }
  return result_c;
}