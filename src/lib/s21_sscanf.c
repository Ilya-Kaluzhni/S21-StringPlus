#include "../s21_string.h"

float s21_atof(const char **str, int width) {
  float res = 0, sign = 1;
  int power = 0;
  int count = 0;

  if (width == 0) {
    width = s21_strlen(*str);
  }

  if (**str == '-') {
    sign = -1;
    (*str)++;
    count++;
  }

  while (count < width && (**str >= '0' && **str <= '9')) {
    res = res * 10 + (**str - '0');
    (*str)++;
    count++;
  }

  if (**str == '.') {
    (*str)++;
    count++;

    while (count < width && (**str >= '0' && **str <= '9')) {
      res = res * 10 + (**str - '0');
      power--;
      (*str)++;
      count++;
    }
  }

  return sign * res * pow(10, power);
}

int s21_hexadecimal(const char **str, Spec *spec) {
  unsigned int res = 0;
  int count = 0;

  if (spec->width == 0) {
    spec->width = s21_strlen(*str);
  }

  if (**str == '0' && ((*str)[1] == 'x' || (*str)[1] == 'X')) {
    *str += 2;
    count += 2;
    while (count < spec->width && isxdigit(**str)) {
      res = (res << 4) + (isdigit(**str)
                              ? **str - '0'
                              : *((char *)s21_to_upper(*str)) - 'A' + 10);
      (*str)++;
      count++;
    }
  }

  while (count < spec->width && isxdigit(**str)) {
    res = (res << 4) + (isdigit(**str)
                            ? **str - '0'
                            : *((char *)s21_to_upper(*str)) - 'A' + 10);
    (*str)++;
    count++;
  }

  if (spec->length == 'h') {
    short unsigned int short_res = (short unsigned int)res;
    return short_res;
  }
  return res;
}

long int s21_strtol(const char **str, char **endptr, int base, int width) {
  long int res = 0;
  int sign = 1;
  int count = 0;

  if (**str == '-' || **str == '+') {
    sign = (**str == '-') ? -1 : 1;
    (*str)++;
    if (width > 0) {
      count++;
    }
  }
  if (endptr != NULL) {
    *endptr = (char *)*str;
  }

  if (width == 0) {
    width = s21_strlen(*str);
  }

  if (base == 0 || base == 16) {
    if (width > 2 && **str == '0' && ((*str)[1] == 'x' || (*str)[1] == 'X')) {
      base = 16;
      (*str) += 2;
      count += 2;
    }
  }
  if (base == 0) {
    base = 10;
  }

  while (count < width && ((**str >= '0' && **str <= '9') ||
                           (base == 16 && ((**str >= 'a' && **str <= 'f') ||
                                           (**str >= 'A' && **str <= 'F'))))) {
    int digit = 0;
    if (**str >= '0' && **str <= '9') {
      digit = **str - '0';
    } else if (base == 16) {
      digit = (*((char *)s21_to_upper(*str)) - 'A') + 10;
    }
    if (digit >= base) {
      break;
    }
    res = res * base + digit;
    (*str)++;
    count++;
  }

  return sign * res;
}

long double s21_strtold(const char **str, char **endptr, int width) {
  long double res = 0.0;
  long double sign = 1.0;
  long double divisor = 10.0;
  int dec_flag = 0;
  int exponent = 0, count = 0;

  if (**str == '-') {
    sign = -1;
    (*str)++;
    count++;
  }
  if (width == 0) {
    width = s21_strlen(*str);
  }
  while (count < width && ((**str >= '0' && **str <= '9') || **str == '.')) {
    if (**str == '.') {
      dec_flag = 1;
      (*str)++;
      count++;
    }
    if (dec_flag) {
      res = res + ((**str - '0') / divisor);
      divisor *= 10.0;
    } else {
      res = res * 10.0 + (**str - '0');
    }
    (*str)++;
    count++;
  }
  if (*((char *)s21_to_lower(*str)) == 'e') {
    (*str)++;
    count++;
    while (count < width && (**str >= '0' && **str <= '9')) {
      exponent = exponent * 10 + (**str - '0');
      (*str)++;
      count++;
    }
  }
  if (endptr != NULL) {
    *endptr = (char *)*str;
  }
  return sign * res * powl(10, exponent);
}

int s21_octal(const char **str, char length, int width) {
  int res = 0;
  int sign = 1;
  int count = 0;

  if (width == 0) {
    width = s21_strlen(*str);
  }

  while ((**str >= '0' && **str <= '7') && count < width) {
    res = res * 8 + (**str - '0');
    (*str)++;
    count++;
  }

  if (length == 'h') {
    short int short_res = (short int)res;
    return short_res * sign;
  }

  return res;
}

int s21_toi(const char **str, Spec *spec) {
  int res = 0, sign = 1, count = 0;

  if (**str == '-' || **str == '+') {
    sign = (**str == '-') ? -1 : 1;
    (*str)++;
    if (spec->width > 0) {
      count++;
    }
  }
  if (spec->width == 0) {
    spec->width = s21_strlen(*str);
  }

  if (**str == '0' && spec->width > 1 &&
      ((*str)[1] == 'x' || (*str)[1] == 'X')) {
    *str += 2;
    count += 2;
    while (count < spec->width && isxdigit(**str)) {
      res = (res << 4) + (isdigit(**str)
                              ? **str - '0'
                              : *((char *)s21_to_upper(*str)) - 'A' + 10);
      (*str)++;
      count++;
    }
  } else if (**str == '0') {
    (*str)++;
    count++;
    while (count < spec->width && **str >= '0' && **str <= '7') {
      res = (res << 3) + (**str - '0');
      (*str)++;
      count++;
    }
  } else {
    while (count < spec->width && **str >= '0' && **str <= '9') {
      res = (res * 10) + (**str - '0');
      (*str)++;
      count++;
    }
  }
  if (spec->length == 'h') {
    short int short_res = (short int)res;
    return short_res * sign;
  }

  return res * sign;
}

int s21_atoi(const char **str, Spec *spec) {
  int res = 0, sign = 1, count = 0;

  if (**str == '-' || **str == '+') {
    sign = (**str == '-') ? -1 : 1;
    (*str)++;
    if (spec->width > 0) {
      count++;
    }
  }

  if (spec->width == 0) {
    spec->width = s21_strlen(*str);
  }

  while (count < spec->width && (**str >= '0' && **str <= '9')) {
    res = res * 10 + (**str - '0');
    (*str)++;
    count++;
  }

  if (spec->length == 'h') {
    short int short_res = (short int)res;
    return short_res * sign;
  }

  return res * sign;
}

wchar_t *s21_wstring(const char **s, Spec *spec) {
  wchar_t *buf = malloc((s21_strlen(*s) + 1) * sizeof(wchar_t));
  int count = 0, wid_count = 0;

  if (spec->width == 0) {
    spec->width = s21_strlen(*s);
  }

  if (spec->type == 's') {
    while (**s && **s != ' ' && wid_count < spec->width) {
      buf[count++] = (wchar_t) * *s;
      (*s)++;
      wid_count++;
    }
    buf[count] = L'\0';
  } else if (spec->type == 'c') {
    buf[0] = (wchar_t) * *s;
    buf[1] = L'\0';
    (*s)++;
  }

  return buf;
}

char *s21_string(const char **s, Spec *spec) {
  char *buf = malloc(s21_strlen(*s) + 1 * sizeof(char));
  int count = 0, wid_count = 0;

  if (spec->width == 0) {
    spec->width = s21_strlen(*s);
  }

  if (spec->type == 's') {
    while (**s && **s != ' ' && wid_count < spec->width) {
      buf[count++] = **s;
      (*s)++;
      wid_count++;
    }
    buf[count] = '\0';
  } else if (spec->type == 'c') {
    buf[0] = **s;
    buf[1] = '\0';
    (*s)++;
  }

  return buf;
}

wchar_t *s21_wcscpy(wchar_t *dest, const wchar_t *src) {
  wchar_t *des = dest;
  const wchar_t *str = src;
  while ((*des++ = *str++) != L'\0') {
  }
  return dest;
}

char *s21_strcpy(char *dest, char *src) {
  char *des = dest;
  const char *str = src;
  while ((*des++ = *str++) != '\0') {
  }
  return dest;
}

int check_string(const char **s, Spec *spec) {
  int count = 0;

  if (spec->type == 's' || spec->type == 'c') {
    if (spec->length == 'l') {
      wchar_t *result = s21_wstring(s, spec);
      if (spec->type == 'c') {
        *((wchar_t *)spec->data) = result[0];
      } else {
        s21_wcscpy((wchar_t *)spec->data, result);
      }
      count++;
    } else {
      char *result = s21_string(s, spec);
      if (spec->type == 'c') {
        *((char *)spec->data) = result[0];
      } else {
        s21_strcpy((char *)spec->data, result);
      }
      free(result);
      count++;
    }
  }

  return count;
}

int check_spec(const char **s, Spec *spec) {
  int count = 0;
  if (spec->type == 'd') {
    if (spec->length == 'l')
      *((long int *)spec->data) = s21_strtol(s, (char **)s, 10, spec->width);
    else
      *((int *)spec->data) = s21_atoi(s, spec);
    count++;
  } else if (spec->type == 'i') {
    if (spec->length == 'l')
      *((long int *)spec->data) = s21_strtol(s, (char **)s, 0, spec->width);
    else
      *((int *)spec->data) = s21_toi(s, spec);
    count++;
  } else if (spec->type == 's' || spec->type == 'c') {
    count = check_string(s, spec);
  } else if (spec->type == 'o') {
    if (spec->length == 'l')
      *((long unsigned int *)spec->data) =
          s21_strtol(s, (char **)s, 8, spec->width);
    else
      *((unsigned int *)spec->data) = s21_octal(s, spec->length, spec->width);
    count++;
  } else if (spec->type == 'x' || spec->type == 'X') {
    if (spec->length == 'l')
      *((long unsigned int *)spec->data) =
          s21_strtol(s, (char **)s, 16, spec->width);
    else
      *((unsigned int *)spec->data) = s21_hexadecimal(s, spec);
    count++;
  } else if (spec->type == 'p' || spec->type == 'u') {
    if (spec->length == 'l')
      *((long unsigned int *)spec->data) =
          s21_strtol(s, (char **)s, 10, spec->width);
    else
      *((unsigned int *)spec->data) = s21_toi(s, spec);
    count++;
  } else if (spec->type == 'e' || spec->type == 'E' || spec->type == 'f' ||
             spec->type == 'g' || spec->type == 'G') {
    if (spec->length == 'L')
      *((long double *)spec->data) = s21_strtold(s, (char **)s, spec->width);
    else
      *((float *)spec->data) = s21_atof(s, spec->width);
    count++;
  }
  return count;
}

void check_width(const char **f, Spec *spec) {
  if (**f >= '0' && **f <= '9') {
    spec->width = s21_atoi(f, spec);
  }
}

void skip_spec(const char **s, const char **f) {
  if (*(*f + 1) == 'c') {
    (*s)++;
    (*f)++;
  } else if (*(*f + 1) == 's') {
    while (**s != ' ' && **s != '\0' && **s != '\t') {
      (*s)++;
    }
    (*f)++;
  } else if (*(*f + 1) == 'd' || *(*f + 1) == 'i' || *(*f + 1) == 'u') {
    while ((**s >= '0' && **s <= '9') || (**s >= 'A' && **s <= 'F') ||
           (**s >= 'a' && **s <= 'f') || **s == '-') {
      (*s)++;
    }
    (*f)++;
  } else if (*(*f + 1) == 'x' || *(*f + 1) == 'X' || *(*f + 1) == 'p') {
    while ((**s >= '0' && **s <= '9') || (**s >= 'A' && **s <= 'F') ||
           (**s >= 'a' && **s <= 'f')) {
      (*s)++;
    }
    (*f)++;
  } else if (*(*f + 1) == 'o') {
    while (**s >= '0' && **s <= '7') {
      (*s)++;
    }
    (*f)++;
  } else if (*(*f + 1) == 'e' || *(*f + 1) == 'E' || *(*f + 1) == 'f' ||
             *(*f + 1) == 'g' || *(*f + 1) == 'G') {
    while ((**s >= '0' && **s <= '9') || **s == '.' || **s == 'e' ||
           **s == 'E' || **s == '+' || **s == '-') {
      (*s)++;
    }
    (*f)++;
  }
}

int s21_sscanf(const char *str, const char *format, ...) {
  va_list args;
  va_start(args, format);
  int count = 0;
  const char *s = str;
  const char *f = format;
  int *n;
  Spec spec;
  if (*f == *s || *f == '%' || *f == ' ') {
    while (*f == ' ') f++;
    while (*s == ' ') s++;
    while (*f) {
      while (*f == ' ') f++;
      while (*s == ' ') s++;
      if (*s == *f && *s != '%') {
        s++;
      } else if (*f == '%') {
        f++;
        check_width(&f, &spec);
        if (*f == 'h' || *f == 'l' || *f == 'L') {
          spec.length = *f;
          f++;
        }
        if (*f == 'n') {
          n = va_arg(args, int *);
          *n = s - str;
        } else if (*f == '%' && *s == '%') {
          s++;
        } else if (*f == '*') {
          skip_spec(&s, &f);
        } else {
          spec.type = *f;
          spec.data = va_arg(args, void *);
          count += check_spec(&s, &spec);
          spec.width = 0;
          spec.length = 0;
        }
      }
      f++;
    }
  }
  va_end(args);
  return count;
}