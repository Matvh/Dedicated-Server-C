all:
	+$(MAKE) -C Gruder
	+$(MAKE) -C Lionel-S
	+$(MAKE) -C Tavish
clean:
	$(MAKE) -C Gruder clean
	$(MAKE) -C Lionel-S clean
	$(MAKE) -C Tavish clean
