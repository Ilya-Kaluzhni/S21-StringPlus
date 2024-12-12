#include "../s21_string.h"

char *s21_strncpy(char *dest, const char *src, s21_size_t n) {
  if (src == S21_NULL || dest == S21_NULL) {
    return S21_NULL;
  }
  s21_size_t i = 0;
  for (; i < n && src[i] != '\0'; i++) {
    dest[i] = src[i];
  }
  while (i < n) {
    dest[i] = '\0';
    i++;
  }
  return dest;
}