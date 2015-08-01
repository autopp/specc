#include "specc.h"
#include <stdio.h>
#include <stdarg.h>
#include <stdlib.h>
#include <signal.h>
#include <setjmp.h>

static void specc_internal_error(const char *fmt, ...){
  va_list vargs;
  fprintf(stderr, "SpecC internal error: ");

  va_start(vargs, fmt);
  fprintf(stderr, fmt, vargs);
  va_end(vargs);

  fprintf(stderr, "\n");

  exit(1);
}

#define INITIAL_DESC_STACK_SIZE 10

int specc_init_desc_(specc_Context_ *cxt, const char *target) {
  if ( cxt->example_name != NULL ) {
    fprintf(stderr, "cannot nest 'describe' in 'it'\n");
    exit(1);
  }

  // push desc stack
  cxt->desc_ptr++;

  if ( cxt->desc_ptr >= cxt->desc_size ) {
    // reallocate
    int new_size = cxt->desc_size * 2;
    if ( new_size <= cxt->desc_size ) {
      specc_internal_error("cannnot expand stack of 'describe'");
    }

    specc_DescStack_ *new_stack = realloc(cxt->desc_stack, sizeof(specc_DescStack_) * new_size);
    if ( new_stack == NULL ) {
      specc_internal_error("cannot expand stack of 'describe'");
    }

    cxt->desc_stack = new_stack;
  }
  cxt->desc_stack[cxt->desc_ptr].target = target;

  return 0;
}

int specc_finish_desc(specc_Context_ *cxt) {
  cxt->desc_ptr--;
  return 1;
}

void specc_setup_(specc_Context_ *cxt){
  // initialize member of cxt
  cxt->desc_stack = calloc(sizeof(struct specc_DescStack_), INITIAL_DESC_STACK_SIZE);
  cxt->desc_ptr = -1;
  cxt->desc_size = INITIAL_DESC_STACK_SIZE;

  if ( cxt->desc_stack == NULL ) {
    specc_internal_error("cannot allocate memory");
  }

  cxt->example_name = NULL;
}

int specc_teardown_(specc_Context_ *cxt){
  return 0;
}
