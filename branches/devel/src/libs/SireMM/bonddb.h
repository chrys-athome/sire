#ifndef SIREMM_BONDDB_H
#define SIREMM_BONDDB_H

#include "SireDB/term12db.h"
#include "SireCAS/symbol.h"

#include "molbondinfo.h"
#include "internaldb.hpp"
#include "bondtable.h"

SIRE_BEGIN_HEADER

namespace SireMM
{

using SireCAS::Expression;
using SireCAS::Symbol;

using SireDB::AssertMatch;
using SireDB::RelateID;
using SireDB::ParamID;

/**
The BondDB is the database component that holds bond parameters. The database can hold a vast range of Bond parameters, as it holds generic functions, and their parameters, rather than specific parameters for standard harmonic bonds.
 
@author Christopher Woods
*/
class SIREMM_EXPORT BondDB : public SireDB::Term12DB, public InternalDB<MolBondInfo>
{
public:
    /** The BondDB stores Expression objects */
    typedef Expression parameter_type;
    
    /** This uses a BondTable to store bond parameters */
    typedef BondTable table_type;
    
    BondDB();
    BondDB(const BondDB &other);
    
    ~BondDB();

    /** Return the full name of this database component */
    static const char* typeName()
    {
        return "SireMM::BondDB";
    }

    const char* what() const
    {
        return BondDB::typeName();
    }

    void addBond(const QString &userid, const Expression &bondfunc);
    Expression getBond(const QString &userid, bool *foundbond = 0);
    
    void relateBond(const AssertMatch<2> &matchbond, const QString &userid);
    void relateBond(const AssertMatch<2> &matchbond, const Expression &bondfunc);
    
    void relateBond(RelateID relateid, const QString &userid);
    void relateBond(RelateID relateid, const Expression &bondfunc);

    Expression getBond(RelateID relateid, bool *foundbond = 0);
    Expression getBond(const RelateIDMap &relateids, bool *foundfunc = 0);

    const Symbol& r() const;
        
    bool assignParameter(const Bond &bond, 
                         const RelateIDMap &relateids,
                         ParameterTable &param_table);

protected:
    void prepareToDump();
    void postLoad();
  
private:
    /** The symbol used to represent the bond length ("r") */
    Symbol _r;
};

/** Return a reference to the symbol used to represent the bond length in 
    the bond functions */
inline const Symbol& BondDB::r() const
{
    return _r;
}

}

Q_DECLARE_METATYPE(SireMM::BondDB)

SIRE_END_HEADER

#endif
