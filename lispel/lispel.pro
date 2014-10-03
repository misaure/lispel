SOURCES += corecmd.cpp driver.cpp environment.cpp evaluator.cpp exception.cpp filecmd.cpp gc.cpp hashcmd.cpp interpreter.cpp lexer.cpp lispelvm.cpp listcmd.cpp mathcmd.cpp nodefactory.cpp nodes.cpp reader.cpp setcmd.cpp trace.cpp userinterface.cpp utilities.cpp vminterp.cpp

HEADERS += context.hh corecmd.hh defs.hh environment.hh evaluator.hh exception.hh filecmd.hh gc.hh hashcmd.hh interpreter.hh iterator.hh lexer.hh lispel.hh lispelvm.hh listcmd.hh mathcmd.hh nodefactory.hh nodes.hh pointer.hh reader.hh setcmd.hh userinterface.hh utilities.hh vminterp.hh

TARGET = lispel

QT += core

CONFIG += qt debug

INCLUDEPATH += ..

