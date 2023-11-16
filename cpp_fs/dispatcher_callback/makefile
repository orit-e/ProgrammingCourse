CXX = g++
CXXFLAGS = -std=c++98 -pedantic-errors -Wall -Wextra -g 

VPATH = ../utils
INCLUDE_VPATH_DIR = $(patsubst %,-I%,$(VPATH))
CXXFLAGS += $(INCLUDE_VPATH_DIR)

OBJ_COMP = $(CXX) $(CXXFLAGS) -c $< -o $@ $(LDFLAGS)
EXE_COMP = $(CXX) $(CXXFLAGS) -o $@ $^ $(LDFLAGS)

DEPS = dispatcher_callback.hpp dispatcher_callback_imp.hpp utils.hpp \
		derived_classes.hpp
OBJS = test_dispatcher_callback.o derived_classes.o 

%.o : %.cpp $(DEPS)
	$(OBJ_COMP)

a.out : $(OBJS)
	$(EXE_COMP)

.PHONY : clean
clean :
	$(RM) a.out $(OBJS)