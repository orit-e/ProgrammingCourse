VPATH = ../stack
INCLUDE_VPATH_DIR = $(patsubst %,-I%,$(VPATH))

CC = gcc -ansi
CFLAGS = $(INCLUDE_VPATH_DIR) -pedantic-errors -Wall -Wextra -g

DEPS = calc.h stack.h
objects = calc.o test_calc.o stack.o -lm


%.o : %.c $(DEPS)
	$(CC) -c -o $@ $< $(CFLAGS)

a.out : $(objects)
	$(CC) $(CFLAGS) -o a.out $(objects)


.PHONY : clean
clean :
	-rm a.out $(objects)

