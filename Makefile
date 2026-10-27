CC          = gcc
BUILD_DIR   = build

GIT_COMMIT := $(shell git rev-parse --short HEAD 2>/dev/null || echo "unknown")
GIT_AUTHOR := $(shell git log -1 --pretty=format:'%an' 2>/dev/null || echo "unknown")
CFLAGS      = -Wall -Wextra -Wno-stringop-truncation -O2 -Iinc -Isrc/rfc6234 \
              -DGIT_COMMIT=\"$(GIT_COMMIT)\" -DGIT_AUTHOR=\"$(GIT_AUTHOR)\"
TARGET      = mangen
SRCS        = src/main.c src/hash.c src/walk.c src/rfc6234/sha224-256.c src/regexp.c src/manifest.c
OBJS        = $(patsubst %.c,$(BUILD_DIR)/%.o,$(SRCS))

all: $(TARGET)

$(TARGET): $(OBJS)
	$(CC) $(CFLAGS) -o $@ $^

$(BUILD_DIR)/%.o: %.c
	@mkdir -p $(dir $@)
	$(CC) $(CFLAGS) -c -o $@ $<

test: $(TARGET)
	@./tests/run_tests.sh

doc:
	doxygen Doxyfile

clean:
	rm -f $(TARGET)
	rm -rf $(BUILD_DIR)
	rm -rf doc/

.PHONY: all clean test doc