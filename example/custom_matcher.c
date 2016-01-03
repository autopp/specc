#include <specc.h>

define_matcher(be_either, int actual, int a, int b) {
  return actual == a || actual == b;
}

define_matcher(be_zero, int actual) {
  return actual == 0;
}

specc {
  describe ("41 + 1") {
    it ("is 42 or 43") {
      expect_to(41 + 1, be_either, 42, 43);
    }

    it ("is not 43 and 44") {
      expect_not_to(41 + 1, be_either, 43, 44);
    }

    it ("is 41 or 43") {
      pending("dummy example");
      expect_to(41 + 1, be_either, 41, 43);
    }

    it ("is not 42 and 43") {
      pending("dummy example");
      expect_not_to(41 + 1, be_either, 42, 43);
    }

    it ("is not 0") {
      expect_not_to(41 + 1, be_zero);
    }
  }
}
