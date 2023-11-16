VPATH = ../heap ../tests ../vector
INCLUDE_VPATH_DIR = $(patsubst %,-I%,$(VPATH))

CC = gcc -ansi
CFLAGS = $(INCLUDE_VPATH_DIR) -pedantic-errors -Wall -Wextra -g

DEPS =  priority_queue.h heap.h tests.h vector.h
objects = priority_queue.o test_priority_queue.o heap.o tests.o vector.o


%.o : %.c $(DEPS)
	$(CC) $(CFLAGS) -c -o $@ $<

a.out : $(objects)
	$(CC) $(CFLAGS) -o a.out $(objects)


.PHONY : clean
clean :
	-rm a.out $(objects)
