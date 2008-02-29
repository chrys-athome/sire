/********************************************\
  *
  *  Sire - Molecular Simulation Framework
  *
  *  Copyright (C) 2008  Christopher Woods
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

#include "ffmolecules.h"

#include "SireMol/atomselection.h"
#include "SireMol/molnum.h"

#include "SireMol/mover.hpp"

#include "SireError/errors.h"
#include "SireMol/errors.h"

#include "tostring.h"

#include "SireStream/datastream.h"
#include "SireStream/shareddatastream.h"

using namespace SireFF;
using namespace SireFF::detail;

using namespace SireStream;

/////////
///////// Implementation of FFMoleculeBase
/////////

static const RegisterMetaType<FFMoleculeBase> r_ffmolbase(MAGIC_ONLY,
                                                  "SireFF::detail::FFMoleculeBase");

/** Serialise to a binary datastream */
QDataStream SIREFF_EXPORT &operator<<(QDataStream &ds,
                                      const FFMoleculeBase &ffmol)
{
    writeHeader(ds, r_ffmolbase, 1);
    
    SharedDataStream sds(ds);
    sds << ffmol.mol;
    
    return ds;
}

/** Extract from a binary datastream */
QDataStream SIREFF_EXPORT &operator>>(QDataStream &ds,
                                      FFMoleculeBase &ffmol)
{
    VersionID v = readHeader(ds, r_ffmolbase);
    
    if (v == 1)
    {
        SharedDataStream sds(ds);
        sds >> ffmol.mol;
    }
    else
        throw version_error(v, "1", r_ffmolbase, CODELOC);
        
    return ds;
}

/** Null constructor */
FFMoleculeBase::FFMoleculeBase()
{}

/** Construct a specialised version of the view of the molecule
    in 'molview' */
FFMoleculeBase::FFMoleculeBase(const PartialMolecule &molview)
           : mol(molview)
{}

/** Destructor */
FFMoleculeBase::~FFMoleculeBase()
{}

/** Copy assignment operator */
FFMoleculeBase& FFMoleculeBase::operator=(const FFMoleculeBase &other)
{
    mol = other.mol;
    return *this;
}

/** Comparison operator */
bool FFMoleculeBase::operator==(const FFMoleculeBase &other) const
{
    return mol == other.mol;
}

/** Comparison operator */
bool FFMoleculeBase::operator!=(const FFMoleculeBase &other) const
{
    return mol != other.mol;
}

/** Return a reference to the non-specialised molecule */
const PartialMolecule& FFMoleculeBase::molecule() const
{
    return mol;
}

/** Return the number of the molecule */
MolNum FFMoleculeBase::number() const
{
    return mol.number();
}

/** Return whether or not this molecule is empty (contains no atoms) */
bool FFMoleculeBase::isEmpty() const
{
    return mol.selection().selectedNone();
}

/** Change this view to use the passed molecule

    \throw SireError::incompatible_error
*/
bool FFMoleculeBase::change(const PartialMolecule &molecule)
{
    if (molecule.number() != mol.number())
        throw SireError::incompatible_error( QObject::tr(
            "You cannot change the molecule %1 using the molecule %2 "
            "as they are different molecules! (their numbers are "
            "different)")
                .arg(mol.number()).arg(molecule.number()), CODELOC );

    if (mol != molecule)
    {
        mol = molecule;
        return true;
    }
    else
        return false;
}

////////
//////// Implementation of FFMoleculesBase
////////

static const RegisterMetaType<FFMoleculesBase> r_ffmolsbase( MAGIC_ONLY,
                                                "SireFF::detail::FFMoleculesBase" );
                                                
/** Serialise to a binary datastream */
QDataStream SIREFF_EXPORT &operator<<(QDataStream &ds,
                                      const FFMoleculesBase &ffmols)
{
    writeHeader(ds, r_ffmolsbase, 1);
    
    SharedDataStream sds(ds);
    sds << ffmols.molnums_by_idx;
    
    return ds;
}

/** Extract from a binary datastream */
QDataStream SIREFF_EXPORT &operator>>(QDataStream &ds,
                                      FFMoleculesBase &ffmols)
{
    VersionID v = readHeader(ds, r_ffmolsbase);
    
    if (v == 1)
    {
        SharedDataStream sds(ds);
        sds >> ffmols.molnums_by_idx;
        ffmols._pvt_reindex();
    }
    else
        throw version_error(v, "1", r_ffmolsbase, CODELOC);
        
    return ds;
}

/** Null constructor */
FFMoleculesBase::FFMoleculesBase()
{}

/** Copy constructor */
FFMoleculesBase::FFMoleculesBase(const FFMoleculesBase &other)
                : molnums_by_idx(other.molnums_by_idx),
                  idxs_by_molnum(other.idxs_by_molnum)
{}

/** Destructor */
FFMoleculesBase::~FFMoleculesBase()
{}

/** Copy assignment operator */
FFMoleculesBase& FFMoleculesBase::operator=(const FFMoleculesBase &other)
{
    molnums_by_idx = other.molnums_by_idx;
    idxs_by_molnum = other.idxs_by_molnum;
    return *this;
}

/** Comparison operator */
bool FFMoleculesBase::operator==(const FFMoleculesBase &other) const
{
    return molnums_by_idx == other.molnums_by_idx;
}

/** Comparison operator */
bool FFMoleculesBase::operator!=(const FFMoleculesBase &other) const
{
    return molnums_by_idx != other.molnums_by_idx;
}

/** Return the array that maps the index in the group to the 
    molecule number */
const QVector<MolNum> FFMoleculesBase::molNumsByIndex() const
{
    return molnums_by_idx;
}

/** Return the hash that maps molecule numbers to their index 
    in the array */
const QHash<MolNum,quint32>& FFMoleculesBase::indexesByMolNum() const
{
    return idxs_by_molnum;
}

/** Return the index of the molecule with number 'molnum'

    \throw SireMol::missing_molecule
*/
quint32 FFMoleculesBase::indexOf(MolNum molnum) const
{
    QHash<MolNum,quint32>::const_iterator it = idxs_by_molnum.constFind(molnum);
    
    if (it == idxs_by_molnum.constEnd())
        throw SireMol::missing_molecule( QObject::tr(
            "There is no molecule with number %1 in this group. "
            "The available molecules are %2.")
                .arg(molnum).arg( Sire::toString(idxs_by_molnum.keys()) ),
                    CODELOC );
                    
    return *it;
}

/** Return whether or not this group contains the molecule with 
    number 'molnum' */
bool FFMoleculesBase::contains(MolNum molnum) const
{
    return idxs_by_molnum.contains(molnum);
}

/** Assert that this group contains the molecule with number 'molnum'

    \throw SireMol::missing_molecule
*/
void FFMoleculesBase::assertContains(MolNum molnum) const
{
    if (not idxs_by_molnum.contains(molnum))
        throw SireMol::missing_molecule( QObject::tr(
            "There is no molecule with number %1 in this group. "
            "The available molecules are %2.")
                .arg(molnum).arg( Sire::toString(idxs_by_molnum.keys()) ),
                    CODELOC );
}

/** Add the molecule 'molnum' to the array, returning its index */
quint32 FFMoleculesBase::_pvt_add(MolNum molnum)
{
    QHash<MolNum,quint32>::const_iterator it = idxs_by_molnum.constFind(molnum);
    
    if (it == idxs_by_molnum.constEnd())
    {
        //this is a new molecule - add it onto the end of the array
        molnums_by_idx.append(molnum);
        idxs_by_molnum.insert(molnum, molnums_by_idx.count() - 1);
        
        return molnums_by_idx.count() - 1;
    }
    else
        return *it;
}

/** Reindex this group */
void FFMoleculesBase::_pvt_reindex()
{
    idxs_by_molnum.clear();
    
    if (molnums_by_idx.isEmpty())
        return;
        
    quint32 nmols = molnums_by_idx.count();
    const MolNum *molnums_array = molnums_by_idx.constData();
    
    idxs_by_molnum.reserve(nmols);
    
    for (quint32 i=0; i<nmols; ++i)
    {
        idxs_by_molnum.insert(molnums_array[i], i);
    }
}

/** Completely remove the molecule with number 'molnum' from this index.
    This does nothing if this molecule is not in this group */
void FFMoleculesBase::_pvt_remove(MolNum molnum)
{
    QHash<MolNum,quint32>::const_iterator it = idxs_by_molnum.constFind(molnum);
    
    if (it != idxs_by_molnum.constEnd())
    {
        quint32 idx = *it;
        
        idxs_by_molnum.remove(molnum);
        
        molnums_by_idx.remove(idx);
        
        if (molnums_by_idx.isEmpty())
        {
            molnums_by_idx.clear();
            idxs_by_molnum.clear();
        }
        else if (idx != quint32(molnums_by_idx.count()))
        {
            //we need to reindex the arrays...
            this->_pvt_reindex();
        }
    }
}

/** Remove all of the molecules whose numbers are in 'molnums' */
void FFMoleculesBase::_pvt_remove(const QList<MolNum> &molnums)
{
    if (molnums.isEmpty())
        return;
    else if (molnums.count() == 1)
    {
        this->_pvt_remove( *(molnums.begin()) );
        return;
    }
    
    QHash<MolNum,quint32>::const_iterator it;
    bool removed_some = false;
    
    foreach (MolNum molnum, molnums)
    {
        it = idxs_by_molnum.constFind(molnum);
        
        if (it != idxs_by_molnum.constEnd())
        {
            molnums_by_idx.remove(*it);
            idxs_by_molnum.remove(molnum);
            removed_some = true;
        }
    }
    
    if (removed_some)
        this->_pvt_reindex();
}
