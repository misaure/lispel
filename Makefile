
all:
	cd lispel; $(MAKE) -f Makefile.simple all; cd ..
	cd tests; $(MAKE) -f Makefile.simple all tests; cd ..

