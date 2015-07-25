CXX		?= $(CROSS_COMPILE)g++
CC		?= $(CROSS_COMPILE)gcc
ROOTDIR	= ../..
SDRONE_SRCDIR = $(ROOTDIR)/sdrone
INCLUDES	= -I$(SDRONE_SRCDIR) -I$(ROOTDIR)/include -I$(ROOTDIR)/libjsonrpc -I$(ROOTDIR)/libcmdargs
INCLUDES   += $(shell pkg-config --cflags glib-2.0) $(shell pkg-config --cflags json-glib-1.0)
LIBS		= -lm -lrt -lpthread -ldl
LIBS        += $(shell pkg-config --libs glib-2.0) $(shell pkg-config --libs json-glib-1.0)
#use -Wno-psabi to turn off warnings about ABI changes for va-list in GCC 4.4
CFLAGS		= -g -O0 -Wall -Wno-psabi
TARGET		= test_jsonrpc
OBJECTS		= \
		test_jsonrpc.o \
		rpcparams.o\
		commandargs.o\
		pluginregistry.o\
		$(ROOTDIR)/libjsonrpc/libjsonrpc.a\
		$(ROOTDIR)/libcmdargs/libcmdargs.a


all : $(TARGET)

.SUFFIXES:	.c .cxx .cpp

.c.o: 
	$(CC) -c $< $(CFLAGS) $(DEBUGFLAGS) $(INCLUDES)

.cxx.o: 
	$(CXX) -std=c++11 -c $< $(CFLAGS) $(DEBUGFLAGS) $(INCLUDES)

.cpp.o: 
	$(CXX) -std=c++11 -c $< $(CFLAGS) $(DEBUGFLAGS) $(INCLUDES)

%.o: $(SDRONE_SRCDIR)/%.cpp
	$(CXX) -std=c++11 $(CFLAGS) $(DEBUGFLAGS) -o $*.o -c $(SDRONE_SRCDIR)/$*.cpp $(INCLUDES)

$(TARGET) : $(OBJECTS)
	$(CXX) -std=c++11 $^ -o $(TARGET) $(LIBS)

clean:
	rm -f *.o *~ $(TARGET)

