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
class Editor : public T
{
public:
    Editor();

    Editor(const T &view);
    
    Editor(const T &view, const AtomSelection &selected_atoms);
    
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

}

SIRE_END_HEADER

#endif
