CC = gcc
CFLAGS  = -g -Wall -Werror -std=c99
LDFLAGS = -L. -lmy_stdio
MYLIB = libmy_stdio.so
SRC = test1.c

OBJ = $(patsubst %.c,%.o, $(SRC))
EXE = $(patsubst %.o,%, $(OBJ))
TEST_FILES = $(wildcard *.in)
TEST_LIST = $(patsubst %.in,%.test,$(TEST_FILES))

all: $(EXE)

.SECONDEXPANSION:
${MYLIB}: $$(patsubst lib%.so, %.c, $$@)
	$(CC) -shared -fPIC -o $@ $(patsubst lib%.so,%.c, $@)

.SECONDEXPANSION:
${EXE}: $$(patsubst %, %.o, $$@) $(MYLIB)
	$(CC) $(LDFLAGS) $< -o $@

%.out: %.in ./test1
	@cat $< | ./test1 >/dev/null 2>$@
	
%.test: %.out %.out.expected
	@if diff $^  >/dev/null; then \
	  echo "**** Test $@ Passed *****"; \
	else \
	  echo "**** Test $@ FAILED *****"; \
	  diff -y $^ ;\
	fi
	
test: $(TEST_LIST)
	
clean:
	rm -rf $(EXE) $(OBJ) $(patsubst lib%.so,%.o, $(MYLIB)) $(MYLIB)
