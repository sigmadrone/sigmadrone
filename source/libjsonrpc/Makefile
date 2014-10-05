CXX	    ?= $(CROSS_COMPILE)g++
CC          ?= $(CROSS_COMPILE)gcc
INCLUDES    = -I../include -I../
INCLUDES   += $(shell pkg-config --cflags glib-2.0) $(shell pkg-config --cflags json-glib-1.0)
LIBS		= -lm -lrt -lpthread -ldl
LIBS        += $(shell pkg-config --libs glib-2.0) $(shell pkg-config --libs json-glib-1.0)

# Hardening
# Make some classes of vulnerabilities unexploitable in case one is discovered.
#
    # This is a workaround for Ubuntu bug #691722, the default -fstack-protector causes
    # -fstack-protector-all to be ignored unless -fno-stack-protector is used first.
    # see: https://bugs.launchpad.net/ubuntu/+source/gcc-4.5/+bug/691722
    HARDENING=-fno-stack-protector

    # Stack Canaries
    # Put numbers at the beginning of each stack frame and check that they are the same.
    # If a stack buffer if overflowed, it writes over the canary number and then on return
    # when that number is checked, it won't be the same and the program will exit with
    # a "Stack smashing detected" error instead of being exploited.
    HARDENING+=-fstack-protector-all -Wstack-protector

    # Make some important things such as the global offset table read only as soon as
    # the dynamic linker is finished building it. This will prevent overwriting of addresses
    # which would later be jumped to.
    LDHARDENING+=-Wl,-z,relro -Wl,-z,now

    # Build position independent code to take advantage of Address Space Layout Randomization
    # offered by some kernels.
    # see doc/build-unix.txt for more information.
    ifdef PIE
        HARDENING+=-fPIE
        LDHARDENING+=-pie
    endif

    # -D_FORTIFY_SOURCE=2 does some checking for potentially exploitable code patterns in
    # the source such overflowing a statically defined buffer.
    HARDENING+=-D_FORTIFY_SOURCE=2
#

CFLAGS		= -g -O0 -Wall -Wno-psabi -static $(HARDENING)
TARGET		= libjsonrpc.a
OBJS		= \
	obj/jsonrpcparser.o\
	obj/jsonreadwrite.o\
	obj/jsonrpcbuilder.o\
	obj/jsonrpctransport.o\
	obj/jsonrpcdispatch.o
	
all : obj $(TARGET)

obj/%.o: %.c
#.c.o: 
	$(CC) -c $(CFLAGS) $(INCLUDES) -MMD -MF $(@:%.o=%.d) -o $@ $<
	@cp $(@:%.o=%.d) $(@:%.o=%.P); \
	sed -e 's/#.*//' -e 's/^[^:]*: *//' -e 's/ *\\$$//' \
	-e '/^$$/ d' -e 's/$$/ :/' < $(@:%.o=%.d) >> $(@:%.o=%.P); \
	rm -f $(@:%.o=%.d)

obj/%.o: %.cpp
#.cpp.o: 
	$(CXX) -c $(CFLAGS) $(INCLUDES) -MMD -MF $(@:%.o=%.d) -o $@ $<
	@cp $(@:%.o=%.d) $(@:%.o=%.P); \
	sed -e 's/#.*//' -e 's/^[^:]*: *//' -e 's/ *\\$$//' \
	-e '/^$$/ d' -e 's/$$/ :/' < $(@:%.o=%.d) >> $(@:%.o=%.P); \
	rm -f $(@:%.o=%.d)

$(TARGET): $(OBJS:obj/%=obj/%)
	$(AR) -crs $@ $^

obj:
	mkdir obj
	
clean:
	rm -f $(TARGET)
	-rm -f obj/*.o
	-rm -rf obj 

