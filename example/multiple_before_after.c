#include <specc.h>
#include <math.h>
#include <stdlib.h>
#include <stdio.h>

typedef struct {
  double x;
  double y;
} Point;

double distance(Point *p, Point *q) {
  return hypot(p->x - q->x, p->y - q->y);;
}

specc {
  describe ("distance") {
    context ("with (7, 10) and (3, 14)") {
      Point *p, *q;
      before {
        if ((p = malloc(sizeof(Point))) != NULL) {
          p->x = 7;
          p->y = 10;
        }
      }

      before {
        if ((q = malloc(sizeof(Point))) != NULL) {
          q->x = 3;
          q->y = 13;
        }
      }

      after {
        if (p != NULL) {
          free(p);
        }
      }

      after {
        if (q != NULL) {
          free(q);
        }
      }

      it ("returns 5") {
        expect_that(distance(p, q) == 5);
      }
    }
  }
}
