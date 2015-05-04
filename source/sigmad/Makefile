ARCH		?= x86
OUTDIR		:= bin-$(ARCH)
AR  		?= $(CROSS_COMPILE)ar
CXX 		?= $(CROSS_COMPILE)g++
CC  		?= $(CROSS_COMPILE)gcc
LD  		?= $(CROSS_COMPILE)ld
LINK 		:= $(CXX)
INCLUDES	+= $(addprefix -I,$(CURDIR) ../ ../include ../libhttp ../librexjson ../libcmdargs ../liblogger)
ifneq ($(BUILD),release)
DEBUGFLAGS	= -g -O0
else
DEBUGFLAGS	= -O2
endif
xCFLAGS		= -Wall -Wextra -Wformat -Wno-unused-parameter $(DEBUGFLAGS) $(INCLUDES) $(CFLAGS)
xCXXFLAGS	= -Wall -Wextra -Wformat -Wno-unused-parameter $(DEBUGFLAGS) $(INCLUDES) $(CXXFLAGS)
xLDFLAGS 	= $(LDFLAGS)
LIBS		= -lattitude -llogger -ldevice -lhttp -lrexjson -lcmdargs -lm -lrt -pthread -ldl -lboost_system -lboost_filesystem -lboost_program_options -lboost_thread
LIBS		+= -L../libattitude/$(OUTDIR) -L../libdevice/$(OUTDIR) -L../libcmdargs/$(OUTDIR) -L../liblogger/$(OUTDIR) -L../libhttp/$(OUTDIR) -L../librexjson/$(OUTDIR)
TARGET 		= $(OUTDIR)/sigmad
OBJS		= \
		$(OUTDIR)/main.o \
		$(OUTDIR)/daemon.o \
		$(OUTDIR)/signals.o \
		$(OUTDIR)/clientapp.o \
		$(OUTDIR)/serverapp.o \
		$(OUTDIR)/userrpcserver.o \
		$(OUTDIR)/attcontroller.o \
		$(OUTDIR)/pidtorque.o \


all: $(OUTDIR) $(TARGET)

# auto-generated dependencies:
-include $(OUTDIR)/*.P

$(OUTDIR)/%.o: %.cpp
	$(CXX) -std=c++11 -c $(xCXXFLAGS) -MMD -MF $(@:%.o=%.d) -o $@ $<
	@cp $(@:%.o=%.d) $(@:%.o=%.P); \
	  sed -e 's/#.*//' -e 's/^[^:]*: *//' -e 's/ *\\$$//' \
	      -e '/^$$/ d' -e 's/$$/ :/' < $(@:%.o=%.d) >> $(@:%.o=%.P); \
	  rm -f $(@:%.o=%.d)

$(OUTDIR)/%.o: %.c
	$(CC) -c $(xCFLAGS) -MMD -MF $(@:%.o=%.d) -o $@ $<
	@cp $(@:%.o=%.d) $(@:%.o=%.P); \
	  sed -e 's/#.*//' -e 's/^[^:]*: *//' -e 's/ *\\$$//' \
	      -e '/^$$/ d' -e 's/$$/ :/' < $(@:%.o=%.d) >> $(@:%.o=%.P); \
	  rm -f $(@:%.o=%.d)

$(OUTDIR):
	mkdir $(OUTDIR)

$(TARGET): $(OBJS:$(OUTDIR)/%=$(OUTDIR)/%)
	$(LINK) $(xCXXFLAGS) -o $@ $^ $(xLDFLAGS) $(LIBS)

clean:
	-rm -f $(TARGET)
	-rm -f $(OUTDIR)/*.o
	-rm -rf $(OUTDIR) 
