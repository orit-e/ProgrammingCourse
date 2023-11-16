CXX = g++
CXXFLAGS = -std=c++98 -pedantic-errors -Wall -Wextra -g 

VPATH = ../utils
INCLUDE_VPATH_DIR = $(patsubst %,-I%,$(VPATH))
CXXFLAGS += $(INCLUDE_VPATH_DIR)

OBJ_COMP = $(CXX) $(CXXFLAGS) -c $< -o $@ $(LDFLAGS)
EXE_COMP = $(CXX) $(CXXFLAGS) -o $@ $^ $(LDFLAGS)

DEPS = bit_array.hpp bit_array_imp.hpp utils.hpp helping_classes.hpp
OBJS = test_bit_array.o helping_classes.o 

%.o : %.cpp $(DEPS)
	$(OBJ_COMP)

a.out : $(OBJS)
	$(EXE_COMP)

.PHONY : clean
clean :
	$(RM) a.out $(OBJS)