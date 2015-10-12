#include <specc.h>
#include <math.h>
#include <stdlib.h>
#include <stdio.h>

typedef struct {
  double x;
  double y;
} Point;

double distance(Point *p, Point *q) {
  return hypot(p->x - q->x, p->y - q->y);
}

specc {
  describe ("distance()") {
    context ("with (7, 10) and (3, 13)") {
      Point *p, *q;

      /*
       * Execution order: (A) -> (B) -> (E) -> (D) -> (C)
       */

      /* (A) */
      before {
        if ((p = malloc(sizeof(Point))) != NULL) {
          p->x = 7;
          p->y = 10;
        }
      }

      /* (B) */
      before {
        if ((q = malloc(sizeof(Point))) != NULL) {
          q->x = 3;
          q->y = 13;
        }
      }

      /* (C) */
      after {
        if (p != NULL) {
          free(p);
        }
      }

      /* (D) */
      after {
        if (q != NULL) {
          free(q);
        }
      }

      /* (E) */
      it ("returns 5") {
        expect_that(distance(p, q) == 5);
      }
    }
  }
}
