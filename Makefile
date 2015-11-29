CXX = g++
WARNING_FALGS = -Wreturn-type -Wunused-value -Wunused-parameter -Wsign-conversion -Wparentheses
CXXFLAGS = -std=c++11 -pthread -g -Wall -fmessage-length=0
CXXFLAGS += $(WARNING_FALGS)


INC = -I.

OBJS = ef.o 					\
	   server/controller.o 		\
	   server/io_handler.o 		\
	   server/worker.o 			\
	   common/mem_pool.o 		\
	   common/poller.o			\
	   common/logger.o			\
	   common/util.o			\
	   common/shift_writer.o	\
	   common/io_wrapper.o		\
	   common/event_notifier.o	\
	   common/string_util.o		\
	   common/string_number.o	\
	   common/c_map.o			\
	   common/socket_buffer.o	\
	   common/task_pool.o		\
	   common/task_queue.o

LIBS =

TARGET = ef

$(TARGET):	$(OBJS)
	@ echo LD $(TARGET)
	@ $(CXX) $(INC) $(CXXFLAGS) -o $(TARGET) $(OBJS) $(LIBS)

%.o: %.cpp
	@ echo 'CXX $@'
	@ $(CXX) $(INC) $(CXXFLAGS) -c -o $@ $<

%.o: %.c
	@ echo 'CC $@'
	@ $(CXX) $(INC) $(CXXFLAGS) -c -o $@ $<

all:	$(TARGET)

clean:
	@ echo cleaning...
	@ rm -f $(OBJS) $(TARGET)
