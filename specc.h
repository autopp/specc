#ifndef SPECC_H
#define SPECC_H

#include <setjmp.h>

typedef struct specc_DescStack {
  const char *target;
} specc_DescStack;

typedef struct specc_Context {
  specc_DescStack *desc_stack;
  int desc_ptr;
  int desc_size;
  const char *example_name;
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
int specc_initjmp(specc_Context *cxt);
extern sigjmp_buf specc_jmpbuf;

#define it(name)\
  for (int specc_signum, specc_example_done = specc_init_example(specc_cxt, name);\
    !specc_example_done;\
    specc_example_done = specc_finish_example(specc_cxt) )\
    if ((specc_signum = (specc_initjmp(specc_cxt), sigsetjmp(specc_jmpbuf, 1)))) {\
      fprintf(stderr, "catch signal %d\n", specc_signum);\
    }\
    else

/* expect */
void expect_that_body(const char *expr_str, int val);
#define expect_that(expr) expect_that_body(#expr, expr)

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

#endif
