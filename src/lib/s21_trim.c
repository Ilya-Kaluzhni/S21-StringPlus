#include "../s21_string.h"

int find_index(const char *src, const char *trim_chars, int index) {
  index -= 1;
  int res = 0;
  for (int i = 0; i < (int)s21_strlen(trim_chars); i++) {
    if (src[index] == trim_chars[i]) res = 1;
  }
  return res;
}

void *s21_trim(const char *src, const char *trim_chars) {
  if (src == S21_NULL || trim_chars == S21_NULL) {
    return S21_NULL;
  }
  char *res = NULL;
  if (src) {
    if (trim_chars && trim_chars[0]) {
      int left = 1, right = s21_strlen(src);
      while (find_index(src, trim_chars, left)) left++;
      if (left != right) {
        while (find_index(src, trim_chars, right)) right--;
      }
      int n_len = right - left;
      res = calloc(n_len + 1, sizeof(char));
      if (res) {
        for (int i = left - 1, j = 0; i <= right; i++, j++) {
          res[j] = src[i];
        }
        res[n_len + 1] = '\0';
      }
    }
  }

  return res;
}