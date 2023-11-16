CXX = g++
CXXFLAGS = -std=c++98 -pedantic-errors -Wall -Wextra -g 
LDFLAGS = -lboost_thread -pthread -ldl

VPATH = ../utils
INCLUDE_VPATH_DIR = $(patsubst %,-I%,$(VPATH))
CXXFLAGS += $(INCLUDE_VPATH_DIR)

OBJ_COMP = $(CXX) $(CXXFLAGS) -c $< -o $@ $(LDFLAGS)
EXE_COMP = $(CXX) $(CXXFLAGS) -o $@ $^ $(LDFLAGS)

DEPS = dir_monitor.hpp utils.hpp
OBJS = test_dir_monitor.o dir_monitor.o

%.o : %.cpp $(DEPS)
	$(OBJ_COMP)

a.out : $(OBJS)
	$(EXE_COMP)

.PHONY : clean
clean :
	$(RM) a.out $(OBJS)