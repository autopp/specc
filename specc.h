#ifndef SPECC_H_
#define SPECC_H_

#ifndef SPECC_CONTXT_NAME
#define SPECC_CONTXT_NAME specc_cxt_
#endif

#define specc_cxt_ SPECC_CONTXT_NAME

#define specc_main \
  void specc_main(specc_Context_ *specc_cxt_); \
  \
  int main(void) { \
    specc_Context cxt; \
    specc_setup_(&cxt); \
    specc_main_(&cxt); \
    return specc_teardown_(&cxt); \
  } \
  \
  int specc_main_(specc_Context_ *specc_cxt_)

#endif
