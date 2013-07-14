#ifndef _lispel_CONTEXT_HH_
#define _lispel_CONTEXT_HH_

class NodeFactory;
class Environment;
class Evaluator;
class Interpreter;
class Handle;

struct Context {
  //Heap *heap; //FIXME: heap really should be implemented separately!
  NodeFactory *factory;
  Environment *toplevel;
  Environment *currentEnv;
  Evaluator *eval;
  Interpreter *interp;
  Handle *NIL;
  Handle *TRUE;
  Handle *FALSE;
};

#endif /*lispel_CONTEXT_HH_*/

