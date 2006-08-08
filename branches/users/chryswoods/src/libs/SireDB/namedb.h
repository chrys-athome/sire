#ifndef SIREDB_NAMEDB_H
#define SIREDB_NAMEDB_H

#include <QHash>
#include <QCache>

#include <boost/tuple/tuple.hpp>

#include "dbbase.h"
#include "nameidset.h"

SIRE_BEGIN_HEADER

inline uint qHash(const boost::tuple<QString,QString> &strings)
{
    return ( qHash( strings.get<0>() ) << 16 ) | ( qHash( strings.get<1>() )  & 0x0000FFFF );
}

inline bool operator==(const boost::tuple<QString,QString> &pair0,
                       const boost::tuple<QString,QString> &pair1)
{
    return pair0.get<0>() == pair1.get<0>() and 
           pair0.get<1>() == pair1.get<1>();
}

namespace SireDB
{

using boost::tuple;

/**
This database stores names, giving a unique number for each name in the database. This also has knowledge of the case-sensitivity requirements of the different types of names stored, thus allowing all case-sensitivity matching to be performed in this database, rather than being spread out over the rest of the code. This ensures that there is consistency in the case-sensitivity of names used.

@author Christopher Woods
*/
class SIREDB_EXPORT NameDB : public DBBase
{
public:
    NameDB();
    NameDB(const NameDB &other);
    
    ~NameDB();

    static const char* typeName()
    {
        return "SireDB::NameDB";
    }
    
    const char* what() const
    {
        return NameDB::typeName();
    }

    void setCaseSensitiveMoleculeNames();
    void setCaseSensitiveResidueNames();
    void setCaseSensitiveAtomNames();
    
    void setCaseInsensitiveMoleculeNames();
    void setCaseInsensitiveResidueNames();
    void setCaseInsensitiveAtomNames();
    
    bool caseSensitiveMoleculeNames() const;
    bool caseSensitiveResidueNames() const;
    bool caseSensitiveAtomNames() const;

    NameID addMoleculeName(const QString &name);
    NameID addResidueName(const QString &name);
    NameID addAtomName(const QString &name);
    
    NameID getMoleculeNameID(const QString &name);
    NameID getResidueNameID(const QString &name);
    NameID getAtomNameID(const QString &name);

    QString getMoleculeName(NameID id);
    QString getResidueName(NameID id);
    QString getAtomName(NameID id);

    void setCaseSensitive(const QString &type);
    void setCaseInsensitive(const QString &type);
    bool caseSensitive(const QString &type) const;
    
    NameID addName(const QString &type, const QString &name);
    NameID getNameID(const QString &type, const QString &name);
    QString getName(const QString &type, NameID id);

    int nNames() const;

protected:
    void initialise();
    void prepareToDump();
    void postLoad();
    
    NameID getNewNameID();

private:
    NameID getName(const QString &type, const QString &name, bool add);

    QString getTable(const QString &type);

    /** Set containing all of the name types whose name
        matching is case-sensitive. */
    QSet<QString> is_case_sensitive;

    /** Cache of all tables that have been created in the database */
    QCache<QString, QString> tablecache;

    /** Cache allowing rapid lookup of recently accessed names */
    QCache<tuple<QString,QString>, NameID> namecache;
    
    /** The number of names matched */
    NameID nnames;
};

}

Q_DECLARE_METATYPE(SireDB::NameDB)

SIRE_END_HEADER

#endif
