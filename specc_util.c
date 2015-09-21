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

static const char *COLOR_ESC_CODES[] = {
  "\x1b[39m",
  "\x1b[31m",
  "\x1b[32m",
  "\x1b[33m",
  "\x1b[34m",
  "\x1b[36m"
};

void specc_fcprintf_indented(FILE *fp, specc_Color color, int level, const char *fmt, ...) {
  while (level-- > 0) {
    fprintf(fp, SPECC_INDENT);
  }

  if (isatty(fileno(fp))) {
    fprintf(fp, "%s", COLOR_ESC_CODES[color]);
  }

  va_list varg;
  va_start(varg, fmt);
  vfprintf(fp, fmt, varg);
  va_end(varg);

  if (isatty(fileno(fp))) {
    fprintf(fp, "%s", COLOR_ESC_CODES[specc_DEFAULT_COLOR]);
  }
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
