/********************************************\
  *
  *  Sire - Molecular Simulation Framework
  *
  *  Copyright (C) 2009  Christopher Woods
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

#ifndef SIREBASE_VERSIONREGISTRY_H
#define SIREBASE_VERSIONREGISTRY_H

#include "Siren/handle.h"
#include "Siren/hanref.h"

#include "sharedversion.h"

SIRE_BEGIN_HEADER

namespace SireBase
{

/** This class provides a registry of that last version number
    assigned to the objects identified by keys of type 'T' 
    
    @author Christopher Woods
*/
template<class T>
class SIREBASE_EXPORT VersionRegistry : 
        public Siren::ImplementsHandle< VersionRegistry<T>,
                         Siren::Handles< QHash<T,Siren::WeakHandle> > >
{
public:
    VersionRegistry();
    ~VersionRegistry();

    VersionRegistry& operator=(const VersionRegistry &other);
    
    bool operator==(const VersionRegistry &other) const;
    bool operator!=(const VersionRegistry &other) const;

    SharedVersion registerObject(const T &key);

    bool registered(const T &key);

    QString toString() const;
    uint hashCode() const;
};

#ifndef SIRE_SKIP_INLINE_FUNCTIONS

/** Constructor */
template<class T>
SIRE_OUTOFLINE_TEMPLATE
VersionRegistry<T>::VersionRegistry()
                   : Siren::ImplementsHandle< VersionRegistry<T>,
                        Siren::Handles< QHash<T,Siren::WeakHandle> > >(
                            new QHash<T,Siren::WeakHandle>() )
{}

/** Destructor */
template<class T>
SIRE_OUTOFLINE_TEMPLATE
VersionRegistry<T>::~VersionRegistry()
{}

/** Return whether or not an object with key 'key' has been  
    registered */
template<class T>
SIRE_OUTOFLINE_TEMPLATE
bool VersionRegistry<T>::registered(const T &key)
{
    Siren::HandleLocker lkr(*this);
    return not this->resource().value(key).expired();
}

/** Register the object identified by the key 'key' and return
    the version number of this object. This will be 1:0 if this
    is a new object, or one major version higher than the last
    assigned number of any other object identified by this key */
template<class T>
SIRE_OUTOFLINE_TEMPLATE
SharedVersion VersionRegistry<T>::registerObject(const T &key)
{
    Siren::HandleLocker lkr(*this);

    if (this->resource().contains(key))
    {
        Siren::HanRef obj = this->resource().value(key).lock();
                                        
        if (not obj.isNull())
        {
            SharedVersion v = obj.asA<SharedVersion>();
            v.incrementMajor();
            return v;
        }
    }
    
    //either we don't know about this object, or we used to, but
    //not any more!
    SharedVersion v(1,0);
    
    this->resource().insert( key, Siren::WeakHandle(v) );

    if (this->resource().count() % 100 == 0)
    {
        //there are exactly a multiple of 100 objects in the registry
        // - so lets try and clean it up a little
        QMutableHashIterator<T,Siren::WeakHandle> it(this->resource());
        
        while (it.hasNext())
        {
            it.next();
            
            if (it.value().expired())
                //this object has since been removed
                it.remove();
        }

    }
    
    return v;
}

#endif // SIRE_SKIP_INLINE_FUNCTIONS

}

SIRE_END_HEADER

#endif
