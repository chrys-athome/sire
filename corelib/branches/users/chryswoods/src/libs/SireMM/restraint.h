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

#ifndef SIREMM_RESTRAINT_H
#define SIREMM_RESTRAINT_H

#include "SireBase/property.h"
#include "SireBase/propertymap.h"

#include "SireUnits/dimensions.h"

SIRE_BEGIN_HEADER

namespace SireMM
{
class Restraint;
class Restraint3D;

class NullRestraint;

class Restraints;
class Restraints3D;
}

QDataStream& operator<<(QDataStream&, const SireMM::Restraint&);
QDataStream& operator>>(QDataStream&, SireMM::Restraint&);

QDataStream& operator<<(QDataStream&, const SireMM::Restraint3D&);
QDataStream& operator>>(QDataStream&, SireMM::Restraint3D&);

QDataStream& operator<<(QDataStream&, const SireMM::NullRestraint&);
QDataStream& operator>>(QDataStream&, SireMM::NullRestraint&);

QDataStream& operator<<(QDataStream&, const SireMM::Restraints&);
QDataStream& operator>>(QDataStream&, SireMM::Restraints&);

QDataStream& operator<<(QDataStream&, const SireMM::Restraints3D&);
QDataStream& operator>>(QDataStream&, SireMM::Restraints3D&);

namespace SireMol
{
class MoleculeData;
}

namespace SireFF
{
class MolForceTable;
}

namespace SireMM
{

using SireBase::PropertyMap;

using SireMol::MoleculeData;

using SireFF::MolForceTable;

/** This is the base class of all restraints. A restraint is a
    function that calculates the energy or force acting on
    a molecule caused by external potential, e.g. a harmonic
    restraining potential, or a solvent cap potential, or
    a dihedral restraint potential
    
    @author Christopher Woods
*/
class SIREMM_EXPORT Restraint : public SireBase::Property
{

friend QDataStream& ::operator<<(QDataStream&, const Restraint&);
friend QDataStream& ::operator>>(QDataStream&, Restraint&);

public:
    Restraint();
    
    Restraint(const Restraint &other);
    
    virtual ~Restraint();
    
    static const char* typeName()
    {
        return "SireMM::Restraint";
    }
    
    virtual Restraint* clone() const=0;
    
    virtual SireUnits::Dimension::MolarEnergy energy() const=0;

    virtual void update(const MoleculeData &moldata,
                        const PropertyMap &map)=0;

    static const NullRestraint& null();

protected:
    Restraint& operator=(const Restraint &other);
    
    bool operator==(const Restraint &other) const;
    bool operator!=(const Restraint &other) const;
};

/** This is the base class of all restraints that operate in 3 dimensions,
    and so can thus return the force on the molecule caused by the restraint
    
    @author Christopher Woods
*/
class SIREMM_EXPORT Restraint3D : public Restraint
{

friend QDataStream& ::operator<<(QDataStream&, const Restraint3D&);
friend QDataStream& ::operator>>(QDataStream&, Restraint3D&);

public:
    Restraint3D();
    
    Restraint3D(const Restraint3D &other);
    
    virtual ~Restraint3D();

    static const char* typeName()
    {
        return "SireMM::Restraint3D";
    }

    virtual Restraint3D* clone() const=0;

    virtual void force(MolForceTable &forcetable, double scale_force=1) const=0;
    
protected:
    Restraint3D& operator=(const Restraint3D &other);
    
    bool operator==(const Restraint3D &other) const;
    bool operator!=(const Restraint3D &other) const;
};

/** This is a null restraint, that does not affect the energy
    or force on any molecule
    
    @author Christopher Woods
*/
class SIREMM_EXPORT NullRestraint 
            : public SireBase::ConcreteProperty<NullRestraint,Restraint3D>
{

friend QDataStream& ::operator<<(QDataStream&, const NullRestraint &other);
friend QDataStream& ::operator>>(QDataStream&, NullRestraint &other);

public:
    NullRestraint();
    
    NullRestraint(const NullRestraint &other);
    
    ~NullRestraint();
    
    NullRestraint& operator=(const NullRestraint &other);
    
    bool operator==(const NullRestraint &other) const;
    bool operator!=(const NullRestraint &other) const;
    
    static const char* typeName();
    
    SireUnits::Dimension::MolarEnergy energy() const;

    void force(MolForceTable &forcetable, double scale_force=1) const;
    
    void update(const MoleculeData &moldata, const PropertyMap &map);
};

typedef SireBase::PropPtr<Restraint> RestraintPtr;
typedef SireBase::PropPtr<Restraint3D> Restraint3DPtr;

}

Q_DECLARE_METATYPE( SireMM::NullRestraint )
//Q_DECLARE_METATYPE( SireMM::Restraints )
//Q_DECLARE_METATYPE( SireMM::Restraints3D )

SIRE_EXPOSE_CLASS( SireMM::Restraint )
SIRE_EXPOSE_CLASS( SireMM::Restraint3D )
SIRE_EXPOSE_CLASS( SireMM::NullRestraint)

//S I RE_EXPOSE_CLASS( SireMM::Restraints )
//S I RE_EXPOSE_CLASS( SireMM::Restraints3D )

SIRE_EXPOSE_PROPERTY( SireMM::RestraintPtr, Restraint )
SIRE_EXPOSE_PROPERTY( SireMM::Restraint3DPtr, Restraint3D )

SIRE_END_HEADER

#endif
