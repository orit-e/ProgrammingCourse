VPATH = ../../ds/tests ../../ds/scheduler ../../ds/task \
		../../ds/priority_queue ../../ds/heap ../../ds/vector ../../ds/uid
INCLUDE_VPATH_DIR = $(patsubst %,-I%,$(VPATH))

CC = gcc -ansi
CFLAGS = $(INCLUDE_VPATH_DIR) -pedantic-errors -Wall -Wextra -g -pthread

DEPS = watchdog.h tests.h scheduler.h task.h priority_queue.h heap.h vector.h \
		uid.h
objects = watchdog.o tests.o scheduler.o task.o \
			priority_queue.o heap.o vector.o uid.o
user_object = test_watchdog.o
wd_object = watchdog_proc.o

%.o : %.c $(DEPS)
	$(CC) -c -o $@ $< $(CFLAGS)

a.out : $(objects) $(user_object)
	$(CC) $(CFLAGS) -o a.out $(objects) $(user_object)

.PHONY : watchdog_proc
watchdog_proc : $(objects) $(wd_object)
	$(CC) $(CFLAGS) -o watchdog.out $(objects) $(wd_object)

.PHONY : clean
clean :
	-rm a.out $(objects)