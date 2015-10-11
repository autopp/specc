#include <specc.h>
#include <stdio.h>

#define STACK_SIZE 20
int stack[STACK_SIZE] = {0};
int sp = 0;

void push(int x) {
  stack[sp++] = x;
}

void pop() {
  sp--;
}

int sum_stack() {
  int r = 0;

  for (int i = 0; i < sp; i++) {
    r += stack[i];
  }

  return r;
}

specc {
  describe ("sum_stack()") {
    context ("when 1, 2 are on the stack") {
      before {
        push(1);
        push(2);
      }

      after {
        pop();
        pop();
      }

      it ("returns 3") {
        expect_that(sum_stack() == 3);
      }

      context ("and 3, 4 are also on the stack") {
        before {
          push(3);
          push(4);
        }

        after {
          pop();
          pop();
        }

        it ("returns 10") {
          expect_that(sum_stack() == 10);
        }
      }
      context ("and 5, 6 are also on the stack") {
        before {
          push(5);
          push(6);
        }

        after {
          pop();
          pop();
        }

        it ("returns 14") {
          expect_that(sum_stack() == 14);
        }
      }
    }
  }
}
