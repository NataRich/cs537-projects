CC=gcc
FLAGS=-Wall -Werror -g

SRCS=main.c linkedlist.c hashmap.c mapreduce.c
OBJS=$(SRCS:%.c=%.o)
DEPS=$(SRCS:%.c=%.h)

OUT=mapreduce

all: $(OUT)

$(OUT): $(OBJS)
	$(CC) -o $@ $^ -lpthread

%.o: %.c $(DEPS)
	$(CC) -c $(FLAGS) $< -o $@ -lpthread

.PHONY: clean
clean:
		rm -f mapreduce *.o
