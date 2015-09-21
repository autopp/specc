#include <specc.h>
#include <stdio.h>

int inc(int x){
  return x + 1;
}

int n = 0;
int get_n(){
  return n;
}

int echo(int x){
  return x;
}

/**
 * This test code contains 4 examples, and 3 examples should fail.
 */
specc_main {
  describe("inc"){
    describe("with 41"){
      it("returns 42"){
        expect_that(inc(41) == 42);
      }

      it("returns 40"){
        expect_that(inc(41) == 41);
      }

      it("raise SIGFPE"){
        int x = 10 / get_n();
        fprintf(stderr, "x = %d\n", 100 / echo(0));
      }

      it("raise SIGSEGV"){
        pending("for SIGSEGV");
        int *x = NULL;
        *x = 100;
      }
    }
    describe("with 42") {
      it("returns 43") {
        expect_that(inc(42) == 43);
      }
    }
  }

  describe("atoi(\"42\")") {
    it ("returns 42") {
      pending("not implemeted");
      expect_that(atoi("42") == 42);
    }
  }
}
