CFLAGS=-Wall -pedantic -Werror -fPIC --std=gnu99 -c -MMD -MP -g
LDFLAGS=-shared
CC=gcc

.PHONY=all clean test valgrind

TARGET=libspecc.so
SRCS=specc.c rt_util.c before_after.c expect.c internal.c
OBJS=$(SRCS:%.c=%.o)
DEPDIR=dep
DEPS=$(SRCS:%.c=%.d)

$(TARGET): $(OBJS)
	$(CC) $(LDFLAGS) $^ -o $@ 2>&1

-include  $(DEPS)

%.o: %.c
	$(CC) $(CFLAGS) $<

all: clean $(TARGET)

clean:
	rm -f $(TARGET) $(OBJS) $(DEPS)

test: all
	@./run_test.sh

valgrind: all
	@./run_valgrind.sh
