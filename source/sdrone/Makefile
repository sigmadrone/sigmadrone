#CROSS_COMPILE=arm-linux-gnueabi-
CXX	    ?= $(CROSS_COMPILE)g++
CC          ?= $(CROSS_COMPILE)gcc
INCLUDES    = -I../include
INCLUDES   += $(shell pkg-config --cflags glib-2.0) $(shell pkg-config --cflags json-glib-1.0)
LIBS		= -lm -lrt -lpthread -ldl -lboost_system -lboost_filesystem -lboost_thread
LIBS        += $(shell pkg-config --libs glib-2.0) $(shell pkg-config --libs json-glib-1.0)
#use -Wno-psabi to turn off warnings about ABI changes for va-list in GCC 4.4
CFLAGS		= -g -O0 -Wall -Wno-psabi
TARGET		= sdroned
OBJECTS		= \
	commandargs.o\
	pluginchain.o\
	plugincontext.o\
	main.o\
	drone.o\
	imudevice.o\
	servodevice.o\
	quadrotorpilot.o\
	clientserver.o\
	imureader.o\
	navigator.o\
	imufilterplugin.o\
	imuremap.o\
	imubias.o\
	tracelogplugin.o\
	imulowpassfilter.o\
	iopacket.o\
	kalmanattitudefilter.o\
	jsonrpcparser.o\
	jsonreadwrite.o\
	jsonrpcbuilder.o\
	cmdargs.o \
	logger.o

all : $(TARGET)

.SUFFIXES:	.c .cxx .cpp

.c.o: 
	$(CC) -c $< $(CFLAGS) $(INCLUDES)

.cxx.o: 
	$(CXX) -c $< $(CFLAGS) $(INCLUDES)

.cpp.o: 
	$(CXX) -c $< $(CFLAGS) $(INCLUDES)

$(TARGET) : $(OBJECTS)
	$(CXX) $^ -o $(TARGET) $(LIBS)

clean:
	rm -f *.o *~ $(TARGET)

