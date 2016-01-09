ARCH		?= x86
OUTDIR		:= bin-$(ARCH)
AR  		?= $(CROSS_COMPILE)ar
CXX 		?= $(CROSS_COMPILE)g++
CC  		?= $(CROSS_COMPILE)gcc
LD  		?= $(CROSS_COMPILE)ld
LINK 		:= $(CXX)
INCLUDES	+= $(addprefix -I,$(CURDIR) ../.. ../../include ../../libhttp ../../librexjson ../../librexjsonrpc)
INCLUDES	+= $(addprefix -I,../../libcmdargs ../../liblogger ../../libdevice ../../libattitude)
INCLUDES	+= $(addprefix -I,../../libgoogletest/include)
ifneq ($(BUILD),release)
DEBUGFLAGS	= -g -O0
else
DEBUGFLAGS	= -O2
endif
xCFLAGS		= -Wall -Wextra -Wformat -Wno-unused-parameter $(INCLUDES) $(CFLAGS) $(DEBUGFLAGS)
xCXXFLAGS	= -Wall -Wextra -Wformat -Wno-unused-parameter $(INCLUDES) $(CXXFLAGS) $(DEBUGFLAGS)
xLDFLAGS 	= $(LDFLAGS)
LIBS		= -lattitude -llogger -lrexjsonrpc -lrexjson -lhttp -lcmdargs -lgtest_main
LIBS		+= -lm -lrt -pthread -ldl -lboost_system -lboost_filesystem -lboost_program_options -lboost_thread
LIBS		+= $(addprefix -L,../../libattitude/$(OUTDIR) ../../libcmdargs/$(OUTDIR) ../../libgoogletest/$(OUTDIR))
LIBS		+= $(addprefix -L,../../liblogger/$(OUTDIR) ../../libhttp/$(OUTDIR) ../../librexjson/$(OUTDIR) ../../librexjsonrpc/$(OUTDIR))
TARGET1		= $(OUTDIR)/sample1
OBJS1		= \
		$(OUTDIR)/sample1.o \
		$(OUTDIR)/sample1_unittest.o \

TARGET2		= $(OUTDIR)/sample2
OBJS2		= \
		$(OUTDIR)/sample2.o \
		$(OUTDIR)/sample2_unittest.o \

TARGET3		= $(OUTDIR)/sample3
OBJS3		= \
		$(OUTDIR)/sample3_unittest.o \

TARGET4		= $(OUTDIR)/sample4
OBJS4		= \
		$(OUTDIR)/sample4.o \
		$(OUTDIR)/sample4_unittest.o \

TARGET5		= $(OUTDIR)/sample5
OBJS5		= \
		$(OUTDIR)/sample1.o \
		$(OUTDIR)/sample5_unittest.o \

TARGET6		= $(OUTDIR)/sample6
OBJS6		= \
		$(OUTDIR)/sample6_unittest.o \

TARGET7		= $(OUTDIR)/sample7
OBJS7		= \
		$(OUTDIR)/sample7_unittest.o \

TARGET8		= $(OUTDIR)/sample8
OBJS8		= \
		$(OUTDIR)/sample8_unittest.o \

TARGET9		= $(OUTDIR)/sample9
OBJS9		= \
		$(OUTDIR)/sample9_unittest.o \

TARGET10	= $(OUTDIR)/sample10
OBJS10		= \
		$(OUTDIR)/sample10_unittest.o \


TARGETS		= $(TARGET1) $(TARGET2) $(TARGET3) $(TARGET4) $(TARGET5) $(TARGET6) $(TARGET7) $(TARGET8) $(TARGET9) $(TARGET10)

all: $(OUTDIR) $(TARGETS)


test: all
	@for target in $(TARGETS); do $$target; done

# auto-generated dependencies:
-include $(OUTDIR)/*.P

$(OUTDIR)/%.o: %.cpp
	$(CXX) -std=c++11 -c $(xCXXFLAGS) -MMD -MF $(@:%.o=%.d) -o $@ $<
	@cp $(@:%.o=%.d) $(@:%.o=%.P); \
	  sed -e 's/#.*//' -e 's/^[^:]*: *//' -e 's/ *\\$$//' \
	      -e '/^$$/ d' -e 's/$$/ :/' < $(@:%.o=%.d) >> $(@:%.o=%.P); \
	  rm -f $(@:%.o=%.d)

$(OUTDIR)/%.o: %.cc
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

$(TARGET1): $(OBJS1:$(OUTDIR1)/%=$(OUTDIR)/%)
	$(LINK) $(xCXXFLAGS) -o $@ $^ $(xLDFLAGS) $(LIBS)

$(TARGET2): $(OBJS2:$(OUTDIR)/%=$(OUTDIR)/%)
	$(LINK) $(xCXXFLAGS) -o $@ $^ $(xLDFLAGS) $(LIBS)

$(TARGET3): $(OBJS3:$(OUTDIR)/%=$(OUTDIR)/%)
	$(LINK) $(xCXXFLAGS) -o $@ $^ $(xLDFLAGS) $(LIBS)

$(TARGET4): $(OBJS4:$(OUTDIR)/%=$(OUTDIR)/%)
	$(LINK) $(xCXXFLAGS) -o $@ $^ $(xLDFLAGS) $(LIBS)

$(TARGET5): $(OBJS5:$(OUTDIR)/%=$(OUTDIR)/%)
	$(LINK) $(xCXXFLAGS) -o $@ $^ $(xLDFLAGS) $(LIBS)

$(TARGET6): $(OBJS6:$(OUTDIR)/%=$(OUTDIR)/%)
	$(LINK) $(xCXXFLAGS) -o $@ $^ $(xLDFLAGS) $(LIBS)

$(TARGET7): $(OBJS7:$(OUTDIR)/%=$(OUTDIR)/%)
	$(LINK) $(xCXXFLAGS) -o $@ $^ $(xLDFLAGS) $(LIBS)

$(TARGET8): $(OBJS8:$(OUTDIR)/%=$(OUTDIR)/%)
	$(LINK) $(xCXXFLAGS) -o $@ $^ $(xLDFLAGS) $(LIBS)

$(TARGET9): $(OBJS9:$(OUTDIR)/%=$(OUTDIR)/%)
	$(LINK) $(xCXXFLAGS) -o $@ $^ $(xLDFLAGS) $(LIBS)

$(TARGET10): $(OBJS10:$(OUTDIR)/%=$(OUTDIR)/%)
	$(LINK) $(xCXXFLAGS) -o $@ $^ $(xLDFLAGS) $(LIBS)

clean:
	-rm -f $(TARGET)
	-rm -f $(TARGETS)
	-rm -f $(OUTDIR)/*.o
	-rm -rf $(OUTDIR) 
