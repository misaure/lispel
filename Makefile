all:
	(cd lispel; $(MAKE); cd ..)
	(cd tests; $(MAKE) all tests; cd ..)

