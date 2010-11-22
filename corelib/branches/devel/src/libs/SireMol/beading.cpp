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

#include "beading.h"
#include "beadidx.h"
#include "atomidx.h"
#include "atomselection.h"
#include "moleculeinfodata.h"

#include "SireError/errors.h"
#include "SireBase/errors.h"

#include "SireStream/datastream.h"
#include "SireStream/shareddatastream.h"

using namespace SireMol;
using namespace SireBase;
using namespace SireStream;

//////////
////////// Implementation of Beading
//////////

static const RegisterMetaType<Beading> r_beading( MAGIC_ONLY,
                                                  Beading::typeName() );
                                                  
QDataStream SIREMOL_EXPORT &operator<<(QDataStream &ds, const Beading &beading)
{
    writeHeader(ds, r_beading, 1);
    
    ds << static_cast<const MolViewProperty&>(beading);
    
    return ds; 
}

QDataStream SIREMOL_EXPORT &operator>>(QDataStream &ds, Beading &beading)
{
    VersionID v = readHeader(ds, r_beading);
    
    if (v == 1)
    {
        ds >> static_cast<MolViewProperty&>(beading);
    }
    else
        throw version_error(v, "1", r_beading, CODELOC);
        
    return ds;
}

/** Constructor */
Beading::Beading() : MolViewProperty()
{}

/** Copy constructor */
Beading::Beading(const Beading &other) : MolViewProperty(other)
{}

/** Destructor */
Beading::~Beading()
{}

void Beading::assertValidIndex(BeadIdx idx, const MoleculeInfoData &molinfo) const
{
    idx.map( this->nBeads(molinfo) );
}

bool Beading::isCompatibleWith(const MoleculeInfoData&) const
{
    return true;
}

/** Copy assignment operator */
Beading& Beading::operator=(const Beading &other)
{
    MolViewProperty::operator=(other);
    return *this;
}

/** Comparison operator */
bool Beading::operator==(const Beading &other) const
{
    return MolViewProperty::operator==(other);
}

/** Comparison operator */
bool Beading::operator!=(const Beading &other) const
{
    return MolViewProperty::operator!=(other);
}

const char* Beading::typeName()
{
    return "SireMol::Beading";
}

NullBeading Beading::null()
{
    return NullBeading();
}

//////////
////////// Implementation of MoleculeBeading
//////////


//////////
////////// Implementation of ResidueBeading
//////////

static const RegisterMetaType<ResidueBeading> r_resbeading;

QDataStream SIREMOL_EXPORT &operator<<(QDataStream &ds, const ResidueBeading &resbeading)
{
    writeHeader(ds, r_resbeading, 1);
    
    ds << static_cast<const Beading&>(resbeading);
    
    return ds;
}

QDataStream SIREMOL_EXPORT &operator>>(QDataStream &ds, ResidueBeading &resbeading)
{
    VersionID v = readHeader(ds, r_resbeading);
    
    if (v == 1)
    {
        ds >> static_cast<Beading&>(resbeading);
    }
    else
        throw version_error(v, "1", r_resbeading, CODELOC);
        
    return ds;
}

/** Constructor */
ResidueBeading::ResidueBeading() : ConcreteProperty<ResidueBeading,Beading>()
{}

/** Copy constructor */
ResidueBeading::ResidueBeading(const ResidueBeading &other)
               : ConcreteProperty<ResidueBeading,Beading>(other)
{}

/** Destructor */
ResidueBeading::~ResidueBeading()
{}

/** Copy assignment operator */
ResidueBeading& ResidueBeading::operator=(const ResidueBeading &other)
{
    Beading::operator=(other);
    return *this;
}

/** Comparison operator */
bool ResidueBeading::operator==(const ResidueBeading &other) const
{
    return Beading::operator==(other);
}

/** Comparison operator */
bool ResidueBeading::operator!=(const ResidueBeading &other) const
{
    return Beading::operator!=(other);
}

const char* ResidueBeading::typeName()
{
    return QMetaType::typeName( qMetaTypeId<ResidueBeading>() );
}

/** Return the number of beads */
int ResidueBeading::nBeads(const MoleculeInfoData &moldata) const
{
    return moldata.nResidues();
}

/** Return the index of the ith atom in the bead with index 'i' */
AtomIdx ResidueBeading::atomIdx(const MoleculeInfoData &moldata,
                                const BeadIdx &bead, int i) const
{
    return moldata.getAtom( ResIdx(bead.value()), i );
}

/** Return the atom properties in bead/atom order */
PropertyPtr ResidueBeading::atomProperty(const MoleculeData &moldata,
                                         const SireBase::PropertyName &key) const
{
    throw SireError::incomplete_code( "TODO", CODELOC );
    
    return PropertyPtr();
}
                         
/** Return the atoms that are in all of the beads */
AtomSelection ResidueBeading::selection(const MoleculeInfoData &moldata) const
{
    return AtomSelection(moldata);
}

/** Return the atoms that are in the bead with index 'bead' 

    \throw SireError::invalid_index
*/                         
AtomSelection ResidueBeading::selection(const MoleculeInfoData &moldata,
                                        const BeadIdx &bead) const
{
    AtomSelection selected_atoms(moldata);
    return selected_atoms.selectNone().select( ResIdx(bead.value()) );
}

/** Return the indexes of all of the atoms in the beads */
QList<AtomIdx> ResidueBeading::atomIdxs(const MoleculeInfoData &moldata) const
{
    QList<AtomIdx> atoms;
    
    for (int i=0; i<moldata.nAtoms(); ++i)
    {
        atoms.append( AtomIdx(i) );
    }
    
    return atoms;
}

/** Return the indexes of the atoms in the bead with index 'bead'

    \throw SireError::invalid_index
*/
QList<AtomIdx> ResidueBeading::atomIdxs(const MoleculeInfoData &moldata,
                                        const BeadIdx &bead) const
{
    return moldata.getAtomsIn( ResIdx(bead.value()) );
}

//////////
////////// Implementation of UserBeading
//////////


//////////
////////// Implementation of NullBeading
//////////

static const RegisterMetaType<NullBeading> r_nullbeading;

QDataStream SIREMOL_EXPORT &operator<<(QDataStream &ds, const NullBeading &nullbeading)
{
    writeHeader(ds, r_nullbeading, 1);
    
    ds << static_cast<const Beading&>(nullbeading);
    
    return ds;
}

QDataStream SIREMOL_EXPORT &operator>>(QDataStream &ds, NullBeading &nullbeading)
{
    VersionID v = readHeader(ds, r_nullbeading);
    
    if (v == 1)
    {
        ds >> static_cast<Beading&>(nullbeading);
    }
    else
        throw version_error(v, "1", r_nullbeading, CODELOC);
        
    return ds;
}

/** Constructor */
NullBeading::NullBeading() : ConcreteProperty<NullBeading,Beading>()
{}

/** Copy constructor */
NullBeading::NullBeading(const NullBeading &other)
            : ConcreteProperty<NullBeading,Beading>(other)
{}

/** Destructor */
NullBeading::~NullBeading()
{}

/** Copy assignment operator */
NullBeading& NullBeading::operator=(const NullBeading &other)
{
    Beading::operator=(other);
    return *this;
}

/** Comparison operator */
bool NullBeading::operator==(const NullBeading &other) const
{
    return Beading::operator==(other);
}

/** Comparison operator */
bool NullBeading::operator!=(const NullBeading &other) const
{
    return Beading::operator!=(other);
}

const char* NullBeading::typeName()
{
    return QMetaType::typeName( qMetaTypeId<NullBeading>() );
}

/** Return the number of beads in the passed molecule */
int NullBeading::nBeads(const MoleculeInfoData &moldata) const
{
    return 0;
}

/** Return the AtomIdx of the ith atom in the bead with index 'bead' */
AtomIdx NullBeading::atomIdx(const MoleculeInfoData &moldata,
                             const BeadIdx &bead, int i) const
{
    throw SireError::invalid_index( QObject::tr(
            "NullBeading creates zero beads, so there are no atoms in any bead!"),
                CODELOC );
                
    return AtomIdx();
}

/** Return the atom properties of all of the beads in bead, then atom order */
PropertyPtr NullBeading::atomProperty(const MoleculeData &moldata,
                                      const PropertyName &key) const
{
    throw SireBase::missing_property( QObject::tr(
            "There are no Beads using a NullBeading, so no property with "
            "key %1.").arg(key.toString()), CODELOC );
            
    return PropertyPtr();
}
                         
/** Return the atoms from the molecule that are part of the beads */
AtomSelection NullBeading::selection(const MoleculeInfoData &moldata) const
{
    return AtomSelection();
}
                         
/** Return the atoms from the molecule that in the bead with index 'bead'

    \throw SireError::invalid_index
*/
AtomSelection NullBeading::selection(const MoleculeInfoData &moldata,
                                     const BeadIdx &bead) const
{
    throw SireError::invalid_index( QObject::tr(
            "The NullBeading beader creates no beads, so there is no bead with "
            "index %1.").arg(bead.value()), CODELOC );
            
    return AtomSelection();
}

/** Return the atom indexes of the atoms that are part of the beads */
QList<AtomIdx> NullBeading::atomIdxs(const MoleculeInfoData &moldata) const
{
    return QList<AtomIdx>();
}

/** Return the atom indexes that are part of the bead with index 'bead' */
QList<AtomIdx> NullBeading::atomIdxs(const MoleculeInfoData &moldata,
                                     const BeadIdx &bead) const
{
    throw SireError::invalid_index( QObject::tr(
            "The NullBeading beader creates no beads, so there is no bead with "
            "index %1.").arg(bead), CODELOC );

    return QList<AtomIdx>();
}

