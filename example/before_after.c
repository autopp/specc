#include <specc.h>
#include <stdlib.h>
#include <string.h>

typedef struct {
  const char *name;
  int age;
} *Person;

Person new_person(const char *name, int age) {
  Person p = malloc(sizeof(Person));

  p->name = name;
  p->age = age;

  return p;
}

#define get_person_name(p) ((p)->name)

#define get_person_age(p) ((p)->age)

void delete_person(Person p) {
  free(p);
}

specc {
  describe ("Person") {
    context ("with \"Jhon\" and 42") {
      Person person;

      before {
        // This code is executed before each example
        person = new_person("Jhon", 42);
      }

      after {
        // This code is executed after each example
        delete_person(person);
      }

      describe ("get_person_name()") {
        it ("returns \"Jhon\"") {
          expect_that(strcmp(get_person_name(person), "Jhon") == 0);
        }
      }

      describe ("get_person_age()") {
        it ("returns 42") {
          expect_that(get_person_age(person) == 42);
        }
      }
    }
  }
}