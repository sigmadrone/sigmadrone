SUBDIRS += axesdata
SUBDIRS += alldata
SUBDIRS += servo
SUBDIRS += attitudefilter
SUBDIRS += flightdeck

all:
	@for dir in $(SUBDIRS); do \
		$(MAKE) -C $$dir; \
	done

clean:
	@for dir in $(SUBDIRS); do \
		$(MAKE) clean -C $$dir; \
	done
