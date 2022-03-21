CFLAGS=-Wall -Wextra -Werror -ggdb -std=c11 -pedantic

tests: tests.c wav.h
	$(CC) $(CFLAGS) -o tests tests.c
