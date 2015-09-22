#include <stdio.h>
#include <stdarg.h>
#include <stdlib.h>
#include <signal.h>
#include <setjmp.h>
#include <string.h>

#include "specc.h"
#include "specc_util.h"

static void specc_add_failure(specc_Context *cxt, specc_FailureType type, const char *msg);

int specc_init_desc(specc_Context *cxt, const char *target) {
  if (cxt->example != NULL) {
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
  specc_printfln_indented(cxt->desc_ptr * 2, "%s", target);

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

  cxt->example_failed = 0;
  cxt->example = name;
  cxt->example_len = strlen(name);
  cxt->example_count += 1;
  cxt->pending_reason = NULL;

  return 0;
}

int specc_finish_example(specc_Context *cxt)
{
  if (cxt->example_failed) {
    if (cxt->pending_reason != NULL) {
      specc_cprintfln_indented(specc_YELLOW, (cxt->desc_ptr + 1) * 2, "%s (PENDING: %s)", cxt->example, cxt->pending_reason);
    } else {
      specc_cprintfln_indented(specc_RED, (cxt->desc_ptr + 1) * 2, "%s (FAILED - %d)", cxt->example, cxt->failure_count);
    }
  } else {
    if (cxt->pending_reason != NULL) {
      // fixed
      const char *msg = specc_saprintf("Expected pending `%s' to fail. No Error was raised.", cxt->pending_reason);
      specc_add_failure(cxt, specc_FAILURE_FIXED, msg);
      specc_cprintfln_indented(specc_RED, (cxt->desc_ptr + 1) * 2, "%s (FAILED - %d)", cxt->example, cxt->failure_count);
    } else {
      specc_cprintfln_indented(specc_GREEN, (cxt->desc_ptr + 1) * 2, "%s", cxt->example);
    }
  }
  cxt->example = NULL;
  cxt->pending_reason = NULL;
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

#define SIGNUM_TO_NAME(num) ((num) == SIGSEGV ? "SIGSEGV" : ((num) == SIGFPE ? "SIGFPE" : ((num) == SIGPIPE ? "SIGPIPE" : NULL)))

static const char *specc_full_example_name(specc_Context *cxt) {
  // Store full name of the failed example
  int i, size = 0;
  for (i = 0; i <= cxt->desc_ptr; i++) {
    size += cxt->desc_stack[i].target_len + 1;
  }
  size += cxt->example_len;

  char *full_name = malloc(sizeof(char) * (size + 1));
  if (full_name == NULL) {
    specc_internal_error("cannot allocate memory");
  }

  char *ptr = full_name;
  for (i = 0; i <= cxt->desc_ptr; i++) {
    ptr += sprintf(ptr, "%s ", cxt->desc_stack[i].target);
  }
  sprintf(ptr, "%s", cxt->example);

  return full_name;
}

static void specc_add_failure(specc_Context *cxt, specc_FailureType type, const char *msg) {
  // expand failure logs if needed
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

  cxt->failures[cxt->failure_count].type = type;
  cxt->failures[cxt->failure_count].full_name = specc_full_example_name(cxt);
  cxt->failures[cxt->failure_count].msg = msg;
  cxt->failure_count++;
}

static void specc_add_pending(specc_Context *cxt, const char *msg) {
  // expand pending logs if needed
  if (cxt->pending_count == cxt->pendings_size) {
    int new_size = cxt->pendings_size * 2;
    if (new_size <= cxt->pendings_size) {
      specc_internal_error("cannot expand pending logs");
    }

    specc_Pending *new_pendings = realloc(cxt->pendings, sizeof(specc_Pending) * new_size);

    if (new_pendings == NULL) {
      specc_internal_error("cannot expand pending logs");
    }

    cxt->pendings = new_pendings;
    cxt->pendings_size = new_size;
  }

  cxt->pendings[cxt->pending_count].full_name = specc_full_example_name(cxt);
  cxt->pendings[cxt->pending_count].msg = msg;
  cxt->pendings[cxt->pending_count].reason = cxt->pending_reason;
  cxt->pending_count++;
}

void specc_failure_example(specc_Context *cxt, int signum) {
  // switch failure flag
  cxt->example_failed = 1;

  // Create the failure message
  const char *msg;
  if (signum > 0) {
    // Signal case
    const char *signame = SIGNUM_TO_NAME(signum);

    if (signame == NULL) {
      msg = specc_saprintf("signal %d raised", signum);
    } else {
      msg = specc_saprintf("%s raised", signame);
    }
  } else {
    msg = cxt->recent_failure_msg;
  }

  if (cxt->pending_reason == NULL) {
    specc_add_failure(cxt, specc_FAILURE_ERROR, msg);
  } else {
    specc_add_pending(cxt, msg);
  }
}

/* expect */
void specc_expect_that(specc_Context *cxt, const char *expr_str, int val) {
  if (!val) {
    cxt->recent_failure_msg = specc_saprintf("The condition `%s' failed", expr_str);

    siglongjmp(specc_jmpbuf, -1);
  }
}

/* pending */
void specc_pending(specc_Context *cxt, const char *reason) {
  cxt->pending_reason = reason;
}

#define INITIAL_DESC_STACK_SIZE 10
#define INITIAL_FAILURES_SIZE 10
#define INITIAL_PENDINGS_SIZE 10
void specc_setup(specc_Context *cxt){
  // initialize member of cxt
  cxt->desc_stack = calloc(sizeof(struct specc_DescStack), INITIAL_DESC_STACK_SIZE);
  cxt->desc_ptr = -1;
  cxt->desc_size = INITIAL_DESC_STACK_SIZE;

  if (cxt->desc_stack == NULL ) {
    specc_internal_error("cannot allocate memory");
  }

  cxt->example = NULL;
  cxt->example_count = 0;

  cxt->failures = calloc(sizeof(struct specc_Failure), INITIAL_FAILURES_SIZE);
  cxt->failures_size = INITIAL_FAILURES_SIZE;
  cxt->failure_count = 0;

  if (cxt->failures == NULL) {
    specc_internal_error("cannot allocate memory");
  }

  cxt->pendings = calloc(sizeof(struct specc_Pending), INITIAL_PENDINGS_SIZE);
  cxt->pendings_size = INITIAL_PENDINGS_SIZE;
  cxt->pending_count = 0;

  if (cxt->pendings == NULL) {
    specc_internal_error("cannot allocate memory");
  }
}

int specc_teardown(specc_Context *cxt){
  int i;

  // output detail of pending
  if (cxt->pending_count > 0) {
    specc_newline();
    specc_printfln("Pending: (Failures listed here are expected and do not affect your suite's status)");
    specc_newline();
  }

  int info_indent = 5;
  for (i = 0; i < cxt->pending_count; i++) {
    specc_Pending *p = cxt->pendings + i;

    if (i % 10 == 9) {
      info_indent++;
    }

    specc_printfln_indented(2, "%d) %s", i + 1, p->full_name);
    specc_cprintfln_indented(specc_CYAN, info_indent, "# %s", p->reason);
    specc_cprintfln_indented(specc_YELLOW, info_indent, "Failure/Error: %s", p->msg);
  }

  // output detail of failures
  if (cxt->failure_count > 0) {
    specc_newline();
    specc_printfln("Failures:");
  }

  info_indent = 5;
  for (i = 0; i < cxt->failure_count; i++) {
    specc_Failure *failure = cxt->failures + i;

    if (i % 10 == 9) {
      info_indent++;
    }

    specc_newline();
    switch (failure->type) {
    case specc_FAILURE_ERROR:
      specc_printfln_indented(2, "%d) %s", i + 1, failure->full_name);
      specc_cprintfln_indented(specc_RED, info_indent, "Failure/Error: %s", failure->msg);
      break;

    case specc_FAILURE_FIXED:
      specc_printfln_indented(2, "%d) %s FIXED", i + 1, failure->full_name);
      specc_cprintfln_indented(specc_BLUE, info_indent, "%s", failure->msg);
      break;

    default:
      specc_internal_error("unknown failure type %d", failure->type);
    }
  }

  specc_printfln("");
  int color = cxt->failure_count > 0 ? specc_RED : (cxt->pending_count > 0 ? specc_YELLOW : specc_GREEN);

  if (cxt->pending_count > 0) {
    specc_cprintfln(color, "%d examples, %d failures, %d pending", cxt->example_count, cxt->failure_count, cxt->pending_count);
  } else {
    specc_cprintfln(color, "%d examples, %d failures", cxt->example_count, cxt->failure_count);
  }

  return cxt->failure_count != 0;
}
