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

#ifndef SIREMOL_MOLECULES_H
#define SIREMOL_MOLECULES_H

#include <QHash>

#include "partialmolecule.h"

SIRE_BEGIN_HEADER

namespace SireMol
{
class Molecules;
}

QDataStream& operator<<(QDataStream&, const SireMol::Molecules&);
QDataStream& operator>>(QDataStream&, SireMol::Molecules&);

namespace SireMol
{

/** This class provides a container for lots of molecules. This will eventually
    form the general purpose molecule container, which is used as the argument
    to functions which expect to be passed lots of molecules or parts
    of molecules

    @author Christopher Woods
*/
class SIREMOL_EXPORT Molecules
{

friend QDataStream& ::operator<<(QDataStream&, const Molecules&);
friend QDataStream& ::operator>>(QDataStream&, Molecules&);

public:

    typedef QHash<MoleculeID,PartialMolecule>::iterator iterator;
    typedef QHash<MoleculeID,PartialMolecule>::const_iterator const_iterator;

    Molecules();

    Molecules(const PartialMolecule &molecule)
    {
        mols.insert( molecule.ID(), molecule );
    }

    Molecules(const MolDataView &molecule)
    {
        PartialMolecule mol(molecule);
        mols.insert( mol.ID(), mol );
    }

    Molecules(const QHash<MoleculeID,PartialMolecule> &molecules);

    template<class T>
    Molecules(const T &molecules);

    Molecules(const Molecules &other);

    ~Molecules();

    bool isEmpty() const
    {
        return mols.isEmpty();
    }

    int count() const
    {
        return mols.count();
    }

    const_iterator begin() const
    {
        return mols.begin();
    }

    const_iterator end() const
    {
        return mols.end();
    }

    const_iterator constBegin() const
    {
        return mols.constBegin();
    }

    const_iterator constEnd() const
    {
        return mols.constEnd();
    }

    QSet<MoleculeID> IDs() const;

    void reserve(int count)
    {
        mols.reserve(count);
    }

    void add(const PartialMolecule &molecule);

    void change(const PartialMolecule &molecule);

    void remove(const PartialMolecule &molecule);
    void remove(MoleculeID molid);

    const PartialMolecule& molecule(MoleculeID molid) const
    {
        QHash<MoleculeID,PartialMolecule>::const_iterator it = mols.find(molid);

        if (it == mols.end())
            assertContains(molid);

        return *it;
    }

    void assertContains(MoleculeID molid) const;

private:
    /** Hash that contains all of the molecules */
    QHash<MoleculeID,PartialMolecule> mols;

};

/** Converting constructor used to convert from general
    containers of molecules to this container */
template<class T>
Molecules::Molecules(const T &molecules)
{
    if (molecules.count() == 0)
        return;

    mols.reserve(molecules.count());

    for (typename T::const_iterator it = molecules.begin();
         it != molecules.end();
         ++it)
    {
        mols.insert(it->ID(), *it);
    }
}

}

Q_DECLARE_METATYPE(SireMol::Molecules);

SIRE_END_HEADER

#endif
