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

/** Return the set of atoms that match this ID in the molecule groups
    set 'molgroups' */
QHash< MolNum,Selector<Atom> >
AtomID::selectAllFrom(const MolGroupsBase &molgroups) const
{
    //get a list of all of the molecules in this group
    const Molecules all_mols = molgroups.molecules();
    
    QHash< MolNum,Selector<Atom> > selected_atoms;
    
    //loop over all molecules...
    for (Molecules::const_iterator it = all_mols.constBegin();
         it != all_mols.constEnd();
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
    
    if (all_mols.isEmpty())
        throw SireMol::missing_atom( QObject::tr(
            "There was no atom matching the ID \"%1\" in "
            "the set of molecules.")
                .arg(this->toString()), CODELOC );

    return all_mols;
}

//fully instantiate Specify<AtomID>
template class Specify<AtomID>;
