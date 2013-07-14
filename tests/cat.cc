#include <iostream>
#include <fstream>

void cat( const char *filename)
{
   ifstream infile( filename);
   char c;
   while (!infile.eof()) {
      infile >> c;
      cout << c;
   } 
}

int main( int argc, char *argv[])
{
   if (1 == argc) {
      cat( "-");
      return 0;
   }

   int i;
   for (i=1; i<argc; ++i) {
      cout << "::::::::::::" << endl;
      cout << argv[i] << endl;
      cout << "::::::::::::" << endl;
      cat( argv[i]);
   }

   return 0;
}

