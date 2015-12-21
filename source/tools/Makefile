ARCH ?= x86
SUBDIRS += uartrpc
ifeq ($(ARCH),x86)
SUBDIRS += flightdeck
SUBDIRS += glplane
SUBDIRS += datastream

endif

all:
	@for dir in $(SUBDIRS); do \
		$(MAKE) -C $$dir; \
	done

clean:
	@for dir in $(SUBDIRS); do \
		$(MAKE) clean -C $$dir; \
	done
