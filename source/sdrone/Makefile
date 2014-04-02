#CROSS_COMPILE=arm-linux-gnueabi-
CXX	    ?= $(CROSS_COMPILE)g++
CC          ?= $(CROSS_COMPILE)gcc
HTTPSERVER_SRCDIR = ../http
INCLUDES    = -I../include -I../
INCLUDES   += $(shell pkg-config --cflags glib-2.0) $(shell pkg-config --cflags json-glib-1.0)
LIBS		= -lm -lrt -lpthread -ldl
LIBS        += $(shell pkg-config --libs glib-2.0) $(shell pkg-config --libs json-glib-1.0)
LIBS		+= -lboost_system -lboost_filesystem -lboost_program_options -lboost_thread
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
	jsonrpctransport.o\
	jsonrpcdispatch.o\
	cmdargs.o\
	$(HTTPSERVER_SRCDIR)/libhttp.a
	
	
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

