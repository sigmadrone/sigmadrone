SUBDIRS += axesdata
SUBDIRS += alldata
SUBDIRS += servo
SUBDIRS += attitudefilter


all:
	@for dir in $(SUBDIRS); do \
		$(MAKE) -C $$dir; \
	done

clean:
	@for dir in $(SUBDIRS); do \
		$(MAKE) clean -C $$dir; \
	done
