ARCH ?= x86
SUBDIRS += axesdata
SUBDIRS += alldata
SUBDIRS += servo
SUBDIRS += attitudefilter
SUBDIRS += uartrpc
ifeq ($(ARCH),x86)
SUBDIRS += flightdeck
SUBDIRS += glplane
endif

all:
	@for dir in $(SUBDIRS); do \
		$(MAKE) -C $$dir; \
	done

clean:
	@for dir in $(SUBDIRS); do \
		$(MAKE) clean -C $$dir; \
	done
