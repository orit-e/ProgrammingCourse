CXX = g++
CXXFLAGS = -std=c++98 -pedantic-errors -Wall -Wextra -g 
LDFLAGS = -lboost_thread -lboost_system -lboost_chrono -pthread

VPATH = ../utils ../waitable_queue ../priority_queue_wrapper ../thread_pool \
		../singleton
INCLUDE_VPATH_DIR = $(patsubst %,-I%,$(VPATH))
CXXFLAGS += $(INCLUDE_VPATH_DIR)

OBJ_COMP = $(CXX) $(CXXFLAGS) -c $< -o $@ $(LDFLAGS)
EXE_COMP = $(CXX) $(CXXFLAGS) -o $@ $^ $(LDFLAGS)

DEPS = logger.hpp utils.hpp thread_pool.hpp singleton.hpp future_task_imp.hpp \
		singleton_imp.hpp waitable_queue.hpp waitable_queue_imp.hpp \
		priority_queue_wrapper.hpp
OBJS = logger.o thread_pool.o 
SINGLE_OBJS = single_thread_logger_test.o
MULTI_OBJ = multi_threads_logger_test.o

.PHONY : clean single_test.out multi_test.out

%.o : %.cpp $(DEPS)
	$(OBJ_COMP)

single_test.out : $(SINGLE_OBJS) $(OBJS)
	$(EXE_COMP)

multi_test.out : $(MULTI_OBJ) $(OBJS)
	$(EXE_COMP)

clean :
	$(RM) single_test.out multi_test.out $(OBJS) $(SINGLE_OBJS) $(MULTI_OBJ)