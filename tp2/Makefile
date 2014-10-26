CC = gcc
CFLAGS  = -g -Wall -Werror -std=c99
LDFLAGS = -L. -lmy_stdio
MYLIB = libmy_stdio.so
SRC = test1.c

OBJ = $(patsubst %.c,%.o, $(SRC))
EXE = $(patsubst %.o,%, $(OBJ))

all: $(EXE)

.SECONDEXPANSION:
${MYLIB}: $$(patsubst lib%.so, %.c, $$@)
	$(CC) -shared -fPIC -o $@ $(patsubst lib%.so,%.c, $@)

.SECONDEXPANSION:
${EXE}: $$(patsubst %, %.o, $$@) $(MYLIB)
	$(CC) $(LDFLAGS) $< -o $@
	

my_stdio_test:test1.o my_stdio.o 
	$(CC) $(CFLAGS) $^ -o $@
	#./my_stdio_test infile outfile
	#diff infile outfile

bench:
	LD_PRELOAD=./libmy_stdio.so ps 2>&1 > /dev/null
	
clean:
	rm -rf $(EXE) $(OBJ) $(patsubst lib%.so,%.o, $(MYLIB)) $(MYLIB)
