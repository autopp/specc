#include <specc.h>
#include <stdio.h>
#include <string.h>

define_matcher(eq_int, int actual, int expected) {
  return actual == expected;
}

define_matcher(eq_str, const char *actual, const char *expected) {
  return strcmp(actual, expected) == 0;
}

specc {

  describe ("41 + 1") {
    it ("evaluates to 42") {
      expect_not_to(41 + 1, eq_int, 42);
    }
  }

  describe ("sprintf(buf, \"%03d\", 42)") {
    it ("should be \"042\"") {
      char buf[32];
      sprintf(buf, "%03d", 42);
      expect_to(buf, eq_str, "042");
    }
  }
}
