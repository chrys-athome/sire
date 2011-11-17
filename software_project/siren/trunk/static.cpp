/********************************************\
  *
  *  Siren - C++ metaobject library
  *
  *  Copyright (C) 2011  Christopher Woods
  *
  *  This program is free software; you can redistribute it and/or modify
  *  it under the terms of the GNU General Public License as published by
  *  the Free Software Foundation; either version 2 of the License, or
  *  (at your option) any later version.
  *
  *  This program is distributed in the hope that it will be useful,
  *  but WITHOUT ANY WARRANTY; without even the implied warranty of
  *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
  *  GNU General Public License for more details.
  *
  *  You should have received a copy of the GNU General Public License
  *  along with this program; if not, write to the Free Software
  *  Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307  USA
  *
  *  For full details of the license please see the COPYING file
  *  that should have come with this distribution.
  *
  *  You can contact the authors via the developer's mailing list
  *  at http://siremol.org
  *
\*********************************************/

#include "Siren/static.h"
#include "Siren/testreport.h"
#include "Siren/mutex.h"

using namespace Siren;
using namespace Siren::detail;

/** This internal class holds the registry of
    all global static objects that must be created and
    deleted */
class StaticRegistry
{
public:
    StaticRegistry();
    ~StaticRegistry();
    
    typedef void (*CreateFunction)();
    typedef void (*DeleteFunction)();
    
    static void registerObject(CreateFunction create_function,
                               DeleteFunction delete_function);
    
    static void unregisterObject(CreateFunction create_function,
                                 DeleteFunction delete_function);
    
    static void createAll();
    static void deleteAll();
    
private:
    Mutex m;
    List<CreateFunction>::Type create_funcs;
    List<DeleteFunction>::Type delete_funcs;
};

AtomicPointer<StaticRegistry>::Type static_registry;

StaticRegistry::StaticRegistry()
{}

StaticRegistry::~StaticRegistry()
{}

static void createRegistry()
{
    while (static_registry == 0)
    {
        StaticRegistry *newreg = new StaticRegistry();
        
        if (not static_registry.testAndSetAcquire(0,newreg))
            delete newreg;
    }
}

void StaticRegistry::registerObject(CreateFunction cre, DeleteFunction del)
{
    createRegistry();
    MutexLocker lkr( &(static_registry->m) );
    
    static_registry->create_funcs.append(cre);
    static_registry->delete_funcs.append(del);
}

void StaticRegistry::unregisterObject(CreateFunction cre, DeleteFunction del)
{
    createRegistry();
    MutexLocker lkr( &(static_registry->m) );
    
    static_registry->create_funcs.removeAll(cre);
    static_registry->delete_funcs.removeAll(del);
}

void StaticRegistry::createAll()
{
    createRegistry();
    
    MutexLocker lkr( &(static_registry->m) );
    
    for (List<CreateFunction>::const_iterator 
                        it = static_registry->create_funcs.constBegin();
         it != static_registry->create_funcs.constEnd();
         ++it)
    {
        (*it)();
    }
}

void StaticRegistry::deleteAll()
{
    createRegistry();
    
    MutexLocker lkr( &(static_registry->m) );
    
    for (List<CreateFunction>::const_iterator 
                        it = static_registry->delete_funcs.constBegin();
         it != static_registry->delete_funcs.constEnd();
         ++it)
    {
        (*it)();
    }
}

Static::Static( void (*create_function)(), void (*delete_function)() )
{
    sirenDebug() << "REGISTERING FUNCTIONS...";
    StaticRegistry::registerObject(create_function, delete_function);
}

Static::~Static()
{}

void Static::createAll()
{
    StaticRegistry::createAll();
}

void Static::deleteAll()
{
    StaticRegistry::deleteAll();
}
