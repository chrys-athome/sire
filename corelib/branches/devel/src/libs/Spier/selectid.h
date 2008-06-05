/********************************************\
  *
  *  Sire - Molecular Simulation Framework
  *
  *  Copyright (C) 2006  Christopher Woods
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

#ifndef SPIER_SELECTID_H
#define SPIER_SELECTID_H

#include "viewobjptr.h"

#include <QString>
#include <boost/shared_ptr.hpp>

#include "sireglobal.h"

SIRE_BEGIN_HEADER

namespace Spier
{

class Viewer;

class SelectIDBase;
class MolSelectID;

/** SelectID is an implicitly shared class that represents a selection. 
    The selection is held in the virtual SelectIDBase class. 
    
    @author Christopher Woods
*/
class SPIER_EXPORT SelectID : private boost::shared_ptr<SelectIDBase>
{
public:
    SelectID();
    SelectID( SelectIDBase *base );
    SelectID(const SelectID &other);
    
    ~SelectID();
    
    SelectID& operator=(const SelectID &other);
    
    bool operator==(const SelectID &other) const;
    bool operator!=(const SelectID &other) const;
    
    bool isNull() const;
    QString toString() const;
    
    ViewObjPtr object() const;
    
    void highlight();
    
    void select();
    void deselect();
    void toggle();
    
    const MolSelectID* atomSelection() const;
    
    bool selected() const;
};

/**
This class represents a selection. It contains all of the information necessary to represent a single selected item. This class is overloaded to provide more specific hierarchical information (e.g. atom 3 in molecule 4).
 
@author Christopher Woods
*/
class SelectIDBase
{
public:
    SelectIDBase();
    SelectIDBase(const ViewObjPtr &obj);
    virtual ~SelectIDBase();

    const ViewObjPtr& object() const;

    bool isNull() const;

    const MolSelectID* atomSelection() const;

    virtual QString toString() const=0;

    virtual void highlight()=0;
    
    virtual void select()=0;
    virtual void deselect()=0;
    virtual void toggle()=0;

    virtual bool selected() const=0;

    virtual bool operator==(const SelectIDBase &other) const=0;
    virtual bool operator!=(const SelectIDBase &other) const=0;

private:

    /** The viewable object that has been selected */
    ViewObjPtr selectobj;

};

/** Return the object that has been selected */
inline const ViewObjPtr& SelectIDBase::object() const
{
    return selectobj;
}

/** Return whether or not this is a null selection */
inline bool SelectIDBase::isNull() const
{
    return selectobj.get() == 0;
}

}

SIRE_END_HEADER

#endif
