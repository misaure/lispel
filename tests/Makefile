CC		= gcc
CXX		= c++
CPPFLAGS	= -I.. -I/usr/local/include
CFLAGS		= -g -Wall $(CPPFLAGS)
CXXFLAGS	= -g -Wall $(CPPFLAGS)
#CFLAGS		= -Wall -O3 -I..
#CXXFLAGS	= -Wall -O3 -I..
LDFLAGS		= -L../lispel -L/usr/local/lib

RM		= rm -f

EXEEXT		=

deps		=
cxxsrc		= chartypes.cc scanner0.cc nodefactory0.cc	\
		  nodefactory1.cc reader0.cc environment0.cc cat.cc	\
		  interp0.cc interp1.cc

testprogs	= chartypes$(EXEEXT) scanner0$(EXEEXT)  \
		  nodefactory0$(EXEEXT) nodefactory1$(EXEEXT)		  \
		  reader0$(EXEEXT) environment0$(EXEEXT) cat$(EXEEXT)	  \
		  interp0$(EXEEXT) interp1$(EXEEXT) 	  \
		  xlateChar$(EXEEXT) boosttests$(EXEEXT) mathcmdtests$(EXEEXT)

all:		$(testprogs) runall

runall:		$(testprogs)
	./boosttests
	./mathcmdtests
	./chartypes
	./environment0
	./nodefactory0
	./nodefactory1
	./reader0
	./scanner0
	./xlateChar
	

# -------------------------------------------------------------------------
.cc.o:
	$(CXX) $(CXXFLAGS) -c $<

boosttests$(EXEEXT):	boosttests.o
	$(CXX) $(LDFLAGS) -o boosttests$(EXEEXT) boosttests.o  -lboost_unit_test_framework -lispel

mathcmdtests$(EXEEXT):	mathcmdtests.o
	$(CXX) $(LDFLAGS) -o mathcmdtests$(EXEEXT) mathcmdtests.o  -lboost_unit_test_framework -lispel


nil0$(EXEEXT):		nil0.o $(deps)
	$(CXX) $(LDFLAGS) -o nil0$(EXEEXT) nil0.o -lispel

interp1$(EXEEXT):	interp1.o $(deps)
	$(CXX) $(LDFLAGS) -o interp1$(EXEEXT) interp1.o -lispel

interp0$(EXEEXT):	interp0.o $(deps)
	$(CXX) $(LDFLAGS) -o interp0$(EXEXT) interp0.o -lispel

reader0$(EXEEXT):	reader0.o $(deps)
	$(CXX) $(LDFLAGS) -o reader0$(EXEEXT) reader0.o -lispel

nodefactory0$(EXEEXT):	nodefactory0.o $(deps)
	$(CXX) $(LDFLAGS) -o nodefactory0$(EXEEXT) nodefactory0.o -lispel

nodefactory1$(EXEEXT):	nodefactory1.o $(deps)
	$(CXX) $(LDFLAGS) -o nodefactory1$(EXEEXT) nodefactory1.o -lispel

checkeval$(EXEEXT):	checkeval.o $(deps)
	$(CXX) $(LDFLAGS) -o checkeval$(EXEEXT) checkeval.o -lispel

chartypes$(EXEEXT):	chartypes.o $(deps)
	$(CXX) $(LDFLAGS) -o chartypes$(EXEEXT) chartypes.o -lispel

scanner0$(EXEEXT):	scanner0.o $(deps)
	$(CXX) $(LDFLAGS) -o scanner0$(EXEEXT) scanner0.o -lispel

cat$(EXEEXT):	cat.o $(deps)
	$(CXX) $(LDFLAGS) -o cat$(EXEEXT) cat.o -lispel

xlateChar$(EXEEXT):	xlateChar.o $(deps)
	$(CXX) $(LDFLAGS) -o xlateChar$(EXEEXT) xlateChar.o -lispel

environment0$(EXEEXT):	environment0.o $(deps)
	$(CXX) $(LDFLAGS) -o environment0$(EXEEXT) environment0.o -lispel

# -------------------------------------------------------------------------
.PHONY:	clean distclean dep wc

clean:
	-$(RM) *.o

distclean:	clean
	-$(RM) *~
	-$(RM) $(testprogs)
	-$(RM) core

dep:
	$(CC) -M $(CFLAGS) $(cxxsrc) > depend

wc:
	wc *.cc *.scm *.tst

