#include <algorithm>
#include <cassert>
#include <lispel/defs.hh>
#include <lispel/environment.hh>

Environment::Environment()
{
   registerEnv( this);
}

Environment::Environment( Environment* env)
{
  assert( 0);
  registerEnv( this);
}

Environment::~Environment()
{
   MCAssertValidInstance();
}

Environment::value_type 
Environment::lookup( const std::string &name)
{
  MCAssertValidInstance();
  Environment *current = this;
  for (;;) {
    if (0 == current)
      return 0;        // value not defined in any of the environments
    map_type::iterator pos = current->m_entries.find( name);
    if (pos != current->m_entries.end())
      return (*pos).second;              // found a value
    current = current->m_parent;  // search parent environment
  }

  MCAssertNotReached( 0);
}

//FIXME: follow inheritance path
bool 
Environment::exists( const std::string &name)
{
  MCAssertValidInstance();
  Environment *current = this;
  for (;;) {
    if (0 == current)
      return false;
    map_type::iterator pos = current->m_entries.find( name);
    if (pos != current->m_entries.end())
      return true;
    current = current->m_parent;
  }

  MCAssertNotReached( 0);
}

void 
Environment::put( const std::string name, Environment::value_type envptr)
{
  MCAssertValidInstance();
  m_entries[name] = envptr;
}

void 
Environment::clear()
{
  m_entries.clear();
}

Environment *
Environment::makeChildEnvironment( )
{
  MCAssertValidInstance();
  Environment *newEnv = new Environment();
  newEnv->setParent( this);
  return newEnv;
}

void 
Environment::recycleUnusedEnvs( std::set<Environment*> active)
{
  std::set<Environment*> unusedEnvs;
  std::insert_iterator<std::set<Environment*> > iiter( unusedEnvs, unusedEnvs.begin());
  set_difference( M_envs.begin(), M_envs.end(), active.begin(), active.end(),
  	          iiter);
  std::set<Environment*>::iterator pos;
  for (pos=unusedEnvs.begin(); pos!=unusedEnvs.end(); ++pos) {
#if defined( DEBUG) && DEBUG > 4
    std::cerr << "recycleUnusedEnv( " << *pos << ")" << std::endl;
#endif
    delete *pos;
  }
  M_envs = active;
}

std::ostream &
operator<<( std::ostream &os, const Environment &env)
{
  Environment::const_iterator pos;
  for (pos = env.begin(); pos != env.end(); ++pos)
    os << (*pos).first << " = " << (*pos).second << std::endl;

  return os;
}

std::set<Environment*> Environment::M_envs;

