#ifndef SIREDB_PARAMETERTABLE_H
#define SIREDB_PARAMETERTABLE_H

#include <QString>
#include <QStringList>

#include "tablebase.h"
#include "matchmrdata.h"

#include "SireMol/molecule.h"
#include "SireBase/dynamicsharedptr.hpp"

SIRE_BEGIN_HEADER

namespace SireDB
{
class ParameterTable;
}

class QDataStream;
QDataStream& operator<<(QDataStream&, const SireDB::ParameterTable&);
QDataStream& operator>>(QDataStream&, SireDB::ParameterTable&);

namespace SireDB
{

class TableBase;
class assign_parameters;
class ParameterDB;

using SireMol::Molecule;
using SireBase::DynamicSharedPtr;

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
    
    ParameterTable(const Molecule &molecule);
    
    ParameterTable(const ParameterTable &other);
    
    ~ParameterTable();

    bool isEmpty() const;

    QStringList types() const;

    bool isA(const QString &type_name) const;

    template<class T>
    bool isA() const;
    
    const TableBase& asA(const QString &type_name) const;
    TableBase& asA(const QString &type_name);
    
    const Molecule& molecule() const;
    
    template<class T>
    const T& asA() const;

    template<class T>
    T& asA();

    template<class T>
    T& addTable();
    template<class T>
    T& setTable();
    
    TableBase& addTable(const QString &type_name);
    TableBase& setTable(const QString &set_name);
    
    TableBase& addTable(const TableBase &table);
    TableBase& setTable(const TableBase &table);

    void assign( ParameterDB &db, const assign_parameters &assigners,
                 const MatchMRData &matchmr = MatchMRData() );

    template<class T>
    void removeTable();
    
    void removeTable(const QString &type_name);

    void setMolecule(const Molecule &molecule);
    
private:

    /** The molecule whose parameters are contained in this table */
    Molecule mol;

    typedef QHash< QString, DynamicSharedPtr<TableBase> > hash_type;

    /** All of the tables in this database, indexed by typename */
    hash_type tables;
};

/** Return the molecule whose parameters are stored in this table. 
    This will return an empty molecule if this is an empty table */
inline const Molecule& ParameterTable::molecule() const
{
    return mol;
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

/** Return this parameter table cast as type 'T'. Note that this 
    will have undefined results unless isA<T> returns true. 
    
    Note that the referenced returned is temporary - it can be 
    made invalid by a call to 'removeTable' or 'setTable'
*/
template<class T>
T& ParameterTable::asA()
{
    return tables.find(T::typeName()).value().asA<T>();
}

/** Create the table of type T using the default molecule 
    constructor - T(molecule).
    
    This replaces any existing table of this type.
*/
template<class T>
T& ParameterTable::setTable()
{
    return this->setTable( T(mol) ).asA<T>();
}

/** Add the table T created using the default molecule 
    constructor - T(molecule).
    
    This adds to any existing table of this type.
*/
template<class T>
T& ParameterTable::addTable()
{
    hash_type::iterator it = tables.find( T::typeName() );
    
    if (it != tables.end())
        return it.value()->asA<T>();
    else
        return tables.insert( T::typeName(),
                              DynamicSharedPtr<TableBase>(new T(mol)) ).value()->asA<T>();
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
