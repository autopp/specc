#include "specc.h"
#include "internal.h"
#include <stdlib.h>

void specc_store_before(specc_Context *cxt, specc_BeforeFunc func, const char *filename, int line) {
  if (cxt->desc_ptr < 0) {
    specc_syntax_error(filename, line, "cannot use `before' at outside of `deescribe'");
  }

  if (cxt->example != NULL) {
    specc_syntax_error(filename, line, "cannot use `before' at inside of `it'");
  }

  specc_DescStack *cur_desc = specc_current_desc(cxt);

  if (cur_desc->before_func_count == cur_desc->before_funcs_size - 1) {
    // Expand before_funcs
    int new_size = cur_desc->before_funcs_size * 2;

    if (new_size <= cur_desc->before_funcs_size) {
      specc_internal_error("cannot expand array for `before'");
    }

    specc_BeforeFunc *new_before_funcs = realloc(cur_desc->before_funcs, sizeof(specc_BeforeFunc) * new_size);
    if (new_before_funcs == NULL) {
      specc_internal_error("cannot expand array for `before'");
    }

    cur_desc->before_funcs_size = new_size;
    cur_desc->before_funcs = new_before_funcs;
  }

  cur_desc->before_funcs[cur_desc->before_func_count++] = func;
}

void specc_store_after(specc_Context *cxt, specc_AfterFunc func, const char *filename, int line) {
  if (cxt->desc_ptr < 0) {
    specc_syntax_error(filename, line, "cannot use `after' at outside of `deescribe'");
  }

  if (cxt->example != NULL) {
    specc_syntax_error(filename, line, "cannot use `after' at inside of `it'");
  }

  specc_DescStack *cur_desc = specc_current_desc(cxt);

  if (cur_desc->after_func_count == cur_desc->after_funcs_size - 1) {
    // Expand after_funcs
    int new_size = cur_desc->after_funcs_size * 2;

    if (new_size <= cur_desc->after_funcs_size) {
      specc_internal_error("cannot expand array for `after'");
    }

    specc_AfterFunc *new_after_funcs = realloc(cur_desc->after_funcs, sizeof(specc_AfterFunc) * new_size);
    if (new_after_funcs == NULL) {
      specc_internal_error("cannot expand array for `after'");
    }

    cur_desc->after_funcs_size = new_size;
    cur_desc->after_funcs = new_after_funcs;
  }

  cur_desc->after_funcs[cur_desc->after_func_count++] = func;
}
