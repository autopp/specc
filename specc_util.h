#ifndef SPECC_UTIL_H_
#define SPECC_UTIL_H_

#include <stdio.h>

#define SPECC_INDENT "  "

void specc_internal_error(const char *fmt, ...);

/**
 * format print for stdout with specifieid indent
 * @param level indenet level
 * @param fmt   printf style format string
 */
void specc_fprintf_indented(FILE *fp, int level, const char *fmt, ...);

#define specc_printf_indented(level, ...) specc_fprintf_indented(stdout, level, __VA_ARGS__)

#define specc_printf(...) specc_printf_indented(0, __VA_ARGS__)

#define specc_fprintfln_indented(fp, level, ...)\
  do {\
    specc_fprintf_indented(fp, level, __VA_ARGS__);\
    fprintf(fp, "\n");\
  } while (0)

#define specc_printfln_indented(level, ...)\
  specc_fprintfln_indented(stdout, level, __VA_ARGS__)

#define specc_printfln(...) \
  specc_printfln_indented(0, __VA_ARGS__)

#endif
