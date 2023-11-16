VPATH = ../utils
INCLUDE_VPATH_DIR = $(patsubst %,-I%,$(VPATH))

CXX = g++
CXXFLAGS = $(INCLUDE_VPATH_DIR) -std=c++98 -pedantic-errors -Wall -Wextra -g -O

DEPS = rc_string.hpp rc_string_imp.hpp utils.hpp
objects = proxy_char.o rc_string.o test_rc_string.o


%.o : %.c $(DEPS)
	$(CXX) -c -o $@ $< $(CXXFLAGS)

a.out : $(objects) $(DEPS)
	$(CXX) $(CXXFLAGS) -o a.out $(objects)


.PHONY : clean
clean :
	-rm a.out $(objects)