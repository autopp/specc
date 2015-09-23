# SpecC

SpecC: RSpec like lightweight test framework for C99

## Features

* RSpec like syntax (`describe` and `it`)
* Pending test with message
* Catch and report a signal such as SIGSEGV
* Colored output

Most features are inspired by the [RSpec](http://rspec.info/).

## Install

```bash
$ git clone https://github.com/autopp/specc.git
$ cd specc
$ make      # Create libspecc.so
$ export LD_LIBRARY_PATH=${LD_LIBRARY_PATH}:$(pwd)
$ make test # Run examples
```

When build success, `libspecc.so` should be created.

## Usage

### Writing

```c
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
    describe ("with 222 and 444") {
      it ("returns 666") {
        expect_that(add(222, 444) == 666);
      }
    }

    describe ("with 1") {
      describe ("and 99") {
        it ("returns 100") {
          expect_that(add(1, 99) == 100);
        }
      }
    }
  }
}
```

There are some examples in `example/`.

### Executing

```bash
$ cc -I/path/to/specc -L/path/to/specc --std=gnu99 sample_test.c -o sample_test -lspecc
$ ./sample_test
```

You should get output below:

![](result.png)

## More Examples

### Failed Test

When a test code is bellow:

```c
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
specc_main {
  describe ("fact()") {
    describe ("with 5") {
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
    describe ("with 40 and 2") {
      it ("returns 42") {
        // Pending example that should success
        pending("not implemeted");
        expect_that(add(40, 2) == 42);
      }
    }
  }
}
```

You get a output bellow:

![](./result_failed.png)

Note that SpecC handle SIGSEGV in the second example.

### Pending

When a test code is bellow:

```c
#include <specc.h>
#include <stdio.h>
#include <string.h>

int add(int x, int y) {
  return x + y;
}

// There are incomplete functions
int fact(int n) {
  // fake implemetation
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
    describe ("with 5") {
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
    describe ("with 40 and 2") {
      it ("returns 42") {
        expect_that(add(40, 2) == 42);
      }
    }
  }
}
```

You get a output bellow:

![](./result_pending.png)

Note that failed pending example do not affect exit status.

## TODO
* More assert functions.
* Show failure position in example.
* Command line options for handling the behavior of SpecC.
* RSpec's `before`/`after`

## Links
* [RSpec](http://rspec.info/): Inspired most syntax and feature.
* [Cello](http://libcello.org/): Many of the implementation technique was here in reference.

## Author
[@AuToPP](https://twitter.com/AuToPP)

## License
[MIT](LICENSE.txt)
