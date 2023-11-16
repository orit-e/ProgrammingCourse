VPATH = ../task ../priority_queue ../heap ../vector ../uid
INCLUDE_VPATH_DIR = $(patsubst %,-I%,$(VPATH))

CC = gcc -ansi
CFLAGS = $(INCLUDE_VPATH_DIR) -pedantic-errors -Wall -Wextra -g


DEPS = scheduler.h task.h priority_queue.h heap.h vector.h uid.h
objects = test_scheduler.o scheduler.o task.o priority_queue.o heap.o vector.o uid.o

%.o : %.c $(DEPS)
	$(CC) -c -o $@ $< $(CFLAGS)

a.out : $(objects)
	$(CC) $(CFLAGS) -o a.out $(objects)


.PHONY : clean
clean :
	-rm a.out $(objects)

