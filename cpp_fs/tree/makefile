VPATH = ../utils
INCLUDE_VPATH_DIR = $(patsubst %,-I%,$(VPATH))

CXX = g++
CXXFLAGS = $(INCLUDE_VPATH_DIR) -std=c++98 -pedantic-errors -Wall -Wextra -g -O

DEPS = entry.hpp file.hpp directory.hpp utils.hpp tree.hpp
objects = entry.o file.o directory.o test_tree.o tree.o


%.o : %.c $(DEPS)
	$(CXX) -c -o $@ $< $(CXXFLAGS)

tree : $(objects) $(DEPS)
	$(CXX) $(CXXFLAGS) -o tree $(objects)


.PHONY : clean
clean :
	-rm test_tree $(objects)