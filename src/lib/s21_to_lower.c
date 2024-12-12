#include "../s21_string.h"

void *s21_to_lower(const char *str) {
  if (str == S21_NULL) {
    return S21_NULL;
  }
  char *new_string = malloc(sizeof(char) * (s21_strlen(str) + 1));
  for (s21_size_t i = 0; i < s21_strlen(str); i++) {
    if (str[i] >= 65 && str[i] <= 90) {
      new_string[i] = str[i] + 32;
    } else
      new_string[i] = str[i];
  }

  new_string[s21_strlen(str)] = '\0';
  return new_string;
}
