#include "specc_util.h"
#include <stdio.h>
#include <stdlib.h>
#include <stdarg.h>

void specc_internal_error(const char *fmt, ...){
  va_list vargs;
  fprintf(stderr, "SpecC internal error: ");

  va_start(vargs, fmt);
  vfprintf(stderr, fmt, vargs);
  va_end(vargs);

  fprintf(stderr, "\n");

  exit(1);
}

void specc_fprintf_indented(FILE *fp, int level, const char *fmt, ...)
{
  while (level-- > 0) {
    fprintf(fp, SPECC_INDENT);
  }

  va_list varg;
  va_start(varg, fmt);
  vfprintf(fp, fmt, varg);
  va_end(varg);
}
