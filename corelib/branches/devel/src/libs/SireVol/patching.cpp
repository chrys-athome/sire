/********************************************\
  *
  *  Sire - Molecular Simulation Framework
  *
  *  Copyright (C) 2010  Christopher Woods
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

#include "patching.h"

#include "SireStream/datastream.h"
#include "SireStream/shareddatastream.h"

#include "SireUnits/units.h"

#include "SireError/errors.h"

using namespace SireVol;
using namespace SireBase;
using namespace SireUnits;
using namespace SireStream;

/////////////
///////////// Implementation of Patching
/////////////

static const RegisterMetaType<Patching> r_patching( MAGIC_ONLY,
                                                    Patching::typeName() );
                                                    
QDataStream SIREVOL_EXPORT &operator<<(QDataStream &ds, const Patching &patching)
{
    writeHeader(ds, r_patching, 1);
    
    SharedDataStream sds(ds);
    
    sds << patching.spce << static_cast<const Property&>(patching);
    
    return ds;
}

QDataStream SIREVOL_EXPORT &operator>>(QDataStream &ds, Patching &patching)
{
    VersionID v = readHeader(ds, r_patching);
    
    if (v == 1)
    {
        SharedDataStream sds(ds);
        
        sds >> patching.spce >> static_cast<Property&>(patching);
    }
    else
        throw version_error(v, "1", r_patching, CODELOC);
        
    return ds;
}

/** Null constructor */
Patching::Patching() : Property()
{}

/** Internal constructor used to set the space */
Patching::Patching(const Space &space) : Property(), spce(space)
{}

/** Copy constructor */
Patching::Patching(const Patching &other)
         : Property(other), spce(other.spce)
{}

/** Destructor */
Patching::~Patching()
{}

/** Copy assignment operator */
Patching& Patching::operator=(const Patching &other)
{
    if (this != &other)
    {
        spce = other.spce;
        Property::operator=(other);
    }
    
    return *this;
}

/** Comparison operator */
bool Patching::operator==(const Patching &other) const
{
    return spce == other.spce;
}

/** Comparison operator */
bool Patching::operator!=(const Patching &other) const
{
    return not Patching::operator==(other);
}

const char* Patching::typeName()
{
    return "SireVol::Patching";
}
    
/** Return the space used to create this patching scheme */
const Space& Patching::space() const
{
    return spce.read();
}

/** Rebalance the patching so that the patches for the passed space contain
    roughly equal numbers of CoordGroups */
PatchingPtr Patching::rebalance(const Space &space,
                                const QVector<CoordGroupArray> &patchcoords) const
{
    return PatchingPtr(*this);
}

/** Retunr the null patching object */
NullPatching Patching::null()
{
    return NullPatching();
}

/////////////
///////////// Implementation of NullPatching
/////////////

static const RegisterMetaType<NullPatching> r_nullpatching;

QDataStream SIREVOL_EXPORT &operator<<(QDataStream &ds, const NullPatching &nullpatching)
{
    writeHeader(ds, r_nullpatching, 1);
    
    ds << static_cast<const Patching&>(nullpatching);
    
    return ds;
}

QDataStream SIREVOL_EXPORT &operator>>(QDataStream &ds, NullPatching &nullpatching)
{
    VersionID v = readHeader(ds, r_nullpatching);
    
    if (v == 1)
    {
        ds >> static_cast<Patching&>(nullpatching);
    }
    else
        throw version_error(v, "1", r_nullpatching, CODELOC);
        
    return ds;
}

/** Constructor */
NullPatching::NullPatching() : ConcreteProperty<NullPatching,Patching>()
{}

/** Copy constructor */
NullPatching::NullPatching(const NullPatching &other)
             : ConcreteProperty<NullPatching,Patching>(other)
{}

/** Destructor */
NullPatching::~NullPatching()
{}

const char* NullPatching::typeName()
{
    return QMetaType::typeName( qMetaTypeId<NullPatching>() );
}

/** Copy assignment operator */
NullPatching& NullPatching::operator=(const NullPatching &other)
{
    Patching::operator=(other);
    return *this;
}

/** Comparison operator */
bool NullPatching::operator==(const NullPatching &other) const
{
    return Patching::operator==(other);
}

/** Comparison operator */
bool NullPatching::operator!=(const NullPatching &other) const
{
    return Patching::operator!=(other);
}

/** Return the number of patches */
int NullPatching::nPatches() const
{
    return 1;
}

/** Return the patch index of the passed point */
int NullPatching::patchIndex(const Vector &point) const
{
    return 0;
}

/** Return the patch index and the center of the patch */
QPair<int,Vector> NullPatching::patchIndexAndCenter(const Vector &point) const
{
    return QPair<int,Vector>(0, Vector(0));
}

/** Repatch this patching for the passed space */
PatchingPtr NullPatching::repatch(const Space &new_space) const
{
    return *this;
}

/////////////
///////////// Implementation of BoxPatching
/////////////

static const RegisterMetaType<BoxPatching> r_boxpatching;

QDataStream SIREVOL_EXPORT &operator<<(QDataStream &ds, const BoxPatching &boxpatching)
{
    writeHeader(ds, r_boxpatching, 1);
    
    SharedDataStream sds(ds);
    
    sds << boxpatching.patch_size.to(angstrom)
        << boxpatching.gridvec
        << boxpatching.nx << boxpatching.ny << boxpatching.nz
        << static_cast<const Patching&>(boxpatching);
        
    return ds;
}

QDataStream SIREVOL_EXPORT &operator>>(QDataStream &ds, BoxPatching &boxpatching)
{
    VersionID v = readHeader(ds, r_boxpatching);
    
    if (v == 1)
    {
        SharedDataStream sds(ds);
        
        double patch_size;
        
        sds >> patch_size >> boxpatching.gridvec
            >> boxpatching.nx >> boxpatching.ny >> boxpatching.nz
            >> static_cast<Patching&>(boxpatching);
           
        boxpatching.patch_size = patch_size*angstrom;
              
        return ds;
    }
    else
        throw version_error(v, "1", r_boxpatching, CODELOC);
        
    return ds;
}

/** Constructor */
BoxPatching::BoxPatching()
            : ConcreteProperty<BoxPatching,Patching>(),
              patch_size(0), gridvec(0), nx(0), ny(0), nz(0)
{}
              
BoxPatching::BoxPatching(const Space &space, SireUnits::Dimension::Length patch_size);

/** Construct for the passed space - this tries to divide the space
    using a patch size of 8 A */
BoxPatching::BoxPatching(const Space &space)
            : ConcreteProperty<BoxPatching,Patching>()
{
    this->operator=( BoxPatching(space, 8*angstrom) );
}

BoxPatching::~BoxPatching();

const char* BoxPatching::typeName();

BoxPatching& BoxPatching::operator=(const BoxPatching &other);

bool BoxPatching::operator==(const BoxPatching &other) const;
bool BoxPatching::operator!=(const BoxPatching &other) const;

int BoxPatching::nPatches() const;

int BoxPatching::patchIndex(const Vector &point) const;

QPair<int,Vector> BoxPatching::patchIndexAndCenter(const Vector &point) const;

PatchingPtr BoxPatching::repatch(const Space &new_space) const;

