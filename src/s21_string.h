#include <ctype.h>
#include <limits.h>
#include <math.h>
#include <stdarg.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <wchar.h>
#define s21_size_t unsigned long long
#define S21_NULL (void *)0
#define W 0
#define P 1
#define L 2
#define J 3
#define M 4
#define S 5
#define G 6
#define Z 7
#define SIZE 30 + 1
#define EXPSIZE 38 + 1 + 3 + 1

void *s21_memchr(const void *str, int c, s21_size_t n);
char *s21_strchr(const char *str, int c);
char *s21_strrchr(const char *str, int c);
char *s21_strerror(int errnum);
char *s21_strpbrk(const char *str1, const char *str2);
char *s21_strstr(const char *haystack, const char *needle);
int s21_memcmp(const void *str1, const void *str2, s21_size_t n);
void *s21_memcpy(void *dest, const void *src, s21_size_t n);
void *s21_memset(void *str, int c, s21_size_t n);
char *s21_strncpy(char *dest, const char *src, s21_size_t n);
int s21_strncmp(const char *str1, const char *str2, s21_size_t n);
char *s21_strncat(char *dest, const char *src, s21_size_t n);
s21_size_t s21_strcspn(const char *str1, const char *str2);
s21_size_t s21_strlen(const char *str);
char *s21_strtok(char *str, const char *delim);
void *s21_to_upper(const char *str);
void *s21_to_lower(const char *str);
void *s21_insert(const char *src, const char *str, size_t start_index);
void *s21_trim(const char *src, const char *trim_chars);
int s21_sscanf(const char *str, const char *format, ...);
int s21_sprintf(char *str, char *format, ...);

typedef struct {
  char type;
  void *data;
  int width;
  char length;
} Spec;

typedef struct tag_exp {
  long double number;
  int sign;
  int exp;
} exponent;

int getstrfromspec(char *buffer, char *str, va_list *args);
bool isspecifier(char symbol);
bool setflag(char symbol, char *smbls);
bool ispercent(char symbol);
bool isasterisk(char symbol);
bool isdot(char symbol);
bool islength(char symbol);
bool chcksmbl(const char *smbls, const unsigned char bit);
void setsymbol(char *smbls, const unsigned char bit);
void handle_gG(char *tmpstr, char *smbls, va_list *args, int precision,
               bool flag, char len);
void handle_width(char *tmpstr, char *smbls, char *str, int width);
void handle_digit(char *buffer, char *smbls, char *tmpsmbl, int *ptr_buffer,
                  int *width, int *precision, va_list *args);
int counttens(long double *number);
int countfractions(long double *number);
char *exptostr(exponent e, bool flag, int precision, char *);
char *toexponent(long double number, bool flag, int precision, char *);
char gethexhigh(int value);
char gethexlow(int value);
void reverse(char *str);
char *tohex(unsigned long long int value, bool reg, char *smbls);
char *tooct(unsigned long long int value, char *smbls);
char *todec(long long int value, char *smbls, int precision);
char *toudec(unsigned long long int value, char *smbls, int precision);
char *tofloat(long double value, char *smbls, int precision);
void make_spaces(int width, char *str, int wl, char symbl);
void add_format_char(char *str, char *format, int ptr);
char *s21_strcat(char *dest, const char *src);