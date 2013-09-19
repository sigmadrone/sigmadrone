CPP	    	= $(CROSS_COMPILE)g++
CC          = $(CROSS_COMPILE)gcc
INCLUDES    = -I../../include
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
	$(CPP) -c $< $(CFLAGS) $(INCLUDES)

.cpp.o: 
	$(CPP) -c $< $(CFLAGS) $(INCLUDES)

$(TARGET) : $(OBJECTS)
	$(CPP) $^ -o $(TARGET) $(LIBS)

clean:
	rm -f *.o *~ $(TARGET)

