#include "../s21_string.h"

void *s21_memcpy(void *dest, const void *src, s21_size_t n) {
  if (src == S21_NULL || dest == S21_NULL) {
    return S21_NULL;
  }
  for (s21_size_t i = 0; i < n; i++) {
    *((char *)dest + i) = *((char *)src + i);
  }
  return dest;
}
