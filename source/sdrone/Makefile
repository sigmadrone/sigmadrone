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
xCFLAGS		= -Wall -Wextra -Wformat -Wno-unused-parameter $(INCLUDES) $(CFLAGS) $(DEBUGFLAGS)
xCXXFLAGS	= -Wall -Wextra -Wformat -Wno-unused-parameter $(INCLUDES) $(CXXFLAGS) $(DEBUGFLAGS)
xLDFLAGS 	= $(LDFLAGS)

LIBHTTP = ../libhttp
LIBJSONRPC = ../libjsonrpc
LIBCMDARGS = ../libcmdargs
LIBDEVICE = ../libdevice
LIBATTITUDE = ../libattitude
LIBREXJSON = ../librexjson
LIBLOGGER = ../liblogger

INCLUDES    = -I../include -I.. -I$(LIBJSONRPC) -I$(LIBDEVICE) -I$(LIBATTITUDE)
INCLUDES   += $(shell pkg-config --cflags glib-2.0) $(shell pkg-config --cflags json-glib-1.0)
LIBS		= -llogger -lm -lrt -pthread -ldl -ldevice -lattitude -ljsonrpc -lhttp -lcmdargs
LIBS        += $(shell pkg-config --libs glib-2.0) $(shell pkg-config --libs json-glib-1.0)
LIBS		+= -lboost_system -lboost_system -lboost_filesystem -lboost_program_options -lboost_thread -lcmdargs
LIBS		+= -L$(LIBATTITUDE)/$(OUTDIR) -L$(LIBDEVICE)/$(OUTDIR) -L$(LIBCMDARGS)/$(OUTDIR) -L$(LIBJSONRPC)/$(OUTDIR)
LIBS        += -L$(LIBLOGGER)/$(OUTDIR) -L$(LIBHTTP)/$(OUTDIR) -L$(LIBREXJSON)/$(OUTDIR)
#use -Wno-psabi to turn off warnings about ABI changes for va-list in GCC 4.4

TARGET 		= $(OUTDIR)/sdroned
OBJS		= \
		$(OUTDIR)/main.o \
		$(OUTDIR)/commandargs.o\
		$(OUTDIR)/pluginchain.o\
		$(OUTDIR)/plugincontext.o\
		$(OUTDIR)/main.o\
		$(OUTDIR)/drone.o\
		$(OUTDIR)/servoplugin.o\
		$(OUTDIR)/quadrotorpilot.o\
		$(OUTDIR)/imureader.o\
		$(OUTDIR)/navigator.o\
		$(OUTDIR)/imufilterplugin.o\
		$(OUTDIR)/imuremap.o\
		$(OUTDIR)/imubias.o\
		$(OUTDIR)/tracelogplugin.o\
		$(OUTDIR)/imulowpassfilter.o\
		$(OUTDIR)/iopacket.o\
		$(OUTDIR)/kalmanattitudefilter.o\
		$(OUTDIR)/daemon.o\
		$(OUTDIR)/dronerpcclient.o\
		$(OUTDIR)/droneconfig.o\
		$(OUTDIR)/rpcparams.o\
		$(OUTDIR)/pidpilotsd.o\
		$(OUTDIR)/pidtorque.o\
		$(OUTDIR)/pluginregistry.o\
		$(OUTDIR)/thrustcorrectionplugin.o

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