#include <lispel/vmspeccmd.hh>
#include <lispel/lispel.hh>

Handle_ptr
VmspecCommand::execute( Context &ctx, Environment *env, Handle_ptr expr)
{
  assert_valid_instance();
  assert( 0 != env && 0 != ctx.eval && 0 != expr);

  Handle_ptr args = expr->cdr();
  list<Handle_ptr> argumentList;
  if (!eq( args->car(), ctx.NIL)) {
     back_insert_iterator<list<Handle_ptr> > iiter( argumentList);
     copyList( args->car(), iiter);
  }
  if (3 != argumentList.size()) {
    cerr << "vmspec need exactly 3 arguments" << endl;
  }
  list<Handle_ptr>::iterator pos = argumentList.begin();
  if ((*pos)->hasType( Handle::ntSYMBOL))
    cout << (*pos)->stringValue() << endl;
  return ctx.TRUE;
}

