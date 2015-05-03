AR  		?= $(CROSS_COMPILE)ar
CXX 		?= $(CROSS_COMPILE)g++
CC  		?= $(CROSS_COMPILE)gcc
LD  		?= $(CROSS_COMPILE)ld
LINK 		:= $(CXX)
DEFS 		+= $(addprefix -I,$(CURDIR) $(CURDIR)/obj)
DEBUGFLAGS 	= -g -O0
INCLUDES	= -I../include -I../ -I../libhttp -I../librexjson -I../libcmdargs -I../liblogger
xCXXFLAGS 	= -O2 -Wall -Wextra -Wformat -Wno-unused-parameter $(INCLUDES) $(DEBUGFLAGS) $(DEFS) $(CXXFLAGS)
xLDFLAGS 	= $(LDFLAGS)
LIBS		= -lattitude -llogger -ldevice -lhttp -lrexjson -lcmdargs -lm -lrt -pthread -ldl -lboost_system -lboost_filesystem -lboost_program_options -lboost_thread
LIBS		+= -L../libattitude -L../libdevice -L../libcmdargs -L../liblogger -L../libhttp -L../librexjson
TARGET 		= sigmad
OBJS		= \
		obj/main.o \
		obj/daemon.o \
		obj/signals.o \
		obj/clientapp.o \
		obj/serverapp.o \
		obj/userrpcserver.o \
		obj/attcontroller.o \
		obj/pidtorque.o \


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
	$(LINK) $(xCXXFLAGS) -o $@ $^ $(xLDFLAGS) $(LIBS)

clean:
	-rm -f $(TARGET)
	-rm -f obj/*.o
	-rm -rf obj 
