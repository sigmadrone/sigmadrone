ARCH		?= x86
OUTDIR		:= bin-$(ARCH)
AR			?= $(CROSS_COMPILE)ar
CXX			?= $(CROSS_COMPILE)g++
CC			?= $(CROSS_COMPILE)gcc
LD			?= $(CROSS_COMPILE)ld
INCLUDES	+= $(addprefix -I,$(CURDIR) include ../include ..)
ifneq ($(BUILD),release)
DEBUGFLAGS	= -g -O0
else
DEBUGFLAGS	= -O2
endif
xCFLAGS		= -Wall -Wextra -Wformat -Wno-unused-parameter $(INCLUDES) $(CFLAGS) $(DEBUGFLAGS)
xCXXFLAGS	= -Wall -Wextra -Wformat -Wno-unused-parameter $(INCLUDES) $(CXXFLAGS) $(DEBUGFLAGS)
xLDFLAGS	= $(LDFLAGS)
TARGET1		= $(OUTDIR)/libgtest_main.a
TARGET2		= $(OUTDIR)/libgtest.a


OBJS1= \
	$(OUTDIR)/gtest-all.o \
	$(OUTDIR)/gtest_main.o \

OBJS2= \
	$(OUTDIR)/gtest-all.o \

all: $(OUTDIR) $(TARGET1) $(TARGET2)

# auto-generated dependencies:
-include $(OUTDIR)/*.P

$(OUTDIR)/%.o: src/%.cpp
	$(CXX) -std=c++11 -c $(xCXXFLAGS) -MMD -MF $(@:%.o=%.d) -o $@ $<
	@cp $(@:%.o=%.d) $(@:%.o=%.P); \
	  sed -e 's/#.*//' -e 's/^[^:]*: *//' -e 's/ *\\$$//' \
	      -e '/^$$/ d' -e 's/$$/ :/' < $(@:%.o=%.d) >> $(@:%.o=%.P); \
	  rm -f $(@:%.o=%.d)

$(OUTDIR)/%.o: src/%.cc
	$(CXX) -std=c++11 -c $(xCXXFLAGS) -MMD -MF $(@:%.o=%.d) -o $@ $<
	@cp $(@:%.o=%.d) $(@:%.o=%.P); \
	  sed -e 's/#.*//' -e 's/^[^:]*: *//' -e 's/ *\\$$//' \
	      -e '/^$$/ d' -e 's/$$/ :/' < $(@:%.o=%.d) >> $(@:%.o=%.P); \
	  rm -f $(@:%.o=%.d)

$(OUTDIR)/%.o: src/%.c
	$(CC) -c $(xCFLAGS) -MMD -MF $(@:%.o=%.d) -o $@ $<
	@cp $(@:%.o=%.d) $(@:%.o=%.P); \
	  sed -e 's/#.*//' -e 's/^[^:]*: *//' -e 's/ *\\$$//' \
	      -e '/^$$/ d' -e 's/$$/ :/' < $(@:%.o=%.d) >> $(@:%.o=%.P); \
	  rm -f $(@:%.o=%.d)

$(OUTDIR):
	mkdir $(OUTDIR)

$(TARGET1): $(OBJS1:$(OUTDIR)/%=$(OUTDIR)/%)
	$(AR) -crs $@ $^

$(TARGET2): $(OBJS2:$(OUTDIR)/%=$(OUTDIR)/%)
	$(AR) -crs $@ $^

clean:
	-rm -f $(TARGET1)
	-rm -f $(TARGET2)	
	-rm -f $(OUTDIR)/*.o
	-rm -rf $(OUTDIR) 
