${name}: force_look
	$(ECHO) looking into ${name} : $(MAKE) $(MFLAGS)
	cd ${name}; $(MAKE) $(MFLAGS)

# insert new build rule (do not remove this!!!)
${add_example_rule}
