CC=clang
CFLAGS=-std=c99 -Wall -fPIC

.PHONY=all clean test

TARGET=libspecc.so

all: $(TARGET)

$(TARGET): specc.c specc.h
	$(CC) -shared $(CFLAGS) specc.c -o $@

clean:
	rm -f $(TARGET)

test: all
	@./run_test.sh
