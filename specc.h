#ifndef SPECC_H
#define SPECC_H

#include <setjmp.h>

#define specc_VERSION "0.1.0"

struct specc_Context;
typedef struct specc_Context specc_Context;

typedef void (*specc_BeforeFunc)(specc_Context *cxt);
typedef void (*specc_AfterFunc)(specc_Context *cxt);

typedef struct specc_DescStack {
  const char *target;
  int target_len;
  specc_BeforeFunc before_func;
  specc_AfterFunc after_func;
} specc_DescStack;

typedef enum specc_FailureType {
  specc_FAILURE_ERROR,
  specc_FAILURE_FIXED
} specc_FailureType;

typedef struct specc_Failure {
  specc_FailureType type;
  const char *full_name;
  const char *msg;
  const char *filename;
  int line;
} specc_Failure;

typedef struct specc_Pending {
  const char *full_name;
  const char *msg;
  const char *reason;
} specc_Pending;

struct specc_Context {
  specc_DescStack *desc_stack;
  int desc_ptr;
  int desc_size;

  int example_failed;
  const char *example;
  int example_len;
  int example_count;
  const char *example_filename;
  int example_line;

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

  // start time
  double start_time;
};

#ifndef SPECC_CONTXT_NAME
#define SPECC_CONTXT_NAME specc_cxt
#endif

#define specc_cxt SPECC_CONTXT_NAME

int specc_init_desc(specc_Context *cxt, const char *target, const char *filename, int line);
int specc_finish_desc(specc_Context *cxt);

/* describe */
#define describe(target)\
  for (int specc_desc_done = specc_init_desc(specc_cxt, target, __FILE__, __LINE__);\
        !specc_desc_done;\
        specc_desc_done = specc_finish_desc(specc_cxt) )

/**
 * Alias of `describe'
 * @param  target
 */
#define context(target) describe(target)

/* it */
int specc_init_example(specc_Context *cxt, const char *name, const char *filename, int line);
int specc_finish_example(specc_Context *cxt);
void specc_failure_example(specc_Context *cxt, int signum);
int specc_initjmp(specc_Context *cxt);
extern sigjmp_buf specc_jmpbuf;

#define it(name)\
  for (int specc_signum, specc_example_done = specc_init_example(specc_cxt, name, __FILE__, __LINE__);\
    !specc_example_done;\
    specc_example_done = specc_finish_example(specc_cxt) )\
    if (specc_initjmp(specc_cxt) && (specc_signum = sigsetjmp(specc_jmpbuf, 1))) {\
      specc_failure_example(specc_cxt, specc_signum);\
    }\
    else

/* expect */
void specc_expect_that(specc_Context *cxt, const char *expr_str, int val, const char *filename, int line);
#define expect_that(expr) specc_expect_that(specc_cxt, #expr, expr, __FILE__, __LINE__)

/* pending */
void specc_pending(specc_Context *cxt, const char *reason, const char *filename, int line);
#define pending(reason) (specc_pending(specc_cxt, (reason), __FILE__, __LINE__))

/* before */
void specc_store_before(specc_Context *cxt, specc_BeforeFunc func, const char *filename, int line);

#define before\
  auto void specc_before(specc_Context *cxt);\
  specc_store_before(specc_cxt, specc_before, __FILE__, __LINE__);\
  void specc_before(specc_Context *cxt)

/* after */
void specc_store_after(specc_Context *cxt, specc_AfterFunc func, const char *filename, int line);

#define after\
  auto void specc_after(specc_Context *cxt);\
  specc_store_after(specc_cxt, specc_after, __FILE__, __LINE__);\
  void specc_after(specc_Context *cxt)

void specc_setup(specc_Context *);
int specc_teardown(specc_Context *);

#define specc\
  void specc(specc_Context *specc_cxt);\
  \
  int main(void) {\
    specc_Context cxt;\
    specc_setup(&cxt);\
    specc(&cxt);\
    return specc_teardown(&cxt);\
  }\
  \
  void specc(specc_Context *specc_cxt)

#endif
