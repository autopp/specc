# SpecC

SpecC: RSpec like lightwaight test framework for C99

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

## TODO
* More assert functions.
* Show failure position in example.
* Command line options for handling the behavior of SpecC.

## Author
[@AuToPP](https://twitter.com/AuToPP)

## License
[MIT](LICENSE.txt)
