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

#ifndef SIREDB_TABLEBASE_H
#define SIREDB_TABLEBASE_H

#include <QSharedData>

#include "SireMol/moleculeinfo.h"

SIRE_BEGIN_HEADER

namespace SireDB
{
class TableBase;
}

class QDataStream;
QDataStream& operator<<(QDataStream&, const SireDB::TableBase&);
QDataStream& operator>>(QDataStream&, SireDB::TableBase&);

namespace SireMol
{
class Molecule;
}

namespace SireDB
{

using SireMol::MoleculeInfo;
using SireMol::Molecule;

/**
This is the virtual base class of all of the parameter tables. This class is not useful on its own, but instead provides an interface for the low-level table classes.

@author Christopher Woods
*/
class SIREDB_EXPORT TableBase : public QSharedData
{

friend QDataStream& ::operator<<(QDataStream&, const TableBase&);
friend QDataStream& ::operator>>(QDataStream&, TableBase&);

public:
    TableBase();
    TableBase(const MoleculeInfo &info);

    TableBase(const TableBase &other);

    virtual ~TableBase();

    const MoleculeInfo& info() const;

    bool isCompatibleWith(const MoleculeInfo &molinfo) const;

    virtual TableBase* clone() const=0;
    virtual const char* what() const=0;

    virtual TableBase* create(const MoleculeInfo &molinfo) const=0;

    virtual bool isEmpty() const=0;

    virtual void add(const TableBase &other)=0;

    template<class T>
    bool isA() const
    {
        return dynamic_cast<const T*>(this) != 0;
    }

    template<class T>
    const T& asA() const
    {
        return dynamic_cast<const T&>(*this);
    }

    template<class T>
    T& asA()
    {
        return dynamic_cast<T&>(*this);
    }

    void assertCompatibleWith(const Molecule &molecule) const;

private:
    /** The metadata for the molecule whose parameters
        are in this table */
    MoleculeInfo molinfo;
};

/** Return the metadata for the molecule whose parameters are in this
    table */
inline const MoleculeInfo& TableBase::info() const
{
    return molinfo;
}

}

SIRE_END_HEADER

#endif
