AR  ?= $(CROSS_COMPILE)ar
CXX ?= $(CROSS_COMPILE)g++
CC  ?= $(CROSS_COMPILE)gcc
LD  ?= $(CROSS_COMPILE)ld
DEFS += $(addprefix -I,$(CURDIR) $(CURDIR)/obj ..)
DEBUGFLAGS = -g -O0
xCXXFLAGS = -O2 -Wall -Wextra -Wformat -Wno-unused-parameter $(DEBUGFLAGS) $(DEFS) $(CXXFLAGS)
xLDFLAGS = $(LDFLAGS)
TARGET = libjsonspirit.a

OBJS= \
	obj/json_spirit_reader.o \
	obj/json_spirit_value.o \
	obj/json_spirit_writer.o \
	obj/rpcclient.o \


all: obj $(TARGET)

# auto-generated dependencies:
-include obj/*.P

obj/%.o: %.cpp
	$(CXX) -std=c++11 -c $(xCXXFLAGS) -MMD -MF $(@:%.o=%.d) -o $@ $<
	@cp $(@:%.o=%.d) $(@:%.o=%.P); \
	  sed -e 's/#.*//' -e 's/^[^:]*: *//' -e 's/ *\\$$//' \
	      -e '/^$$/ d' -e 's/$$/ :/' < $(@:%.o=%.d) >> $(@:%.o=%.P); \
	  rm -f $(@:%.o=%.d)

obj/%.o: %.c
	$(CXX) -std=c++11 -c $(xCXXFLAGS) -MMD -MF $(@:%.o=%.d) -o $@ $<
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
