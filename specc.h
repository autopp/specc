#ifndef SPECC_H_
#define SPECC_H_

typedef struct specc_Context_ {

} specc_Context_;

#ifndef SPECC_CONTXT_NAME
#define SPECC_CONTXT_NAME specc_cxt_
#endif

#define specc_cxt_ SPECC_CONTXT_NAME

void specc_setup_(specc_Context_ *);
int specc_teardown_(specc_Context_ *);

#define specc_main \
  void specc_main_(specc_Context_ *specc_cxt_); \
  \
  int main(void) { \
    specc_Context_ cxt; \
    specc_setup_(&cxt); \
    specc_main_(&cxt); \
    return specc_teardown_(&cxt); \
  } \
  \
  void specc_main_(specc_Context_ *specc_cxt_)

#endif
