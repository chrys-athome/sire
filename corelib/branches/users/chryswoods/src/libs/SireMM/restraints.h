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

#ifndef SIREMM_RESTRAINTS_H
#define SIREMM_RESTRAINTS_H

#include "restraint.h"

SIRE_BEGIN_HEADER

/*
namespace SireMM
{
class Restraints;
class Restraints3D;
}

QDataStream& operator<<(QDataStream&, const SireMM::Restraints&);
QDataStream& operator>>(QDataStream&, SireMM::Restraints&);

QDataStream& operator<<(QDataStream&, const SireMM::Restraints3D&);
QDataStream& operator>>(QDataStream&, SireMM::Restraints3D&);

namespace SireMM
{
*/
/** This class holds a collection of restraints 
    
    @author Christopher Woods
*//*
class SIREMM_EXPORT Restraints 
        : public SireBase::ConcreteProperty<Restraints,SireBase::Property>
{

friend QDataStream& ::operator<<(QDataStream&, const Restraints&);
friend QDataStream& ::operator>>(QDataStream&, Restraints&);

public:
    Restraints();
    
    Restraints(const Restraints &other);
    
    ~Restraints();
    
    Restraints& operator=(const Restraints &other);
    
    bool operator==(const Restraints &other) const;
    bool operator!=(const Restraints &other) const;
    
    Restraints& operator+=(const Restraint &other);
    Restraints& operator-=(const Restraint &other);
    
    Restraints& operator+=(const Restraints &other);
    Restraints& operator-=(const Restraints &other);

    const Restraint& operator[](int i) const;
    
    int nRestraints() const;

    virtual void add(const Restraint &other);
    virtual void remove(const Restraint &other);
    
    virtual void add(const Restraints &other);
    virtual void remove(const Restraints &other);
    
    
};

}
*/
//Q_ DECLARE_METATYPE( SireMM::Restraints )
//Q_ DECLARE_METATYPE( SireMM::Restraints3D )

SIRE_END_HEADER

#endif
