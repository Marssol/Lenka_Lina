SRCS = $(wildcard *.c)
OBJS = $(SRCS:.c=.o)
EXEC = bar

CC = clang

CFLAGS = -Wall -Wextra
LDFLAGS = -lpthread

all : $(EXEC)

%.o : %.c
	$(CC) -o $@ -c $< $(CFLAGS)

$(EXEC) : $(OBJS)
	$(CC) -o $@ $^ $(LDFLAGS)

clean:
	rm -f $(EXEC) *~ *.o
