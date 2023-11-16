CXX = g++
CXXFLAGS = -std=c++98 -pedantic-errors -Wall -Wextra -g -O

DEPS = complex.hpp complex_imp.hpp
objects = complex.o test_complex.o


%.o : %.c $(DEPS)
	$(CXX) -c -o $@ $< $(CXXFLAGS)

a.out : $(objects) $(DEPS)
	$(CXX) $(CXXFLAGS) -o a.out $(objects)


.PHONY : clean
clean :
	-rm a.out $(objects)