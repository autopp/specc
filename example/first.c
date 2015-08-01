#include <specc.h>
#include <stdio.h>

int inc(int x){
  return x + 1;
}

specc_main {
  describe("inc"){
    describe("with 41"){
      it("returns 42"){
        expect_that(inc(41) == 42);
      }
    }
  }
}
