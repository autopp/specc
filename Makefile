CC=clang
CFLAGS=-Wall -pedantic -Werror -fPIC

.PHONY=all clean test

TARGET=libspecc.so

all: $(TARGET)

$(TARGET): specc.c specc.h specc_util.c specc_util.h
	$(CC) -shared $(CFLAGS) specc.c specc_util.c -o $@ 2>&1

clean:
	rm -f $(TARGET)

test: all
	@./run_test.sh
