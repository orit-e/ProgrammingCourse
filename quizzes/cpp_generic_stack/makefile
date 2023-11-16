VPATH = ../../cpp_fs/utils
INCLUDE_VPATH_DIR = $(patsubst %,-I%,$(VPATH))

CXX = g++
CXXFLAGS = $(INCLUDE_VPATH_DIR) -std=c++98 -pedantic-errors -Wall -Wextra -g -O

DEPS = generic_stack.hpp utils.hpp
objects = generic_stack.o test_generic_stack.o


%.o : %.c $(DEPS)
	$(CXX) -c -o $@ $< $(CXXFLAGS)

a.out : $(objects) $(DEPS)
	$(CXX) $(CXXFLAGS) -o a.out $(objects)


.PHONY : clean
clean :
	-rm a.out $(objects)