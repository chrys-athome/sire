
#include "matchdata.h"
#include "matchmradata.h"
#include "matchmra.h"
#include "relatemradb.h"

using namespace SireDB;

/** Null constructor */
MatchMRAData::MatchMRAData() : MatchData()
{}

/** Construct a MatchMRAData containing only molecule matching criteria */
MatchMRAData::MatchMRAData(const MatchMol &mol) : MatchData(), mrdata(mol)
{}

/** Construct a MatchMRAData containing only residue matching criteria */
MatchMRAData::MatchMRAData(const MatchRes &res) : MatchData(), mrdata(res)
{}

/** Construct a MatchMRAData containing only atom matching criteria */
MatchMRAData::MatchMRAData(const MatchAtom &atom) : MatchData(), atomdata(atom)
{}
    
/** Construct a MatchMRAData containing only molecule and residue matching criteria */
MatchMRAData::MatchMRAData(const MatchMR &molresdata)
             : MatchData(), mrdata(molresdata)
{}
    
/** Construct a MatchMRAData containing only molecule and residue matching criteria */
MatchMRAData::MatchMRAData(const MatchMRData &molresdata)
             : MatchData(), mrdata(molresdata)
{}
    
/** Construct a MatchMRAData from a Match object */
MatchMRAData::MatchMRAData(const MatchMRA &match)
             : MatchData(), mrdata(match.molecule(), match.residue()), 
               atomdata(match.atom())
{}

/** Construct a MatchMRAData matching atom 'atm' to the residue criteria in 'res' */
MatchMRAData::MatchMRAData(uint atm, const MatchRes &resdata)
             : MatchData(), mrdata(atm,resdata)
{}

/** Construct a MatchMRAData matching atom 'atm' to the atom criteria in 'atom' */
MatchMRAData::MatchMRAData(uint atm, const MatchAtom &atmdata)
             : MatchData(), atomdata(atm,atmdata)
{}
    
/** Copy constructor */
MatchMRAData::MatchMRAData(const MatchMRAData &other)
             : MatchData(), mrdata(other.mrdata), atomdata(other.atomdata)
{}

/** Destructor */
MatchMRAData::~MatchMRAData()
{}

/** Return a string representation of this MatchMRAData */
QString MatchMRAData::toString() const
{
    QStringList parts;
        
    if (not mrdata.isEmpty())
        parts.append(mrdata.toString());
        
    if (not atomdata.isEmpty())
        parts.append(atomdata.toString());
        
    if (parts.isEmpty())
        return "*";
    else
        return parts.join( QObject::tr(" and ") );
}

/** Combine (and) this MatchMRAData with 'other'. */
MatchMRAData MatchMRAData::operator&&(const MatchMRAData &other) const
{
    MatchMRAData ret;
    
    ret.mrdata = mrdata and other.mrdata;
    ret.atomdata = atomdata and other.atomdata;
    
    return ret;
}

/** Is this MatchMRAData empty (matches everything) */
bool MatchMRAData::isEmpty() const
{
    return mrdata.isEmpty() and atomdata.isEmpty();
}

/** Return the match for atom 'atm' (e.g. atom0, atom1 in a multi-atom match) */
MatchMRA MatchMRAData::getMatch(uint atm) const
{
    return MatchMRA( mrdata.getMatch(atm), atomdata.getMatch(atm) );
}

/** Return the number of atoms specified in this group of atoms
    (e.g. matching atom0-atom1-atom2 would return 3 atoms) */
int MatchMRAData::nMatches() const
{
    return qMax( mrdata.nMatches(), atomdata.nMatches() );
}

/** Return whether this contains molecule matching criteria */
bool MatchMRAData::hasMoleculeCriteria() const
{
    return mrdata.hasMoleculeCriteria();
}

/** Return whether this contains residue matching criteria */
bool MatchMRAData::hasResidueCriteria() const
{
    return mrdata.hasResidueCriteria();
}

/** Return whether this contains atom matching criteria */
bool MatchMRAData::hasAtomCriteria() const
{
    return not atomdata.isEmpty();
}

/** Add the relationship implied by this Match to the parameter database 'db',
    specified to match 'n' atoms. Returns the unique ID number that identifies
    this relationship in the database. */
RelateID MatchMRAData::addTo(ParameterDB &db, uint n) const
{
    //ensure that the database has a RelateMRADB component
    db.addComponent<RelateMRADB>();
    
    return db.asA<RelateMRADB>().add(*this, n);
}

/** Add the mirrored form of the relationship implied by this Match to the 
    parameter database 'db', specified to match 'n' atoms. Returns the 
    unique ID number that identifies this relationship in the database. */
RelateID MatchMRAData::addMirroredTo(ParameterDB &db, uint n) const
{
    if (n == 1)
        return this->addTo(db,1);
    else
    {
        //create the mirrored form of this match
        MatchMRAData mirrored;
      
        for (uint i=0; i<n; ++i)
        {
            mirrored.set( n-i-1, this->getMatch(i) );
        }

        return mirrored.addTo(db, n);
    }
}

/** Set the match for the 'nth' atom to be equal to 'match' */
void MatchMRAData::set(uint n, const MatchMRA &match)
{
    mrdata.set(n, match.molAndRes());
    atomdata.set(n, match.atom());
}

/** Simplify the match data - if all atoms have the same matching 
    criteria then pull them all together */
void MatchMRAData::simplify(uint maxatm)
{
    mrdata.simplify(maxatm);
    atomdata.simplify(maxatm);
}

MatchMRAData SIREDB_EXPORT SireDB::operator&&(const MatchMRAData &matchmra, 
                                              const MatchMRData &matchmr)
{
    return ( matchmra and MatchMRAData(matchmr) );
}

MatchMRAData SIREDB_EXPORT SireDB::operator&&(const MatchMRData &matchmr, 
                                              const MatchMRAData &matchmra)
{
    return ( MatchMRAData(matchmr) and matchmra );
}

MatchMRAData SIREDB_EXPORT SireDB::operator&&(const MatchMRAData &matchmra, 
                                              const MatchAtom &matchatom)
{
    return ( matchmra and MatchMRAData(matchatom) );
}

MatchMRAData SIREDB_EXPORT SireDB::operator&&(const MatchAtom &matchatom, 
                                              const MatchMRAData &matchmra)
{
    return ( MatchMRAData(matchatom) and matchmra );
}

MatchMRAData SIREDB_EXPORT SireDB::operator&&(const MatchMRData &matchmr, 
                                              const MatchAtom &matchatom)
{
    return ( MatchMRAData(matchmr) and MatchMRAData(matchatom) );
}

MatchMRAData SIREDB_EXPORT SireDB::operator&&(const MatchAtom &matchatom, 
                                              const MatchMRData &matchmr)
{
    return ( MatchMRAData(matchatom) and MatchMRAData(matchmr) );
}

MatchMRAData SIREDB_EXPORT SireDB::operator&&(const MatchMRAData &matchmra, 
                                              const MatchRes &matchres)
{
    return ( matchmra and MatchMRAData(matchres) );
}

MatchMRAData SIREDB_EXPORT SireDB::operator&&(const MatchRes &matchres,
                                              const MatchMRAData &matchmra)
{
    return ( MatchMRAData(matchres) and matchmra );
}

MatchMRAData SIREDB_EXPORT SireDB::operator&&(const MatchMRAData &matchmra, 
                                              const MatchMol &matchmol)
{
    return ( matchmra and MatchMRAData(matchmol) );
}

MatchMRAData SIREDB_EXPORT SireDB::operator&&(const MatchMol &matchmol, 
                                              const MatchMRAData &matchmra)
{
    return ( MatchMRAData(matchmol) and matchmra );
}

MatchMRAData SIREDB_EXPORT SireDB::operator&&(const MatchMRAData &matchmra, 
                                              const MatchMR &matchmr)
{
    return ( matchmra and MatchMRAData(matchmr) );
}

MatchMRAData SIREDB_EXPORT SireDB::operator&&(const MatchMR &matchmr, 
                                              const MatchMRAData &matchmra)
{
    return ( MatchMRAData(matchmr) and matchmra );
}

MatchMRAData SIREDB_EXPORT SireDB::operator&&(const MatchMRAData &mradata, 
                                              const MatchMRA &mra)
{
    return ( mradata and MatchMRAData(mra) );
}

MatchMRAData SIREDB_EXPORT SireDB::operator&&(const MatchMRA &mra, 
                                              const MatchMRAData &mradata)
{
    return ( MatchMRAData(mra) and mradata );
}

MatchMRAData SIREDB_EXPORT SireDB::operator&&(const MatchMRA &mra, 
                                              const MatchMRData &mr)
{
    return ( MatchMRAData(mra) and MatchMRAData(mr) );
}

MatchMRAData SIREDB_EXPORT SireDB::operator&&(const MatchMRData &mr, 
                                              const MatchMRA &mra)
{
    return ( MatchMRAData(mr) and MatchMRAData(mra) );
}
