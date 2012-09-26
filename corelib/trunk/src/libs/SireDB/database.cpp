/********************************************\
  *
  *  Sire - Molecular Simulation Framework
  *
  *  Copyright (C) 2009  Christopher Woods
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

#include "database.h"

#include "ThirdParty/sqlite3.h" // CONDITIONAL_INCLUDE

#include "SireStream/datastream.h"
#include "SireStream/shareddatastream.h"

#include "SireError/errors.h"
#include "SireDB/errors.h"

using namespace SireDB;
using namespace SireDB::detail;
using namespace SireBase;
using namespace SireStream;

//////////////
////////////// Implementation of SqliteDB
//////////////

namespace SireDB
{
namespace detail
{

class Sqlite3DB
{
public:
    Sqlite3DB();
    ~Sqlite3DB();
    
    static Sqlite3DB* createFromSql(const QString &sql);
    
    QString dumpSql() const;
};

} // end of namespace detail
} // end of namespace SireDB

//////////////
////////////// Implementation of Sqlite3DB
//////////////

Sqlite3DB::Sqlite3DB()
{}

Sqlite3DB::~Sqlite3DB()
{}

Sqlite3DB* Sqlite3DB::createFromSql(const QString &sql)
{
    return 0;
}

QString Sqlite3DB::dumpSql() const
{
    return QString();
}

//////////////
////////////// Implementation of Database
//////////////

static const RegisterMetaType<Database> r_db;

/** Serialise to a binary datastream */
QDataStream SIREDB_EXPORT &operator<<(QDataStream &ds, const Database &db)
{
    writeHeader(ds, r_db, 1);
    
    ds << db.dumpSql();
    
    return ds;
}

/** Extract from a binary datastream */
QDataStream SIREDB_EXPORT &operator>>(QDataStream &ds, Database &db)
{
    VersionID v = readHeader(ds, r_db);
    
    if (v == 1)
    {
        QString sql;
        ds >> sql;
        
        db = Database(sql);
    }
    else
        throw version_error(v, "1", r_db, CODELOC);
        
    return ds;
}

/** Construct a null database */
Database::Database() : ConcreteProperty<Database,Property>()
{}

/** Construct a database from the passed SQL string */
Database::Database(const QString &sql)
         : ConcreteProperty<Database,Property>()
{
    db.reset( Sqlite3DB::createFromSql(sql) );
}

/** Copy constructor */
Database::Database(const Database &other)
         : ConcreteProperty<Database,Property>(other), db(other.db)
{}

/** Destructor */
Database::~Database()
{}

const char* Database::typeName()
{
    return QMetaType::typeName( qMetaTypeId<Database>() );
}

/** Copy assignment operator */
Database& Database::operator=(const Database &other)
{
    db = other.db;
    Property::operator=(other);
    
    return *this;
}

/** Comparison operator */
bool Database::operator==(const Database &other) const
{
    return db.get() == other.db.get();
}

/** Comparison operator */
bool Database::operator!=(const Database &other) const
{
    return not Database::operator==(other);
}

/** Dump the entire database out as raw SQL */
QString Database::dumpSql() const
{
    if (db.get() == 0)
        return QString();
    else
        return db->dumpSql();
}
