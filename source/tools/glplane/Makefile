CPP	    	= g++
CC          	= gcc
INCLUDES    	= -I../../include -I../mmath
LIBS		= -lglut -lGL -lGLEW -lX11 -lm -lrt
CFLAGS		= -g -O0 -Wall
TARGET		= glplane
OBJECTS		= main.o glshaders.o glutprogram.o glshape.o plane.o

all : $(TARGET)


.SUFFIXES:	.c .cxx .cpp

.c.o: 
	$(CC) -c $< $(CFLAGS) $(INCLUDES)

.cxx.o: 
	$(CPP) -c $< $(CFLAGS) $(INCLUDES)

.cpp.o: 
	$(CPP) -c $< $(CFLAGS) $(INCLUDES)

$(TARGET) : $(OBJECTS)
	$(CPP) $^ -o $(TARGET) $(LIBS)

clean:
	rm -f *.o *~ $(TARGET)

