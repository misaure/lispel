/*
 * mathcmd ---
 *
 *   Implementation of math functions.
 *
 * (c) 1999 by Michael Saure
 * RCSID: $Id: mathcmd.hh,v 1.1.1.1 2001/10/31 15:44:42 michael Exp $
 */

#ifndef _lispel_MATHCMD_HH_
#define _lispel_MATHCMD_HH_

#include <lispel/nodes.hh>
#include <lispel/interpreter.hh>
#include <lispel/context.hh>

extern Handle_ptr 
eqCommand( CBuiltinAdapter*, Context&, Environment*, std::vector<Handle_ptr>);

extern Handle_ptr
ltCommand( CBuiltinAdapter*, Context&, Environment*, std::vector<Handle_ptr>);

extern Handle_ptr
gtCommand( CBuiltinAdapter*, Context&, Environment*, std::vector<Handle_ptr>);

extern Handle_ptr
plusCommand( CBuiltinAdapter*, Context&, Environment*, std::vector<Handle_ptr>);

extern Handle_ptr
subtrCommand( CBuiltinAdapter*, Context&, Environment*, std::vector<Handle_ptr>);

extern Handle_ptr
mulCommand( CBuiltinAdapter*, Context&, Environment*, std::vector<Handle_ptr>);

extern Handle_ptr
divCommand( CBuiltinAdapter*, Context&, Environment*, std::vector<Handle_ptr>);

void addMathCommands( Interpreter &interp);

#endif /*_lispel_MATHCMD_HH_*/
