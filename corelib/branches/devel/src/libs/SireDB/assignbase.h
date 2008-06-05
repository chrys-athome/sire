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

#ifndef SIREDB_ASSIGNBASE_H
#define SIREDB_ASSIGNBASE_H

#include <QStringList>

#include "assigninstruction.h"
#include "matchmrdata.h"

SIRE_BEGIN_HEADER

namespace SireMol
{
class Molecule;
}

namespace SireDB
{
class AssignBase;
}

class QDataStream;
QDataStream& operator<<(QDataStream&, const SireDB::AssignBase&);
QDataStream& operator>>(QDataStream&, SireDB::AssignBase&);

namespace SireDB
{

class ParameterDB;
class ParameterTable;

class using_database;
class using_relationships_base;
class using_parameters_base;

using SireMol::Molecule;

/**
This is the base class of all of the 'assign' types, e.g. assign_atoms in SireDB, and assign_bonds, assign_angles and assign_dihedrals in SireMM.

@author Christopher Woods
*/
class SIREDB_EXPORT AssignBase : public AssignInstruction
{

friend QDataStream& ::operator<<(QDataStream&, const AssignBase&);
friend QDataStream& ::operator>>(QDataStream&, AssignBase&);

public:
    AssignBase();

    AssignBase(const AssignBase &other);

    ~AssignBase();

    virtual void assignParameters( const Molecule &molecule,
                                   ParameterTable &param_table,
                                   ParameterDB &database,
                                   const MatchMRData &matchmr = MatchMRData() ) const=0;

    virtual AssignBase* clone() const=0;

    QStringList parameterDataBases() const;
    QStringList relationshipDataBases() const;

    void addDataBase(const using_relationships_base &relationshipdbs);
    void addDataBase(const using_parameters_base &parameterdbs);
    void addDataBase(const using_database &dbs);

    bool overwriteParameters() const;

    void setOverwriteParameters(bool flag);

private:
    void addParameterDataBase(QString db_name);
    void addRelationshipDataBase(QString db_name);

    /** The list of database classes to use to assign these parameters */
    QStringList paramdbs;

    /** The list of database classes used to get the relationships
        between the parts of the molecule and the actual parameters */
    QStringList relatedbs;

    /** Whether or not to overwrite */
    bool _overwrite;
};

}

SIRE_END_HEADER

#endif
