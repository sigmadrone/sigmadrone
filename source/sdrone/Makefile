#CROSS_COMPILE=arm-linux-gnueabi-
CXX	    ?= $(CROSS_COMPILE)g++
CC          ?= $(CROSS_COMPILE)gcc
HTTPSERVER_SRCDIR = ../libhttp
JSONRPC_SRCDIR = ../libjsonrpc
CMDARGS_SRCDIR = ../libcmdargs
LIBDEVICE_SRCDIR = ../libdevice
LIBATTITUDE_SRCDIR = ../libattitude
INCLUDES    = -I../include -I.. -I$(JSONRPC_SRCDIR) -I$(LIBDEVICE_SRCDIR) -I$(LIBATTITUDE_SRCDIR)
INCLUDES   += $(shell pkg-config --cflags glib-2.0) $(shell pkg-config --cflags json-glib-1.0)
LIBS		= -llogger -lm -lrt -pthread -ldl -ldevice -lattitude
LIBS        += $(shell pkg-config --libs glib-2.0) $(shell pkg-config --libs json-glib-1.0)
LIBS		+= -lboost_system -lboost_filesystem -lboost_program_options -lboost_thread
LIBS		+= -L$(LIBDEVICE_SRCDIR) -L$(LIBATTITUDE_SRCDIR) -L../liblogger
#use -Wno-psabi to turn off warnings about ABI changes for va-list in GCC 4.4
CFLAGS		= -g -O0 -Wall -Wno-psabi
TARGET		= sdroned
OBJECTS		= \
	commandargs.o\
	pluginchain.o\
	plugincontext.o\
	main.o\
	drone.o\
	servoplugin.o\
	quadrotorpilot.o\
	imureader.o\
	navigator.o\
	imufilterplugin.o\
	imuremap.o\
	imubias.o\
	tracelogplugin.o\
	imulowpassfilter.o\
	iopacket.o\
	kalmanattitudefilter.o\
	daemon.c\
	dronerpcclient.o\
	droneconfig.o\
	rpcparams.o\
	pidpilotsd.o\
	pidtorque.o\
	$(JSONRPC_SRCDIR)/libjsonrpc.a\
	$(HTTPSERVER_SRCDIR)/libhttp.a\
	$(CMDARGS_SRCDIR)/libcmdargs.a

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

