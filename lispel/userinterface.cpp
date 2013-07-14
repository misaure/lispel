#include <iostream>
#include <lispel/userinterface.hh>
#include <lispel/defs.hh>

UserInterface::UserInterface()
{
}

UserInterface::~UserInterface()
{
}


SimpleShellInterface::SimpleShellInterface()
{
}

SimpleShellInterface::~SimpleShellInterface()
{
}

void SimpleShellInterface::displayPrompt()
{
  std::cout << "> "; std::cout.flush();
}

void SimpleShellInterface::displayExpression( Handle_ptr expr)
{
  std::cout << (*expr) << std::endl;
}

void SimpleShellInterface::error( const std::string &msg)
{
  std::cout << "ERROR: " << msg << std::endl;
}

void SimpleShellInterface::warning( const std::string &msg)
{
  std::cout << "WARNING: " << msg << std::endl;
}


BatchExecInterface::BatchExecInterface()
{
}

BatchExecInterface::~BatchExecInterface()
{
}

void BatchExecInterface::displayPrompt()
{
}

void BatchExecInterface::displayExpression( Handle_ptr expr)
{
}

void BatchExecInterface::error( const std::string &msg)
{
   std::cerr << "error: " << msg << std::endl;
}

void BatchExecInterface::warning( const std::string &msg)
{
   std::cerr << "warning: " << msg << std::endl;
}

