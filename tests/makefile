CC = clang
CFLAGS ?= -Wall -Wextra -Wpedantic -g --std=c11
CPPFLAGS += -I..
SANITIZERS ?=

LIB := ../libderp.a
TARGET := bin/tests
SOURCES := main.c tests.c tests_dmem.c tests_ddebug.c tests_darena.c \
           tests_dbuf.c tests_dstr.c tests_dfile.c

all: $(TARGET)

$(TARGET): $(SOURCES) tests.h ../derp.h ../d_buf_macro.h $(LIB) | bin
	$(CC) $(CPPFLAGS) $(CFLAGS) $(SANITIZERS) $(SOURCES) $(LIB) \
		$(LDFLAGS) $(SANITIZERS) -o $@

$(LIB):
	$(MAKE) -C .. libderp.a

bin:
	mkdir -p $@

test: $(TARGET)
	./$(TARGET)

clean:
	rm -rf bin

.PHONY: all test clean
