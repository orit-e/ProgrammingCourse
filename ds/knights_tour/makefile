VPATH = ../tests ../stack
INCLUDE_VPATH_DIR = $(patsubst %,-I%,$(VPATH))

CC = gcc -ansi
CFLAGS = $(INCLUDE_VPATH_DIR) -pedantic-errors -Wall -Wextra -g

DEPS = knights_tour.h tests.h stack.h
objects = knights_tour.o test_knights_tour.o tests.o stack.o


%.o : %.c $(DEPS)
	$(CC) -c -o $@ $< $(CFLAGS)

a.out : $(objects)
	$(CC) $(CFLAGS) -o a.out $(objects)


.PHONY : clean
clean :
	-rm a.out $(objects)