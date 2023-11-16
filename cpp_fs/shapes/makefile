VPATH = ../utils
INCLUDE_VPATH_DIR = $(patsubst %,-I%,$(VPATH))

CXX = g++
CXXFLAGS = $(INCLUDE_VPATH_DIR) -std=c++98 -pedantic-errors -Wall -Wextra -g -O

DEPS = shapes.hpp shapes_imp.hpp utils.hpp
objects = shapes.o test_shapes.o


%.o : %.c $(DEPS)
	$(CXX) -c -o $@ $< $(CXXFLAGS)

a.out : $(objects) $(DEPS)
	$(CXX) $(CXXFLAGS) -o a.out $(objects)


.PHONY : clean
clean :
	-rm a.out $(objects)