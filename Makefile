CC = clang
CFLAGS = -g -Wall -std=c99 -I src

SRC = $(wildcard src/*.c) main.c
OBJ = $(SRC:.c=.o) 


all: main

main: $(OBJ)
	$(CC) $(CFLAGS) -o main $(OBJ)


%.o: %.c	
	$(CC) $(CFLAGS) -c $< -o $@

tests: tests/test_insert.c	tests/test_find.c	tests/test_remove_entry.c	src/hashmap.c
	$(CC) $(CFLAGS) -o test_insert tests/test_insert.c src/hashmap.c
	$(CC) $(CFLAGS) -o test_find tests/test_find.c src/hashmap.c
	$(CC) $(CFLAGS) -o test_remove_entry tests/test_remove_entry.c src/hashmap.c

clean:
	rm -f main main.o hashmap.o test_find test_insert test_remove_entry