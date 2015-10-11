#include <specc.h>
#include <stdio.h>
#include <string.h>

int add(int x, int y) {
  return x + y;
}

// There are incomplete functions
int fact(int n) {
  // fale implemetation
  return -1;
}

char *copy_hello(char *s) {
  s = NULL; // This is BUG
  sprintf(s, "hello world");
  return s;
}

// This test code contains 3 examples and 2 pending, 1 success
specc_main {
  describe ("fact()") {
    context ("with 5") {
      it ("returns 120") {
        pending("not implemeted");
        expect_that(fact(5) == 120);
      }
    }
  }

  describe ("copy_hello()") {
    it ("returns \"hello world\"") {
      char s[20];
      pending("now debugging");
      expect_that(strcmp(copy_hello(s), "hello world") == 0);
    }
  }

  describe ("add()") {
    context ("with 40 and 2") {
      it ("returns 42") {
        expect_that(add(40, 2) == 42);
      }
    }
  }
}
