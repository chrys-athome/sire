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
#include "editor.h"

SIRE_BEGIN_HEADER

namespace SireMol
{

/** This is the class that is used to edit the molecule view
    of type 'T'
    
    @author Christopher Woods
*/
template<class T>
class Editor : public T, public EditorBase
{
public:
    Editor();

    Editor(const T &view);
    
    Editor(const Editor<T> &other);
    
    ~Editor();
    
    Editor<T>& operator=(const Editor<T> &other);
    
    static const char* typeName()
    {
        return QMetaType::typeName( qMetaTypeId< Editor<T> >() );
    }
    
    const char* what() const
    {
        return Editor<T>::typeName();
    }
    
    Editor<T>* clone() const
    {
        return new Editor<T>(*this);
    }

    template<class V>
    Editor<T>& setProperty(const QString &key, const V &value,
                           bool clear_metadata = true);
                           
    template<class V>
    Editor<T>& setMetadata(const QString &metakey, const V &value);
    
    template<class V>
    Editor<T>& setMetadata(const QString &key, const QString &metakey,
                           const V &value);
                           
    Editor<T>& removeProperty(const QString &key);
    Editor<T>& removeMetadata(const QString &metakey);
    Editor<T>& removeMetadata(const QString &key, const QString &metakey);
};

/** Null constructor */
template<class T>
SIRE_OUTOFLINE_TEMPLATE
Editor<T>::Editor() : T(), EditorBase()
{}

/** Construct an editor of the passed view */
template<class T>
SIRE_OUTOFLINE_TEMPLATE
Editor<T>::Editor(const T &view) : T(view), EditorBase()
{}

/** Copy constructor */
template<class T>
SIRE_OUTOFLINE_TEMPLATE
Editor<T>::Editor(const Editor<T> &other) : T(other), EditorBase(other)
{}

/** Destructor */
template<class T>
SIRE_OUTOFLINE_TEMPLATE
Editor<T>::~Editor()
{}

/** Copy assignment operator */
template<class T>
SIRE_OUTOFLINE_TEMPLATE
Editor<T>& Editor<T>::operator=(const Editor<T> &other)
{
    T::operator=(other);
    EditorBase::operator=(other);
    
    return *this;
}

/** Expose the protected 'T::setProperty()' function */
template<class T>
template<class V>
SIRE_OUTOFLINE_TEMPLATE
Editor<T>& Editor<T>::setProperty(const QString &key, const V &value)
{
    T::setProperty(key, value);
    return *this;
}

/** Expose the protected 'T::setMetadata()' function */
template<class T>
template<class V>
SIRE_OUTOFLINE_TEMPLATE
Editor<T>& Editor<T>::setMetadata(const QString &metakey, const V &value)
{
    T::setMetadata(metakey, value);
    return *this;
}

/** Expose the protected 'T::setMetadata()' function 

    \throw SireBase::missing_property
*/
template<class T>
template<class V>
SIRE_OUTOFLINE_TEMPLATE
Editor<T>& Editor<T>::setMetadata(const QString &key, const QString &metakey,
                                  const V &value)
{
    T::setMetadata(key, metakey, value);
    return *this;
}

/** Completely remove the property 'key', if this is valid
    property for this view. Note that this will remove this
    property for *all* views, e.g. if this is a Mover<Atom>, 
    then this will remove the property if it is an AtomProp,
    and it will remove the property for *all* atoms. 
    
    \throw SireBase::missing_property
*/
template<class T>
SIRE_OUTOFLINE_TEMPLATE
Editor<T>& Editor<T>::removeProperty(const QString &key)
{
    T::assertContainsProperty(key);
    EditorBase::removeProperty(*d, key);
    return *this;
}

/** Completely remove the metadata 'metakey', if this is valid
    property for this view. Note that this will remove this
    property for *all* views, e.g. if this is a Mover<Atom>, 
    then this will remove the property if it is an AtomProp,
    and it will remove the property for *all* atoms. 
    
    \throw SireBase::missing_property
*/
template<class T>
SIRE_OUTOFLINE_TEMPLATE
Editor<T>& Editor<T>::removeMetadata(const QString &metakey)
{
    T::assertContainsMetadata(metakey);
    EditorBase::removeMetadata(*d, metakey);
    return *this;
}

/** Completely remove metadata with metakey 'metakey' from
    the property with 'key', if this is valid
    property for this view. Note that this will remove this
    property for *all* views, e.g. if this is a Mover<Atom>, 
    then this will remove the property if it is an AtomProp,
    and it will remove the property for *all* atoms. 
    
    \throw SireBase::missing_property
*/
template<class T>
SIRE_OUTOFLINE_TEMPLATE
Editor<T>& Editor<T>::removeMetadata(const QString &key, const QString &metakey,
                                     const QString &metakey)
{
    T::assertContainsMetadata(key, metakey);
    EditorBase::removeMetadata(*d, key, metakey);
    return *this;
}

}

SIRE_END_HEADER

#endif
