ARCH		?= x86
OUTDIR		:= bin-$(ARCH)
AR  		?= $(CROSS_COMPILE)ar
CXX 		?= $(CROSS_COMPILE)g++
CC  		?= $(CROSS_COMPILE)gcc
LD  		?= $(CROSS_COMPILE)ld
LINK 		:= $(CXX)
INCLUDES	+= $(addprefix -I,$(CURDIR) ../.. ../../include ../../libgps)
ifneq ($(BUILD),release)
DEBUGFLAGS	= -g -O0
else
DEBUGFLAGS	= -O2
endif
xCFLAGS		= -Wall -Wextra -Wformat -Wno-unused-parameter $(INCLUDES) $(CFLAGS) $(DEBUGFLAGS)
xCXXFLAGS	= -Wall -Wextra -Wformat -Wno-unused-parameter $(INCLUDES) $(CXXFLAGS) $(DEBUGFLAGS)
xLDFLAGS 	= $(LDFLAGS)
LIBS		= -lgps -lm -lrt -pthread -ldl
LIBS		+= -L../../libgps/$(OUTDIR)
TARGET 		= $(OUTDIR)/testgps
OBJS		= \
		$(OUTDIR)/testgps.o \


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
