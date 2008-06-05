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

#include "cgid.h"
#include "cgidentifier.h"
#include "specify.hpp"

#include "atom.h"
#include "selector.hpp"

#include "mover.hpp"
#include "editor.hpp"

#include "molinfo.h"

#include "partialmolecule.h"
#include "segment.h"
#include "chain.h"
#include "residue.h"
#include "cutgroup.h"
#include "atom.h"

#include "molecules.h"
#include "molgroup.h"
#include "molgroups.h"

#include "SireMol/errors.h"

#include "tostring.h"

using namespace SireMol;
using namespace SireID;

/** Constructor */
CGID::CGID() : ID()
{}

/** Copy constructor */
CGID::CGID(const CGID &other) : ID(other)
{}

/** Destructor */
CGID::~CGID()
{}
  
/** Return a specific atom that matches this ID */
Specify<CGID> CGID::operator[](int i) const
{
    return Specify<CGID>(*this, i);
}

/** Return a specific atom that matches this ID */
Specify<CGID> CGID::operator()(int i) const
{
    return this->operator[](i);
}

/** Return a range of atoms that match this ID */
Specify<CGID> CGID::operator()(int i, int j) const
{
    return Specify<CGID>(*this, i, j);
}

/** Return the atoms in the matching residues */
AtomsIn<CGID> CGID::atoms() const
{
    return AtomsIn<CGID>(*this);
}

/** Return a specific atom in the matching residues */
AtomsIn<CGID> CGID::atom(int i) const
{
    return AtomsIn<CGID>(*this, i);
}

/** Return a range of atoms in the matching residues */
AtomsIn<CGID> CGID::atoms(int i, int j) const
{
    return AtomsIn<CGID>(*this, i, j);
}

/** Return all of the CutGroups from the 'molecules' that match
    this ID
    
    \throw SireMol::missing_cutgroup
*/
QHash< MolNum,Selector<CutGroup> >
CGID::selectAllFrom(const Molecules &molecules) const
{
    QHash< MolNum,Selector<CutGroup> > selected_cutgroups;
    
    //loop over all molecules...
    for (Molecules::const_iterator it = molecules.constBegin();
         it != molecules.constEnd();
         ++it)
    {
        try
        {
            //try to find this atom in this molecule
            selected_cutgroups.insert( it.key(),
                                       it->all().selectAll(*this) );
        }
        catch(...)
        {}
    }
    
    if (selected_cutgroups.isEmpty())
        throw SireMol::missing_cutgroup( QObject::tr(
            "There was no CutGroup matching the ID \"%1\" in "
            "the set of molecules.")
                .arg(this->toString()), CODELOC );

    return selected_cutgroups;
}

/** Return the CutGroup from the molecules 'molecules' that matches
    this ID
    
    \throw SireMol::missing_cutgroup
    \throw SireMol::duplicate_cutgroup
*/
CutGroup CGID::selectFrom(const Molecules &molecules) const
{
    QHash< MolNum,Selector<CutGroup> > mols = this->selectAllFrom(molecules);
    
    if (mols.count() > 1)
        throw SireMol::duplicate_cutgroup( QObject::tr(
            "More than one molecule contains a CutGroup that "
            "matches this ID (%1). These molecules have numbers %2.")
                .arg(this->toString()).arg(Sire::toString(mols.keys())),
                    CODELOC );
                    
    const Selector<CutGroup> &cutgroups = *(mols.constBegin());
    
    if (cutgroups.count() > 1)
        throw SireMol::duplicate_cutgroup( QObject::tr(
            "While only one molecule (MolNum == %1) "
            "contains a CutGroup that matches this ID (%2), it contains "
            "more than one CutGroup that matches.")
                .arg(cutgroups.data().number()).arg(this->toString()),
                    CODELOC );
                    
    return cutgroups[0];
}

/** Return the CutGroup from the molecule group 'molgroup' that matches
    this ID
    
    \throw SireMol::missing_cutgroup
    \throw SireMol::duplicate_cutgroup
*/
CutGroup CGID::selectFrom(const MolGroup &molgroup) const
{
    return CGID::selectFrom(molgroup.molecules());
}

/** Return the CutGroups from the molecule group 'molgroup' that match
    this ID
    
    \throw SireMol::missing_cutgroup
*/
QHash< MolNum,Selector<CutGroup> >
CGID::selectAllFrom(const MolGroup &molgroup) const
{
    return CGID::selectAllFrom(molgroup.molecules());
}

/** Return the CutGroup from the molecule groups 'molgroups' that matches 
    this ID
    
    \throw SireMol::missing_cutgroup
    \throw SireMol::duplicate_cutgroup
*/
CutGroup CGID::selectFrom(const MolGroupsBase &molgroups) const
{
    return CGID::selectFrom(molgroups.molecules());
}

/** Return the set of CutGroups that match this ID in the molecule groups
    set 'molgroups' 
    
    \throw SireMol::missing_cutgroup
*/
QHash< MolNum,Selector<CutGroup> >
CGID::selectAllFrom(const MolGroupsBase &molgroups) const
{
    return CGID::selectAllFrom(molgroups.molecules());
}

//fully instantiate Specify<CGID> and AtomsIn<CGID>
template class Specify<CGID>;
template class AtomsIn<CGID>;
