CXX		?= $(CROSS_COMPILE)g++
CC		?= $(CROSS_COMPILE)gcc
SDRONE_SRCDIR	= ../../sdrone
HTTPSERVER_SRCDIR = ../../http
INCLUDES	= -I../.. -I$(SDRONE_SRCDIR) -I$(HTTPSERVER_SRCDIR)
LIBS		= -lboost_system -lboost_filesystem -lboost_program_options -lboost_thread -lm -lrt -lpthread -ldl
#use -Wno-psabi to turn off warnings about ABI changes for va-list in GCC 4.4
CFLAGS		= -g -O0 -Wall -Wno-psabi
TARGET		= base64
OBJECTS		= \
		testbase64.o \
		$(HTTPSERVER_SRCDIR)/libhttp.a



all : $(TARGET)

.SUFFIXES:	.c .cxx .cpp

.c.o: 
	$(CC) -c $< $(CFLAGS) $(INCLUDES)

.cxx.o: 
	$(CXX) -c $< $(CFLAGS) $(INCLUDES)

.cpp.o: 
	$(CXX) -c $< $(CFLAGS) $(INCLUDES)

%.o: $(SDRONE_SRCDIR)/%.cpp
	$(CXX) $(CFLAGS) -o $*.o -c $(SDRONE_SRCDIR)/$*.cpp

$(TARGET) : $(OBJECTS)
	$(CXX) $^ -o $(TARGET) $(LIBS)

clean:
	rm -f *.o *~ $(TARGET)

