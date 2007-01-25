/********************************************\
  *
  *  Sire - Molecular Simulation Framework
  *
  *  Copyright (C) 2006  Christopher Woods
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

#ifndef SIREMOL_ATOMINFOGROUP_H
#define SIREMOL_ATOMINFOGROUP_H

#include "atominfo.h"

SIRE_BEGIN_HEADER

namespace SireMol
{
class AtomInfoGroup;
}

QDataStream& operator<<(QDataStream&, const SireMol::AtomInfoGroup&);
QDataStream& operator>>(QDataStream&, SireMol::AtomInfoGroup&);

namespace SireMol
{

class Atom;
class AtomID;

/**
This class holds the group of AtomInfo objects of Atoms in a CutGroup. This class is designed to be used with CutGroup and CoordGroup, and allows the data of a CutGroup to be split into two; the coordinates go to CutGroup while the remaining meta information goes to AtomInfoGroup.

@author Christopher Woods
*/
class SIREMOL_EXPORT AtomInfoGroup
{

friend QDataStream& ::operator<<(QDataStream&, const AtomInfoGroup&);
friend QDataStream& ::operator>>(QDataStream&, AtomInfoGroup&);

public:
    AtomInfoGroup();
    AtomInfoGroup(int size);
    AtomInfoGroup(int size, const AtomInfo &value);
    AtomInfoGroup(const QVector<AtomInfo> &atoms);
    AtomInfoGroup(const QVector<Atom> &atoms);

    AtomInfoGroup(const AtomInfoGroup &other);

    ~AtomInfoGroup();

   ////// Operators ////////////////////////////////////////
    AtomInfoGroup& operator=(const AtomInfoGroup &other);
    AtomInfoGroup& operator=(const QVector<AtomInfo> &other);

    bool operator==(const AtomInfoGroup &other) const;
    bool operator!=(const AtomInfoGroup &other) const;

    const AtomInfo& operator[](AtomID i) const;
   /////////////////////////////////////////////////////////

   ///// Querying the group ////////////////////////////////
    const AtomInfo& at(AtomID i) const;

    const AtomInfo& atom(AtomID i) const;

    AtomID indexOf(const AtomIndex &atom) const;

    QVector<AtomInfo> atoms() const;

    QHash<AtomID,AtomInfo> atoms( const QSet<AtomID> &idxs ) const;

    QString toString() const;

    int nAtoms() const;
    int nAtoms(ResNum resnum) const;

    bool contains(AtomID atomid) const;

    bool isEmpty() const;

    const AtomInfo* constData() const;

    QVector<ResNum> residueNumbers() const;

    void assertAtomExists(AtomID atomid) const;
   /////////////////////////////////////////////////////////

private:
    /** Vector of the AtomInfo data */
    QVector<AtomInfo> atms;
};

}

SIRE_END_HEADER

Q_DECLARE_METATYPE(SireMol::AtomInfoGroup)

#endif
