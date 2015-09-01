SUBDIRS += libattitude
SUBDIRS += libdevice
SUBDIRS += libhttp
SUBDIRS += libcmdargs
SUBDIRS += liblogger
SUBDIRS += librexjson
SUBDIRS += librexjsonrpc
SUBDIRS += tools
SUBDIRS += plugins
SUBDIRS += sigmad
SUBDIRS += tests

all:
	@for dir in $(SUBDIRS); do \
		$(MAKE) -C $$dir; \
	done

clean:
	@for dir in $(SUBDIRS); do \
		$(MAKE) clean -C $$dir; \
	done
