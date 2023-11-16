CC = gcc -ansi
CFLAGS = -pedantic-errors -Wall -Wextra -g

VPATH = ../uid/
DEPS = uid.h task.h
objects = test_task.o task.o uid.o

a.out : $(objects)
	$(CC) $(CFLAGS) -o a.out $(objects)
task.o : task.c $(DEPS)
	$(CC) $(CFLAGS) -c task.c
test_task.o : test_task.c $(DEPS)
	$(CC) $(CFLAGS) -c test_task.c
uid.o : $(VPATH)uid.c $(VPATH)uid.h
	$(CC) $(CFLAGS) -c $(VPATH)uid.c

.PHONY : clean
clean :
	-rm a.out $(objects)
