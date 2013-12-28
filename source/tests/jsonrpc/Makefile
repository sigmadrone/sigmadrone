CPP		= $(CROSS_COMPILE)g++
CC		= $(CROSS_COMPILE)gcc
ROOTDIR	= ../..
SDRONE_SRCDIR = $(ROOTDIR)/sdrone
INCLUDES	= -I$(SDRONE_SRCDIR) -I$(ROOTDIR)/include
INCLUDES   += $(shell pkg-config --cflags glib-2.0) $(shell pkg-config --cflags json-glib-1.0)
LIBS		= -lm -lrt -lpthread -ldl
LIBS        += $(shell pkg-config --libs glib-2.0) $(shell pkg-config --libs json-glib-1.0)
#use -Wno-psabi to turn off warnings about ABI changes for va-list in GCC 4.4
CFLAGS		= -g -O0 -Wall -Wno-psabi
TARGET		= test_jsonrpc
OBJECTS		= \
		test_jsonrpc.o \
		jsonrpcparser.o


all : $(TARGET)

.SUFFIXES:	.c .cxx .cpp

.c.o: 
	$(CC) -c $< $(CFLAGS) $(INCLUDES)

.cxx.o: 
	$(CPP) -c $< $(CFLAGS) $(INCLUDES)

.cpp.o: 
	$(CPP) -c $< $(CFLAGS) $(INCLUDES)

%.o: $(SDRONE_SRCDIR)/%.cpp
	$(CPP) $(CFLAGS) -o $*.o -c $(SDRONE_SRCDIR)/$*.cpp $(INCLUDES)

$(TARGET) : $(OBJECTS)
	$(CPP) $^ -o $(TARGET) $(LIBS)

clean:
	rm -f *.o *~ $(TARGET)

