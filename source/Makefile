SUBDIRS += http
SUBDIRS += tools
SUBDIRS += jsonrpc
SUBDIRS += plugins
SUBDIRS += sdrone
SUBDIRS += tests

all:
	@for dir in $(SUBDIRS); do \
		$(MAKE) -C $$dir; \
	done

clean:
	@for dir in $(SUBDIRS); do \
		$(MAKE) clean -C $$dir; \
	done
