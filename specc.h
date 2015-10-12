#ifndef SPECC_H
#define SPECC_H

#include <stddef.h>
#include <setjmp.h>

/**
 * Version string of SpecC
 */
#define specc_VERSION "0.2.0"

struct specc_Context;
typedef struct specc_Context specc_Context;

/**
 * Type of `before' function
 * @param  cxt
 */
typedef void (*specc_BeforeFunc)(specc_Context *cxt);

/**
 * Type of `after' function
 * @param  cxt
 */
typedef void (*specc_AfterFunc)(specc_Context *cxt);

/**
 * Internal stack frame for `describe' block
 */
typedef struct specc_DescStack {
  const char *target; /// Name of `describe'
  int target_len; /// length of `target'
  specc_BeforeFunc *before_funcs; /// Registered `before' functions
  size_t before_funcs_size; /// Size of before_funcs
  size_t before_func_count; /// Count of registered `before' functions
  specc_AfterFunc *after_funcs; /// Registered `before' functions
  size_t after_funcs_size; /// Size of after_funcs
  size_t after_func_count; /// Count of registered `after' functions
} specc_DescStack;

/**
 * Type of failure
 */
typedef enum specc_FailureType {
  specc_FAILURE_ERROR, /// Fail by some error
  specc_FAILURE_FIXED /// Fail by no error in pending example
} specc_FailureType;

/**
 * Infomation of faiied example
 */
typedef struct specc_Failure {
  specc_FailureType type; // Failure type
  const char *full_name; // Full name of the example failed
  const char *msg; // Failure message
  const char *filename; // File name that example is written
  int line; // Line number that example is written
} specc_Failure;

/**
 * infomation of failed pending example
 */
typedef struct specc_Pending {
  const char *full_name; /// Full name of the pending example
  const char *msg; /// Error message
  const char *reason; /// Reason for pending
} specc_Pending;

/**
 * Context of SpecC
 */
struct specc_Context {
  specc_DescStack *desc_stack; /// Stack of `describe'
  int desc_ptr; /// stack pointer of desc_stack (-1 if empty)
  int desc_size; /// size of desc_stack

  int example_failed; /// Count of failures
  const char *example; /// current example's name (If NULL, scope is not `it')
  int example_len; /// length of example
  int example_count; /// Count of example
  const char *example_filename; /// File name that current example is written
  int example_line; /// Line number that current example is written

  // failure logs
  specc_Failure *failures; /// Array of occured failures
  int failures_size; /// Size of failures
  int failure_count; /// Count of falure
  const char *recent_failure_msg;

  // pending logs
  specc_Pending *pendings; /// Array of failed pending examples
  int pendings_size; /// size of pendings
  int pending_count; /// Count of failed pending examples
  const char *pending_reason; /// reason for current pending (if NULL, not pending)

  // start time
  double start_time; /// Start time second of test
};

/**
 * Global buffer for sigsetjmp/siglongjmp that SpecC use
 */
extern sigjmp_buf specc_jmpbuf;

/**
 * Initialize `describe' block
 * @param  cxt
 * @param  target   The described target's name
 * @param  filename File name that `describe' written
 * @param  line     Line number that `describe' written
 * @return          0
 */
int specc_init_desc(specc_Context *cxt, const char *target, const char *filename, int line);

/**
 * Finalize `describe' block
 * @param  cxt
 * @return     1
 */
int specc_finish_desc(specc_Context *cxt);

/**
 * Entry of `describe' block
 * @param  target The described  target's name
 */
#define describe(target)\
  for (int specc_desc_done = specc_init_desc(specc_cxt, target, __FILE__, __LINE__);\
        !specc_desc_done;\
        specc_desc_done = specc_finish_desc(specc_cxt) )

/**
 * Alias of `describe'
 * @param  target
 */
#define context(target) describe(target)

/**
 * Initialize `it' block
 * @param  cxt
 * @param  name     Name of example
 * @param  filename File name that `it' written
 * @param  line     Line number that `it' written
 * @return          1
 */
int specc_init_example(specc_Context *cxt, const char *name, const char *filename, int line);

/**
 * Finalize `it' block
 * @param  cxt
 * @return     0
 */
int specc_finish_example(specc_Context *cxt);

/**
 * Fail current example explicitly
 * @param cxt
 * @param signum Signal number of causes (If <= 0, Signal is not cause)
 */
void specc_fail_example(specc_Context *cxt, int signum);

/**
 * Initilize specc_jmpbuf
 * @param  cxt
 * @return     1
 */
int specc_initjmp(specc_Context *cxt);

/**
 * Entry of `it' block
 * @param  name Name of example
 */
#define it(name)\
  for (int specc_signum, specc_example_done = specc_init_example(specc_cxt, name, __FILE__, __LINE__);\
    !specc_example_done;\
    specc_example_done = specc_finish_example(specc_cxt))\
    if (specc_initjmp(specc_cxt) && (specc_signum = sigsetjmp(specc_jmpbuf, 1))) {\
      specc_fail_example(specc_cxt, specc_signum);\
    }\
    else

/**
 * Assert val is true
 * @param cxt
 * @param expr_str source that yield val
 * @param val      A value expected true
 * @param filename File name that written `expect_that'
 * @param line     Line number that written `expect_that'
 */
void specc_expect_that(specc_Context *cxt, const char *expr_str, int val, const char *filename, int line);

/**
 * Entry of `expect_that'
 * @param  expr target of assert
 */
#define expect_that(expr) specc_expect_that(specc_cxt, #expr, expr, __FILE__, __LINE__)

/**
 * Pending current example
 * @param cxt
 * @param reason   Reason of pending
 * @param filename File name that written `pending'
 * @param line     Line number that written `pending'
 */
void specc_pending(specc_Context *cxt, const char *reason, const char *filename, int line);

/**
 * Entry of `pending'
 * @param reason Reason of pending
 */
#define pending(reason) (specc_pending(specc_cxt, (reason), __FILE__, __LINE__))

/**
 * Store declarated `before' function to context
 * @param cxt
 * @param func     `before' function
 * @param filename File name that written `before' (For error message)
 * @param line     Line number that written `before' (For error message)
 */
void specc_store_before(specc_Context *cxt, specc_BeforeFunc func, const char *filename, int line);

/**
 * Entry of `before' block
 */
#define before\
  auto void specc_before(specc_Context *cxt);\
  specc_store_before(specc_cxt, specc_before, __FILE__, __LINE__);\
  void specc_before(specc_Context *cxt)

/**
 * Store declarated `after' function to context
 * @param cxt
 * @param func     `after' function
 * @param filename File name that written `after' (For error message)
 * @param line     Line number that written `after' (For error message)
 */
void specc_store_after(specc_Context *cxt, specc_AfterFunc func, const char *filename, int line);

/**
 * Entry of `after' block
 */
#define after\
  auto void specc_after(specc_Context *cxt);\
  specc_store_after(specc_cxt, specc_after, __FILE__, __LINE__);\
  void specc_after(specc_Context *cxt)

/**
 * Setup SpecC (Called once)
 */
void specc_setup(specc_Context *specc_cxt);

/**
 * Teardown SpecC (Called once)
 */
int specc_teardown(specc_Context *specc_cxt);

/**
 * Main function
 * @param specc_cxt
 */
void specc_main(specc_Context *specc_cxt);

/**
 * Main entry of test code
 */
#define specc\
  int main(void) {\
    specc_Context cxt;\
    specc_setup(&cxt);\
    specc_main(&cxt);\
    return specc_teardown(&cxt);\
  }\
  \
  void specc_main(specc_Context *specc_cxt)

#endif
