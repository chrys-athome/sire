#ifndef SIREDB_PARAMETERTABLE_H
#define SIREDB_PARAMETERTABLE_H

#include <QString>
#include <QStringList>

#include "tablebase.h"
#include "matchmrdata.h"

#include "SireMol/moleculeinfo.h"
#include "SireBase/sharedpolypointer.hpp"

SIRE_BEGIN_HEADER

namespace SireDB
{
class ParameterTable;
}

class QDataStream;
QDataStream& operator<<(QDataStream&, const SireDB::ParameterTable&);
QDataStream& operator>>(QDataStream&, SireDB::ParameterTable&);

namespace SireMol
{
class Molecule;
}

namespace SireDB
{

class TableBase;
class assign_parameters;
class ParameterDB;

using SireMol::Molecule;
using SireMol::MoleculeInfo;
using SireBase::SharedPolyPointer;

/**
This class holds all of the parameter tables in a single object.

@author Christopher Woods
*/
class SIREDB_EXPORT ParameterTable
{

friend QDataStream& ::operator<<(QDataStream&, const ParameterTable&);
friend QDataStream& ::operator>>(QDataStream&, ParameterTable&);

public:
    ParameterTable();

    ParameterTable(const MoleculeInfo &molinfo);

    ParameterTable(const ParameterTable &other);

    ~ParameterTable();

    bool isEmpty() const;

    QStringList types() const;

    bool isA(const QString &type_name) const;

    template<class T>
    bool isA() const;

    const TableBase& asA(const QString &type_name) const;

    const MoleculeInfo& info() const;

    template<class T>
    const T& asA() const;

    void createTable(const QString &type_name);

    template<class T>
    void createTable();

    void addTable(const TableBase &table);

    void setTable(const TableBase &table);

    void assign( const Molecule &molecule,
                 ParameterDB &db, const assign_parameters &assigners,
                 const MatchMRData &matchmr = MatchMRData() );

    template<class T>
    void removeTable();

    void removeTable(const QString &type_name);

    void assertTableCompatible(const TableBase &table) const;
    void assertCompatibleWith(const Molecule &molecule) const;

private:

    /** The info for the molecule whose parameters are
        held in this table */
    MoleculeInfo molinfo;

    typedef QHash< QString, SharedPolyPointer<TableBase> > hash_type;

    /** All of the tables in this database, indexed by typename */
    hash_type tables;
};

/** Return the molecule whose parameters are stored in this table.
    This will return an empty molecule if this is an empty table */
inline const MoleculeInfo& ParameterTable::info() const
{
    return molinfo;
}

/** Is this a parameter table that contains a table of type 'T' */
template<class T>
bool ParameterTable::isA() const
{
    hash_type::const_iterator it = tables.find( T::typeName() );

    return it != tables.end() and it.value().isA<T>();
}

/** Return this parameter table cast as type 'T'. Note that this
    will have undefined results unless isA<T> returns true.

    Note that the referenced returned is temporary - it can be
    made invalid by a call to 'removeTable' or 'setTable'
*/
template<class T>
const T& ParameterTable::asA() const
{
    return tables.find(T::typeName()).value().asA<T>();
}

/** Create a new table of type 'T' and add it to this table.
    This does nothing if this type of table already exists. */
template<class T>
void ParameterTable::createTable()
{
    //use boost to assert that this is a TableBase type!
    this->createTable( T::typeName() );
}

/** Remove the table 'T' - does nothing if this does not contain
    a table of this type. */
template<class T>
void ParameterTable::removeTable()
{
    if (this->isA<T>())
        tables.remove( T::typeName() );
}

}

Q_DECLARE_METATYPE(SireDB::ParameterTable)

SIRE_END_HEADER

#endif
