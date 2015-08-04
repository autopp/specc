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

specc_main {
  describe("inc"){
    describe("with 41"){
      it("returns 42"){
        expect_that(inc(41) == 42);
      }

      it("raise FPE"){
        int x = 10 / get_n();
        fprintf(stderr, "x = %d\n", 100 / echo(0));
      }

      it("raise SEGV"){
        int *x = NULL;
        *x = 100;
      }

    }
  }

  printf("done\n");
}
