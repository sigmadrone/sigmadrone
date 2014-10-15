CXX	    	?= g++
CC          	?= gcc
INCLUDES    	= -I../../include -I../..
LIBS		= -lglut -lGL -lGLEW -lX11 -lm -lrt -lpthread -lcmdargs -llogger -ljsonspirit -lhttp -lcmdargs 
LIBS		+= -lboost_system -lboost_filesystem -lboost_program_options -lboost_thread
LIBS		+= -L../../libhttp -L../../libjsonspirit -L../../libcmdargs -L../../liblogger
LIBS		+= `pkg-config --libs gtkmm-3.0`
CFLAGS		= -g -O0 -Wall
CFLAGS		+= `pkg-config --cflags gtkmm-3.0`
TARGET		= flightdeck
OBJECTS		= \
		main.o \
		controlwindow.o \


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

