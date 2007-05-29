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

#ifndef SIREMOVE_MOLMAPPINGFUNCTION_H
#define SIREMOVE_MOLMAPPINGFUNCTION_H

#include "SireVol/mappingfunction.h"

#include "SireMol/moleculegroupid.h"

SIRE_BEGIN_HEADER

namespace SireMove
{
class MolMappingFunctionBase;
class MolMappingFunction;
class MapAsMolecules;
class MapAsCutGroups;
}

QDataStream& operator<<(QDataStream&, const SireMove::MolMappingFunctionBase&);
QDataStream& operator>>(QDataStream&, SireMove::MolMappingFunctionBase&);

QDataStream& operator<<(QDataStream&, const SireMove::MolMappingFunction&);
QDataStream& operator>>(QDataStream&, SireMove::MolMappingFunction&);

QDataStream& operator<<(QDataStream&, const SireMove::MapAsMolecules&);
QDataStream& operator>>(QDataStream&, SireMove::MapAsMolecules&);

QDataStream& operator<<(QDataStream&, const SireMove::MapAsCutGroups&);
QDataStream& operator>>(QDataStream&, SireMove::MapAsCutGroups&);

namespace SireMol
{
class MoleculeGroup;
class MoleculeGroups;
class Molecules;
}

namespace SireVol
{
class Space;
}

namespace SireSystem
{
class QuerySystem;
}

namespace SireMove
{

using SireMol::MoleculeGroup;
using SireMol::MoleculeGroups;
using SireMol::Molecules;
using SireMol::MoleculeGroupID;

using SireVol::MappingFunction;
using SireVol::Space;

using SireSystem::QuerySystem;

/** This is the base class of functions that map molecules from one space
    to another.

    @author Christopher Woods
*/
class SIREMOVE_EXPORT MolMappingFunctionBase : public QSharedData
{

friend QDataStream& ::operator<<(QDataStream&, const MolMappingFunctionBase&);
friend QDataStream& ::operator>>(QDataStream&, MolMappingFunctionBase&);

public:
    MolMappingFunctionBase(const MappingFunction &mappingfunction);

    MolMappingFunctionBase(const MolMappingFunctionBase &other);

    virtual ~MolMappingFunctionBase();

    Molecules operator()(QuerySystem &system,
                         const Space &new_space) const;

    static const char* typeName()
    {
        return "SireMove::MolMappingFunction";
    }

    virtual const char* what() const;

    virtual MolMappingFunctionBase* clone() const;

    virtual Molecules map(QuerySystem &system,
                          const Space &new_space) const=0;

protected:
    /** The mapping function used to map CoordGroups between spaces */
    MappingFunction mapfunc;
};

/** This function maps each molecule as a single unit - this
    means that even multi-cutgroup molecules are mapped as a
    single unit.

    @author Christopher Woods
*/
class SIREMOVE_EXPORT MapAsMolecules : public MolMappingFunctionBase
{

friend QDataStream& ::operator<<(QDataStream&, const MapAsMolecules&);
friend QDataStream& ::operator>>(QDataStream&, MapAsMolecules&);

public:
    MapAsMolecules();

    MapAsMolecules(const MoleculeGroup &molgroup,
                        const MappingFunction &mappingfunction = MappingFunction());

    MapAsMolecules(const QList<MoleculeGroup> &molgroups,
                        const MappingFunction &mappingfunction = MappingFunction());

    MapAsMolecules(const MoleculeGroups &molgroups,
                        const MappingFunction &mappingfunction = MappingFunction());

    MapAsMolecules(const MapAsMolecules &other);

    ~MapAsMolecules();

    static const char* typeName()
    {
        return "SireMove::MapAsMolecules";
    }

    const char* what() const
    {
        return MapAsMolecules::typeName();
    }

    MapAsMolecules* clone() const
    {
        return new MapAsMolecules(*this);
    }

    Molecules map(QuerySystem &system, const Space &new_space) const;

private:
    /** The IDs of the molecule groups that have to be mapped */
    QSet<MoleculeGroupID> groupids;
};

/** This function maps each cutgroup within each molecule as
    separate units. This will change the internal geometry
    of multi-cutgroup molecules.

    @author Christopher Woods
*/
class SIREMOVE_EXPORT MapAsCutGroups : public MolMappingFunctionBase
{

friend QDataStream& ::operator<<(QDataStream&, const MapAsCutGroups&);
friend QDataStream& ::operator>>(QDataStream&, MapAsCutGroups&);

public:
    MapAsCutGroups();

    MapAsCutGroups(const MoleculeGroup &molgroup,
                        const MappingFunction &mappingfunction = MappingFunction());

    MapAsCutGroups(const QList<MoleculeGroup> &molgroups,
                        const MappingFunction &mappingfunction = MappingFunction());

    MapAsCutGroups(const MoleculeGroups &molgroups,
                        const MappingFunction &mappingfunction = MappingFunction());

    MapAsCutGroups(const MapAsCutGroups &other);

    ~MapAsCutGroups();

    static const char* typeName()
    {
        return "SireMove::MapAsCutGroups";
    }

    const char* what() const
    {
        return MapAsCutGroups::typeName();
    }

    MapAsCutGroups* clone() const
    {
        return new MapAsCutGroups(*this);
    }

    Molecules map(QuerySystem &system, const Space &new_space) const;

private:
    /** The IDs of the molecule groups that have to be mapped */
    QSet<MoleculeGroupID> groupids;
};

/** This is the holder for the complete hierarchy of MolMappingFunctions

    @author Christopher Woods
*/
class SIREMOVE_EXPORT MolMappingFunction
{

friend QDataStream& ::operator<<(QDataStream&, const MolMappingFunction&);
friend QDataStream& ::operator>>(QDataStream&, MolMappingFunction&);

public:
    MolMappingFunction();

    MolMappingFunction(const MolMappingFunctionBase &mapfunc);

    MolMappingFunction(const MolMappingFunction &other);

    ~MolMappingFunction();

    MolMappingFunction& operator=(const MolMappingFunctionBase &mapfunc);
    MolMappingFunction& operator=(const MolMappingFunction &other);

    Molecules operator()(QuerySystem &system,
                         const Space &new_space) const;

    const char* what() const;

    const MolMappingFunctionBase& base() const;

    Molecules map(QuerySystem &system, const Space &new_space) const;

    template<class T>
    bool isA() const
    {
        return d->isA<T>();
    }

    template<class T>
    const T& asA() const
    {
        return d->asA<T>();
    }

private:
    /** Implicitly shared pointer to the actual mapping function */
    SireBase::SharedPolyPointer<MolMappingFunctionBase> d;
};

}

Q_DECLARE_METATYPE(SireMove::MapAsMolecules);
Q_DECLARE_METATYPE(SireMove::MapAsCutGroups);
Q_DECLARE_METATYPE(SireMove::MolMappingFunction);

SIRE_END_HEADER

#endif
