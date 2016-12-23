/*
 * environment.hh ---
 *
 *   Declarations of binding environments which are used to establish
 *   name to value mappings.
 *
 * (c) 1999-2014 by Michael Saure <MichaelSaure@web.de>
 * RCSID: $Id: environment.hh,v 1.1.1.1 2001/10/31 15:44:42 michael Exp $
 */

#ifndef lispel_ENVIRONMENT_HH_
#define lispel_ENVIRONMENT_HH_ 1

#include <map>
#include <set>
#include <string>
#include <iostream>
#include <lispel/nodes.hh>

/*!
   Environments are the place where the interpreter stores all named
   values. Environments are ordered in a hierachy to aid the implementation
   of scoping. Environments can be used to build hierachical structures 
   through parent links provided for each Environment instance.
   @version 0.3
*/
class Environment {
public:
  // public type declarations
  typedef std::map<std::string,Handle_ptr> map_type;
  typedef map_type::iterator iterator;
  typedef map_type::const_iterator const_iterator;
  typedef map_type::reference reference;
  typedef map_type::const_reference const_reference;
  typedef Handle_ptr value_type;

protected:
  static Environment *GLOBAL_ENVIRONMENT;
  map_type m_entries;
  Environment *m_parent;

public:
  Environment();
  
  /**
     Create a new Environment instance with a given parent environment.
     @param vptr The parent environment.
  */
  explicit Environment( Environment *vptr);
  ~Environment();
 

  /**
     STL-iterator which refers to the first element.
     @return First element iterator.
  */
  iterator begin() { return m_entries.begin(); }

  /**
     STL-iterator which to the (non-existing) element behind the last
     element.
     @return Past-end iterator.
  */
  iterator end() { return m_entries.end(); }

  const_iterator begin() const { return m_entries.begin(); }
  const_iterator end() const { return m_entries.end(); }

  /**
     Retrieve a value bound to the name given.
     @param name Name to which a value is connected.
     @return A value of end() when name doesn't exist.
  */
  value_type lookup( const std::string &name);

  /**
     Checks if a named value exists.
     @return true if a value is bound to name
  */
  bool exists( const std::string &name);

  /**
     Insert a named value to the environment.
  */
  void put( const std::string name, value_type vptr);

  /**
     Removes all entries from the environment.
  */
  void clear();

  /**
     Get the parent environment.
     @return The parent environment.
  */
  Environment *getParent() { return m_parent; }

  /**
     Create a new Environment instance which has the current Environment
     instance as parent.
  */
  Environment *makeChildEnvironment( );

  /**
     Set the parent environment.
  */
  void setParent( Environment *vptr) { m_parent = vptr; }

  bool isToplevelEnvironment() { return (0 == m_parent); }

  /**
    Delete all Environment instances which aren't referenced from outside
    anymore.
    @param active A set containing all Environment instances currently active
    (i.e. referenced from somewhere outside)
   */
  static void recycleUnusedEnvs( std::set<Environment*> active);

protected:
  /**
    M_env is a set used to keep track of all Environments that have been
    allocated so far.
   */
  static std::set<Environment*> M_envs;

  /**
    Called in each of the Environment constructors.
   */
  static void registerEnv( Environment *env) { M_envs.insert( env); }
};


std::ostream &operator<< ( std::ostream &os, const Environment &env);

#endif /*lispel_ENVIRONMENT_HH_*/
