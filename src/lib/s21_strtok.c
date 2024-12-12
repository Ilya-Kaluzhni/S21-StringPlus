#include "../s21_string.h"

char *s21_strtok(char *str, const char *delim) {
  static char *end;
  register int copy = 0;

  while (s21_strchr(delim, copy)) {
    if ((copy = *str++) == '\0') {
      return 0;
    }
  }
  str--;
  end = str + s21_strcspn(str, delim);
  if (*end != 0) {
    *end++ = 0;
  }
  return str;
}