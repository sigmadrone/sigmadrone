SUBDIRS += cmdargs
SUBDIRS += jsonrpc
SUBDIRS += httpserver
SUBDIRS += httpclient
SUBDIRS += base64

all:
	@for dir in $(SUBDIRS); do \
		$(MAKE) -C $$dir; \
	done

clean:
	@for dir in $(SUBDIRS); do \
		$(MAKE) clean -C $$dir; \
	done
