/*
 * setcmd.hh ---
 *
 *  Set related builtins.
 *
 * (c) 1999-2001 by Michael Saure <MichaelSaure@web.de>
 * RCSID: $Id: setcmd.hh,v 1.1.1.1 2001/10/31 15:44:42 michael Exp $
 */

#ifndef _lispel_SETCMD_HH_
#define _lispel_SETCMD_HH_

#include <lispel/nodes.hh>

extern SetValue *asSet( Handle_ptr handle);

extern Handle_ptr
makesetCommand( CBuiltinAdapter*, Context&, Environment*, std::vector<Handle_ptr>);

extern Handle_ptr
setputCommand( CBuiltinAdapter*, Context&, Environment*, std::vector<Handle_ptr>);

extern Handle_ptr
setexistsCommand( CBuiltinAdapter*, Context&, Environment*, std::vector<Handle_ptr>);

extern Handle_ptr
setremoveCommand( CBuiltinAdapter*, Context&, Environment*, std::vector<Handle_ptr>);

extern Handle_ptr
setclearCommand( CBuiltinAdapter*, Context&, Environment*, std::vector<Handle_ptr>);

extern Handle_ptr
setsizeCommand( CBuiltinAdapter*, Context&, Environment*, std::vector<Handle_ptr>);

extern Handle_ptr //MISSING
setunionCommand( CBuiltinAdapter*, Context&, Environment*, std::vector<Handle_ptr>);

extern Handle_ptr //MISSING
setsymdiffCommand( CBuiltinAdapter*, Context&, Environment*, std::vector<Handle_ptr>);
//MISSING: list->set, set->list

extern void addSetCommands( Interpreter &interp);

#endif /*_lispel_SETCMD_HH_*/
