CFLAGS=-Wall -pedantic -Werror -fPIC --std=gnu99
CC=gcc

.PHONY=all clean test valgrind

TARGET=libspecc.so

all: $(TARGET)

$(TARGET): specc.c specc.h specc_util.c specc_util.h
	$(CC) -shared $(CFLAGS) specc.c specc_util.c -o $@ 2>&1

clean:
	rm -f $(TARGET)

test: all
	@./run_test.sh

valgrind: all
	@./run_valgrind.sh
