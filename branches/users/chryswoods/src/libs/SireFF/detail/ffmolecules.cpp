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
