TOP=..

CXX	= c++
CC	= gcc
CFLAGS	= -Wall -g -I.
CXXFLAGS= $(CFLAGS)

MOC	= $(QTDIR)/bin/moc
KLIBS	= -lkdeui -lkfile -lkfm -lkdecore -lqt -lXext
KLDFLAGS= -L/usr/X11/lib -L$(QTDIR)/lib -L$(KDEDIR)/lib
KINCLUDE= -I$(KDEDIR)/include/kde -I$(QTDIR)/include

progs	= LispelShell
src	= LispelShell.cpp LSMainWindow.cpp
mocfiles= LSMainWindow.moc.cpp
objs	= $(src:.cpp=.o) $(mocfiles:.cpp=.o)

all:	$(progs)

.cpp.o:
	$(CXX) $(CXXFLAGS) $(KINCLUDE) -c $<

%.moc.cpp:	%.hh
	$(MOC) $< -o $(<:.hh=.moc.cpp)

LispelShell:		$(objs)
	$(CXX) $(CXXFLAGS) $(KINCLUDE) $(LDFLAGS) $(KLDFLAGS) -o LispelShell $(objs) $(KLIBS)

messages.po:	LispelShell.cpp
	xgettext -C -ki18n -x$(KDEDIR)/include/kde/kde.pot $(src)

.PHONY:	distclean

clean:
	-$(RM) *.o

distclean:	clean
	-$(RM) $(progs)
	-$(RM) core
	-$(RM) *~


