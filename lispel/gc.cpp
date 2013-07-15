#include <set>
#include <lispel/defs.hh>
#include <lispel/gc.hh>
#include <lispel/nodefactory.hh>
#include <lispel/environment.hh>

GarbageCollector::GarbageCollector( Context &ctx)
   : m_ctx( ctx)
{
  m_factory = ctx.factory;
}

GarbageCollector::~GarbageCollector()
{
  MCAssertValidInstance();
}


SimpleGC::SimpleGC( Context &ctx)
   : GarbageCollector( ctx)
{
  m_incrementalMode = false;
}

SimpleGC::~SimpleGC()
{
  MCAssertValidInstance();
}

int SimpleGC::fullGC()
{
  MCAssertValidInstance();
  m_factory->untagNonEmpty(); // reset reference counts

  // first collect all environments still in use
  std::set<Environment*> activeEnvs = m_factory->activeEnvironments();
  activeEnvs.insert( m_ctx.toplevel);
  Environment::recycleUnusedEnvs( activeEnvs);
  assert( activeEnvs.size() > 0);

  // tag all nodes referenced from environments
  std::set<Environment*>::iterator envIter;
  for (envIter=activeEnvs.begin(); envIter!=activeEnvs.end(); ++envIter) {
    Environment::iterator bindingIter;
    for (bindingIter=(*envIter)->begin(); bindingIter!=(*envIter)->end();
	 ++bindingIter)
      (*bindingIter).second->incRef();
  }

  // tag all nodes referenced by nodes tagged in the previous step
  //NodeFactory::iterator pos;
  //for (pos=m_factory->begin(); pos!=m_factory->end(); ++pos) {
  //  (*pos).incRef();
  //}

  return m_factory->recycleUntagged();
}

int SimpleGC::incrementalGC( int amount)
{
  MCAssertValidInstance();
  return 0;
}

void SimpleGC::finishCycle()
{
}

