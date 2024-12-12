#include "../s21_string.h"

int getstrfromspec(char *buffer, char *str, va_list *args) {
  int result = 0;
  int ptr_buffer = 0;
  int width = 0;
  int precision = -1;
  char len = '0';

  char *smbls = calloc(1, sizeof(char));

  while (buffer[ptr_buffer] != '\0') {
    char tmpsmbl = buffer[ptr_buffer];
    char tmpstr[10000];
    tmpstr[0] = '\0';

    setflag(tmpsmbl, smbls);

    if (isdot(tmpsmbl)) {
      setsymbol(smbls, P);
      precision = 0;
    }

    if (isdigit(tmpsmbl) || isasterisk(tmpsmbl)) {
      handle_digit(buffer, smbls, &tmpsmbl, &ptr_buffer, &width, &precision,
                   args);
    }
    if (islength(tmpsmbl) && !chcksmbl(smbls, L)) {
      len = tmpsmbl;
      setsymbol(smbls, L);
    } else if (islength(tmpsmbl) && chcksmbl(smbls, L)) {
      ptr_buffer++;
      tmpsmbl = buffer[ptr_buffer];
    }

    if (isspecifier(tmpsmbl)) {
      switch (tmpsmbl) {
        case 'c':
          tmpstr[0] = va_arg(*args, int);
          tmpstr[1] = '\0';
          break;
        case 'd':
          if (chcksmbl(smbls, L)) {
            if (len == 'h')
              s21_strcat(tmpstr, todec(va_arg(*args, int), smbls, precision));
            else if (len == 'l')
              s21_strcat(tmpstr,
                         todec(va_arg(*args, long int), smbls, precision));
          } else
            s21_strcat(tmpstr, todec(va_arg(*args, int), smbls, precision));
          break;
        case 'f':
          if (chcksmbl(smbls, L) && len == 'L')
            s21_strcat(tmpstr,
                       tofloat(va_arg(*args, long double), smbls, precision));
          else
            s21_strcat(tmpstr,
                       tofloat(va_arg(*args, double), smbls, precision));
          break;
        case 's':
          if (precision)
            s21_strncat(tmpstr, va_arg(*args, const char *), precision);
          else
            s21_strcat(tmpstr, va_arg(*args, const char *));
          break;
        case 'u':
          if (chcksmbl(smbls, L)) {
            if (len == 'h')
              s21_strcat(tmpstr,
                         toudec(va_arg(*args, unsigned int), smbls, precision));
            else if (len == 'l')
              s21_strcat(tmpstr, toudec(va_arg(*args, long unsigned int), smbls,
                                        precision));
          } else
            s21_strcat(tmpstr,
                       toudec(va_arg(*args, unsigned int), smbls, precision));
          break;
        case 'g':
          handle_gG(tmpstr, smbls, args, precision, false, len);
          break;
        case 'G':
          handle_gG(tmpstr, smbls, args, precision, true, len);
          break;
        case 'e':
          s21_strcat(tmpstr, toexponent(va_arg(*args, double), false, precision,
                                        smbls));
          break;
        case 'E':
          s21_strcat(tmpstr,
                     toexponent(va_arg(*args, double), true, precision, smbls));
          break;
        case 'x':
          s21_strcat(tmpstr, tohex(va_arg(*args, unsigned int), false, smbls));
          break;
        case 'X':
          s21_strcat(tmpstr, tohex(va_arg(*args, unsigned int), true, smbls));
          break;
        case 'o':
          s21_strcat(tmpstr, tooct(va_arg(*args, unsigned int), smbls));
          break;
        case 'p':
          setsymbol(smbls, G);
          s21_strcat(tmpstr, tohex(va_arg(*args, unsigned int), false, smbls));
          break;
      }

      handle_width(tmpstr, smbls, str, width);
    }
    ptr_buffer++;
  }

  free(smbls);
  smbls = NULL;
  return result;
}

void make_spaces(int width, char *str, int wl, char symbl) {
  char widthstr[width];
  for (int i = 0; i < wl; i++) widthstr[i] = symbl;
  widthstr[wl] = '\0';
  s21_strcat(str, widthstr);
}

int s21_sprintf(char *str, char *format, ...) {
  int ptr_format = 0;
  int ptr_str = 0;
  int count_result = 0;
  bool was_spec = false;

  str[0] = '\0';

  va_list arguments;
  va_start(arguments, format);
  while (format[ptr_format] != '\0' && ptr_str != -1) {
    if (!was_spec && ispercent(format[ptr_format])) {
      was_spec = true;
      ptr_format++;
      continue;
    } else if (was_spec && ispercent(format[ptr_format])) {
      add_format_char(str, format, ptr_format++);
      was_spec = false;
    } else if (was_spec) {
      int count = 0;
      char buffer[10000];
      while (format[ptr_format] != '\0' && !isspecifier(format[ptr_format])) {
        buffer[count++] = format[ptr_format++];
      }

      buffer[count++] = format[ptr_format++];
      buffer[count++] = '\0';

      int res = getstrfromspec(buffer, str, &arguments);
      if (res) count_result += res;

      ptr_str = s21_strlen(str);

      was_spec = false;
    } else
      add_format_char(str, format, ptr_format++);
  }

  if (ptr_str) {
    str[ptr_str++] = '\0';
    ptr_str += count_result;
  }
  va_end(arguments);
  return ptr_str;
}

void add_format_char(char *str, char *format, int ptr) {
  char tmp = format[ptr];
  s21_strncat(str, &tmp, 1);
}

bool isspecifier(char symbol) {
  bool result = false;
  switch (symbol) {
    case 'c':
    case 'd':
    case 'e':
    case 'E':
    case 'f':
    case 'g':
    case 'G':
    case 'o':
    case 's':
    case 'u':
    case 'x':
    case 'X':
    case 'p':
    case 'n':
    case '%':
      result = true;
  }
  return result;
}

bool setflag(char symbol, char *smbls) {
  bool result = false;
  switch (symbol) {
    case '-':
      setsymbol(smbls, J);
      result = true;
      break;
    case '+':
      setsymbol(smbls, M);
      result = true;
      break;
    case ' ':
      setsymbol(smbls, S);
      result = true;
      break;
    case '#':
      setsymbol(smbls, G);
      result = true;
      break;
    case '0':
      setsymbol(smbls, Z);
      result = true;
      break;
  }
  return result;
}

bool ispercent(char symbol) { return symbol == '%'; }

bool isasterisk(char symbol) { return symbol == '*'; }

bool isdot(char symbol) { return symbol == '.'; }

bool islength(char symbol) {
  bool result = false;
  switch (symbol) {
    case 'h':
    case 'l':
    case 'L':
      result = true;
  }
  return result;
}

bool chcksmbl(const char *smbls, const unsigned char bit) {
  return (*smbls & (1 << bit));
}

void setsymbol(char *smbls, const unsigned char bit) {
  *smbls = *smbls | (1 << bit);
}

int counttens(long double *number) {
  int result = -1;
  while (*number > 1) {
    result++;
    *number /= 10;
  }
  return result;
}

int countfractions(long double *number) {
  int result = 0;
  while (*number < 1.0) {
    result++;
    *number *= 10;
  }
  return result;
}

char *toexponent(long double number, bool flag, int precision, char *smbls) {
  exponent e;
  if (number > 1.0)
    e.sign = 1;
  else
    e.sign = 0;

  if (e.sign)
    e.exp = counttens(&number);
  else
    e.exp = countfractions(&number);

  if (e.sign) number *= 10;
  e.number = number;

  return exptostr(e, flag, precision, smbls);
}

char *exptostr(exponent e, bool flag, int precision, char *smbls) {
  if (precision == -1) precision = 6;
  static char result[EXPSIZE];
  result[0] = '\0';
  int ptr = 2;

  s21_strcat(result, tofloat(e.number, smbls, precision));

  ptr += precision;

  if (flag)
    result[ptr++] = 'E';
  else
    result[ptr++] = 'e';

  if (e.sign)
    result[ptr++] = '+';
  else
    result[ptr++] = '-';

  if (e.exp > 10)
    result[ptr++] = e.exp / 10 + '0';
  else
    result[ptr++] = '0';

  result[ptr++] = e.exp + '0';
  result[ptr] = '\0';

  return result;
}

char gethexlow(int value) {
  char result = value + '0';
  switch (value) {
    case 10:
      result = 'a';
      break;
    case 11:
      result = 'b';
      break;
    case 12:
      result = 'c';
      break;
    case 13:
      result = 'd';
      break;
    case 14:
      result = 'e';
      break;
    case 15:
      result = 'f';
      break;
  }
  return result;
}

char gethexhigh(int value) {
  char result = value + '0';
  switch (value) {
    case 10:
      result = 'A';
      break;
    case 11:
      result = 'B';
      break;
    case 12:
      result = 'C';
      break;
    case 13:
      result = 'D';
      break;
    case 14:
      result = 'E';
      break;
    case 15:
      result = 'F';
      break;
  }
  return result;
}

void reverse(char *str) {
  int len = s21_strlen(str);
  for (int i = 0; i < len / 2; i++) {
    char tmp = str[len - i - 1];
    str[len - i - 1] = str[i];
    str[i] = tmp;
  }
}

char *tohex(unsigned long long int value, bool reg, char *smbls) {
  static char result[SIZE];
  result[0] = '\0';
  int tmp = value % 16;
  char res;
  int ptr = 0;

  while (value > 15 && ptr < SIZE) {
    if (reg)
      res = gethexhigh(tmp);
    else
      res = gethexlow(tmp);
    result[ptr++] = res;
    value = value / 16;
    tmp = value % 16;
  }

  if (reg)
    res = gethexhigh(tmp);
  else
    res = gethexlow(tmp);

  if (ptr < SIZE - 1) {
    result[ptr++] = res;
    if (chcksmbl(smbls, G)) {
      result[ptr++] = 'x';
      result[ptr++] = '0';
    }
    result[ptr] = '\0';
  }
  reverse(result);
  return result;
}

char *tooct(unsigned long long int value, char *smbls) {
  static char result[SIZE];
  int ptr = 0;
  int tmp = value % 8;
  char res;
  result[0] = '\0';

  while (value > 7 && ptr < SIZE) {
    res = tmp + '0';
    result[ptr++] = res;
    value = value / 8;
    tmp = value % 8;
  }

  res = tmp + '0';

  if (ptr < SIZE - 1) {
    result[ptr++] = res;
    if (chcksmbl(smbls, G)) result[ptr++] = '0';
    result[ptr] = '\0';
  }

  reverse(result);

  return result;
}

char *todec(long long int value, char *smbls, int precision) {
  if (precision == -1) precision = 0;
  int sign = 1;
  if (value < 0) sign = -1;
  value *= sign;
  static char result[SIZE];
  int ptr = 0;
  int tmp = value % 10;
  result[ptr++] = tmp + '0';

  while (value > 9 || (precision > ptr)) {
    value = value / 10;
    tmp = value % 10;
    result[ptr++] = tmp + '0';
  }

  if (ptr < SIZE - 1) {
    if (sign < 0) result[ptr++] = '-';
    if (sign > 0 && chcksmbl(smbls, M)) result[ptr++] = '+';
    if (sign > 0 && chcksmbl(smbls, S) && !chcksmbl(smbls, M))
      result[ptr++] = ' ';
    result[ptr++] = '\0';
  }

  reverse(result);
  return result;
}

char *toudec(unsigned long long int value, char *smbls, int precision) {
  if (precision == -1) precision = 0;
  static char result[SIZE];
  int ptr = 0;
  int tmp = value % 10;
  result[ptr++] = tmp + '0';

  while (value > 9 || (precision > ptr)) {
    value = value / 10;
    tmp = value % 10;
    result[ptr++] = tmp + '0';
  }

  if (ptr < SIZE - 1) {
    if (chcksmbl(smbls, M)) result[ptr++] = '+';
    if (chcksmbl(smbls, S) && !chcksmbl(smbls, M)) result[ptr++] = ' ';
    result[ptr++] = '\0';
  }

  reverse(result);
  return result;
}

char *tofloat(long double value, char *smbls, int precision) {
  if (precision == -1) precision = 6;
  static char result[SIZE];
  int intpart = value;
  int ptr = 0;
  double decpart = fabsl(value - intpart) * 10;
  result[0] = '\0';

  s21_strcat(result, todec(intpart, smbls, -1));

  ptr = s21_strlen(result);

  if (precision) {
    result[ptr++] = '.';
    while (precision > 0 && ptr < SIZE) {
      result[ptr++] = (int)decpart + '0';
      decpart -= (int)decpart;
      decpart *= 10;
      precision--;
    }
  }

  result[ptr++] = '\0';

  return result;
}

void handle_digit(char *buffer, char *smbls, char *tmpsmbl, int *ptr_buffer,
                  int *width, int *precision, va_list *args) {
  if (!chcksmbl(smbls, W) && !chcksmbl(smbls, P)) {
    setsymbol(smbls, W);
    if (isasterisk(*tmpsmbl))
      *width = va_arg(*args, int);
    else {
      *width = *tmpsmbl - '0';
      while (isdigit(buffer[*ptr_buffer + 1])) {
        (*ptr_buffer)++;
        *tmpsmbl = buffer[*ptr_buffer];
        *width = 10 * *width + (*tmpsmbl - '0');
      }
    }
  } else if (chcksmbl(smbls, P)) {
    if (isasterisk(*tmpsmbl))
      *precision = va_arg(*args, int);
    else {
      (*precision) = *tmpsmbl - '0';
      while (isdigit(buffer[*ptr_buffer + 1])) {
        (*ptr_buffer)++;
        *tmpsmbl = buffer[*ptr_buffer];
        *precision = 10 * (*precision) + (*tmpsmbl - '0');
      }
    }
  }
}

void handle_width(char *tmpstr, char *smbls, char *str, int width) {
  int wl = width - s21_strlen(tmpstr);
  if (wl > 0 && !chcksmbl(smbls, J)) {
    if (chcksmbl(smbls, Z))
      make_spaces(width, str, wl, '0');
    else
      make_spaces(width, str, wl, ' ');
  }
  s21_strcat(str, tmpstr);
  if (wl > 0 && chcksmbl(smbls, J)) {
    if (chcksmbl(smbls, Z))
      make_spaces(width, str, wl, '0');
    else
      make_spaces(width, str, wl, ' ');
  }
}

void handle_gG(char *tmpstr, char *smbls, va_list *args, int precision,
               bool flag, char len) {
  char *res_f, *res_e, *res = NULL;
  long double tmp = 0.0;

  if (chcksmbl(smbls, L) && len == 'L')
    tmp = va_arg(*args, long double);
  else
    tmp = va_arg(*args, double);
  res = tofloat(tmp, smbls, precision);
  res_f = malloc((s21_strlen(res) + 1) * sizeof(char));
  s21_memcpy(res_f, res, s21_strlen(res));
  res_e = toexponent(tmp, flag, 5, smbls);
  if (s21_strlen(res_f) > s21_strlen(res_e))
    s21_strcat(tmpstr, res_e);
  else
    s21_strcat(tmpstr, res_f);
  free(res_f);
  res_f = NULL;
}

char *s21_strcat(char *dest, const char *src) {
  char *str = dest + s21_strlen(dest);
  while (*src != '\0') {
    *str++ = *src++;
  }
  *str = '\0';
  return dest;
}