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

#include "selectionfrommol.h"
#include "moleculeid.h"
#include "idmolatom.h"

#include "resnum.h"
#include "resid.h"
#include "cutgroupnum.h"
#include "cutgroupid.h"

#include "atomselection.h"

using namespace SireMol;

SelectionFromMol::SelectionFromMol() : type(SelectionFromMol::EMPTY), things(0)
{}

SelectionFromMol::SelectionFromMol(ResNum resnum)
                 : type(SelectionFromMol::RESNUM), things(&resnum)
{}

SelectionFromMol::SelectionFromMol(ResID resid)
                 : type(SelectionFromMol::RESID), things(&resid)
{}

SelectionFromMol::SelectionFromMol(CutGroupID cgid)
                 : type(SelectionFromMol::CUTGROUPID), things(&cgid)
{}

SelectionFromMol::SelectionFromMol(CutGroupNum cgnum)
                 : type(SelectionFromMol::CUTGROUPNUM), things(&cgnum)
{}

SelectionFromMol::SelectionFromMol(const IDMolAtom &atomid)
                 : type(SelectionFromMol::IDMOLATOM), things(&atomid)
{}

SelectionFromMol::SelectionFromMol( const AtomSelection &selection )
                 : type(SelectionFromMol::ATOMSELECTION), things(&selection)
{}

SelectionFromMol::SelectionFromMol( const QSet<ResNum> &resnums )
                 : type(SelectionFromMol::QSET_RESNUM), things(&resnums)
{}

SelectionFromMol::SelectionFromMol( const QSet<ResID> &resids )
                 : type(SelectionFromMol::QSET_RESID), things(&resids)
{}

SelectionFromMol::SelectionFromMol( const QSet<CutGroupID> &cgids )
                 : type(SelectionFromMol::QSET_CUTGROUPID), things(&cgids)
{}

SelectionFromMol::SelectionFromMol( const QSet<CutGroupNum> &cgnums )
                 : type(SelectionFromMol::QSET_CUTGROUPNUM), things(&cgnums)
{}

SelectionFromMol::SelectionFromMol( const QList<IDMolAtom> &atomids )
                 : type(SelectionFromMol::QLIST_IDMOLATOM), things(&atomids)
{}

SelectionFromMol::SelectionFromMol( const QList<AtomSelection> &selections )
                 : type(SelectionFromMol::QLIST_ATOMSELECTION), things(&selections)
{}

SelectionFromMol::SelectionFromMol( const QList<SelectionFromMol> &selections )
                 : type(SelectionFromMol::QLIST_THIS), things(&selections)
{}

/** Copy constructor */
SelectionFromMol::SelectionFromMol( const SelectionFromMol &other)
                 : type(other.type), things(other.things)
{}

/** Copy assignment operator */
SelectionFromMol& SelectionFromMol::operator=(const SelectionFromMol &other)
{
    type = other.type;
    things = other.things;

    return *this;
}

template<class T>
AtomSelection SelectionFromMol::selectAll(const AtomSelection &selected_atoms,
                                          const QSet<T> &things) const
{
    AtomSelection retval = selected_atoms;

    for (typename QSet<T>::const_iterator it = things.begin();
         it != things.end();
         ++it)
    {
        retval._pvt_select(*it);
    }

    return retval;
}

template<class T>
AtomSelection SelectionFromMol::selectAll(const AtomSelection &selected_atoms,
                                          const QList<T> &things) const
{
    AtomSelection retval = selected_atoms;

    for (typename QList<T>::const_iterator it = things.begin();
         it != things.end();
         ++it)
    {
        retval._pvt_select(*it);
    }

    return retval;
}

template<class T>
AtomSelection SelectionFromMol::deselectAll(const AtomSelection &selected_atoms,
                                            const QSet<T> &things) const
{
    AtomSelection retval = selected_atoms;

    for (typename QSet<T>::const_iterator it = things.begin();
         it != things.end();
         ++it)
    {
        retval._pvt_deselect(*it);
    }

    return retval;
}

template<class T>
AtomSelection SelectionFromMol::deselectAll(const AtomSelection &selected_atoms,
                                            const QList<T> &things) const
{
    AtomSelection retval = selected_atoms;

    for (typename QList<T>::const_iterator it = things.begin();
         it != things.end();
         ++it)
    {
        retval._pvt_deselect(*it);
    }

    return retval;
}

template<class T>
const T& cast(const void *ptr)
{
    //evil - no type checking!
    return *( (const T*)(ptr) );
}

AtomSelection SelectionFromMol::selectAllFrom(const AtomSelection &selected_atoms) const
{
    switch( type )
    {
        case EMPTY:
            break;

        case RESNUM:
            return selected_atoms.selectAll( cast<ResNum>(things) );

        case RESID:
            return selected_atoms.selectAll(
                          selected_atoms.info().residueNumber( cast<ResID>(things) ) );

        case CUTGROUPID:
            return selected_atoms.selectAll( cast<CutGroupID>(things) );

        case CUTGROUPNUM:
            return selected_atoms.selectAll(
                          selected_atoms.info().cutGroupID( cast<CutGroupNum>(things) ) );

        case IDMOLATOM:
            return selected_atoms.select( cast<IDMolAtom>(things) );

        case ATOMSELECTION:
            return selected_atoms.selectAll( cast<AtomSelection>(things) );

        case QSET_RESNUM:
            return this->selectAll( selected_atoms,
                                    cast< QSet<ResNum> >(things) );

        case QSET_RESID:
            return this->selectAll( selected_atoms,
                                    cast< QSet<ResID> >(things) );

        case QSET_CUTGROUPID:
            return this->selectAll( selected_atoms,
                                    cast< QSet<CutGroupID> >(things) );

        case QSET_CUTGROUPNUM:
            return this->selectAll( selected_atoms,
                                    cast< QSet<CutGroupNum> >(things) );

        case QLIST_THIS:
            return this->selectAll( selected_atoms,
                                    cast< QList<SelectionFromMol> >(things) );

        case QLIST_IDMOLATOM:
            return this->selectAll( selected_atoms,
                                    cast< QList<IDMolAtom> >(things) );

        case QLIST_ATOMSELECTION:
            return this->selectAll( selected_atoms,
                                    cast< QList<AtomSelection> >(things) );
    }

    return selected_atoms;
}

AtomSelection SelectionFromMol::deselectAllIn(const AtomSelection &selected_atoms) const
{
    switch( type )
    {
        case EMPTY:
            break;

        case RESNUM:
            return selected_atoms.deselectAll( cast<ResNum>(things) );

        case RESID:
            return selected_atoms.deselectAll(
                          selected_atoms.info().residueNumber( cast<ResID>(things) ) );

        case CUTGROUPID:
            return selected_atoms.deselectAll( cast<CutGroupID>(things) );

        case CUTGROUPNUM:
            return selected_atoms.deselectAll(
                          selected_atoms.info().cutGroupID( cast<CutGroupNum>(things) ) );

        case IDMOLATOM:
            return selected_atoms.select( cast<IDMolAtom>(things) );

        case ATOMSELECTION:
            return selected_atoms.deselectAll( cast<AtomSelection>(things) );

        case QSET_RESNUM:
            return this->deselectAll( selected_atoms,
                                    cast< QSet<ResNum> >(things) );

        case QSET_RESID:
            return this->deselectAll( selected_atoms,
                                    cast< QSet<ResID> >(things) );

        case QSET_CUTGROUPID:
            return this->deselectAll( selected_atoms,
                                    cast< QSet<CutGroupID> >(things) );

        case QSET_CUTGROUPNUM:
            return this->deselectAll( selected_atoms,
                                    cast< QSet<CutGroupNum> >(things) );

        case QLIST_THIS:
            return this->deselectAll( selected_atoms,
                                    cast< QList<SelectionFromMol> >(things) );

        case QLIST_IDMOLATOM:
            return this->deselectAll( selected_atoms,
                                    cast< QList<IDMolAtom> >(things) );

        case QLIST_ATOMSELECTION:
            return this->deselectAll( selected_atoms,
                                    cast< QList<AtomSelection> >(things) );
    }

    return selected_atoms;
}
