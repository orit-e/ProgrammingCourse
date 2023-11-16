CXX = g++
CXXFLAGS = -std=c++98 -pedantic-errors -Wall -Wextra -g 
LDFLAGS = -lboost_thread -lboost_system -pthread

VPATH = ../utils
INCLUDE_VPATH_DIR = $(patsubst %,-I%,$(VPATH))
CXXFLAGS += $(INCLUDE_VPATH_DIR)

OBJ_COMP = $(CXX) $(CXXFLAGS) -c $< -o $@ $(LDFLAGS)
EXE_COMP = $(CXX) $(CXXFLAGS) -o $@ $^ $(LDFLAGS)

DEPS = reactor.hpp utils.hpp
OBJS = reactor.o test_reactor.o

%.o : %.cpp $(DEPS)
	$(OBJ_COMP)

a.out : $(OBJS)
	$(EXE_COMP)

.PHONY : clean
clean :
	$(RM) a.out $(OBJS)