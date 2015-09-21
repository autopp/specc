#ifndef SPECC_H
#define SPECC_H

#include <setjmp.h>

typedef struct specc_DescStack {
  const char *target;
  int target_len;
} specc_DescStack;

typedef enum specc_FailureType {
  specc_FAILURE_ERROR,
  specc_FAILURE_FIXED
} specc_FailureType;

typedef struct specc_Failure {
  specc_FailureType type;
  const char *full_name;
  const char *msg;
} specc_Failure;

typedef struct specc_Pending {
  const char *full_name;
  const char *msg;
  const char *reason;
} specc_Pending;

typedef struct specc_Context {
  specc_DescStack *desc_stack;
  int desc_ptr;
  int desc_size;
  int example_failed;
  const char *example;
  int example_len;
  int example_count;

  // failure logs
  specc_Failure *failures;
  int failures_size;
  int failure_count;
  const char *recent_failure_msg;

  // pending logs
  specc_Pending *pendings;
  int pendings_size;
  int pending_count;
  const char *pending_reason;
} specc_Context;

#ifndef SPECC_CONTXT_NAME
#define SPECC_CONTXT_NAME specc_cxt
#endif

#define specc_cxt SPECC_CONTXT_NAME

/* describe */
int specc_init_desc(specc_Context *cxt, const char *target);
int specc_finish_desc(specc_Context *cxt);

#define describe(target)\
  for (int specc_desc_done = specc_init_desc(specc_cxt, target);\
        !specc_desc_done;\
        specc_desc_done = specc_finish_desc(specc_cxt) )

/* it */
int specc_init_example(specc_Context *cxt, const char *name);
int specc_finish_example(specc_Context *cxt);
void specc_failure_example(specc_Context *cxt, int signum);
int specc_initjmp(specc_Context *cxt);
extern sigjmp_buf specc_jmpbuf;

#define it(name)\
  for (int specc_signum, specc_example_done = specc_init_example(specc_cxt, name);\
    !specc_example_done;\
    specc_example_done = specc_finish_example(specc_cxt) )\
    if (specc_initjmp(specc_cxt) && (specc_signum = sigsetjmp(specc_jmpbuf, 1))) {\
      specc_failure_example(specc_cxt, specc_signum);\
    }\
    else

/* expect */
void expect_that_body(specc_Context *cxt, const char *expr_str, int val);
#define expect_that(expr) expect_that_body(specc_cxt, #expr, expr)

void specc_setup(specc_Context *);
int specc_teardown(specc_Context *);

#define specc_main\
  void specc_main(specc_Context *specc_cxt);\
  \
  int main(void) {\
    specc_Context cxt;\
    specc_setup(&cxt);\
    specc_main(&cxt);\
    return specc_teardown(&cxt);\
  }\
  \
  void specc_main(specc_Context *specc_cxt)

/* pending */
void specc_pending(specc_Context *cxt, const char *reason);
#define pending(reason) (specc_pending(specc_cxt, (reason)))

#endif
