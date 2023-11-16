CXX = g++
CXXFLAGS = -std=c++98 -pedantic-errors -Wall -Wextra -g 
LDFLAGS = -lboost_thread -lboost_system -lboost_chrono -pthread -rdynamic

VPATH = ../utils ../waitable_queue ../priority_queue_wrapper ../thread_pool \
		../singleton ../reactor ../factory ../input_mediator \
		../plug_in_manager ../logger ../dir_monitor ../dispatcher_callback
INCLUDE_VPATH_DIR = $(patsubst %,-I%,$(VPATH))
CXXFLAGS += $(INCLUDE_VPATH_DIR)

OBJ_COMP = $(CXX) $(CXXFLAGS) -c $< -o $@ $(LDFLAGS)
EXE_COMP = $(CXX) $(CXXFLAGS) -o $@ $^ $(LDFLAGS) -L. -ltest -lfoo -ldl

DEPS = thread_pool.hpp future_task_imp.hpp utils.hpp waitable_queue.hpp \
		priority_queue_wrapper.hpp singleton.hpp singleton_imp.hpp reactor.hpp \
		factory.hpp input_mediator.hpp plug_in_manager.hpp framwork.hpp \
		logger.hpp dir_monitor.hpp dispatcher_callback.hpp \
		dispatcher_callback_imp.hpp
OBJS = test_framework.o input_mediator.o thread_pool.o reactor.o \
		plug_in_manager.o framework.o logger.o dir_monitor.o

%.o : %.cpp $(DEPS)
	$(OBJ_COMP)

a.out : $(OBJS)
	$(EXE_COMP)



.PHONY : clean
clean :
	$(RM) a.out $(OBJS)