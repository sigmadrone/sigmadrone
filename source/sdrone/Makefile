CROSS_COMPILE=arm-linux-gnueabi-
CPP	    	= $(CROSS_COMPILE)g++
CC          = $(CROSS_COMPILE)gcc
INCLUDES    = -I../include
LIBS		= -lm -lrt -lpthread -ldl
#use -Wno-psabi to turn off warnings about ABI changes for va-list in GCC 4.4
CFLAGS		= -g -O0 -Wall -Wno-psabi
TARGET		= sdroned
OBJECTS		= \
	commandargs.o\
	pluginchain.o\
	plugincontext.o\
	main.o\
	drone.o\
	imudevice.o\
	servodevice.o\
	quadrotorpilot.o\
	clientserver.o\
	imureader.o\
	navigator.o\
	imufilterplugin.o\
	imuremap.o\
	imubias.o\
	tracelogplugin.o\
	imulowpassfilter.o\
	iopacket.o

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

