#ifndef _lispel_USERINTERFACE_HH_
#define _lispel_USERINTERFACE_HH_

#include <string>
#include <iostream>
#include <lispel/nodes.hh>

/**
   Instances of this class determine how the interpreter interacts with a user
   when an interactive interpreter is used.
   @version 0.1
*/
class UserInterface {
public:
  UserInterface();
  virtual ~UserInterface();

  /// Show the interpreter's prompt.
  virtual void displayPrompt() =0;
  /// Print an expression.
  virtual void displayExpression( Handle_ptr expr) =0;
  /// Display an error message.
  virtual void error( const std::string &msg) =0;
  /// Display a warning.
  virtual void warning( const std::string &msg) =0;
};
 
/**
   A minimal terminal interface for an interactive interpreter.
*/
class SimpleShellInterface : public UserInterface {
public:
  SimpleShellInterface();
  virtual ~SimpleShellInterface();

  virtual void displayPrompt();
  virtual void displayExpression( Handle_ptr expr);
  virtual void error( const std::string &msg);
  virtual void warning( const std::string &msg);
};

class BatchExecInterface : public UserInterface {
public:
   BatchExecInterface();
   virtual ~BatchExecInterface();

   virtual void displayPrompt();
   virtual void displayExpression( Handle_ptr expr);
   virtual void error( const std::string &msg);
   virtual void warning( const std::string &msg);
};

#endif /*_lispel_USERINTERFACE_*/
