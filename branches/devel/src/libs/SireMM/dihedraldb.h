#ifndef SIREMM_DIHEDRALDB_H
#define SIREMM_DIHEDRALDB_H

#include "SireDB/term14db.h"
#include "SireCAS/symbol.h"

#include "moldihedralinfo.h"
#include "internaldb.hpp"
#include "dihedraltable.h"

SIRE_BEGIN_HEADER

namespace SireMM
{

using SireCAS::Expression;
using SireCAS::Symbol;
using SireCAS::Function;

using SireDB::AssertMatch;
using SireDB::RelateID;
using SireDB::ParamID;
using SireDB::RelateIDMap;

/**
The DihedralDB is the database component that holds dihedral parameters. The database can hold a vast range of Dihedral parameters, as it holds generic functions, and their parameters, rather than specific parameters for standard cosine-based dihedrals.

@author Christopher Woods
*/
class SIREMM_EXPORT DihedralDB : public SireDB::Term14DB,
                                 public InternalDB<MolDihedralInfo>
{
public:
    /** The DihedralDB stores Expression objects */
    typedef Expression parameter_type;

    /** This uses a DihedralTable to store dihedral parameters */
    typedef DihedralTable table_type;

    DihedralDB();
    DihedralDB(const DihedralDB &other);

    ~DihedralDB();

    /** Return the full name of this database component */
    static const char* typeName()
    {
        return "SireMM::DihedralDB";
    }

    const char* what() const
    {
        return DihedralDB::typeName();
    }

    void addDihedral(const QString &userid, const Expression &dihfunc);
    Expression getDihedral(const QString &userid, bool *founddih = 0);

    void addDihedral(const Function &function, const Expression &expression);
    Expression getDihedral(const Function &function, bool *founddih = 0);

    void relateDihedral(const AssertMatch<4> &matchdih, const QString &userid);
    void relateDihedral(const AssertMatch<4> &matchdih, const Expression &dihfunc);

    void relateDihedral(RelateID relateid, const QString &userid);
    void relateDihedral(RelateID relateid, const Expression &dihfunc);

    Expression getDihedral(RelateID relateid, bool *founddih = 0);
    Expression getDihedral(const RelateIDMap &relateids, bool *founddih = 0);

    const Symbol& r() const;
    const Symbol& phi() const;

    void createTable(ParameterTable &param_table,
                     const MolDihedralInfo &dihinfo) const;

    bool assignParameter(const Dihedral &dihedral,
                         const RelateIDMap &relateids,
                         ParameterTable &param_table);

protected:
    void prepareToDump();
    void postLoad();

private:
    /** The symbol used to represent the dihedral angle */
    Symbol _phi;
    /** The symbol used to represent the distance between
        the 1-4 atoms of the dihedral */
    Symbol _r;
};

/** Return the symbol used to represent the dihedral angle (phi) */
inline const Symbol& DihedralDB::phi() const
{
    return _phi;
}

/** Return the symbol used to represent the distance between the 1-4
    atoms of the dihedral (r) */
inline const Symbol& DihedralDB::r() const
{
    return _r;
}

}

Q_DECLARE_METATYPE(SireMM::DihedralDB)

SIRE_END_HEADER

#endif
