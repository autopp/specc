#include "specc.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdarg.h>

static const char *specc_default_failure_msg(
  int expected, const char *actual_expr, const char *matcher_name,
  int argc, const char *arg_strs[]
) {
  // calculate size of massage buffer
  size_t msg_size = 0;
  msg_size += strlen("expected ") + strlen(actual_expr);
  if (!expected) {
    msg_size += strlen(" not");
  }

  msg_size += strlen(" to ") + strlen(matcher_name);

  switch (argc) {
  case 0:
    // do nothing
    break;
  case 1:
    msg_size += strlen(" ") + strlen(arg_strs[0]);
    break;
  default:
    for (int i = 0; i < argc - 1; i++) {
      msg_size += strlen(" ") + strlen(arg_strs[i]) + strlen(",");
    }
    msg_size += strlen(" and ") + strlen(arg_strs[argc - 1]);
    break;
  }
  // generate message
  char *msg = malloc(sizeof(char) * (msg_size + 1));

  if (msg == NULL) {
    specc_internal_error("cannot allocate memory");
  }

  char *write_pos = msg;

  write_pos += sprintf(write_pos, "expected %s", actual_expr);
  if (!expected) {
    write_pos += sprintf(write_pos, " not");
  }

  write_pos += sprintf(write_pos, " to %s", matcher_name);

  switch (argc) {
  case 0:
    // do nothing
    break;
  case 1:
    sprintf(write_pos, " %s", arg_strs[0]);
    break;
  default:
    for (int i = 0; i < argc - 1; i++) {
      write_pos += sprintf(write_pos, " %s,", arg_strs[i]);
    }
    sprintf(write_pos, " and %s", arg_strs[argc - 1]);
    break;
  }

  return msg;
}

int specc_init_expect(
  specc_Context *cxt, const char *actual_expr, const char *matcher_name,
  int matcher_arg_count, ...
) {
  cxt->actual_expr = actual_expr;
  cxt->matcher_name = matcher_name;
  cxt->matcher_arg_count = matcher_arg_count;

  const char **matcher_arg_exprs = malloc(sizeof(const char *) * matcher_arg_count);
  if (matcher_arg_exprs == NULL) {
    specc_internal_error("cannot allocate memory");
  }

  va_list vargs;
  va_start(vargs, matcher_arg_count);
  for (int i = 0; i < matcher_arg_count; i++) {
    matcher_arg_exprs[i] = va_arg(vargs, const char *);
  }
  va_end(vargs);

  cxt->matcher_arg_exprs = matcher_arg_exprs;

  // reset failure messages
  cxt->failure_msg = cxt->failure_msg_when_negated = NULL;
  return 0;
}

int specc_finish_expect(specc_Context *cxt) {
  free(cxt->matcher_arg_exprs);

  if (cxt->recent_failure_msg != NULL) {
    siglongjmp(specc_jmpbuf, -1);
  }
  return 1;
}

void specc_expect_to(specc_Context *cxt, int expected, int evaluated) {
  if (expected != !!evaluated) {
    const char *failure_msg = expected ? cxt->failure_msg : cxt->failure_msg_when_negated;

    if (failure_msg == NULL) {
      failure_msg = specc_default_failure_msg(
        expected, cxt->actual_expr, cxt->matcher_name,
        cxt->matcher_arg_count, cxt->matcher_arg_exprs);
    }

    printf("failure_msg = %p\n", failure_msg);

    cxt->recent_failure_msg = failure_msg;
  }
}
