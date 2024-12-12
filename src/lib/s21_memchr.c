#include "../s21_string.h"

void *s21_memchr(const void *str, int c, s21_size_t n) {
  const char *string = str;
  char unsign = c;

  for (s21_size_t i = 0; i < n; i++) {
    if (string[i] == unsign) {
      return (void *)(string + i);
    }
  }
  return S21_NULL;
}
