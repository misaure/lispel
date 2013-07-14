#include <iostream>
#include <fstream>
#include <lispel/vminterp.hh>

int main( int argc, char *argv[])
{
  std::istream *infile = 0;
  if (1 == argc) {
    infile = &std::cin;
  } else if (2 == argc) {
    std::ifstream filestream( argv[1]);
    if (!filestream) {
      std::cerr << "error opening input file '" << argv[1] << "'." << std::endl;
      exit( EXIT_FAILURE);
    }
  } else {
    std::cout << "usage: " << argv[0] << "[<infile>]" << std::endl;
    exit( EXIT_FAILURE);
  }

  VmCommandReader cmdReader( infile);
  while (!cmdReader.done()) {
    VmCommand cmd = cmdReader.readCommand();
    std::cout << cmd << std::endl;
  }

  exit( EXIT_SUCCESS);
}
