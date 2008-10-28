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

#include "atomid.h"
#include "atomidentifier.h"
#include "specify.hpp"

#include "atomatomid.h"
#include "groupatomids.h"
#include "molatomid.h"

#include "atom.h"
#include "selector.hpp"

#include "mover.hpp"
#include "editor.hpp"

#include "segment.h"
#include "chain.h"
#include "residue.h"
#include "cutgroup.h"
#include "atom.h"

#include "molecules.h"
#include "moleculegroup.h"
#include "moleculegroups.h"

#include "SireMol/errors.h"

#include "tostring.h"

#include <QDebug>

using namespace SireMol;
using namespace SireID;

/** Constructor */
AtomID::AtomID() : ID()
{}

/** Copy constructor */
AtomID::AtomID(const AtomID &other) : ID(other)
{}

/** Destructor */
AtomID::~AtomID()
{}
  
/** Return a specific atom that matches this ID */
Specify<AtomID> AtomID::operator[](int i) const
{
    return Specify<AtomID>(*this, i);
}

/** Return a specific atom that matches this ID */
Specify<AtomID> AtomID::operator()(int i) const
{
    return this->operator[](i);
}

/** Return a range of atoms that match this ID */
Specify<AtomID> AtomID::operator()(int i, int j) const
{
    return Specify<AtomID>(*this, i, j);
}

/** Combine with other ID types */
AtomAtomID AtomID::operator+(const AtomID &other) const
{
    return AtomAtomID(*this, other);
}

/** Combine with other ID types */
CGAtomID AtomID::operator+(const CGID &other) const
{
    return CGAtomID(other, *this);
}

/** Combine with other ID types */
ResAtomID AtomID::operator+(const ResID &other) const
{
    return ResAtomID(other, *this);
}

/** Combine with other ID types */
ChainAtomID AtomID::operator+(const ChainID &other) const
{
    return ChainAtomID(other, *this);
}

/** Combine with other ID types */
SegAtomID AtomID::operator+(const SegID &other) const
{
    return SegAtomID(other, *this);
}

/** Combine with other ID types */
MolAtomID AtomID::operator+(const MolID &other) const
{
    return MolAtomID(other, *this);
}

/** Return all of the atoms from the 'molecules' that match
    this ID
    
    \throw SireMol::missing_atom
*/
QHash< MolNum,Selector<Atom> >
AtomID::selectAllFrom(const Molecules &molecules) const
{
    QHash< MolNum,Selector<Atom> > selected_atoms;
    
    //loop over all molecules...
    for (Molecules::const_iterator it = molecules.constBegin();
         it != molecules.constEnd();
         ++it)
    {
        try
        {
            //try to find this atom in this molecule
            selected_atoms.insert( it.key(),
                                   it->selectAll(*this) );
        }
        catch(...)
        {}
    }
    
    if (selected_atoms.isEmpty())
        throw SireMol::missing_atom( QObject::tr(
            "There was no atom matching the ID \"%1\" in "
            "the set of molecules.")
                .arg(this->toString()), CODELOC );

    return selected_atoms;
}

/** Return the atom from the molecules 'molecules' that matches
    this ID
    
    \throw SireMol::missing_atom
    \throw SireMol::duplicate_atom
*/
Atom AtomID::selectFrom(const Molecules &molecules) const
{
    QHash< MolNum,Selector<Atom> > mols = this->selectAllFrom(molecules);
    
    if (mols.count() > 1)
        throw SireMol::duplicate_atom( QObject::tr(
            "More than one molecule contains an atom that "
            "matches this ID (%1). These molecules have numbers %2.")
                .arg(this->toString()).arg(Sire::toString(mols.keys())),
                    CODELOC );
                    
    const Selector<Atom> &atoms = *(mols.constBegin());
    
    if (atoms.count() > 1)
        throw SireMol::duplicate_atom( QObject::tr(
            "While only one molecule (MolNum == %1) "
            "contains an atom that matches this ID (%2), it contains "
            "more than one atom that matches.")
                .arg(atoms.data().number()).arg(this->toString()),
                    CODELOC );
                    
    return atoms[0];
}

/** Return the atom from the molecule group 'molgroup' that matches
    this ID
    
    \throw SireMol::missing_atom
    \throw SireMol::duplicate_atom
*/
Atom AtomID::selectFrom(const MoleculeGroup &molgroup) const
{
    QHash< MolNum,Selector<Atom> > mols = this->selectAllFrom(molgroup);
    
    if (mols.count() > 1)
        throw SireMol::duplicate_atom( QObject::tr(
            "More than one molecule contains an atom that "
            "matches this ID (%1). These molecules have numbers %2.")
                .arg(this->toString()).arg(Sire::toString(mols.keys())),
                    CODELOC );
                    
    const Selector<Atom> &atoms = *(mols.constBegin());
    
    if (atoms.count() > 1)
        throw SireMol::duplicate_atom( QObject::tr(
            "While only one molecule (MolNum == %1) "
            "contains an atom that matches this ID (%2), it contains "
            "more than one atom that matches.")
                .arg(atoms.data().number()).arg(this->toString()),
                    CODELOC );
                    
    return atoms[0];
}

/** Return the atoms from the molecule group 'molgroup' that match
    this ID
    
    \throw SireMol::missing_atom
*/
QHash< MolNum,Selector<Atom> >
AtomID::selectAllFrom(const MoleculeGroup &molgroup) const
{
    return AtomID::selectAllFrom(molgroup.molecules());
}

/** Return the atom from the molecule groups 'molgroups' that matches 
    this ID
    
    \throw SireMol::missing_atom
    \throw SireMol::duplicate_atom
*/
Atom AtomID::selectFrom(const MolGroupsBase &molgroups) const
{
    QHash< MolNum,Selector<Atom> > mols = this->selectAllFrom(molgroups);
    
    if (mols.count() > 1)
        throw SireMol::duplicate_atom( QObject::tr(
            "More than one molecule contains an atom that "
            "matches this ID (%1). These molecules have numbers %2.")
                .arg(this->toString()).arg(Sire::toString(mols.keys())),
                    CODELOC );
                    
    const Selector<Atom> &atoms = *(mols.constBegin());
    
    if (atoms.count() > 1)
        throw SireMol::duplicate_atom( QObject::tr(
            "While only one molecule (MolNum == %1) "
            "contains an atom that matches this ID (%2), it contains "
            "more than one atom that matches.")
                .arg(atoms.data().number()).arg(this->toString()),
                    CODELOC );
                    
    return atoms[0];
}

/** Return the set of atoms that match this ID in the molecule groups
    set 'molgroups' 
    
    \throw SireMol::missing_atom
*/
QHash< MolNum,Selector<Atom> >
AtomID::selectAllFrom(const MolGroupsBase &molgroups) const
{
    qDebug() << this->what();
    qDebug() << CODELOC;
    return AtomID::selectAllFrom(molgroups.molecules());
}

//fully instantiate Specify<AtomID>
template class Specify<AtomID>;
