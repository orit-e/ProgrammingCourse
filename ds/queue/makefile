CC = gcc -ansi
CFLAGS = -pedantic-errors -Wall -Wextra -g

VPATH = ../slist/
DEPS = queue.h slist.h
objects = test_queue.o queue.o slist.o

a.out : $(objects)
	$(CC) $(CFLAGS) -o a.out $(objects)
queue.o : queue.c $(DEPS)
	$(CC) $(CFLAGS) -c queue.c
test_queue.o : test_queue.c queue.h
	$(CC) $(CFLAGS) -c test_queue.c
slist.o : $(VPATH)slist.c slist.h
	$(CC) $(CFLAGS) -c $(VPATH)slist.c


.PHONY : clean
clean :
	-rm a.out $(objects)

