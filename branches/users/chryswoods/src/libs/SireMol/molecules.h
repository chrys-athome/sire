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

#include <boost/tuple/tuple.hpp>

#include "viewsofmol.h"
#include "molnum.h"

SIRE_BEGIN_HEADER

namespace SireMol
{
class Molecules;
}

QDataStream& operator<<(QDataStream&, const SireMol::Molecules&);
QDataStream& operator>>(QDataStream&, SireMol::Molecules&);

namespace SireMol
{

class MolNumViewIdx;

/** This class provides a container for lots of molecules. This
    forms a general purpose molecule container, which is used as the argument
    to functions which expect to be passed lots of molecules or parts
    of molecules. This class holds the Molecules using the
    ViewsOfMol class, thereby allowing multiple arbitrary views of each 
    molecule to be held.

    @author Christopher Woods
*/
class SIREMOL_EXPORT Molecules
{

friend QDataStream& ::operator<<(QDataStream&, const Molecules&);
friend QDataStream& ::operator>>(QDataStream&, Molecules&);

public:

    typedef QHash<MolNum,ViewsOfMol>::const_iterator const_iterator;
    typedef QHash<MolNum,ViewsOfMol>::iterator iterator;

    Molecules();

    Molecules(const MoleculeView &molecule);
    Molecules(const ViewsOfMol &molviews);

    template<class T>
    Molecules(const QList<T> &molecules);

    template<class T>
    Molecules(const QVector<T> &molecules);

    Molecules(const Molecules &other);

    ~Molecules();

    static const char* typeName()
    {
        return QMetaType::typeName( qMetaTypeId<Molecules>() );
    }

    const char* what() const
    {
        return Molecules::typeName();
    }

    Molecules* clone() const
    {
        return new Molecules(*this);
    }

    Molecules& operator=(const Molecules &other);
    
    bool operator==(const Molecules &other) const;
    bool operator!=(const Molecules &other) const;
    
    const ViewsOfMol& operator[](MolNum molnum) const;
    PartialMolecule operator[](const boost::tuple<MolNum,SireID::Index> &viewidx) const;
    
    const ViewsOfMol& at(MolNum molnum) const;
    PartialMolecule at(const boost::tuple<MolNum,SireID::Index> &viewidx) const;

    PartialMolecule at(MolNum molnum, SireID::Index idx) const;

    Molecules operator+(const Molecules &other) const;
    Molecules operator-(const Molecules &other) const;

    const ViewsOfMol& molecule(MolNum molnum) const;

    Molecules add(const MoleculeView &molview) const;
    Molecules add(const ViewsOfMol &molviews) const;
    Molecules add(const Molecules &molecules) const;

    Molecules unite(const MoleculeView &molview) const;
    Molecules unite(const ViewsOfMol &molviews) const;
    Molecules unite(const Molecules &other) const;
    
    Molecules remove(const MoleculeView &molview) const;
    Molecules remove(const ViewsOfMol &molviews) const;
    Molecules remove(const Molecules &molecules) const;
    
    Molecules remove(MolNum molnum) const;
    
    Molecules removeAll() const;
    
    Molecules subtract(const MoleculeView &molview) const;
    Molecules subtract(const ViewsOfMol &molviews) const;
    Molecules subtract(const Molecules &molecules) const;

    Molecules subtract(MolNum molnum) const;

    Molecules uniteViews() const;
    Molecules removeDuplicates() const;

    Molecules update(const MoleculeData &moldata) const;
    Molecules update(const MoleculeView &molview) const;
    Molecules update(const Molecules &molecules) const;

    bool isEmpty() const;

    bool contains(MolNum molnum) const;
    bool contains(const MoleculeView &molview) const;
    bool contains(const ViewsOfMol &molviews) const;
    bool contains(const Molecules &molecules) const;

    bool intersects(const MoleculeView &molview) const;
    bool intersects(const Molecules &other) const;

    int count() const;
    int nMolecules() const;
    int nViews(MolNum molnum) const;

    const_iterator begin() const;
    const_iterator end() const;

    const_iterator constBegin() const;
    const_iterator constEnd() const;

    const_iterator find(MolNum molnum) const;
    const_iterator constFind(MolNum molnum) const;

    QSet<MolNum> molNums() const;

    void assertContains(MolNum molnum) const;

private:
    template<class T>
    static Molecules from(const T &molecules);

    void _pvt_add(const MoleculeView &molview);
    void _pvt_add(const ViewsOfMol &molviews);
    void _pvt_add(const Molecules &molecules);
    
    void _pvt_sub(const MoleculeView &molview);
    void _pvt_sub(const ViewsOfMol &molviews);
    void _pvt_sub(const Molecules &molecules);

    void _pvt_unite(const MoleculeView &molview);
    void _pvt_unite(const ViewsOfMol &molviews);
    void _pvt_unite(const Molecules &molecules);

    void _pvt_update(const MoleculeData &moldata);

    void _pvt_removeDupes();
    void _pvt_uniteViews();

    /** Hash that contains all of the views of
        all of the molecules, indexed by 
        their molecule number */
    QHash<MolNum,ViewsOfMol> mols;

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
        QHash<MolNum,ViewsOfMol>::iterator mol = mols.mols.find(it->number());
        
        if (mol != mols.mols.end())
        {
            *mol = mol->add(*it);
        }
        else
        {
            mols.mols.insert(it->number(), *it);
        }
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

}

Q_DECLARE_METATYPE(SireMol::Molecules);

SIRE_END_HEADER

#endif
