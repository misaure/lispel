/*
 * driver.cpp ---
 *
 *   A small main program for a stand-alone version of the interpreter.
 *
 * (c) 1999-2001 by Michael Saure
 * RCSID: $Id: driver.cpp,v 1.2 2001/12/16 21:10:09 michael Exp $
 */

#include <list>
#include <fstream>
#include <getopt.h>
#include <lispel/lispel.hh>
#include "version.h"

static const char *STATUS = "(prerelease)";

static const char *shortOpts = "hv";

static struct option longOpts[] = {
  { "help", 	   no_argument,	      0, 'h' },
  { "version",	   no_argument,	      0, 'v' },
  { "heap-size",   required_argument, 0, 'm' },
  { "stack-limit", required_argument, 0, 'l' },
  { "file",        required_argument, 0, 'f' },
  { 0, 		   0, 		      0,  0  }
};

static void usage( const char *exename)
{
  std::cout << "usage: " << exename << " -[hvf] [sourcefile]" << std::endl;
  std::cout << "Use option --help (-h) to display a short help on the options available." << std::endl;
}

static void help()
{
  std::cout << "--help\t\t[-h]  show this help" << std::endl
       << "--version\t[-v]  show version information" << std::endl
       << "--heap-size n\t[-m]  sets initial heap size" << std::endl
       << "--stack-limit n\t[-l]  set maximum stack depth (0 = no limit)"
       << std::endl
       << "--file f\t[-f]  source file and exit" << std::endl;
}

static void version()
{
  std::cout << "Lispel " << LISPEL_VERSION << " " << STATUS
       << ", (c) 1999 by Michael Saure" << std::endl;
}

static void banner()
{
  std::cout << "\n\n Welcome to Lispel, version "
       << LISPEL_VERSION << " " << STATUS << "," << std::endl
       << " the lisp-like extension language, "
       << " Copyright 1999-2001 by Michael Saure\n" << std::endl
       << " Lispel comes with ABSOLUTELY NO WARRANTY. This is free software"
       << " and can be " << std::endl
       << " redistributed under the terms described in the Lispel release"
       << " notes.\n" << std::endl
       << "type ctrl-d or '(exit)' to quit" << std::endl;
}

int main( int argc, char *argv[])
{
  // process command line arguments
  int optionIndex;
  std::string filename = "-";
  for (;;) {
    int c = getopt_long( argc, argv, shortOpts, longOpts, &optionIndex);
    if (-1 == c)
      break;
    switch( c) {
    case 'v': // print version info and exit
      version();
      return 0;

    case 'h': // print help on command line arguments and exit
      version();
      help();
      return 0;

    case 'm': // adjust heap size
      break;

    case 'l': // set stack limit
      break;

    case 'f': // source a file
      filename = optarg;
      break;

    default:
      usage( argv[0]);
      return 1;
    }
  }

  Interpreter interp;
  try {
    Lexer::initLexerModule();

    addCoreCommands( interp);
    addListCommands( interp);
    addMathCommands( interp);
    addHashCommands( interp);
    addSetCommands( interp);

    // gather unprocessed command line arguments
    std::list<Handle_ptr> args;
    int i;
    for (i=optind; i<argc; ++i) {
        args.push_back( interp.nodeFactory()->makeString( argv[i]));
    }
    interp.toplevelDefine( "commandline-arguments",
                        makeList( args.begin(), args.end(), interp.context()));
    if (std::string("-") == filename) {
      banner();
      SimpleShellInterface ui;
      interp.repl( &std::cin, &ui);
    } else {
      BatchExecInterface ui;
      std::ifstream infile( filename.c_str());
      interp.repl( &infile, &ui);
    }
  }
  catch( Exception &e) {
    std::cerr << "\nexception caught:" << std::endl;
    std::cerr << e.what() << std::endl;
  }
  catch( ...) {
    // exceptions thrown by the C++ library
    std::cerr << "\nunknown exception!" << std::endl;
  }

  return 0;
}


