SUBDIRS += cmdargs
SUBDIRS += httpserver
SUBDIRS += httpclient
SUBDIRS += base64
SUBDIRS += quaternion
SUBDIRS += matrix
SUBDIRS += gps

all:
	@for dir in $(SUBDIRS); do \
		$(MAKE) -C $$dir; \
	done

clean:
	@for dir in $(SUBDIRS); do \
		$(MAKE) clean -C $$dir; \
	done
