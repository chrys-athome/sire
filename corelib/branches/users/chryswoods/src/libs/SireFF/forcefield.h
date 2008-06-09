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

#ifndef SIREFF_FORCEFIELD_H
#define SIREFF_FORCEFIELD_H

#include "ff.h"

#include "SireBase/property.h"

SIRE_BEGIN_HEADER

namespace SireFF
{
class ForceField;
}

QDataStream& operator<<(QDataStream&, const SireFF::ForceField&);
QDataStream& operator>>(QDataStream&, SireFF::ForceField&);

namespace SireFF
{

using SireBase::PropertyBase;

/** This is the polymorphic pointer holder for the 
    ForceField (FF derived) classes.
    
    ForceField are ForceField that contain functions
    that allow the intermolecular potential energy of the
    group(s) of molecules to be evaluated. As such, they 
    are central to the concept of a molecular simulation
    program
    
    @author Christopher Woods
*/
class SIREFF_EXPORT ForceField : public SireBase::Property
{

friend QDataStream& ::operator<<(QDataStream&, const ForceField&);
friend QDataStream& ::operator>>(QDataStream&, ForceField&);

public:
    ForceField();
    ForceField(const PropertyBase &property);
    ForceField(const FF &molgroup);

    ForceField(const ForceField &other);
    
    ~ForceField();
    
    virtual ForceField& operator=(const PropertyBase &property);
    virtual ForceField& operator=(const FF &other);

    const FF* operator->() const;
    const FF& operator*() const;
    
    const FF& read() const;
    FF& edit();
    
    const FF* data() const;
    const FF* constData() const;
    
    FF* data();
    
    operator const FF&() const;

    static const ForceField& shared_null();
};

}

Q_DECLARE_METATYPE(SireFF::ForceField);

SIRE_EXPOSE_PROPERTY( SireFF::ForceField, SireFF::FF )

SIRE_END_HEADER

#endif
