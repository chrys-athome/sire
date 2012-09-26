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

#ifndef SIREDB_DATABASE_H
#define SIREDB_DATABASE_H

#include "SireBase/property.h"

#include <boost/shared_ptr.hpp>

SIRE_BEGIN_HEADER

namespace SireDB
{
class Database;
}

QDataStream& operator<<(QDataStream&, const SireDB::Database&);
QDataStream& operator>>(QDataStream&, SireDB::Database&);

namespace SireDB
{

namespace detail
{
/** This internal class provides the API used by the SireDB
    classes to access the underlying sqlite3 database */
class Sqlite3DB;
} // end of namespace detail

/** This class provides an interface to a simple SQL database. 
    The entire database is stored within this object, and,
    as this is a Property, access to the database is thread-safe,
    with copy-on-write semantics
    
    @author Christopher Woods
*/
class SIREDB_EXPORT Database 
        : public SireBase::ConcreteProperty<Database,SireBase::Property>
{

friend QDataStream& ::operator<<(QDataStream&, const Database&);
friend QDataStream& ::operator>>(QDataStream&, Database&);

public:
    Database();
    Database(const QString &sql);
    
    Database(const Database &other);
    
    ~Database();
    
    static const char* typeName();
    
    Database& operator=(const Database &other);
    
    bool operator==(const Database &other) const;
    bool operator!=(const Database &other) const;
    
    QString dumpSql() const;
    
    //void createTable(const QString &table_name, const TableHeader &header);

    //void addRow(const QString &table_name, const TableRow &row);

    //void updateRow(const QString &table_name, const TableRow &row,
    //               const QString &where);

    //TableRow getRow(const QString &table_name, const QString &where) const;

    //TableRow getRow(const QString &table_name,
    //                const QString &column,
    //                const QString &where) const;

    //TableRow getRow(const QString &table_name,
    //                const QStringList &columns,
    //                const QString &where) const;

private:
    /** PIMPL pointer to the underlying Sqlite3 database */
    boost::shared_ptr<detail::Sqlite3DB> db;
};

}

Q_DECLARE_METATYPE( SireDB::Database )

SIRE_EXPOSE_CLASS( SireDB::Database )

SIRE_END_HEADER

#endif
