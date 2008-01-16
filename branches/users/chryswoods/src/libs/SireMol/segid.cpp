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

#include "segid.h"
#include "segidentifier.h"
#include "specify.hpp"

#include "atom.h"
#include "selector.hpp"

#include "mover.hpp"
#include "editor.hpp"

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
SegID::SegID() : ID()
{}

/** Copy constructor */
SegID::SegID(const SegID &other) : ID(other)
{}

/** Destructor */
SegID::~SegID()
{}
  
/** Return a specific atom that matches this ID */
Specify<SegID> SegID::operator[](int i) const
{
    return Specify<SegID>(*this, i);
}

/** Return a specific atom that matches this ID */
Specify<SegID> SegID::operator()(int i) const
{
    return this->operator[](i);
}

/** Return a range of atoms that match this ID */
Specify<SegID> SegID::operator()(int i, int j) const
{
    return Specify<SegID>(*this, i, j);
}

/** Return the atoms in the matching residues */
AtomsIn<SegID> SegID::atoms() const
{
    return AtomsIn<SegID>(*this);
}

/** Return a specific atom in the matching residues */
AtomsIn<SegID> SegID::atom(int i) const
{
    return AtomsIn<SegID>(*this, i);
}

/** Return a range of atoms in the matching residues */
AtomsIn<SegID> SegID::atoms(int i, int j) const
{
    return AtomsIn<SegID>(*this, i, j);
}

/** Return all of the segments from the 'molecules' that match
    this ID
    
    \throw SireMol::missing_segment
*/
QHash< MolNum,Selector<Segment> >
SegID::selectAllFrom(const Molecules &molecules) const
{
    QHash< MolNum,Selector<Segment> > selected_segments;
    
    //loop over all molecules...
    for (Molecules::const_iterator it = molecules.constBegin();
         it != molecules.constEnd();
         ++it)
    {
        try
        {
            //try to find this segment in this molecule
            selected_segments.insert( it.key(),
                                      it->all().selectAll(*this) );
        }
        catch(...)
        {}
    }
    
    if (selected_segments.isEmpty())
        throw SireMol::missing_segment( QObject::tr(
            "There was no segment matching the ID \"%1\" in "
            "the set of molecules.")
                .arg(this->toString()), CODELOC );

    return selected_segments;
}

/** Return the segment from the molecules 'molecules' that matches
    this ID
    
    \throw SireMol::missing_segment
    \throw SireMol::duplicate_segment
*/
Segment SegID::selectFrom(const Molecules &molecules) const
{
    QHash< MolNum,Selector<Segment> > mols = this->selectAllFrom(molecules);
    
    if (mols.count() > 1)
        throw SireMol::duplicate_segment( QObject::tr(
            "More than one molecule contains a segment that "
            "matches this ID (%1). These molecules have numbers %2.")
                .arg(this->toString()).arg(Sire::toString(mols.keys())),
                    CODELOC );
                    
    const Selector<Segment> &segments = *(mols.constBegin());
    
    if (segments.count() > 1)
        throw SireMol::duplicate_segment( QObject::tr(
            "While only one molecule (MolNum == %1) "
            "contains a segment that matches this ID (%2), it contains "
            "more than one segment that matches.")
                .arg(segments.data().number()).arg(this->toString()),
                    CODELOC );
                    
    return segments[0];
}

/** Return the segment from the molecule group 'molgroup' that matches
    this ID
    
    \throw SireMol::missing_segment
    \throw SireMol::duplicate_segment
*/
Segment SegID::selectFrom(const MolGroup &molgroup) const
{
    return SegID::selectFrom(molgroup.molecules());
}

/** Return the segments from the molecule group 'molgroup' that match
    this ID
    
    \throw SireMol::missing_segment
*/
QHash< MolNum,Selector<Segment> >
SegID::selectAllFrom(const MolGroup &molgroup) const
{
    return SegID::selectAllFrom(molgroup.molecules());
}

/** Return the segment from the molecule groups 'molgroups' that matches 
    this ID
    
    \throw SireMol::missing_segment
    \throw SireMol::duplicate_segment
*/
Segment SegID::selectFrom(const MolGroupsBase &molgroups) const
{
    return SegID::selectFrom(molgroups.molecules());
}

/** Return the set of segments that match this ID in the molecule groups
    set 'molgroups' 
    
    \throw SireMol::missing_segment
*/
QHash< MolNum,Selector<Segment> >
SegID::selectAllFrom(const MolGroupsBase &molgroups) const
{
    return SegID::selectAllFrom(molgroups.molecules());
}

//fully instantiate template classes
template class Specify<SegID>;
template class AtomsIn<SegID>;
