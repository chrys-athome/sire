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

#ifndef SIREDB_DBBASE_H
#define SIREDB_DBBASE_H

#include <QSqlDatabase>
#include <boost/assert.hpp>

#include "relateidmap.h"
#include "sireglobal.h"

class QSqlQuery;

SIRE_BEGIN_HEADER

namespace SireDB
{

class ParameterDB;

/** A DBBase is the base class of all of the parameter database classes.
    This class is used to access the ParameterDB class that must be 
    multipally inherited with the collection of parameter database classes
    that form the complete database.
    
    The parameter database classes are designed to be modular, and to be
    plugged together to form a complete, functional class. The parameter
    database classes cannot be used on their own.
    
    @author Christopher Woods
*/
class SIREDB_EXPORT DBBase
{

friend class ParameterDB;  //friend so that it can pass a pointer to itself

public:
    DBBase();
    DBBase(const DBBase &other);
    
    virtual ~DBBase();

    static const char* typeName()
    {
        return "SireDB::DBBase";
    }

    virtual const char* what() const=0;

    virtual QString toString() const;
    
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

protected:
    void setParent(ParameterDB *parent);
    
    const QString& prefix() const;
    
    virtual void initialise();
    virtual void prepareToDump();
    virtual void postLoad();
    
    ParameterDB& parent();
    const ParameterDB& parent() const;
    
    QSqlDatabase& database();
    void addLog(const QString &logtext);
    void checkErrors(QSqlQuery &q, const QString &throwloc);
    QString executeSQL(const QString &query);
    
    RelateID getNewRelateID();
    ParamID getNewParamID();
    
    template<class T>
    void saveParameter(const QString &name, const QVariant &value);
    
    template<class T>
    QVariant loadParameter(const QString &name);
    
    /** Parent database */
    ParameterDB *prnt;
    
    /** Prefix used to allow this object to put its
        tables into its own namespace */
    QString table_prefix;
};

/** The prefix given to the table name to prevent name clashes */
inline const QString& DBBase::prefix() const
{
    return table_prefix;
}

}

#include "parameterdb.h"

namespace SireDB
{

/** Return a reference to the parent ParameterDB class */
inline ParameterDB& DBBase::parent()
{
    BOOST_ASSERT( prnt != 0 );
    return *prnt;
}

/** Return a reference to the actual database */
inline QSqlDatabase& DBBase::database()
{
    return parent().database();
}

/** Return a reference to the parent database class - All child DBBase classes
    must have a single, valid ParameterDB class. Using a child without a parent
    is a programming error.
    
    \throw SireError::program_bug
*/
inline const ParameterDB& DBBase::parent() const
{
    return const_cast<DBBase*>(this)->parent();
}

/** Convienience function that calls the function on the parent */
inline QString DBBase::executeSQL(const QString &query)
{
    return parent().executeSQL(query);
}

/** Check for any errors, and if detected, add them to the database log. */
inline void DBBase::checkErrors(QSqlQuery &q, const QString &throwloc)
{
    parent().checkErrors(q, throwloc);
}

/** Add a comment to the log */
inline void DBBase::addLog(const QString &logtext)
{
    parent().addLog(logtext);
}

/** Get a new relationship ID number */
inline RelateID DBBase::getNewRelateID()
{
    return parent().getNewRelateID();
}

/** Get a new parameter ID number */
inline ParamID DBBase::getNewParamID()
{
    return parent().getNewParamID();
}

/** Save the parameter called 'name' to the global parameter table. This replaces
    any existing parameter with the same name. Note that name-lookups are
    case-sensitive */
template<class T>
inline void DBBase::saveParameter(const QString &name, const QVariant &value)
{
    parent().saveParameter<T>(name,value);
}

/** Load and return the parameter called 'name' in the global parameter table,
    or return a null QVariant if there is no such parameter. Note that 
    name-lookups are case-sensitive. */
template<class T>
inline QVariant DBBase::loadParameter(const QString &name)
{
    return parent().loadParameter<T>(name);
}

}

SIRE_END_HEADER

#endif
