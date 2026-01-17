CC=clang
SRC=.
OBJ=obj
PROG_NAME = libderp

TEST := test/bin/test

CFLAGS := -Wall -pedantic -g -DDEBUG --std=c11

all: $(OBJ) debug

$(OBJ):
	mkdir -p ./$(OBJ)

INCLUDE_FILES :=

SRC_FILES := $(wildcard $(SRC)/**.c)
OBJ_FILES := $(patsubst $(SRC)/%.c, $(OBJ)/%.o, $(SRC_FILES))

check: $(PROG_NAME).a
	make -C test test

debug: CFLAGS += -DDERP_DEBUG -DDLOG_COLORS

release: CFLAGS += -O2
release: clean

debug release: $(PROG_NAME).a

$(PROG_NAME).a: $(OBJ_FILES)
	ar rcs $@ $(OBJ_FILES)

$(OBJ)/%.o: $(SRC)/%.c
	@echo building $@...
	$(CC) -c $< -o $@ $(CFLAGS) $(INCLUDE_FILES)

$(OBJ)/%.o: $(SRC)/*/%.c
	@echo building $@...
	$(CC) -c $< -o $@ $(CFLAGS) $(INCLUDE_FILES)

$(TEST_OBJ_DIR)/%.o: $(TEST_SRC_DIR)/%.c $(TEST_DIR)/%.c
	$(CC) -c $< -o $@ $(CFLAGS) $(INCLUDE_FILES)

clean:
	rm -rf $(OBJ)/* $(PROG_NAME).a test/bin/*

.PHONY: all clean
