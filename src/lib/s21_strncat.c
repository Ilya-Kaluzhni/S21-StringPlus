#include "../s21_string.h"

char *s21_strncat(char *dest, const char *src, s21_size_t n) {
  if (src == S21_NULL || dest == S21_NULL) {
    return S21_NULL;
  }
  s21_size_t origin_len = s21_strlen(dest);
  s21_size_t i = 0;
  for (; i < n && src[i] != '\0'; i++) {
    dest[origin_len + i] = src[i];
  }
  dest[origin_len + i] = '\0';
  return dest;
}
