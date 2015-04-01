# a simple Makefile for systems which are not able to run the autoconfiguration scripts

CC		= gcc
CXX		= c++
ifeq ($(debug),yes)
  DEFS		= -DHAVE_CONFIG_H -DDEBUG=4
  CXXFLAGS	= -g -Wall -pipe -I.. $(DEFS)
  CFLAGS	= -g -Wall -pipe -I.. $(DEFS)
else
  DEFS		= -DHAVE_CONFIG_H -UDEBUG
  CXXFLAGS	= -Wall -pipe -I.. -O2 $(DEFS)
  CFLAGS	= -Wall -pipe -I.. -O2 $(DEFS)
endif
LDFLAGS		= -L.

RM		= rm -f

c_src		= 
cxx_src		= lexer.cpp nodes.cpp environment.cpp nodefactory.cpp reader.cpp \
		  evaluator.cpp interpreter.cpp corecmd.cpp lispelvm.cpp        \
		  exception.cpp utilities.cpp listcmd.cpp mathcmd.cpp	    \
		  userinterface.cpp gc.cpp hashcmd.cpp setcmd.cpp
cxx_hdr		= lexer.hh nodes.hh environment.hh nodefactory.hh reader.hh \
		  evaluator.hh interpreter.hh lispelvm.hh corecmd.hh	    \
		  exception.hh defs.hh utilities.hh listcmd.hh mathcmd.hh   \
		  userinterface.hh gc.hh iterator.hh hashcmd.hh setcmd.hh

mclibs		= 

objs		= $(c_src:.c=.o) $(cxx_src:.cpp=.o)
libname		= libispel.a

all:	$(libname) lispel

%.o: %.cpp
	$(CXX) $(CXXFLAGS) -c $(notdir $<) -o $@

.c.o:
	$(CC) $(CFLAGS) -c $<

-include depend

lispel:	$(libname) driver.o
	$(CXX) $(LDFLAGS) -o lispel driver.o -lispel

$(libname):	$(objs)
	-$(RM) $(libname)
	ar rvs $(libname) $(objs)
	ranlib $(libname)

.PHONY:	dep clean distclean wc

dep:
	$(CXX) $(CXXFLAGS) -M $(c_src) $(cxx_src) > depend

clean:
	-$(RM) *.o

distclean:	clean
	-$(RM) $(libname)
	-$(RM) lispel
	-$(RM) vminterp
	-$(RM) depend
	-$(RM) *~
	-$(RM) core

wc:
	wc *.cpp *.hh | sort -n

