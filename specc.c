#include "specc.h"
#include <stdio.h>
#include <stdarg.h>
#include <stdlib.h>
#include <signal.h>
#include <setjmp.h>

#define eprintf(...) (fprintf(stderr, __VA_ARGS__))

static void specc_internal_error(const char *fmt, ...){
  va_list vargs;
  fprintf(stderr, "SpecC internal error: ");

  va_start(vargs, fmt);
  fprintf(stderr, fmt, vargs);
  va_end(vargs);

  fprintf(stderr, "\n");

  exit(1);
}


int specc_init_desc(specc_Context *cxt, const char *target) {
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

    specc_DescStack *new_stack = realloc(cxt->desc_stack, sizeof(specc_DescStack) * new_size);
    if ( new_stack == NULL ) {
      specc_internal_error("cannot expand stack of 'describe'");
    }

    cxt->desc_stack = new_stack;
  }
  cxt->desc_stack[cxt->desc_ptr].target = target;

  return 0;
}

int specc_finish_desc(specc_Context *cxt) {
  cxt->desc_ptr--;
  return 1;
}

int specc_init_example(specc_Context *cxt, const char *name) {
  if ( cxt->desc_ptr < 0 ) {
    fprintf(stderr, "outside of 'describe'\n");
    exit(1);
  }

  cxt->example_name = name;
  return 0;
}

int specc_finish_example(specc_Context *cxt)
{
  cxt->example_name = NULL;
  return 1;
}

sigjmp_buf specc_jmpbuf;

static void specc_signal_hander(int signum) {
  siglongjmp(specc_jmpbuf, signum);
}

int specc_initjmp(specc_Context *cxt)
{
  static const int HANDLED_SIGS[] = {
    SIGSEGV,
    SIGFPE,
    SIGPIPE
  };
  static const int NUM_HANDLED_SIGS = sizeof(HANDLED_SIGS) / sizeof(HANDLED_SIGS[0]);

  struct sigaction act;
  int i;
  for ( i = 0; i < NUM_HANDLED_SIGS; i++ ){

    sigemptyset(&act.sa_mask);
    act.sa_handler = specc_signal_hander;
    act.sa_flags = 0 | SA_RESETHAND;

    sigaction(HANDLED_SIGS[i], &act, NULL);
    //sigaction(SIGFPE, &act, NULL);
  }

  return 0;
}

/* expect */
void expect_that_body(const char *expr_str, int val) {

}

#define INITIAL_DESC_STACK_SIZE 10
void specc_setup(specc_Context *cxt){
  // initialize member of cxt
  cxt->desc_stack = calloc(sizeof(struct specc_DescStack), INITIAL_DESC_STACK_SIZE);
  cxt->desc_ptr = -1;
  cxt->desc_size = INITIAL_DESC_STACK_SIZE;

  if ( cxt->desc_stack == NULL ) {
    specc_internal_error("cannot allocate memory");
  }

  cxt->example_name = NULL;
}

int specc_teardown(specc_Context *cxt){
  return 0;
}
