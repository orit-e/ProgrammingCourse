CC = gcc -ansi 
CFLAGS = -pedantic-errors -Wall -Wextra -g
AR = ar rcs

DEPS = foo.h
objects = main.o foo.o

%.o : %.c $(DEPS)
	$(CC) -c -o $@ $< $(CFLAGS)
fooer.out : $(objects)
	$(CC) $(CFLAGS) -o $@ $^
libfoo.ar(foo.o) : foo.o
	$(AR) $@ $%

.PHONY : clean
clean :
	-rm libfoo.ar fooer.out $(objects)


