CXX = g++
CXXFLAGS = -std=c++98 -pedantic-errors -Wall -Wextra -g -O

VPATH = ../utils
INCLUDE_VPATH_DIR = $(patsubst %,-I%,$(VPATH))
CXXFLAGS += $(INCLUDE_VPATH_DIR)

OBJ_COMP = $(CXX) $(CXXFLAGS) -c $< -o $@
EXE_COMP = $(CXX) $(CXXFLAGS) -o $@ $^

DEPS = shared_ptr.hpp shared_ptr_imp.hpp utils.hpp
OBJS = test_shared_ptr.o

%.o : %.c $(DEPS)
	$(OBJ_COMP)

a.out : $(OBJS)
	$(EXE_COMP)

.PHONY : clean
clean :
	$(RM) a.out $(OBJS)