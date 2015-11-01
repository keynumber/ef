CXX = g++
WARNING_FALGS = -Wreturn-type -Wunused-value
CXXFLAGS = -std=c++11 -pthread -O2 -g -Wall -fmessage-length=0
CXXFLAGS += $(WARNING_FALGS)

OBJS = easyframe.o 				\
	   common/mem_pool.o 		\
	   common/poller.o

LIBS =

TARGET = easyframe

$(TARGET):	$(OBJS)
	$(CXX) $(CXXFLAGS) -o $(TARGET) $(OBJS) $(LIBS)

%.o: %.cpp
	$(CXX) $(CXXFLAGS) -c -o $@ $<


all:	$(TARGET)

clean:
	rm -f $(OBJS) $(TARGET)
