CC=gcc
FLAGS=-Wall -Werror -g

SRCS=mysh.c parse.c arg.c alias.c utils.c
OBJS=$(SRCS:%.c=%.o)
DEPS=$(SRCS:%.c=%.h)

OUT=mysh

all: $(OUT)

$(OUT): $(OBJS)
	$(CC) -o $@ $^

%.o: %.c $(DEPS)
	$(CC) -c $(FLAGS) $< -o $@

.PHONY: clean
clean:
		rm -f mysh *.o
