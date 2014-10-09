CXX	    	?= g++
CC          	?= gcc
INCLUDES    	= -I../../include -I../..
LIBS		= -lglut -lGL -lGLEW -lX11 -lm -lrt -lpthread -lcmdargs -lhttp -llogger -ljsonspirit -lcmdargs -lboost_system -lboost_filesystem -lboost_program_options -lboost_thread
LIBS		+= -L../../libhttp -L../../libjsonspirit -L../../libcmdargs -L../../liblogger
CFLAGS		= -g -O0 -Wall
TARGET		= glplane
OBJECTS		= main.o glshaders.o glutprogram.o glshape.o plane.o

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

