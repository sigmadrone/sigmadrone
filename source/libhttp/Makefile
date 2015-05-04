ARCH		?= x86
OUTDIR		:= bin-$(ARCH)
AR			?= $(CROSS_COMPILE)ar
CXX			?= $(CROSS_COMPILE)g++
CC			?= $(CROSS_COMPILE)gcc
LD 			= $(CROSS_COMPILE)ld
INCLUDES	+= $(addprefix -I,$(CURDIR) ../include ..)
ifneq ($(BUILD),release)
DEBUGFLAGS	= -g -O0
else
DEBUGFLAGS	= -O2
endif
xCFLAGS		= -Wall -Wextra -Wformat -Wno-unused-parameter $(DEBUGFLAGS) $(INCLUDES) $(CFLAGS)
xCXXFLAGS	= -Wall -Wextra -Wformat -Wno-unused-parameter $(DEBUGFLAGS) $(INCLUDES) $(CXXFLAGS)
xLDFLAGS	= $(LDFLAGS)
TARGET		= $(OUTDIR)/libhttp.a

OBJS= \
    $(OUTDIR)/connection.o \
    $(OUTDIR)/connection_manager.o \
    $(OUTDIR)/mime_types.o \
    $(OUTDIR)/reply.o \
    $(OUTDIR)/request.o \
    $(OUTDIR)/request_handler.o \
    $(OUTDIR)/request_parser.o \
    $(OUTDIR)/http_server.o \
    $(OUTDIR)/http_client.o \
    $(OUTDIR)/response_parser.o \
    $(OUTDIR)/base64.o \


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
	$(AR) -crs $@ $^


clean:
	-rm -f $(TARGET)
	-rm -f $(OUTDIR)/*.o
	-rm -rf $(OUTDIR) 
