/*
 * hashcmd.hh ---
 *
 *   Builtins related to hash tables.
 *
 * (c) 1999 by Michael Saure
 * RCSID: $Id: hashcmd.hh,v 1.1.1.1 2001/10/31 15:44:42 michael Exp $
 */

#ifndef _lispel_HASHCMD_HH_
#define _lispel_HASHCMD_HH_

#include <lispel/nodes.hh>

/// Retrieve hashtable member from memory cell
extern Hashtable *asHashtable( Handle_ptr handle);

extern Handle_ptr
makehashCommand( CBuiltinAdapter*, Context&, Environment*, std::vector<Handle_ptr>);

extern Handle_ptr
hashsetCommand( CBuiltinAdapter*, Context&, Environment*, std::vector<Handle_ptr>);

extern Handle_ptr
hashgetCommand( CBuiltinAdapter*, Context&, Environment*, std::vector<Handle_ptr>);

extern Handle_ptr
hashremoveCommand( CBuiltinAdapter*, Context&, Environment*, std::vector<Handle_ptr>);

extern Handle_ptr 
hashclearCommand( CBuiltinAdapter*, Context&, Environment*, std::vector<Handle_ptr>);

extern Handle_ptr
hashsizeCommand( CBuiltinAdapter*, Context&, Environment*, std::vector<Handle_ptr>);

extern Handle_ptr
hashexistsCommand( CBuiltinAdapter*, Context&, Environment*, std::vector<Handle_ptr>);

extern Handle_ptr
hashkeysCommand( CBuiltinAdapter*, Context&, Environment*, std::vector<Handle_ptr>);

extern Handle_ptr
hashvalsCommand( CBuiltinAdapter*, Context&, Environment*, std::vector<Handle_ptr>);


extern void
addHashCommands( Interpreter &interp);

#endif /*_lispel_HASHCMD_HH_*/
