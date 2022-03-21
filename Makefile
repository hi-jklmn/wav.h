CFLAGS=-Wall -Werror -ggdb -std=c11

tests: tests.c wav.h
	$(CC) $(CFLAGS) -o tests tests.c
