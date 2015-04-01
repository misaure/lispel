all:
	(cd lispel; $(MAKE); cd ..)
	(cd tests; $(MAKE) -f Makefile.simple all tests; cd ..)

