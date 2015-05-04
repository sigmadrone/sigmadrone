#SUBDIRS += jsonrpc
SUBDIRS += cmdargs
SUBDIRS += httpserver
SUBDIRS += httpclient
SUBDIRS += base64
SUBDIRS += quaternion
SUBDIRS += matrix

all:
	@for dir in $(SUBDIRS); do \
		$(MAKE) -C $$dir; \
	done

clean:
	@for dir in $(SUBDIRS); do \
		$(MAKE) clean -C $$dir; \
	done
