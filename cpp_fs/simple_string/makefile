CXX = g++
CXXFLAGS = -std=c++98 -pedantic-errors -Wall -Wextra -g

DEPS = simple_string.hpp
objects = simple_string.o test_simple_string.o


%.o : %.c $(DEPS)
	$(CXX) -c -o $@ $< $(CXXFLAGS)

a.out : $(objects)
	$(CXX) $(CXXFLAGS) -o a.out $(objects)


.PHONY : clean
clean :
	-rm a.out $(objects)

