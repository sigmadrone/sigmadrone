CXX		?= $(CROSS_COMPILE)g++
CC		?= $(CROSS_COMPILE)gcc
ROOTDIR	= ../..
SDRONE_SRCDIR = $(ROOTDIR)/sdrone
INCLUDES	= -I$(SDRONE_SRCDIR) -I$(ROOTDIR)/include -I$(ROOTDIR)/jsonrpc
INCLUDES   += $(shell pkg-config --cflags glib-2.0) $(shell pkg-config --cflags json-glib-1.0)
LIBS		= -lm -lrt -lpthread -ldl
LIBS        += $(shell pkg-config --libs glib-2.0) $(shell pkg-config --libs json-glib-1.0)
#use -Wno-psabi to turn off warnings about ABI changes for va-list in GCC 4.4
CFLAGS		= -g -O0 -Wall -Wno-psabi
TARGET		= test_jsonrpc
OBJECTS		= \
		test_jsonrpc.o \
		cmdargs.o\
		commandargs.o\
		$(ROOTDIR)/jsonrpc/libjsonrpc.a


all : $(TARGET)

.SUFFIXES:	.c .cxx .cpp

.c.o: 
	$(CC) -c $< $(CFLAGS) $(INCLUDES)

.cxx.o: 
	$(CXX) -c $< $(CFLAGS) $(INCLUDES)

.cpp.o: 
	$(CXX) -c $< $(CFLAGS) $(INCLUDES)

%.o: $(SDRONE_SRCDIR)/%.cpp
	$(CXX) $(CFLAGS) -o $*.o -c $(SDRONE_SRCDIR)/$*.cpp $(INCLUDES)

$(TARGET) : $(OBJECTS)
	$(CXX) $^ -o $(TARGET) $(LIBS)

clean:
	rm -f *.o *~ $(TARGET)

