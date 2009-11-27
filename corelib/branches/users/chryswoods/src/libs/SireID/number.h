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

#ifndef SIREID_NUMBER_H
#define SIREID_NUMBER_H

#include <limits>

#include "id.h"

#include "Siren/stream.h"

SIRE_BEGIN_HEADER

namespace SireID
{

/** This is the base class of all Number ID objects. A Number
    is used to provide an object with an identifying number.
    This could be the number of a residue in a molecule, a 
    user-supplied number of a CutGroup in a molecule, or
    perhaps the automatic unique ID numbers of molecules, 
    forcefields or molecule groups that are assigned by the
    program. The key point of a Number is to provide an ID
    that can be queried and compared rapidly, and that 
    does not change as the object is moved between different
    containers. Generally an object should keep its number
    throughout its lifetime.
    
    Use this class by deriving
    
    class AtomNum : public Implements<AtomNum,Number>,
                    public Interfaces<AtomNum,AtomID>
    
    @author Christopher Woods
*/
class SIREID_EXPORT Number : public Siren::Extends<Number,ID>
{
public:
    Number(qint32 num=0);
    Number(const Number &other);

    ~Number();
    
    uint hashCode() const;
    void stream(Siren::Stream &s);

    bool isNull() const;
    
    operator qint32() const;
    
    static qint32 null();
    
    qint32 value() const;
    
protected:
    Number& operator=(const Number &other);
    
    bool operator==(const Number &other) const;
    bool operator!=(const Number &other) const;

    /** The actual number */
    qint32 _num;
};

}

SIRE_EXPOSE_CLASS( SireID::Number )

SIRE_END_HEADER

#endif
