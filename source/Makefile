SUBDIRS += http
SUBDIRS += tools
SUBDIRS += tests
SUBDIRS += plugins
SUBDIRS += sdrone


all:
	@for dir in $(SUBDIRS); do \
		$(MAKE) -C $$dir; \
	done

clean:
	@for dir in $(SUBDIRS); do \
		$(MAKE) clean -C $$dir; \
	done
