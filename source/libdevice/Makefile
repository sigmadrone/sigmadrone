AR  ?= $(CROSS_COMPILE)ar
CXX ?= $(CROSS_COMPILE)g++
CC  ?= $(CROSS_COMPILE)gcc
LD  ?= $(CROSS_COMPILE)ld
DEFS += $(addprefix -I,$(CURDIR) $(CURDIR)/obj ../include)
DEBUGFLAGS = -g -O0
xCXXFLAGS = -O2 -Wall -Wextra $(DEBUGFLAGS) $(DEFS) $(CXXFLAGS) 
xLDFLAGS = $(LDFLAGS)
TARGET = libcmdargs.a

OBJS= \
    obj/imudevice.o \

all: obj $(TARGET)

# auto-generated dependencies:
-include obj/*.P

obj/%.o: %.cpp
	$(CXX) -c $(xCXXFLAGS) -MMD -MF $(@:%.o=%.d) -o $@ $<
	@cp $(@:%.o=%.d) $(@:%.o=%.P); \
	  sed -e 's/#.*//' -e 's/^[^:]*: *//' -e 's/ *\\$$//' \
	      -e '/^$$/ d' -e 's/$$/ :/' < $(@:%.o=%.d) >> $(@:%.o=%.P); \
	  rm -f $(@:%.o=%.d)

obj/%.o: %.c
	$(CXX) -c $(xCXXFLAGS) -MMD -MF $(@:%.o=%.d) -o $@ $<
	@cp $(@:%.o=%.d) $(@:%.o=%.P); \
	  sed -e 's/#.*//' -e 's/^[^:]*: *//' -e 's/ *\\$$//' \
	      -e '/^$$/ d' -e 's/$$/ :/' < $(@:%.o=%.d) >> $(@:%.o=%.P); \
	  rm -f $(@:%.o=%.d)

obj:
	mkdir obj

$(TARGET): $(OBJS:obj/%=obj/%)
	$(AR) -crs $@ $^


clean:
	-rm -f $(TARGET)
	-rm -f obj/*.o
	-rm -rf obj 
