CXX	    	?= $(CROSS_COMPILE)g++
CC          	?= $(CROSS_COMPILE)gcc
INCLUDES    	= -I../../include
LIBS		= -lm -lrt
CFLAGS		= -g -O0 -Wall
TARGET		= attifilt
OBJECTS		= main.o
SRCS        = main.cpp

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

