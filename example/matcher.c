#include <specc.h>

specc {
  import_builtin_matchers;

  describe ("41 + 1") {
    it ("evaluates to 42") {
      expect_not_to.eq_int(41 + 1, 42);
    }
  }

  describe ("sprintf(buf, \"%03d\", 42)") {
    it ("should be \"042\"") {
      char buf[32];
      sprintf(buf, "%03d", 42);
      expect_to.eq_str(buf, "042");
      expect
    }
  }
}
