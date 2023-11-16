VPATH = ../tests
INCLUDE_VPATH_DIR = $(patsubst %,-I%,$(VPATH))

CC = gcc -ansi
CFLAGS = $(INCLUDE_VPATH_DIR) -pedantic-errors -Wall -Wextra -g

DEPS = dhcp.h tests.h
objects = dhcp.o test_dhcp.o tests.o


%.o : %.c $(DEPS)
	$(CC) -c -o $@ $< $(CFLAGS)

a.out : $(objects)
	$(CC) $(CFLAGS) -o a.out $(objects)


.PHONY : clean
clean :
	-rm a.out $(objects)