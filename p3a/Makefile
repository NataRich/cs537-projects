CC=gcc
FLAGS=-Wall -Werror -g

SRCS=main.c linkedlist.c hashmap.c mapreduce.c
OBJS=$(SRCS:%.c=%.o)
DEPS=$(SRCS:%.c=%.h)

OUT=mapreduce

all: $(OUT)

$(OUT): $(OBJS)
	$(CC) -o $@ $^ -pthread

%.o: %.c $(DEPS)
	$(CC) -c $(FLAGS) $< -o $@ -pthread

.PHONY: clean
clean:
		rm -f mapreduce *.o
