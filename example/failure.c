#include <specc.h>
#include <stdio.h>
#include <string.h>

int add(int x, int y) {
  return x + y;
}

// There are incomplete functions
int fact(int n) {
  if (n > 0) {
    return fact(n - 1) * n;
  } else {
    // This is BUG!
    return 0;
  }
}

char *copy_hello(char *s) {
  s = NULL; // This is BUG
  sprintf(s, "hello world");
  return s;
}

/**
 * This test code contains 2 examples and all should fail
 */
specc {
  describe ("fact()") {
    context ("with 5") {
      it ("returns 120") {
        expect_that(fact(5) == 120);
      }
    }
  }

  describe ("copy_hello()") {
    it ("returns \"hello world\"") {
      char s[20];
      expect_that(strcmp(copy_hello(s), "hello world") == 0);
    }
  }

  describe ("add()") {
    context ("with 40 and 2") {
      it ("returns 42") {
        // Pending example that should success
        pending("not implemeted");
        expect_that(add(40, 2) == 42);
      }
    }
  }
}
