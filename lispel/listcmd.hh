/*
 * listcmd.hh ---
 *
 *   Declarations for builtins related to handling lists.
 *
 * (c) 1999-2001 by Michael Saure <MichaelSaure@web.de>
 * RCSID: $Id: listcmd.hh,v 1.1.1.1 2001/10/31 15:44:42 michael Exp $
 */

#ifndef _lispel_LISTCMD_HH_
#define _lispel_LISTCMD_HH_

#include <lispel/nodes.hh>
#include <lispel/interpreter.hh>
#include <lispel/context.hh>

extern Handle_ptr
lengthCommand( CBuiltinAdapter*, Context&, Environment*, std::vector<Handle_ptr>);

extern Handle_ptr
listCommand( CBuiltinAdapter*, Context&, Environment*, std::vector<Handle_ptr>);

extern Handle_ptr
nullPredicate( CBuiltinAdapter*, Context&, Environment*, std::vector<Handle_ptr>);

extern Handle_ptr
setCarFCommand( CBuiltinAdapter*, Context&, Environment*, std::vector<Handle_ptr>);

extern Handle_ptr
setCdrFCommand( CBuiltinAdapter*, Context&, Environment*, std::vector<Handle_ptr>);

extern void addListCommands( Interpreter &interp);

#endif /*_lispel_LISTCMD_HH_*/
