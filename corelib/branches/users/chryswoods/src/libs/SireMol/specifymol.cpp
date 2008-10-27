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

#include "specifymol.h"

#include "molecules.h"
#include "moleculegroup.h"
#include "moleculegroups.h"

#include "SireMol/errors.h"
#include "SireError/errors.h"

using namespace SireMol;

/** Constructor */
SpecifyMol::SpecifyMol() : MolID(), strt(0), end(-1)
{}

/** Construct to match all of the molecules that match the 
    ID 'molid' */
SpecifyMol::SpecifyMol(const MolID &mol_id)
           : MolID(), molid(mol_id), strt(0), end(-1)
{}

/** Construct to match the ith molecule that matches the ID 'molid' */
SpecifyMol::SpecifyMol(const MolID &mol_id, int i)
           : MolID(), molid(mol_id), strt(i), end(i)
{}

/** Construct to math the range of molecules from i to j that
    match the ID 'molid' */
SpecifyMol::SpecifyMol(const MolID &mol_id, int i, int j)
           : MolID(), molid(mol_id), strt(i), end(j)
{}

/** Copy constructor */
SpecifyMol::SpecifyMol(const SpecifyMol &other)
           : MolID(other), molid(other.molid),
             strt(other.strt), end(other.end)
{}

/** Destructor */
SpecifyMol::~SpecifyMol()
{}

/** Copy assignment operator */
SpecifyMol& SpecifyMol::operator=(const SpecifyMol &other)
{
    molid = other.molid;
    strt = other.strt;
    end = other.end;
    MolID::operator=(other);
    
    return *this;
}

/** Comparison operator */
bool SpecifyMol::operator==(const SpecifyMol &other) const;

/** Comparison operator */
bool SpecifyMol::operator!=(const SpecifyMol &other) const;

/** Comparison operator */
bool SpecifyMol::operator==(const SireID::ID &other) const;

/** Comparison operator */
bool SpecifyMol::operator!=(const SireID::ID &other) const;

uint SpecifyMol::hash() const;

QString SpecifyMol::toString() const;

bool SpecifyMol::isNull() const;

QList<MolNum> SpecifyMol::map(const Molecules &molecules) const;
QList<MolNum> SpecifyMol::map(const MoleculeGroup &molgroup) const;
QList<MolNum> SpecifyMol::map(const MolGroupsBase &molgroups) const;
