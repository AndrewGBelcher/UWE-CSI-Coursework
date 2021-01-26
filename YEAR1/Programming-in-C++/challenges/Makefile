# File: Makefile
# Author: Benedict R. Gaster
# Date: 9th June 2017
# Desc: Makefile for all examples in this folder. Note that
#       it is possible automatically add a new challenge using the
#       create_challenge.sh script (found in ../templates/challenges) and
#       for this to work correclty there are placeholders in this file:
#
# Copyright: University of West of England 2017
#
ifeq ($(origin ROOTDIR), undefined)
	ROOTDIR=..
endif
include $(ROOTDIR)/MakefileDefs.inc

DIRS = challenge001 challenge000 challenge010 challenge011 100 challenge100 ${add_challenge}

all: challenge001 challenge000 challenge010 challenge011 100 challenge100 ${add_challenge}

# insert new build rule (do not remove this!!!)
001: force_look
	$(ECHO) looking into 001 : $(MAKE) $(MFLAGS)
	cd 001; $(MAKE) $(MFLAGS)

# insert new build rule (do not remove this!!!)
challenge001: force_look
	$(ECHO) looking into challenge001 : $(MAKE) $(MFLAGS)
	cd challenge001; $(MAKE) $(MFLAGS)

# insert new build rule (do not remove this!!!)
challenge000: force_look
	$(ECHO) looking into challenge000 : $(MAKE) $(MFLAGS)
	cd challenge000; $(MAKE) $(MFLAGS)

# insert new build rule (do not remove this!!!)
challenge010: force_look
	$(ECHO) looking into challenge010 : $(MAKE) $(MFLAGS)
	cd challenge010; $(MAKE) $(MFLAGS)

# insert new build rule (do not remove this!!!)
challenge011: force_look
	$(ECHO) looking into challenge011 : $(MAKE) $(MFLAGS)
	cd challenge011; $(MAKE) $(MFLAGS)

# insert new build rule (do not remove this!!!)
100: force_look
	$(ECHO) looking into 100 : $(MAKE) $(MFLAGS)
	cd 100; $(MAKE) $(MFLAGS)

# insert new build rule (do not remove this!!!)
challenge100: force_look
	$(ECHO) looking into challenge100 : $(MAKE) $(MFLAGS)
	cd challenge100; $(MAKE) $(MFLAGS)

# insert new build rule (do not remove this!!!)
${add_challenge_rule}

install:
	$(ECHO) installing up in .
	-for d in $(DIRS); do (cd $$d; $(MAKE) install ); done

clean:
	$(ECHO) cleaning up in .
	-for d in $(DIRS); do (cd $$d; $(MAKE) clean ); done

force_look:
	true
