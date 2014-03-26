SUBDIRS += cmdargs
SUBDIRS += jsonrpc
SUBDIRS += httpserver
SUBDIRS += httpclient

all:
	@for dir in $(SUBDIRS); do \
		$(MAKE) -C $$dir; \
	done

clean:
	@for dir in $(SUBDIRS); do \
		$(MAKE) clean -C $$dir; \
	done
