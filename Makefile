CC      = gcc
GIT_COMMIT := $(shell git rev-parse --short HEAD 2>/dev/null || echo "unknown")
GIT_AUTHOR := $(shell git log -1 --pretty=format:'%an' 2>/dev/null || echo "unknown")
CFLAGS = -Wall -Wextra -O2 -Iinc -Isrc/rfc6234 -DGIT_COMMIT=\"$(GIT_COMMIT)\" -DGIT_AUTHOR=\"$(GIT_AUTHOR)\"
TARGET  = mangen
SRCS    = src/main.c src/hash.c src/walk.c src/rfc6234/sha224-256.c src/regexp.c
OBJS    = $(SRCS:.c=.o)


all: $(TARGET)

$(TARGET): $(OBJS)
	$(CC) $(CFLAGS) -o $@ $^

%.o: %.c
	$(CC) $(CFLAGS) -c -o $@ $<

clean:
	rm -f $(OBJS) $(TARGET)

test: $(TARGET)
	@./tests/run_tests.sh
doc:
	doxygen Doxyfile

.PHONY: all clean test doc