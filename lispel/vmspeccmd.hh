#ifndef _lispel_VMSPECCMD_HH_
#define _lispel_VMSPECCMD_HH_

#include <lispel/nodes.hh>
#include <lispel/interpreter.hh>
#include <lispel/context.hh>

class Environment;
class Evaluator;

class VmspecCommand : public SpecialValue {
public:
  VmspecCommand() {}
  virtual ~VmspecCommand() {}
  virtual Handle_ptr execute( Context &ctx, Environment *env, Handle_ptr *exp);
};

#endif /*_lispel_VMSPECCMD_HH_*/
