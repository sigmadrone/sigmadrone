SUBDIRS += libattitude
SUBDIRS += libdevice
SUBDIRS += libhttp
SUBDIRS += libcmdargs
SUBDIRS += liblogger
SUBDIRS += libjsonspirit
SUBDIRS += libjsonrpc
SUBDIRS += tools
SUBDIRS += plugins
SUBDIRS += sdrone
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
