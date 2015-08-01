CC=clang
CFLAGS=-std=c99 -Wall -fPIC

.PHONY=all clean

TARGET=libspecc.so

all: $(TARGET)

$(TARGET): specc.c specc.h
	$(CC) -shared $(CFLAGS) specc.c -o $@

clean:
	rm -f $(TARGET)
