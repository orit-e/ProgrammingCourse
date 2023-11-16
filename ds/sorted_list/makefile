CC = gcc -ansi
CFLAGS = -pedantic-errors -Wall -Wextra -g

VPATH = ../dlist/
DEPS = sorted_list.h dlist.h
objects = test_sorted_list.o sorted_list.o dlist.o

a.out : $(objects)
	$(CC) $(CFLAGS) -o a.out $(objects)
circular_buffer.o : sorted_list.c $(DEPS)
	$(CC) $(CFLAGS) -c sorted_list.c
test_sorted_list.o : test_sorted_list.c sorted_list.h
	$(CC) $(CFLAGS) -c test_sorted_list.c
dlist.o : $(VPATH)dlist.c dlist.h
	$(CC) $(CFLAGS) -c $(VPATH)dlist.c


.PHONY : clean
clean :
	-rm a.out $(objects)

