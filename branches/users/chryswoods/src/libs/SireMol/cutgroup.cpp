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

#include "cutgroup.h"

#include "editor.hpp"
#include "mover.hpp"
#include "selector.hpp"
#include "evaluator.h"

#include "SireStream/datastream.h"
#include "SireStream/shareddatastream.h"

using namespace SireMol;
using namespace SireStream;

RegisterMetaType<CutGroup> r_cg;

/** Serialise to a binary datastream */
QDataStream SIREMOL_EXPORT &operator<<(QDataStream &ds, const CutGroup &cg)
{
    writeHeader(ds, r_cg, 1);

    SharedDataStream sds(ds);
    
    sds << cg.cgidx << static_cast<const MoleculeView&>(CutGroup);

    return ds;
}

/** Deserialise from a binary datastream */
QDataStream SIREMOL_EXPORT &operator>>(QDataStream &ds, CutGroup &cg)
{
    VersionID v = readHeader(ds, r_cg);

    if (v == 1)
    {
        SharedDataStream sds(ds);
        
        sds >> cg.cgidx >> static_cast<MoleculeView&>(CutGroup);
    }
    else
        throw version_error(v, "1", r_cg, CODELOC);

    return ds;
}

/** Null constructor */
CutGroup::CutGroup() : MoleculeView(), CGIdx( CGIdx::null() )
{}

/** Construct the CutGroup at ID 'cgid' in the molecule whose data
    is in 'moldata'
    
    \throw SireMol::missing_CutGroup
    \throw SireMol::duplicate_CutGroup
    \throw SireError::invalid_index
*/
CutGroup::CutGroup(const MoleculeData &moldata, const CGID &cgid)
      : MoleculeView(moldata), cgidx( moldata.info().cgIdx(cgid) )
{}

/** Copy constructor */
CutGroup::CutGroup(const CutGroup &other)
      : MoleculeView(other), cgidx(other.cgidx)
{}

/** Destructor */
CutGroup::~CutGroup()
{}

/** Copy assignment operator */
CutGroup& CutGroup::operator=(const CutGroup &other)
{
    MoleculeView::operator=(other);
    cgidx = other.cgidx;
    return *this;
}

/** Comparison operator */
bool CutGroup::operator==(const CutGroup &other) const
{
    return cgidx == other.cgidx and 
           MoleculeView::operator==(other);
}

/** Comparison operator */
bool CutGroup::operator!=(const CutGroup &other) const
{
    return cgidx != other.cgidx or
           MoleculeView::operator!=(other);
}

/** Return the atoms that are in this CutGroup */
AtomSelection CutGroup::selectedAtoms() const
{
    return selected_atoms;
}

/** Return the name of this CutGroup */
CutGroupName CutGroup::name() const
{
    return d->info().name(cgidx);
}

/** Return the index of this CutGroup in the molecule */
CGIdx CutGroup::index() const
{
    return cgidx;
}

/** Return the info object that describes this CutGroup */
CutGroupInfo CutGroup::info() const
{
    return CutGroupInfo( d->info(), cgidx );
}

/** Return an object that can move a copy of this CutGroup */
Mover<CutGroup> CutGroup::move() const
{
    return Mover<CutGroup>(*this);
}

/** Return an evaluator that can evaluate properties 
    of this CutGroup */
Evaluator CutGroup::evaluate() const
{
    return Evaluator(*this);
}

/** Return an editor that can edit this CutGroup */
Editor<CutGroup> CutGroup::edit() const
{
    return Editor<CutGroup>(*this);
}

/** Return a selector that can change the selection of CutGroups */
Selector<CutGroup> CutGroup::selection() const
{
    return Selector<CutGroup>(*this);
}

/** Return the number of atoms in this CutGroup */
int CutGroup::nAtoms() const
{
    return d->info().nAtoms(cgidx);
}

/** Return the indicies of the atoms in this CutGroup, in the
    order they appear in this CutGroup */
const QList<AtomIdx>& CutGroup::atomIdxs() const
{
    return d->info().getAtomsIn(cgidx);
}

/** Return whether or not this CutGroup contains all of the 
    atoms identified by the ID 'atomid'
*/
bool CutGroup::contains(const AtomID &atomid) const
{
    return d->info().containsAll(cgidx, atomid);
}

/** Return whether or not this CutGroup contains the atom 
    at index 'atomidx' in the molecule */
bool CutGroup::contains(AtomIdx atomidx) const
{
    return d->info().contains(cgidx, atomidx);
}

/** Return whether or not this CutGroup contains all of 
    the atoms that match the ID 'atomid' */
bool CutGroup::containsAll(const AtomID &atomid) const
{
    return d->info().containsAll(cgidx, atomid);
}

/** Return whether or not this CutGroup contains some of
    the atoms that match the ID 'atomid' */
bool CutGroup::containsSome(const AtomID &atomid) const
{
    return d->info().containsSome(cgidx, atomid);
}

/** Return the molecule that contains this CutGroup */
Molecule CutGroup::molecule() const
{
    return Molecule(this->data());
}

/** Return the atom in this CutGroup that also has ID 'atomid'

    \throw SireMol::missing_atom
    \throw SireMol::duplicate_atom
    \throw SireError::invalid_index
*/
Atom CutGroup::atom(const AtomID &atomid) const
{
    return Atom( this->data(), cgidx + atomid );
}

/** Return the atoms in this CutGroup that also have the ID 'atomid'

    \throw SireMol::missing_atom
    \throw SireError::invalid_index
*/
AtomsInMol CutGroup::atoms(const AtomID &atomid) const
{
    return AtomsInMol( this->data(), cgidx + atomid );
}

/** Return all of the atoms in this CutGroup */
AtomsInMol CutGroup::atoms() const
{
    return AtomsInMol( this->data(), cgidx.atoms() );
}

/** Return the atom in this CutGroup that also has ID 'atomid'

    \throw SireMol::missing_atom
    \throw SireMol::duplicate_atom
    \throw SireError::invalid_index
*/
Atom CutGroup::select(const AtomID &atomid) const
{
    return this->atom(atomid);
}

/** Return the atoms in this CutGroup that also have the ID 'atomid'

    \throw SireMol::missing_atom
    \throw SireError::invalid_index
*/
AtomsInMol CutGroup::selectAll(const AtomID &atomid) const
{
    return this->atoms(atomid);
}

/** Return all of the atoms in this CutGroup */
AtomsInMol CutGroup::selectAllAtoms() const
{
    return this->atoms();
}
