VPATH = ../tests ../vector
INCLUDE_VPATH_DIR = $(patsubst %,-I%,$(VPATH))

CC = gcc -ansi
CFLAGS = $(INCLUDE_VPATH_DIR) -pedantic-errors -Wall -Wextra -g

DEPS = heap.h tests.h vector.h
objects = heap.o test_heap.o tests.o vector.o


%.o : %.c $(DEPS)
	$(CC) -c -o $@ $< $(CFLAGS)

a.out : $(objects)
	$(CC) $(CFLAGS) -o a.out $(objects)


.PHONY : clean
clean :
	-rm a.out $(objects)