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
#include "moleculeid.h"

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

    typedef PartialMolecule value_type;

    Molecules();

    Molecules(const PartialMolecule &molecule);
    Molecules(const MolDataView &molecule);

    Molecules(const QHash<MoleculeID,PartialMolecule> &molecules);

    template<class T>
    Molecules(const QList<T> &molecules);

    template<class T>
    Molecules(const QVector<T> &molecules);

    template<class T>
    Molecules(const QSet<T> &molecules);

    template<class K, class T>
    Molecules(const QHash<K,T> &molecules);

    Molecules(const Molecules &other);

    ~Molecules();

    const PartialMolecule& operator[](MoleculeID molid) const;

    Molecules& operator+=(const Molecules &other);
    Molecules& operator-=(const Molecules &other);

    Molecules operator+(const Molecules &other) const;
    Molecules operator-(const Molecules &other) const;

    Molecules unite(const Molecules &other) const;
    Molecules subtract(const Molecules &other) const;
    Molecules intersection(const Molecules &other) const;

    bool intersects(const Molecules &other) const;

    bool isEmpty() const;

    bool contains(MoleculeID molid) const;
    bool contains(const PartialMolecule &molecule) const;

    int count() const;

    const_iterator begin() const;
    const_iterator end() const;

    const_iterator constBegin() const;
    const_iterator constEnd() const;

    QSet<MoleculeID> moleculeIDs() const;

    void reserve(int count);

    bool add(const PartialMolecule &molecule);
    bool append(const PartialMolecule &molecule);

    bool add(const Molecules &molecules);
    bool append(const Molecules &molecules);

    bool change(const PartialMolecule &molecule);
    bool change(const Molecules &molecules);

    bool remove(const PartialMolecule &molecule);
    bool remove(const Molecules &molecules);

    bool remove(MoleculeID molid);
    bool remove(const QSet<MoleculeID> &molids);

    const PartialMolecule& molecule(MoleculeID molid) const;

    void assertContains(MoleculeID molid) const;

private:
    template<class T>
    static Molecules from(const T &molecules);

    /** Hash that contains all of the molecules */
    QHash<MoleculeID,PartialMolecule> mols;

};

template<class T>
SIRE_OUTOFLINE_TEMPLATE
Molecules Molecules::from(const T &molecules)
{
    Molecules mols;

    if (molecules.count() == 0)
        return mols;

    mols.mols.reserve(molecules.count());

    for (typename T::const_iterator it = molecules.begin();
         it != molecules.end();
         ++it)
    {
        mols.mols.insert(it->ID(), *it);
    }

    return mols;
}

/** Converting constructor used to convert from general
    containers of molecules to this container */
template<class T>
SIRE_OUTOFLINE_TEMPLATE
Molecules::Molecules(const QList<T> &molecules)
{
    *this = Molecules::from(molecules);
}

/** Converting constructor used to convert from general
    containers of molecules to this container */
template<class T>
SIRE_OUTOFLINE_TEMPLATE
Molecules::Molecules(const QVector<T> &molecules)
{
    *this = Molecules::from(molecules);
}

/** Converting constructor used to convert from general
    containers of molecules to this container */
template<class T>
SIRE_OUTOFLINE_TEMPLATE
Molecules::Molecules(const QSet<T> &molecules)
{
    *this = Molecules::from(molecules);
}

/** Converting constructor used to convert from general
    containers of molecules to this container */
template<class K, class T>
SIRE_OUTOFLINE_TEMPLATE
Molecules::Molecules(const QHash<K,T> &molecules)
{
    *this = Molecules::from(molecules);
}

/** Return whether or not this container is empty */
inline bool Molecules::isEmpty() const
{
    return mols.isEmpty();
}

/** Return whether this contains at least one atom of any version
    of the molecule with ID == molid */
inline bool Molecules::contains(MoleculeID molid) const
{
    return mols.contains(molid);
}

/** Return the number of molecules in this container */
inline int Molecules::count() const
{
    return mols.count();
}

/** Return an iterator pointing to the first molecule
    in the container */
inline Molecules::const_iterator Molecules::begin() const
{
    return mols.begin();
}

/** Return an iterator pointing one past the last molecule
    in the container */
inline Molecules::const_iterator Molecules::end() const
{
    return mols.end();
}

/** Return an iterator pointing to the first molecule
    in the container */
inline Molecules::const_iterator Molecules::constBegin() const
{
    return mols.constBegin();
}

/** Return an iterator pointing one past the last molecule
    in the container */
inline Molecules::const_iterator Molecules::constEnd() const
{
    return mols.constEnd();
}

/** Return the molecule with ID == molid

    \throw SireMol::missing_molecule
*/
inline const PartialMolecule& Molecules::molecule(MoleculeID molid) const
{
    QHash<MoleculeID,PartialMolecule>::const_iterator it = mols.find(molid);

    if (it == mols.end())
        assertContains(molid);

    return *it;
}

}

Q_DECLARE_METATYPE(SireMol::Molecules);

SIRE_END_HEADER

#endif
