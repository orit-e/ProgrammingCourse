CXX = g++
CXXFLAGS = -std=c++98 -pedantic-errors -Wall -Wextra -g 
LDFLAGS = -lboost_thread -lboost_system -lboost_chrono -pthread

VPATH = ../utils ../waitable_queue ../priority_queue_wrapper 
INCLUDE_VPATH_DIR = $(patsubst %,-I%,$(VPATH))
CXXFLAGS += $(INCLUDE_VPATH_DIR)

OBJ_COMP = $(CXX) $(CXXFLAGS) -c $< -o $@ $(LDFLAGS)
EXE_COMP = $(CXX) $(CXXFLAGS) -o $@ $^ $(LDFLAGS)

DEPS = thread_pool.hpp future_task_imp.hpp utils.hpp waitable_queue.hpp \
		priority_queue_wrapper.hpp
OBJS = test_thread_pool.o thread_pool.o 

%.o : %.cpp $(DEPS)
	$(OBJ_COMP)

a.out : $(OBJS)
	$(EXE_COMP)

.PHONY : clean
clean :
	$(RM) a.out $(OBJS)