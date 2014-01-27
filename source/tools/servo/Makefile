CXX	    	?= $(CROSS_COMPILE)g++
CC          	?= $(CROSS_COMPILE)gcc
INCLUDES    	= -I../../include
LIBS		= -lm 
CFLAGS		= -g -Wall
OBJECTS		= servo.o
SRCS        	= servo.c
TARGET		= servo

all : $(TARGET)


.SUFFIXES:	.c .cxx

.c.o: 
	$(CC) -c $< $(CFLAGS) $(INCLUDES)

.cxx.o: 
	$(CXX) -c $< $(CFLAGS) $(INCLUDES)

$(TARGET) : $(OBJECTS)
	$(CXX) $^ -o $(TARGET) $(LIBS)

clean:
	rm -f *.o *~ $(TARGET)

