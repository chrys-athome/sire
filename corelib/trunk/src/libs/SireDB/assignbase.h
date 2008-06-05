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

    virtual void assignParameters( ParameterTable &param_table, 
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
