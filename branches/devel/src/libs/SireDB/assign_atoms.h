#ifndef SIREDB_ASSIGN_ATOMS_H
#define SIREDB_ASSIGN_ATOMS_H

#include <QSet>

#include "SireMol/atomindex.h"

#include "assignbase.h"

SIRE_BEGIN_HEADER

namespace SireDB
{
class assign_atoms;
}

QDataStream& operator<<(QDataStream&, const SireDB::assign_atoms&);
QDataStream& operator>>(QDataStream&, SireDB::assign_atoms&);

namespace SireDB
{

class using_database;
class MatchMRData;

using SireMol::AtomIndex;

/**
This class is used to hold the instructions necessary to assign atom parameters (together with the actual code necessary to make those assignments)

@author Christopher Woods
*/
class SIREDB_EXPORT assign_atoms : public AssignBase
{

friend QDataStream& ::operator<<(QDataStream&, const assign_atoms&);
friend QDataStream& ::operator>>(QDataStream&, assign_atoms&);

public:
    assign_atoms();

    assign_atoms(const using_database &db0);
    
    assign_atoms(const using_database &db0, const using_database &db1);
    
    assign_atoms(const using_database &db0, const using_database &db1,
                 const using_database &db2);
    
    assign_atoms(const using_database &db0, const using_database &db1,
                 const using_database &db2, const using_database &db3);
    
    assign_atoms(const using_database &db0, const using_database &db1,
                 const using_database &db2, const using_database &db3,
                 const using_database &db4);
    
    assign_atoms(const using_database &db0, const using_database &db1,
                 const using_database &db2, const using_database &db3,
                 const using_database &db4, const using_database &db5);
    
    assign_atoms(const using_database &db0, const using_database &db1,
                 const using_database &db2, const using_database &db3,
                 const using_database &db4, const using_database &db5,
                 const using_database &db6);
    
    assign_atoms(const using_database &db0, const using_database &db1,
                 const using_database &db2, const using_database &db3,
                 const using_database &db4, const using_database &db5,
                 const using_database &db6, const using_database &db7);
    
    assign_atoms(const using_database &db0, const using_database &db1,
                 const using_database &db2, const using_database &db3,
                 const using_database &db4, const using_database &db5,
                 const using_database &db6, const using_database &db7,
                 const using_database &db8);
    
    assign_atoms(const using_database &db0, const using_database &db1,
                 const using_database &db2, const using_database &db3,
                 const using_database &db4, const using_database &db5,
                 const using_database &db6, const using_database &db7,
                 const using_database &db8, const using_database &db9);

    assign_atoms(const QSet<AtomIndex> &atms);

    assign_atoms(const QSet<AtomIndex> &atms,
                 const using_database &db0);
    
    assign_atoms(const QSet<AtomIndex> &atms,
                 const using_database &db0, const using_database &db1);
    
    assign_atoms(const QSet<AtomIndex> &atms,
                 const using_database &db0, const using_database &db1,
                 const using_database &db2);
    
    assign_atoms(const QSet<AtomIndex> &atms,
                 const using_database &db0, const using_database &db1,
                 const using_database &db2, const using_database &db3);
    
    assign_atoms(const QSet<AtomIndex> &atms,
                 const using_database &db0, const using_database &db1,
                 const using_database &db2, const using_database &db3,
                 const using_database &db4);
    
    assign_atoms(const QSet<AtomIndex> &atms,
                 const using_database &db0, const using_database &db1,
                 const using_database &db2, const using_database &db3,
                 const using_database &db4, const using_database &db5);
    
    assign_atoms(const QSet<AtomIndex> &atms,
                 const using_database &db0, const using_database &db1,
                 const using_database &db2, const using_database &db3,
                 const using_database &db4, const using_database &db5,
                 const using_database &db6);
    
    assign_atoms(const QSet<AtomIndex> &atms,
                 const using_database &db0, const using_database &db1,
                 const using_database &db2, const using_database &db3,
                 const using_database &db4, const using_database &db5,
                 const using_database &db6, const using_database &db7);
    
    assign_atoms(const QSet<AtomIndex> &atms,
                 const using_database &db0, const using_database &db1,
                 const using_database &db2, const using_database &db3,
                 const using_database &db4, const using_database &db5,
                 const using_database &db6, const using_database &db7,
                 const using_database &db8);
    
    assign_atoms(const QSet<AtomIndex> &atms,
                 const using_database &db0, const using_database &db1,
                 const using_database &db2, const using_database &db3,
                 const using_database &db4, const using_database &db5,
                 const using_database &db6, const using_database &db7,
                 const using_database &db8, const using_database &db9
                 );

    assign_atoms(const assign_atoms &other);

    ~assign_atoms();
    
    assign_atoms* clone() const
    {
        return new assign_atoms(*this);
    }
    
    static const char* typeName()
    {
        return "SireDB::assign_atoms";
    }
    
    const char* what() const
    {
        return assign_atoms::typeName();
    }

    void assignParameters( ParameterTable &param_table, 
                           ParameterDB &database,
                           const MatchMRData &matchmr = MatchMRData() ) const;

private:
    /** The set of atoms that will be parametised. If this is empty, then
        all of the atoms in the molecule will be parametised */
    QSet<AtomIndex> atoms_to_be_parametised;
};

}

Q_DECLARE_METATYPE(SireDB::assign_atoms)

SIRE_END_HEADER

#endif
