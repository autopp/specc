#ifndef SPECC_UTIL_H_
#define SPECC_UTIL_H_

#include <stdio.h>
#include <unistd.h>

/**
 * Get second
 * @return second
 */
double specc_get_time();

/**
 * Set of color ID
 */
typedef enum specc_Color {
  specc_DEFAULT_COLOR,
  specc_RED,
  specc_GREEN,
  specc_YELLOW,
  specc_BLUE,
  specc_CYAN
} specc_Color;

/**
 * Formatted print for fp with specifieid indent and color
 * @param fp    Output target
 * @param level indenet level
 * @param color Output color ID (Only when fp points tty)
 * @param fmt   printf style format string
 */
void specc_fcprintf_indented(FILE *fp, specc_Color color, int level, const char *fmt, ...);

/**
 * Almost the same as specc_fcprintf_indented, but to output a newline
 * @param fp    Output target
 * @param level indenet level
 * @param color Output color ID (Only when fp points tty)
 */
#define specc_fcprintfln_indented(fp, color, level, ...)\
  do {\
    specc_fcprintf_indented((fp), (color), (level), __VA_ARGS__);\
    fprintf((fp), "\n");\
  } while (0)

/**
 * Formatted print for fp with specifieid indent and color
 * @param level indenet level
 * @param color Output color ID (Only when fp points tty)
 */
#define specc_cprintf_indented(color, level, ...)\
  specc_fcprintf_indented(stdout, (color), (level), __VA_ARGS__)

/**
 * Almost the same as specc_cprintf_indented, but to output a newline
 * @param level indenet level
 * @param color Output color ID (Only when fp points tty)
 */
#define specc_cprintfln_indented(color, level, ...)\
  specc_fcprintfln_indented(stdout, (color), (level), __VA_ARGS__)


/**
 * Formatted print for stdout with specifieid indent
 * @param level indenet level
 */
#define specc_printf_indented(level, ...)\
  specc_cprintf_indented(specc_DEFAULT_COLOR, level, __VA_ARGS__)

/**
 * Almost the same as specc_printf_indented, but to output a newline
 * @param level indenet level
 * @param color Output color ID (Only when fp points tty)
 */
#define specc_printfln_indented(level, ...)\
  specc_cprintfln_indented(specc_DEFAULT_COLOR, level, __VA_ARGS__)

/**
 * Formatted print for stdout
 */
#define specc_printf(...) specc_printf_indented(0, __VA_ARGS__)

/**
 * Almost the same as specc_printf, but to output a newline
 */
#define specc_printfln(...)\
  specc_printfln_indented(0, __VA_ARGS__)

/**
 * Formatted print for stdout with specifieid color
 * @param color Output color ID
 */
#define specc_cprintf(color, ...)\
  specc_cprintf_indented((color), 0, __VA_ARGS__)

/**
 * Almost the same as specc_cprintf, but to output a newline
 * @param color Output color ID
 */
#define specc_cprintfln(color, ...)\
  specc_cprintfln_indented((color), 0, __VA_ARGS__)

/**
 * Print newline
 */
#define specc_newline() (putchar('\n'))

/**
 * Allocate and format string
 * @param fmt printf style format string
 * @return new string (This string needs to be freed at end)
 */
const char *specc_saprintf(const char *fmt, ...);

/**
 * Return Name of signal number (If not signal number, return NULL)
 * @param  signum Signal number
 * @return        Name of signal or NULL (This string should NOT be freed)
 */
const char *specc_signal_name(int signum);

#endif
