/********************************************\
  *
  *  Sire - Molecular Simulation Framework
  *
  *  Copyright (C) 2007  Christopher Woods
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

#ifndef SIREMOL_EDITOR_HPP
#define SIREMOL_EDITOR_HPP

#include "moleculeview.h"

SIRE_BEGIN_HEADER

namespace SireMol
{

/** This is the class that is used to edit the molecule view
    of type 'T'
    
    @author Christopher Woods
*/
template<class Parent, class T>
class Editor : public T
{
public:
    ~Editor();
    
    Editor<Parent,T>& operator=(const Editor<Parent,T> &other);
    Editor<Parent,T>& operator=(const T &other);

    template<class V>
    Parent& setProperty(const SireBase::PropertyName &key, const V &value);
                           
    template<class V>
    Parent& setMetadata(const SireBase::PropertyName &metakey, const V &value);
    
    template<class V>
    Parent& setMetadata(const SireBase::PropertyName &key, 
                        const SireBase::PropertyName &metakey,
                        const V &value);
                           
    Parent& removeProperty(const SireBase::PropertyName &key);
    Parent& removeMetadata(const SireBase::PropertyName &metakey);
    Parent& removeMetadata(const SireBase::PropertyName &key, 
                           const SireBase::PropertyName &metakey);

protected:
    Editor();
    Editor(const T &view);
    
    Editor(const Editor<Parent,T> &other);
};

#ifndef SIRE_SKIP_INLINE_FUNCTIONS

/** Null constructor */
template<class Parent, class T>
SIRE_OUTOFLINE_TEMPLATE
Editor<Parent, T>::Editor() : T()
{}

/** Construct an editor of the passed view */
template<class Parent, class T>
SIRE_OUTOFLINE_TEMPLATE
Editor<Parent, T>::Editor(const T &view) : T(view)
{}

/** Copy constructor */
template<class Parent, class T>
SIRE_OUTOFLINE_TEMPLATE
Editor<Parent, T>::Editor(const Editor<Parent,T> &other) : T(other)
{}

/** Destructor */
template<class Parent, class T>
SIRE_OUTOFLINE_TEMPLATE
Editor<Parent, T>::~Editor()
{}

/** Copy assignment operator */
template<class Parent, class T>
SIRE_OUTOFLINE_TEMPLATE
Editor<Parent, T>& Editor<Parent, T>::operator=(const Editor<Parent,T> &other)
{
    T::operator=(other);
    
    return *this;
}

/** Copy assignment from an object of type T */
template<class Parent, class T>
SIRE_OUTOFLINE_TEMPLATE
Editor<Parent, T>& Editor<Parent, T>::operator=(const T &other)
{
    T::operator=(other);
    
    return *this;
}

/** Expose the protected 'T::setProperty()' function */
template<class Parent, class T>
template<class V>
SIRE_OUTOFLINE_TEMPLATE
Parent& Editor<Parent, T>::setProperty(const SireBase::PropertyName &key, 
                                       const V &value)
{
    if (key.hasSource())
        T::setProperty(key.source(), value);
        
    return static_cast<Parent&>(*this);
}

/** Expose the protected 'T::setMetadata()' function */
template<class Parent, class T>
template<class V>
SIRE_OUTOFLINE_TEMPLATE
Parent& Editor<Parent, T>::setMetadata(const SireBase::PropertyName &metakey, 
                                       const V &value)
{
    if (metakey.hasSource())
        T::setMetadata(metakey.source(), value);
        
    return static_cast<Parent&>(*this);
}

/** Expose the protected 'T::setMetadata()' function 

    \throw SireBase::missing_property
*/
template<class Parent, class T>
template<class V>
SIRE_OUTOFLINE_TEMPLATE
Parent& Editor<Parent, T>::setMetadata(const SireBase::PropertyName &key, 
                                       const SireBase::PropertyName &metakey,
                                       const V &value)
{
    if (key.hasSource() and metakey.hasSource())
        T::setMetadata(key.source(), metakey.source(), value);
        
    return static_cast<Parent&>(*this);
}

/** Completely remove the property 'key', if this is valid
    property for this view. Note that this will remove this
    property for *all* views, e.g. if this is a Mover<Atom>, 
    then this will remove the property if it is an AtomProp,
    and it will remove the property for *all* atoms. 
    
    \throw SireBase::missing_property
*/
template<class Parent, class T>
SIRE_OUTOFLINE_TEMPLATE
Parent& Editor<Parent, T>::removeProperty(const SireBase::PropertyName &key)
{
    if (key.hasSource())
    {
        T::assertContainsProperty(key.source());
        this->d->removeProperty(key.source());
    }
    
    return static_cast<Parent&>(*this);
}

/** Completely remove the metadata 'metakey', if this is valid
    property for this view. Note that this will remove this
    property for *all* views, e.g. if this is a Mover<Atom>, 
    then this will remove the property if it is an AtomProp,
    and it will remove the property for *all* atoms. 
    
    \throw SireBase::missing_property
*/
template<class Parent, class T>
SIRE_OUTOFLINE_TEMPLATE
Parent& Editor<Parent, T>::removeMetadata(const SireBase::PropertyName &metakey)
{
    if (metakey.hasSource())
    {
        T::assertContainsMetadata(metakey.source());
        this->d->removeMetadata(metakey.source());
    }
    
    return static_cast<Parent&>(*this);
}

/** Completely remove metadata with metakey 'metakey' from
    the property with 'key', if this is valid
    property for this view. Note that this will remove this
    property for *all* views, e.g. if this is a Mover<Atom>, 
    then this will remove the property if it is an AtomProp,
    and it will remove the property for *all* atoms. 
    
    \throw SireBase::missing_property
*/
template<class Parent, class T>
SIRE_OUTOFLINE_TEMPLATE
Parent& Editor<Parent, T>::removeMetadata(const SireBase::PropertyName &key, 
                                          const SireBase::PropertyName &metakey)
{
    if (key.hasSource() and metakey.hasSource())
    {
        T::assertContainsMetadata(key.source(), metakey.source());
        this->d->removeMetadata(key.source(), metakey.source());
    }
    
    return static_cast<Parent&>(*this);
}

#endif //SIRE_SKIP_INLINE_FUNCTIONS

}

SIRE_END_HEADER

#endif
