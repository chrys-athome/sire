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

#ifndef SIREMOL_SELECTIONFROMMOL_H
#define SIREMOL_SELECTIONFROMMOL_H

#include <QSet>

#include "idmolatom.h"

SIRE_BEGIN_HEADER

namespace SireMol
{

class ResNum;
class ResID;
class CutGroupID;
class CutGroupNum;

class AtomSelection;

/** This is a simple class, like IDMolAtom, which is used to simplify
    the API of functions that accept atom selections. This class accepts
    lots of different selection syntaxes, and can be used by AtomSelection
    to interpret those syntaxes into full AtomSelection objects.

    This class is not designed to be used with anything other than
    AtomSelection!

    @author Christopher Woods
*/
class SIREMOL_EXPORT SelectionFromMol
{
public:
    SelectionFromMol();
    SelectionFromMol(ResNum resnum);
    SelectionFromMol(ResID resid);
    SelectionFromMol(CutGroupID cgid);
    SelectionFromMol(CutGroupNum cgnum);

    SelectionFromMol(const IDMolAtom &atomid);

    SelectionFromMol(const AtomSelection &selection);

    SelectionFromMol(const QSet<ResNum> &resnums);
    SelectionFromMol(const QSet<ResID> &resids);
    SelectionFromMol(const QSet<CutGroupID> &cgids);
    SelectionFromMol(const QSet<CutGroupNum> &cgnums);

    SelectionFromMol(const QList<IDMolAtom> &atomids);

    SelectionFromMol(const QList<AtomSelection> &selections);
    SelectionFromMol(const QList<SelectionFromMol> &selections);

    SelectionFromMol(const SelectionFromMol &other);

    SelectionFromMol& operator=(const SelectionFromMol &other);

    AtomSelection selectAllFrom(const AtomSelection &selection) const;
    AtomSelection deselectAllIn(const AtomSelection &selection) const;

private:

    template<class T>
    AtomSelection selectAll(const AtomSelection &selection,
                            const QSet<T> &things) const;

    template<class T>
    AtomSelection selectAll(const AtomSelection &selection,
                            const QList<T> &things) const;

    template<class T>
    AtomSelection deselectAll(const AtomSelection &selection,
                            const QSet<T> &things) const;

    template<class T>
    AtomSelection deselectAll(const AtomSelection &selection,
                            const QList<T> &things) const;

    /** The different argument types... */
    enum Type { EMPTY = 0,
                RESNUM = 1,
                RESID = 2,
                CUTGROUPID = 3,
                CUTGROUPNUM = 4,
                IDMOLATOM = 5,
                ATOMSELECTION = 6,

                QSET_RESNUM = 100,
                QSET_RESID = 101,
                QSET_CUTGROUPID = 102,
                QSET_CUTGROUPNUM = 103,

                QLIST_THIS = 200,
                QLIST_IDMOLATOM = 201,
                QLIST_ATOMSELECTION = 202,
              };

    Type type;

    /** Pointer to the things to be selected */
    const void *things;
};

}

SIRE_END_HEADER

#endif
