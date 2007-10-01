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

#ifndef SIREMOL_MOLECULEBONDS_H
#define SIREMOL_MOLECULEBONDS_H

#include <QHash>
#include <QSet>

#include "residuebonds.h"

SIRE_BEGIN_HEADER

namespace SireMol
{
class MoleculeBonds;
}

class QDataStream;
QDataStream& operator<<(QDataStream&, const SireMol::MoleculeBonds&);
QDataStream& operator>>(QDataStream&, SireMol::MoleculeBonds&);

namespace SireMol
{

/**
This class contains all of the bonding information about a molecule. It enables
very fast lookup of which atoms are bonded together, which residues are bonded
together etc. This class is used by both EditMol and Molecule to hold the bonding
information, and is the foundation on which the intramolecular move classes are
constructed (as this class is used to split a molecule into parts
based on the bonding).

This class is implicitly shared, so copying is fast.

@author Christopher Woods
*/
class SIREMOL_EXPORT MoleculeBonds
{

friend QDataStream& ::operator<<(QDataStream&, const MoleculeBonds&);
friend QDataStream& ::operator>>(QDataStream&, MoleculeBonds&);

public:
    MoleculeBonds();
    MoleculeBonds(const MoleculeBonds &other);

    ~MoleculeBonds();

   ////////// Operators ////////////////////////////////
    bool operator==(const MoleculeBonds &other) const;
    bool operator!=(const MoleculeBonds &other) const;
   /////////////////////////////////////////////////////


   ////////// Editing functions ////////////////////////
    void add(const AtomIndex &atom0, const AtomIndex &atom1);
    void add(ResNum resnum, const QString &atom0, const QString &atom1);
    void add(const Bond &bond);

    void remove(const AtomIndex &atom0, const AtomIndex &atom1);
    void remove(ResNum resnum, const QString &atom0, const QString &atom1);
    void remove(const Bond &bond);

    void remove(const AtomIndex &atom);
    void remove(ResNum resnum);

    void removeIntra(ResNum resnum);
    void removeInter(ResNum resnum);

    void removeIntra(const AtomIndex &atom);
    void removeInter(const AtomIndex &atom);

    void removeAll();
    void removeAllIntra();
    void removeAllInter();

    void renumber(ResNum oldnum, ResNum newnum);

    void clear();
    void finalise();
   ////////////////////////////////////////////////////


   ////////// Querying functions //////////////////////
    bool isEmpty() const;

    ResidueBonds residue(ResNum resnum) const;

    QString toString() const;

    QList<ResidueBonds> bondedResidues(ResNum resnum) const;

    QList<Bond> bonds() const;
    QList<Bond> bonds(const AtomIndex &atom) const;
    QList<Bond> bonds(ResNum resnum) const;

    int nBonds() const;
    int nInterBonds() const;
    int nIntraBonds() const;

    int nResidues() const;

    bool contains(ResNum resnum) const;
    bool contains(const AtomIndex &atom) const;
    bool contains(const Bond &bond) const;

    bool bonded(ResNum resnum0, ResNum resnum1) const;
    bool bonded(const AtomIndex &atom0, const AtomIndex &atom1) const;

    QSet<ResNum> resNumsBondedTo(ResNum resnum) const;
    QSet<AtomIndex> atomsBondedTo(const AtomIndex &atom) const;

    QSet<ResNum> resNums() const;

   ////////////////////////////////////////////////////

private:

    ResidueBonds& getResidue(ResNum resnum);

    /** Hash of all of the ResidueBonds objects that describe the
        bonding in each residue of this molecule - indexed by residue number */
    QHash<ResNum, ResidueBonds> resbnds;

};

}

Q_DECLARE_METATYPE(SireMol::MoleculeBonds)

SIRE_END_HEADER

#endif
