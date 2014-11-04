CXX	    	?= g++
CC          	?= gcc
INCLUDES    	= -I../../include -I../..
LIBS		= -lglut -lGL -lGLEW -lX11 -lm -lrt -lpthread -lcmdargs -llogger -ljsonspirit -lhttp -lcmdargs -lboost_system -lboost_filesystem -lboost_program_options -lboost_thread
LIBS		+= -L../../libhttp -L../../libjsonspirit -L../../libcmdargs -L../../liblogger
CFLAGS		= -g -O0 -Wall
TARGET		= glplane
OBJECTS		= main.o glshaders.o glutprogram.o glshape.o plane.o

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

