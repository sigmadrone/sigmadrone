CXX	    	?= g++
CC          	?= gcc
INCLUDES    	= -I../../include -I../..
LIBS		= -lglut -lGL -lGLEW -lX11 -lm -lrt -pthread -lcmdargs -llogger -ljsonspirit -lhttp -lcmdargs 
LIBS		+= -lboost_system -lboost_filesystem -lboost_program_options -lboost_thread
LIBS		+= -L../../libhttp -L../../libjsonspirit -L../../libcmdargs -L../../liblogger
LIBS		+= `pkg-config --libs gtkmm-3.0`
CFLAGS		= -g -O0 -Wall
CFLAGS		+= `pkg-config --cflags gtkmm-3.0`
TARGET		= flightdeck
OBJECTS		= \
		main.o \
		mainwindow.o \


all : $(TARGET)


.SUFFIXES:	.c .cxx .cpp

.c.o: 
	$(CC) -c $< $(CFLAGS) $(INCLUDES)

.cxx.o: 
	$(CXX) -std=c++11 -c $< $(CFLAGS) $(INCLUDES)

.cpp.o: 
	$(CXX) -std=c++11 -c $< $(CFLAGS) $(INCLUDES)

$(TARGET) : $(OBJECTS)
	$(CXX) -std=c++11 $^ -o $(TARGET) $(LIBS)

clean:
	rm -f *.o *~ $(TARGET)

