CXX = g++
WARNING_FALGS = -Wreturn-type -Wunused-value -Wunused-parameter -Wsign-conversion -Wparentheses
CXXFLAGS = -std=c++11 -pthread -O2 -g -Wall -fmessage-length=0
CXXFLAGS += $(WARNING_FALGS)

OBJS = easyframe.o 				\
	   common/mem_pool.o 		\
	   common/poller.o			\
	   common/logger.o			\
	   common/shift_writer.o	\
	   common/io_wrapper.o		\
	   common/event_notifier.o	\
	   common/task_pool.o

LIBS =

TARGET = easyframe

$(TARGET):	$(OBJS)
	$(CXX) $(CXXFLAGS) -o $(TARGET) $(OBJS) $(LIBS)

%.o: %.cpp
	$(CXX) $(CXXFLAGS) -c -o $@ $<


all:	$(TARGET)

clean:
	rm -f $(OBJS) $(TARGET)
