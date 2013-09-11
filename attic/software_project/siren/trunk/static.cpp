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
    
    static bool beingCreated();
    static bool beingDeleted();
    
private:
    Mutex m;
    List<CreateFunction>::Type create_funcs;
    List<DeleteFunction>::Type delete_funcs;
    
    bool being_created;
    bool being_deleted;
};

StaticRegistry* static_registry = 0;

StaticRegistry::StaticRegistry() : being_created(true), being_deleted(false)
{}

StaticRegistry::~StaticRegistry()
{}

void createRegistry()
{
    if (static_registry == 0)
    {
        static_registry = new StaticRegistry();
    }
}

void StaticRegistry::registerObject(CreateFunction cre, DeleteFunction del)
{
    createRegistry();

    MutexLocker lkr( &(static_registry->m) );
    
    static_registry->create_funcs.append(cre);
    static_registry->delete_funcs.append(del);
}

bool StaticRegistry::beingCreated()
{
    if (static_registry == 0)
        return false;

    return static_registry->being_created;
}

bool StaticRegistry::beingDeleted()
{
    if (static_registry == 0)
        return false;

    return static_registry->being_deleted;
}

void StaticRegistry::unregisterObject(CreateFunction cre, DeleteFunction del)
{
    if (static_registry == 0)
        return;

    MutexLocker lkr( &(static_registry->m) );
    
    static_registry->create_funcs.removeAll(cre);
    static_registry->delete_funcs.removeAll(del);
}

void StaticRegistry::createAll()
{
    if (static_registry == 0)
        return;
    
    MutexLocker lkr( &(static_registry->m) );
    
    static_registry->being_created = true;
    static_registry->being_deleted = false;
    
    try
    {
        for (List<CreateFunction>::const_iterator 
                            it = static_registry->create_funcs.constBegin();
             it != static_registry->create_funcs.constEnd();
             ++it)
        {
            (*it)();
        }
    
        static_registry->being_created = false;
    }
    catch(...)
    {
        static_registry->being_created = false;
        throw;
    }
}

void StaticRegistry::deleteAll()
{
    if (static_registry == 0)
        return;
    
    MutexLocker lkr( &(static_registry->m) );
    
    static_registry->being_created = false;
    static_registry->being_deleted = true;
    
    try
    {
        for (List<CreateFunction>::const_iterator 
                            it = static_registry->delete_funcs.constBegin();
             it != static_registry->delete_funcs.constEnd();
             ++it)
        {
            (*it)();
        }
        
        static_registry->being_deleted = false;
    }
    catch(...)
    {
        static_registry->being_deleted = false;
        throw;
    }
}

Static::Static( void (*create_function)(), void (*delete_function)(),
                const char*, const char* )
{
    StaticRegistry::registerObject(create_function, delete_function);
}

Static::~Static()
{}

void Static::createAll()
{
    createRegistry();
    StaticRegistry::createAll();
}

void Static::deleteAll()
{
    StaticRegistry::deleteAll();
}

bool Static::beingCreated()
{
    return StaticRegistry::beingCreated();
}

bool Static::beingDeleted()
{
    return StaticRegistry::beingDeleted();
}
