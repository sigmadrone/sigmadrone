SUBDIRS += d3math

all:
	@for dir in $(SUBDIRS); do \
		$(MAKE) -C $$dir; \
	done

clean:
	@for dir in $(SUBDIRS); do \
		$(MAKE) clean -C $$dir; \
	done

test:
	@for dir in $(SUBDIRS); do \
		$(MAKE) test -C $$dir; \
	done
