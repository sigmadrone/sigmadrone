SUBDIRS += cmdargs
SUBDIRS += jsonrpc
SUBDIRS += httpserver

all:
	@for dir in $(SUBDIRS); do \
		$(MAKE) -C $$dir; \
	done

clean:
	@for dir in $(SUBDIRS); do \
		$(MAKE) clean -C $$dir; \
	done
