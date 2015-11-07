#include <stdio.h>
#include <stdlib.h>
#include <stdarg.h>
#include <signal.h>
#include <sys/time.h>

#include "specc_util.h"

void specc_internal_error(const char *fmt, ...);

/**
 * Escape sequences for each color
 */
static const char *COLOR_ESC_CODES[] = {
  "\x1b[39m",
  "\x1b[31m",
  "\x1b[32m",
  "\x1b[33m",
  "\x1b[34m",
  "\x1b[36m"
};

double specc_get_time() {
  struct timeval tv;
  gettimeofday(&tv, NULL);
  return tv.tv_sec + tv.tv_usec * 1e-6;
}

void specc_fcprintf_indented(FILE *fp, specc_Color color, int level, const char *fmt, ...) {
  // indent
  while (level-- > 0) {
    fputc(' ', fp);
  }

  // if output stream is tty, change color
  if (isatty(fileno(fp))) {
    fprintf(fp, "%s", COLOR_ESC_CODES[color]);
  }

  va_list varg;
  va_start(varg, fmt);
  vfprintf(fp, fmt, varg);
  va_end(varg);

  // if output stream is tty, change back color
  if (isatty(fileno(fp))) {
    fprintf(fp, "%s", COLOR_ESC_CODES[specc_DEFAULT_COLOR]);
  }
}

const char *specc_saprintf(const char *fmt, ...) {
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

const char *specc_signal_name(int signum) {
  static struct specc_SignalInfo {
    int num;
    const char *name;
  } pairs[] = {
    {SIGSEGV, "SIGSEGV"},
    {SIGFPE, "SIGFPE"},
    {SIGPIPE, "SIGPIPE"},
    {-1, NULL}
  };

  struct specc_SignalInfo *p;

  for (p = pairs; p->num >= 0; p++) {
    if (p->num == signum) {
      return p->name;
    }
  }

  return NULL;
}
