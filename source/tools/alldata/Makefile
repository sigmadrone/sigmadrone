CXX	    	?= $(CROSS_COMPILE)g++
CC          	?= $(CROSS_COMPILE)gcc
INCLUDES    	= -I../../include
LIBS		= -lm 
CFLAGS		= -g -Wall
OBJECTS		= alldata.o
SRCS        	= alldata.c
TARGET		= alldata

all : $(TARGET)


.SUFFIXES:	.c .cpp .cxx

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

