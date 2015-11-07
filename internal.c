#include "internal.h"
#include "specc_util.h"
#include <stdlib.h>
#include <stdio.h>
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

void specc_syntax_error(const char *filename, int line, const char *fmt, ...) {
  va_list vargs;

  specc_fcprintf_indented(stderr, specc_RED, 0, "SpecC Syntax Error at %s:%d: ", filename, line);

  va_start(vargs, fmt);
  vfprintf(stderr, fmt, vargs);
  va_end(vargs);

  fprintf(stderr, "\n");

  exit(1);
}
