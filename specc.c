#include <stdio.h>
#include <stdarg.h>
#include <stdlib.h>
#include <signal.h>
#include <setjmp.h>
#include <string.h>

#include "specc.h"
#include "specc_util.h"

int specc_init_desc(specc_Context *cxt, const char *target) {
  if (cxt->example_name != NULL) {
    specc_internal_error("cannot nest 'describe' in 'it'");
  }

  // push desc stack
  cxt->desc_ptr++;

  if (cxt->desc_ptr >= cxt->desc_size) {
    // reallocate
    int new_size = cxt->desc_size * 2;
    if (new_size <= cxt->desc_size) {
      specc_internal_error("cannnot expand stack of 'describe'");
    }

    specc_DescStack *new_stack = realloc(cxt->desc_stack, sizeof(specc_DescStack) * new_size);
    if (new_stack == NULL) {
      specc_internal_error("cannot expand stack of 'describe'");
    }

    cxt->desc_stack = new_stack;
    cxt->desc_size = new_size;
  }
  cxt->desc_stack[cxt->desc_ptr].target = target;
  cxt->desc_stack[cxt->desc_ptr].target_len = strlen(target);
  specc_printfln_indented(cxt->desc_ptr, "%s", target);

  return 0;
}

int specc_finish_desc(specc_Context *cxt) {
  cxt->desc_ptr--;
  return 1;
}

int specc_init_example(specc_Context *cxt, const char *name) {
  if (cxt->desc_ptr < 0 ) {
    specc_internal_error("outside of 'describe'");
  }

  cxt->example_name = name;
  cxt->example_len = strlen(name);
  cxt->example_count += 1;
  specc_printfln_indented(cxt->desc_ptr + 1, "%s", name);

  return 0;
}

int specc_finish_example(specc_Context *cxt)
{
  cxt->example_name = NULL;
  return 1;
}

sigjmp_buf specc_jmpbuf;

static void specc_signal_hander(int signum) {
  // jump to the nearst `it' position
  siglongjmp(specc_jmpbuf, signum);
}

int specc_initjmp(specc_Context *cxt)
{
  // signals that specc handling
  static const int HANDLED_SIGS[] = {
    SIGSEGV,
    SIGFPE,
    SIGPIPE
  };
  static const int NUM_HANDLED_SIGS = sizeof(HANDLED_SIGS) / sizeof(HANDLED_SIGS[0]);

  struct sigaction act;
  int i;
  for (i = 0; i < NUM_HANDLED_SIGS; i++) {
    // Set hook signals
    sigemptyset(&act.sa_mask);
    act.sa_handler = specc_signal_hander;
    act.sa_flags = 0 | SA_RESETHAND;

    sigaction(HANDLED_SIGS[i], &act, NULL);
  }

  return 1;
}

/* expect */
void expect_that_body(specc_Context *cxt, const char *expr_str, int val) {
  if (!val) {
    // create error message
    int i, size = 0;
    for (i = 0; i <= cxt->desc_ptr; i++) {
      size += cxt->desc_stack[i].target_len + 1;
    }
    size += cxt->example_len;

    char *position = malloc(sizeof(char) * (size + 1));
    if (position == NULL) {
      specc_internal_error("cannot allocate memory");
    }

    char *ptr = position;
    for (i = 0; i <= cxt->desc_ptr; i++) {
      ptr += sprintf(ptr, "%s ", cxt->desc_stack[i].target);
    }
    sprintf(ptr, "%s", cxt->example_name);

    size = snprintf(NULL, 0, "The condition `%s' failed", expr_str);
    char *msg = malloc(sizeof(char) * (size + 1));
    snprintf(msg, size + 1, "The condition `%s' failed", expr_str);

    if (cxt->failure_count == cxt->failures_size) {
      int new_size = cxt->failures_size * 2;
      if (new_size <= cxt->failures_size) {
        specc_internal_error("cannot expand failure logs");
      }

      specc_Failure *new_failures = realloc(cxt->failures, sizeof(specc_Failure) * new_size);

      if (new_failures == NULL) {
        specc_internal_error("cannot expand failure logs");
      }

      cxt->failures = new_failures;
      cxt->failures_size = new_size;
    }

    cxt->failures[cxt->failure_count].position = position;
    cxt->failures[cxt->failure_count].msg = msg;

    cxt->failure_count++;
  }
}

#define INITIAL_DESC_STACK_SIZE 10
#define INITIAL_FAILURES_SIZE 10
void specc_setup(specc_Context *cxt){
  // initialize member of cxt
  cxt->desc_stack = calloc(sizeof(struct specc_DescStack), INITIAL_DESC_STACK_SIZE);
  cxt->desc_ptr = -1;
  cxt->desc_size = INITIAL_DESC_STACK_SIZE;

  if (cxt->desc_stack == NULL ) {
    specc_internal_error("cannot allocate memory");
  }

  cxt->example_name = NULL;
  cxt->example_count = 0;

  cxt->failures = calloc(sizeof(struct specc_Failure), INITIAL_FAILURES_SIZE);
  cxt->failures_size = INITIAL_FAILURES_SIZE;
  cxt->failure_count = 0;

  if (cxt->failures == NULL) {
    specc_internal_error("cannot allocate memory");
  }
}

int specc_teardown(specc_Context *cxt){
  int i = 0;

  if (cxt->failure_count > 0) {
    specc_printfln_indented(0, "Failures:");
  }

  for (i = 0; i < cxt->failure_count; i++) {
    puts("");
    specc_printfln_indented(1, "%d) %s", i + 1, cxt->failures[i].position);
    specc_printfln_indented(2, "Failure/Error: %s", cxt->failures[i].msg);
  }

  specc_printfln("");
  specc_printfln("%d examples, %d falures", cxt->example_count, cxt->failure_count);

  return cxt->failure_count != 0;
}
