#include <specc.h>

int answer = 42;

int add(int x, int y) {
  return x + y;
}

/**
 * This test code contains 3 examples, and all should success.
 */
specc_main {
  describe ("answer") {
    it ("is 42") {
      expect_that(answer == 42);
    }
  }

  describe ("add()") {
    context ("with 222 and 444") {
      it ("returns 666") {
        expect_that(add(222, 444) == 666);
      }
    }

    describe ("with 1") {
      context ("and 99") {
        it ("returns 100") {
          expect_that(add(1, 99) == 100);
        }
      }
    }
  }
}
