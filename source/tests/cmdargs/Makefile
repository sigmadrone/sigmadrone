CXX		?= $(CROSS_COMPILE)g++
CC		?= $(CROSS_COMPILE)gcc
ROOTDIR	= ../..
INCLUDES	= -I$(ROOTDIR)/libcmdargs
LIBS		= -lm -lrt -lpthread -ldl
#use -Wno-psabi to turn off warnings about ABI changes for va-list in GCC 4.4
CFLAGS		= -g -O0 -Wall -Wno-psabi
TARGET		= test_cmdargs
OBJECTS		= \
		test_cmdargs.o \
		$(ROOTDIR)/libcmdargs/libcmdargs.a


all : $(TARGET)

.SUFFIXES:	.c .cxx .cpp

.c.o: 
	$(CC) -c $< $(CFLAGS) $(INCLUDES)

.cxx.o: 
	$(CXX) -std=c++11 -c $< $(CFLAGS) $(INCLUDES)

.cpp.o: 
	$(CXX) -std=c++11 -c $< $(CFLAGS) $(INCLUDES)

%.o: $(SDRONE_SRCDIR)/%.cpp
	$(CXX) -std=c++11 $(CFLAGS) -o $*.o -c $(SDRONE_SRCDIR)/$*.cpp

$(TARGET) : $(OBJECTS)
	$(CXX) -std=c++11 $^ -o $(TARGET) $(LIBS)

clean:
	rm -f *.o *~ $(TARGET)

