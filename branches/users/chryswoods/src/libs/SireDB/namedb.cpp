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

#include "namedb.h"

#include "SireDB/errors.h"

#include <QSqlQuery>
#include <QDebug>

#include "SireStream/datastream.h"

using namespace SireStream;
using namespace SireDB;
using namespace SireStream;

Q_DECLARE_METATYPE(QSet<QString>);
static const RegisterMetaType< QSet<QString> > r_qset_qstring;

static const RegisterMetaType<NameDB> r_namedb(NO_STREAM);

const QString moleculeNames = "moleculeNames";
const QString residueNames = "residueNames";
const QString atomNames = "atomNames";

/** Constructor */
NameDB::NameDB() : DBBase(), nnames(0)
{}

/** Copy constructor */
NameDB::NameDB(const NameDB &other) : DBBase(other), nnames(0)
{}

/** Destructor */
NameDB::~NameDB()
{}

/** Return a new, unique, name ID number */
NameID NameDB::getNewNameID()
{
    nnames++;
    return nnames;
}

/** Create the tables used by the NameDB */
void NameDB::initialise()
{
    DBBase::initialise();

    //reset the number of assigned names
    nnames = 0;

    //only table is the table mapping name types to table names
    QString error = executeSQL(
          "create table 'SireDB_NameDB_tables' "
                       "( Type TEXT, TableName TEXT, PRIMARY KEY(Type) )" );

    if (not error.isNull())
        throw SireDB::db_error(error,CODELOC);
}

/** Save the number of assigned names to the global parameters table */
void NameDB::prepareToDump()
{
    DBBase::prepareToDump();

    saveParameter<NameDB>( "version", 1 );

    saveParameter<NameDB>( "nnames", nnames );
    saveParameter<NameDB>( "is_case_sensitive", QVariant::fromValue(is_case_sensitive) );
}

/** Read the number of assigned names from the global parameters table */
void NameDB::postLoad()
{
    DBBase::postLoad();

    int v = loadParameter<NameDB>("version").toInt();

    if ( v == 1 )
    {
        nnames = loadParameter<NameDB>( "nnames" ).toUInt();
        is_case_sensitive = loadParameter<NameDB>( "is_case_sensitive" )
                                          .value< QSet<QString> >();
    }
    else
        throw version_error( v, "1", "SireDB::NameDB", CODELOC );
}

/** Return the name of the table for the type of names 'type' */
QString NameDB::getTable(const QString &type)
{
    QString *tablename = tablecache.object(type);
    if (tablename)
        return *tablename;
    else
    {
        //search for this table in the list of tables...
        QString format_type = formatField(database(), type);

        QSqlQuery q(database());
        q.exec( QString("select TableName from 'SireDB_NameDB_tables' where Type = %1")
                      .arg(format_type) );
        checkErrors(q, CODELOC);

        q.next();
        if (q.isValid())
        {
            tablename = new QString(q.value(0).toString());
        }
        else
        {
            QString name = formatField( database(), QString("SireDB_NameDB_%1")
                                .arg(type.simplified().replace(" ","")) )
                                      .replace("'","");

            //create the new table
            q.exec( QString("create table '%1' ( NameID INTEGER, Name Text, "
                                                "PRIMARY KEY(Name) )")
                            .arg(name) );
            checkErrors(q, CODELOC);

            //insert this entry into the database
            q.exec( QString("insert into 'SireDB_NameDB_tables' values ( %1, '%2' )")
                            .arg(format_type, name) );
            checkErrors(q, CODELOC);

            tablename = new QString(name);
        }

        //store the name in the cache
        tablecache.insert(type, tablename);

        return *tablename;
    }
}

/** Get the ID number of the name 'name' in the table 'table'. If 'add' is true, then
    add the name if it doesn't already exist */
NameID NameDB::getName(const QString &type, const QString &name, bool add)
{
    if (name.isEmpty() or type.isEmpty())
        return 0;

    //see if the ID number for this name is in the cache
    NameID *cache_id = namecache[ tuple<QString,QString>(type,name) ];
    if (cache_id)
    {
        if (cache_id != 0)
            return *cache_id;
        else if (not add)
            return 0;
    }

    //format the name so that it can be used in the database
    QString format_name = formatField(database(),name);
    QString tablename = getTable(type);

    //try to get an exact match
    QSqlQuery q(database());
    q.exec( QString("select NameID from '%1' where Name = %2")
                              .arg(tablename,format_name) );
    checkErrors(q,CODELOC);

    NameID ret = 0;

    q.next();
    if (q.isValid())
    {
        //we have found an exact match - return this ID number
        ret = q.value(0).toUInt();
    }
    else if ( is_case_sensitive.contains(type) )
    {
        if (add)
        {
            //create a new ID number and insert that into the database
            ret = getNewNameID();

            q.exec( QString("insert into '%1' values ( %2, %3 )")
                    .arg(tablename).arg(ret).arg(format_name) );
            checkErrors(q,CODELOC);
        }
    }
    else
    {
        //see if there is a case-insensitive match
        q.exec( QString("select NameID from '%1' where Name like %2")
                    .arg(tablename,format_name) );
        checkErrors(q,CODELOC);

        q.next();
        if (q.isValid())
        {
            //we have found a match - get the ID number
            ret = q.value(0).toUInt();
        }
        else if (add)
        {
            //this is a new name
            ret = getNewNameID();

            //insert this ID number into the database
            q.exec( QString("insert into '%1' values ( %2, %3 )")
                    .arg(tablename).arg(ret).arg(format_name) );
            checkErrors(q,CODELOC);
        }
    }

    if (ret != 0)
        //save the number in the cache
        namecache.insert( tuple<QString,QString>(type,name), new NameID(ret) );

    return ret;
}

/** Add the name 'name' to the table 'table', and return the NameID that is assigned.
    Note that if the name is already in the table then the NameID of the existing
    entry is returned. If 'case_sensitive' is true, then a case-sensitive
    search is performed */
NameID NameDB::addName(const QString &type, const QString &name)
{
    //get the name, adding it to the database if it doesn't already exist
    return getName(type, name, true);
}

/** Retrieve the NameID of 'name' in table 'tablename', or 0 if there is no such name in
    that table. If 'case_sensitive' is true then perform a case-sensitive search. */
NameID NameDB::getNameID(const QString &type, const QString &name)
{
    //get the name, but don't add it if it doesn't exist in the database
    return getName(type, name, false);
}

/** Retrieve the name associated with the NameID 'id' - or return a null string if there
    is no name with this ID */
QString NameDB::getName(const QString &type, NameID id)
{
    if (id == 0)
        return QString::null;
    else
    {
        QSqlQuery q(database());

        QString tablename = getTable(type);

        q.exec( QString("select Name from 'SireDB_NameDB_%1' where NameID = %2")
                    .arg(tablename,id) );
        checkErrors(q,CODELOC);

        q.next();
        if (q.isValid())
            //only return the name of the first hit
            return q.value(0).toString();
        else
            //couldn't find the name
            return QString::null;
    }
}

/** Set the type of names 'type' to use case-sensitive matching */
void NameDB::setCaseSensitive(const QString &type)
{
    is_case_sensitive.insert(type);
}

/** Set the type of names 'type' to use case-insensitive matching */
void NameDB::setCaseInsensitive(const QString &type)
{
    is_case_sensitive.remove(type);
}

/** Return whether the name matching of the type of names 'type' is
    case-sensitive */
bool NameDB::caseSensitive(const QString &type) const
{
    return is_case_sensitive.contains(type);
}

/** Set molecule name matching to be case-sensitive */
void NameDB::setCaseSensitiveMoleculeNames()
{
    setCaseSensitive( moleculeNames );
}

/** Set residue name matching to be case-sensitive */
void NameDB::setCaseSensitiveResidueNames()
{
    setCaseSensitive( residueNames );
}

/** Set atotm name matching to be case-sensitive */
void NameDB::setCaseSensitiveAtomNames()
{
    setCaseSensitive( atomNames );
}

/** Set molecule name matching to be case-insensitive */
void NameDB::setCaseInsensitiveMoleculeNames()
{
    setCaseInsensitive( moleculeNames );
}

/** Set residue name matching to be case-insensitive */
void NameDB::setCaseInsensitiveResidueNames()
{
    setCaseInsensitive( residueNames );
}

/** Set atotm name matching to be case-insensitive */
void NameDB::setCaseInsensitiveAtomNames()
{
    setCaseInsensitive( atomNames );
}

/** Return whether or not molecule name matching is case-sensitive */
bool NameDB::caseSensitiveMoleculeNames() const
{
    return caseSensitive( moleculeNames );
}

/** Return whether or not residue name matching is case-sensitive */
bool NameDB::caseSensitiveResidueNames() const
{
    return caseSensitive( residueNames );
}

/** Return whether or not atom name matching is case-sensitive */
bool NameDB::caseSensitiveAtomNames() const
{
    return caseSensitive( atomNames );
}

/** Add the molecule name 'name' to the database and return its ID number.
    If the name is already in the database then the ID number of the existing
    entry is returned. */
NameID NameDB::addMoleculeName(const QString &name)
{
    return addName( moleculeNames, name );
}

/** Add the residue name 'name' to the database and return its ID number.
    If the name is already in the database then the ID number of the existing
    entry is returned. */
NameID NameDB::addResidueName(const QString &name)
{
    return addName( residueNames, name );
}

/** Add the atom name 'name' to the database and return its ID number.
    If the name is already in the database then the ID number of the existing
    entry is returned. */
NameID NameDB::addAtomName(const QString &name)
{
    return addName( atomNames, name );
}

/** Return the ID of the molecule name 'name', or 0 if it is not in the database */
NameID NameDB::getMoleculeNameID(const QString &name)
{
    return getNameID( moleculeNames, name );
}

/** Return the ID of the residue name 'name', or 0 if it is not in the database */
NameID NameDB::getResidueNameID(const QString &name)
{
    return getNameID( residueNames, name );
}

/** Return the ID of the atom name 'name', or 0 if it is not in the database */
NameID NameDB::getAtomNameID(const QString &name)
{
    return getNameID( atomNames, name );
}

/** Return the name of the molecule with NameID 'id' */
QString NameDB::getMoleculeName(NameID id)
{
    return getName( moleculeNames, id );
}

/** Return the name of the residue with NameID 'id' */
QString NameDB::getResidueName(NameID id)
{
    return getName( residueNames, id );
}

/** Return the name of the atom with NameID 'id' */
QString NameDB::getAtomName(NameID id)
{
    return getName( atomNames, id );
}

/** Return the number of names that have been loaded into the database */
int NameDB::nNames() const
{
    return nnames;
}
