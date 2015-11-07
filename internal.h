#ifndef INTERNAL_H_
#define INTERNAL_H_

#include "specc.h"

/**
 * Get current desc stack frame
 * @param  cxt
 * @return     pointer to current stack frame
 */
#define specc_current_desc(cxt) ((cxt)->desc_stack + (cxt)->desc_ptr)

/**
 * Output formatted error message and exit program
 * @param fmt Printf style format string
 */
void specc_internal_error(const char *fmt, ...);

/**
 * Output syntax error message and exit program
 * @param filename File name syntax error has occurred
 * @param line     Line number syntax error has occurred
 * @param fmt      Printf style format string
 */
void specc_syntax_error(const char *filename, int line, const char *fmt, ...);

#endif
