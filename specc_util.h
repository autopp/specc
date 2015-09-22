#ifndef SPECC_UTIL_H_
#define SPECC_UTIL_H_

#include <stdio.h>
#include <unistd.h>

void specc_internal_error(const char *fmt, ...);

typedef enum specc_Color {
  specc_DEFAULT_COLOR,
  specc_RED,
  specc_GREEN,
  specc_YELLOW,
  specc_BLUE,
  specc_CYAN
} specc_Color;

/**
 * format print for stdout with specifieid indent
 * @param level indenet level
 * @param fmt   printf style format string
 */
void specc_fcprintf_indented(FILE *fp, specc_Color color, int level, const char *fmt, ...);

#define specc_fcprintfln_indented(fp, color, level, ...)\
  do {\
    specc_fcprintf_indented((fp), (color), (level), __VA_ARGS__);\
    fprintf((fp), "\n");\
  } while (0)

#define specc_cprintf_indented(color, level, ...)\
  specc_fcprintf_indented(stdout, (color), (level), __VA_ARGS__)

#define specc_cprintfln_indented(color, level, ...)\
  specc_fcprintfln_indented(stdout, (color), (level), __VA_ARGS__)

#define specc_printf_indented(level, ...)\
  specc_cprintf_indented(specc_DEFAULT_COLOR, level, __VA_ARGS__)

#define specc_printfln_indented(level, ...)\
  specc_cprintfln_indented(specc_DEFAULT_COLOR, level, __VA_ARGS__)

#define specc_printf(...) specc_printf_indented(0, __VA_ARGS__)

#define specc_printfln(...)\
  specc_printfln_indented(0, __VA_ARGS__)

#define specc_cprintf(color, ...)\
  specc_cprintf_indented((color), 0, __VA_ARGS__)

#define specc_cprintfln(color, ...)\
  specc_cprintfln_indented((color), 0, __VA_ARGS__)

/**
 * allocate and format string
 * @param fmt printf style format string
 * @return new string
 */
char *specc_saprintf(const char *fmt, ...);

#endif
