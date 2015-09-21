#include "specc_util.h"
#include <stdio.h>
#include <stdlib.h>
#include <stdarg.h>

void specc_internal_error(const char *fmt, ...) {
  va_list vargs;
  fprintf(stderr, "SpecC internal error: ");

  va_start(vargs, fmt);
  vfprintf(stderr, fmt, vargs);
  va_end(vargs);

  fprintf(stderr, "\n");

  exit(1);
}

void specc_fprintf_indented(FILE *fp, int level, const char *fmt, ...) {
  while (level-- > 0) {
    fprintf(fp, SPECC_INDENT);
  }

  va_list varg;
  va_start(varg, fmt);
  vfprintf(fp, fmt, varg);
  va_end(varg);
}

char *specc_saprintf(const char *fmt, ...) {
  va_list varg;

  // calculate size of string
  va_start(varg, fmt);
  int size = vsnprintf(NULL, 0, fmt, varg);
  va_end(varg);

  char *s = malloc(sizeof(char) * (size + 1));
  if (s == NULL) {
    specc_internal_error("cannot allocate memory");
  }

  // format string
  va_start(varg, fmt);
  vsnprintf(s, size + 1, fmt, varg);
  va_end(varg);

  return s;
}
