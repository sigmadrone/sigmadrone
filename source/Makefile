SUBDIRS += libattitude
SUBDIRS += libhttp
SUBDIRS += libcmdargs
SUBDIRS += liblogger
SUBDIRS += librexjson
SUBDIRS += librexjsonrpc
SUBDIRS += libgps
SUBDIRS += libgoogletest
SUBDIRS += tools
SUBDIRS += sigmad
SUBDIRS += tests
SUBDIRS += unittests

all:
	@for dir in $(SUBDIRS); do \
		$(MAKE) -C $$dir; \
	done

clean:
	@for dir in $(SUBDIRS); do \
		$(MAKE) clean -C $$dir; \
	done

test:
	$(MAKE) test -C unittests; \

