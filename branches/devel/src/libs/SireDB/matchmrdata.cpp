
#include "matchmrdata.h"
#include "matchmr.h"

using namespace SireDB;

/** Create an empty match */
MatchMRData::MatchMRData()
{}

/** Construct a match with molecule matching criteria */
MatchMRData::MatchMRData(const MatchMol &mdata)
            : moldata(mdata)
{}
    
/** Construct a match with residue matching criteria */
MatchMRData::MatchMRData(const MatchRes &rdata)
            : resdata(rdata)
{}
    
/** Construct a match from the passed molecule and residue matching criteria */
MatchMRData::MatchMRData(const MatchMol &mdata, const MatchRes &rdata)
            : moldata(mdata), resdata(rdata)
{}
    
/** Construct a match from the passed mol/res matching criteria */
MatchMRData::MatchMRData(const MatchMR &matchmr)
            : moldata(matchmr.molecule()), resdata(matchmr.residue())
{}
    
/** Construct a match with residue matching criteria for atom 'atm' */
MatchMRData::MatchMRData(uint atm, const MatchRes &rdata)
            : resdata(atm,rdata)
{}
    
/** Copy constructor */
MatchMRData::MatchMRData(const MatchMRData &other)
            : moldata(other.moldata), resdata(other.resdata)
{}

/** Destructor */
MatchMRData::~MatchMRData()
{}

/** Return a string representation of the matching criteria */
QString MatchMRData::toString() const
{
    QStringList parts;
    
    if (not moldata.isEmpty())
        parts.append(moldata.toString());
        
    if (not resdata.isEmpty())
        parts.append(resdata.toString());
        
    if (parts.isEmpty())
        return "*";
    else
        return parts.join( QObject::tr(" and ") );
}

/** Return whether or not this is an empty match (matches everything) */
bool MatchMRData::isEmpty() const
{
    return moldata.isEmpty() and resdata.isEmpty();
}

/** Return the match for atom 'atm' (e.g. atom0 in a multi-atom match) */
MatchMR MatchMRData::getMatch(uint atm) const
{
    return MatchMR( moldata, resdata.getMatch(atm) );
}

/** Return the number of atoms specified in this group of atoms
    (e.g. matching atom0-atom1-atom2 would return 3 atoms) */
int MatchMRData::nMatches() const
{
    return qMax( int( not moldata.isEmpty() ), resdata.nMatches() );
}

/** Return whether or not there are any molecule matching criteria */
bool MatchMRData::hasMoleculeCriteria() const
{
    return not moldata.isEmpty();
}

/** Return whether or not there are any residue matching criteria */
bool MatchMRData::hasResidueCriteria() const
{
    return not resdata.isEmpty();
}

/** Combine (and) two MatchMRData objects together. The data from the last object
    is used if there are any contradictions, e.g. mol_name == "p38" and mol_name == "calix"
    will return a MatchMRData with mol_name == "calix" */
MatchMRData MatchMRData::operator&&(const MatchMRData &other) const
{
    MatchMRData ret;
    
    ret.moldata = moldata and other.moldata;
    ret.resdata = resdata and other.resdata;
    
    return ret;
}

/** Set the match for the 'nth' atom to be equal to 'match' */
void MatchMRData::set(uint n, const MatchMR &match)
{
    moldata = match.molecule();
    resdata.set(n, match.residue());
}

/** Simplify the match data - if all atoms have the same matching 
    criteria then pull them all together */
void MatchMRData::simplify(uint maxatm)
{
    resdata.simplify(maxatm);
}

MatchMRData SIREDB_EXPORT SireDB::operator&&(const MatchRes &res, 
                                             const MatchMRData &mrdata)
{
    return ( MatchMRData(res) and mrdata );
}

MatchMRData SIREDB_EXPORT SireDB::operator&&(const MatchMRData &mrdata, 
                                             const MatchRes &res)
{
    return ( mrdata and MatchMRData(res) );
}

MatchMRData SIREDB_EXPORT SireDB::operator&&(const MatchMol &mol, 
                                             const MatchMRData &mrdata)
{
    return ( MatchMRData(mol) and mrdata );
}

MatchMRData SIREDB_EXPORT SireDB::operator&&(const MatchMRData &mrdata, 
                                             const MatchMol &mol)
{
    return ( mrdata and MatchMRData(mol) );
}

MatchMRData SIREDB_EXPORT SireDB::operator&&(const MatchMRData &mrdata, 
                                             const MatchMR &mr)
{
    return ( MatchMRData(mr.molecule(),mr.residue()) and mrdata );
}

MatchMRData SIREDB_EXPORT SireDB::operator&&(const MatchMR &mr, 
                                             const MatchMRData &mrdata)
{
    return ( mrdata and MatchMRData(mr.molecule(),mr.residue()) );
}
