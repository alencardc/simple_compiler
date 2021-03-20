CC = gcc
CFLAGS =

SUBMITION_NUMBER = 3


UTIL_FILES := $(wildcard ../../utils/**/*.c)
UTIL_FILES := $(filter-out $(wildcard ../../utils/tests/*.c), $(UTIL_FILES))

test_symbol_table:
	$(CC) $(CFLAGS) -o test $(UTIL_FILES) symbol_table_test.c 