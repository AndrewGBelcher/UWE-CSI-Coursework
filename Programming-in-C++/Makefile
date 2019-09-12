# Setup shared variables used in sub makefiles
ROOTDIR=${PWD}
export

include $(ROOTDIR)/MakefileDefs.inc

DIRS	= challenges

all:

#######################################
# build applications
#######################################
challenges: force_look
	$(ECHO) looking into challenges : $(MAKE) $(MFLAGS)
	cd challenges; $(MAKE) $(MFLAGS)

#######################################
# build assignment 
#######################################
assignment: force_look
	$(ECHO) looking into assignment : $(MAKE) $(MFLAGS)
	cd assignment; $(MAKE) $(MFLAGS)

#######################################
# clean everything
#######################################
clean :
	$(ECHO) cleaning up in .
	-$(RM) -f $(EXE) $(OBJS) $(OBJLIBS)
	-for d in $(DIRS); do (cd $$d; $(MAKE) clean ); done

force_look:
	true
