CC=gcc
CERR_FLAGS=-Wall -Wextra
CFLAGS=-std=c99 -lm -g -O1

rwildcard=$(foreach d,$(wildcard $(1:=/*)),$(call rwildcard,$d,$2) $(filter $(subst *,%,$2),$d))

SRCS=$(filter-out src/main.c, $(call rwildcard,src,*.c))
HDRS=$(call rwildcard,src,*.h)
TESTS= $(call rwildcard,test,*.c)
OBJS=$(wildcard *.o)

			

build: $(SRCS) src/main.c
	$(CC) $(CERR_FLAGS) $(CFLAGS) -o ./txl src/main.c $(SRCS)

wc:
	find ./src -type f | xargs wc -l | tail -1

clean:
	rm -f $(OBJS) ./sfl

run:
	./txl

val:
	valgrind --leak-check=full --show-leak-kinds=all --track-origins=yes --verbose --log-file=valgrind/valgrind-out.txt ./txl

.PHONY: pack clean