#ifndef SPECC_H_
#define SPECC_H_

typedef struct specc_DescStack_ {
  const char *target;
} specc_DescStack_;

typedef struct specc_Context_ {
  specc_DescStack_ *desc_stack;
  int desc_ptr;
  int desc_size;
  const char *example_name;
} specc_Context_;

#ifndef SPECC_CONTXT_NAME
#define SPECC_CONTXT_NAME specc_cxt_
#endif

#define specc_cxt_ SPECC_CONTXT_NAME

/* describe */
int specc_init_desc_(specc_Context_ *cxt, const char *target);
int specc_finish_desc(specc_Context_ *cxt);

#define describe(target) for ( int specc_desc_done_ = specc_init_desc_(specc_cxt_, target);\
                               !specc_describe_done_;\
                               specc_describe_done_ = specc_finish_desc(specc_cxt_) )\

void specc_setup_(specc_Context_ *);
int specc_teardown_(specc_Context_ *);

#define specc_main\
  void specc_main_(specc_Context_ *specc_cxt_);\
  \
  int main(void) {\
    specc_Context_ cxt;\
    specc_setup_(&cxt);\
    specc_main_(&cxt);\
    return specc_teardown_(&cxt);\
  }\
  \
  void specc_main_(specc_Context_ *specc_cxt_)

#endif
