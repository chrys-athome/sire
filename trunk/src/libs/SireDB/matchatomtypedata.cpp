
#include "matchmr.h"
#include "matchatomtype.h"
#include "matchatomtypedata.h"
#include "relateatomtypedb.h"

using namespace SireDB;

/** Null constructor */
MatchAtomTypeData::MatchAtomTypeData() : MatchData()
{}

/** Construct from only molecule matching criteria */
MatchAtomTypeData::MatchAtomTypeData(const MatchMol &moldata)
                  : MatchData(), mrdata(moldata)
{}

/** Construct from only residue matching criteria */
MatchAtomTypeData::MatchAtomTypeData(const MatchRes &resdata)
                  : MatchData(), mrdata(resdata)
{}

/** Construct from only atom type matching criteria */
MatchAtomTypeData::MatchAtomTypeData(const MatchAtomType &atmdata)
                  : MatchData(), atomdata(atmdata)
{}
    
/** Construct from only molecule and residue matching criteria */
MatchAtomTypeData::MatchAtomTypeData(const MatchMRData &molresdata)
                  : MatchData(), mrdata(molresdata)
{}
    
/** Construct from only molecule and residue matching criteria */
MatchAtomTypeData::MatchAtomTypeData(const MatchMR &molresdata)
                  : MatchData(), mrdata(molresdata.molecule(),molresdata.residue())
{}

/** Construct from only residue matching criteria for the 'nth' atom */
MatchAtomTypeData::MatchAtomTypeData(uint n, const MatchRes &resdata)
                  : MatchData(), mrdata(n, resdata)
{}

/** Construct from only atom matching criteria for the 'nth' atom */
MatchAtomTypeData::MatchAtomTypeData(uint n, const MatchAtomType &atmdata)
                  : MatchData(), atomdata(n, atmdata)
{}
    
/** Copy constructor */
MatchAtomTypeData::MatchAtomTypeData(const MatchAtomTypeData &other)
                  : MatchData(), mrdata(other.mrdata), atomdata(other.atomdata)
{}

/** Destructor */
MatchAtomTypeData::~MatchAtomTypeData()
{}

/** Combine (and) this match criteria with 'other'. If there are any contradictions
    then the data from 'other' is used. */
MatchAtomTypeData MatchAtomTypeData::operator&&(const MatchAtomTypeData &other) const
{
    MatchAtomTypeData ret;

    ret.mrdata = mrdata and other.mrdata;
    ret.atomdata = atomdata and other.atomdata;
    
    return ret;
}

/** Return whether or not this match is empty (matches everything) */
bool MatchAtomTypeData::isEmpty() const
{
    return atomdata.isEmpty() and mrdata.isEmpty();
}

/** Return a string representation of the match criteria */
QString MatchAtomTypeData::toString() const
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

/** Return the 'nth' match */
tuple<MatchMR,MatchAtomType> MatchAtomTypeData::getMatch(uint n) const
{
    return tuple<MatchMR,MatchAtomType>( mrdata.getMatch(n), atomdata.getMatch(n) );
}

/** Return the maximum number of atoms matched */
int MatchAtomTypeData::nMatches() const
{
    return qMax( atomdata.nMatches(), mrdata.nMatches() );
}

/** Return whether or not this has any molecule matching criteria */
bool MatchAtomTypeData::hasMoleculeCriteria() const
{
    return mrdata.hasMoleculeCriteria();
}

/** Return whether or not this has any residue matching criteria */
bool MatchAtomTypeData::hasResidueCriteria() const
{
    return mrdata.hasResidueCriteria();
}

/** Return whether or not this has any atom matching criteria */
bool MatchAtomTypeData::hasAtomCriteria() const
{
    return not atomdata.isEmpty();
}

/** Add the relationship implied by this Match to the parameter database 'db',
    specified to match 'n' atoms. Returns the unique ID number that identifies
    this relationship in the database. */
RelateID MatchAtomTypeData::addTo(ParameterDB &db, uint n) const
{
    //ensure that the database has the right component
    db.addComponent<RelateAtomTypeDB>();
    
    return db.asA<RelateAtomTypeDB>().add(*this, n);
}

/** Add the relationship implied by mirrored for of this Match to the parameter 
    database 'db', specified to match 'n' atoms. Returns the unique ID number 
    that identifies this relationship in the database. */
RelateID MatchAtomTypeData::addMirroredTo(ParameterDB &db, uint n) const
{
    if (n == 1)
        return this->addTo(db,1);
    else
    {
        //create the mirrored form of this match
        MatchAtomTypeData mirrored;
        
        for (uint i=0; i<n; ++i)
        {
            mirrored.set( n-i-1, this->getMatch(i) );
        }
        
        return mirrored.addTo(db,n);
    }
}

/** Set the data of the nth atom match */
void MatchAtomTypeData::set(uint n, const tuple<MatchMR,MatchAtomType> &match)
{
    mrdata.set( n, match.get<0>() );
    atomdata.set( n, match.get<1>() );
}

/** Set all parts to match the AtomType 'atomtype' */
void MatchAtomTypeData::setAtomType(const AtomType &atomtype)
{
    atomdata.set( MatchAtomType(atomtype) );
}

/** Set all parts to match the AtomType 'atomtype' */
void MatchAtomTypeData::setAtomType(const QString &atomtype)
{
    atomdata.set( MatchAtomType(AtomType(atomtype)) );
}

/** Set atom 'atm' to match the AtomType 'atomtype' */
void MatchAtomTypeData::setAtomType(uint atm, const AtomType &atomtype)
{
    atomdata.set( atm, MatchAtomType(atomtype) );
}

/** Set atom 'atm' to match the AtomType 'atomtype' */
void MatchAtomTypeData::setAtomType(uint atm, const QString &atomtype)
{
    atomdata.set( atm, MatchAtomType(AtomType(atomtype)) );
}
    
/** Set all parts to match the element 'element' */
void MatchAtomTypeData::setElement(const Element &element)
{
    atomdata.set( MatchAtomType(element) );
}

/** Set all parts to match the element with the specified number of protons */
void MatchAtomTypeData::setElement(uint nprotons)
{
    atomdata.set( MatchAtomType(Element(nprotons)) );
}

/** Set all parts to match the element 'element' */
void MatchAtomTypeData::setElement(const QString &element)
{
    atomdata.set( MatchAtomType(Element(element)) );
}

/** Set atom 'atm' to match the element 'element' */
void MatchAtomTypeData::setElement(uint atm, const Element &element)
{
    atomdata.set( atm, MatchAtomType(element) );
}

/** Set atom 'atm' to match the element with the specified number of protons */
void MatchAtomTypeData::setElement(uint atm, uint nprotons)
{
    atomdata.set( atm, MatchAtomType(Element(nprotons)) );
}

/** Set atom 'atm' to match the element 'element' */
void MatchAtomTypeData::setElement(uint atm, const QString &element)
{
    atomdata.set( atm, MatchAtomType(Element(element)) );
}

/** Simplify the data in this match, up to maxatm. */
void MatchAtomTypeData::simplify(uint maxatm)
{
    mrdata.simplify(maxatm);
    atomdata.simplify(maxatm);
}

MatchAtomTypeData SIREDB_EXPORT SireDB::operator&&(const MatchAtomTypeData &matchmra, 
                                     const MatchMRData &matchmr)
{
    return ( matchmra and MatchAtomTypeData(matchmr) );
}

MatchAtomTypeData SIREDB_EXPORT SireDB::operator&&(const MatchMRData &matchmr, 
                                     const MatchAtomTypeData &matchmra)
{
    return ( MatchAtomTypeData(matchmr) and matchmra );
}

MatchAtomTypeData SIREDB_EXPORT SireDB::operator&&(const MatchAtomTypeData &mra, const MatchMR &mr)
{
    return ( mra and MatchAtomTypeData(mr) );
}

MatchAtomTypeData SIREDB_EXPORT SireDB::operator&&(const MatchMR &mr, const MatchAtomTypeData &mra)
{
    return ( MatchAtomTypeData(mr) and mra );
}

MatchAtomTypeData SIREDB_EXPORT SireDB::operator&&(const MatchAtomTypeData &matchmra, 
                                     const MatchMol &matchmol)
{
    return ( matchmra and MatchAtomTypeData(matchmol) );
}

MatchAtomTypeData SIREDB_EXPORT SireDB::operator&&(const MatchMol &matchmol, 
                                     const MatchAtomTypeData &matchmra)
{
    return ( MatchAtomTypeData(matchmol) and matchmra );
}

MatchAtomTypeData SIREDB_EXPORT SireDB::operator&&(const MatchAtomTypeData &matchmra, 
                                     const MatchRes &matchres)
{
    return ( matchmra and MatchAtomTypeData(matchres) );
}

MatchAtomTypeData SIREDB_EXPORT SireDB::operator&&(const MatchRes &matchres, 
                                     const MatchAtomTypeData &matchmra)
{
    return ( MatchAtomTypeData(matchres) and matchmra );
}

MatchAtomTypeData SIREDB_EXPORT SireDB::operator&&(const MatchAtomTypeData &matchmra, 
                                     const MatchAtomType &matchtyp)
{
    return ( matchmra and MatchAtomTypeData(matchtyp) );
}

MatchAtomTypeData SIREDB_EXPORT SireDB::operator&&(const MatchAtomType &matchtyp, 
                                     const MatchAtomTypeData &matchmra)
{
    return ( MatchAtomTypeData(matchtyp) and matchmra );
}

MatchAtomTypeData SIREDB_EXPORT SireDB::operator&&(const MatchAtomType &matchtyp, 
                                     const MatchMRData &matchmr)
{
    return ( MatchAtomTypeData(matchtyp) and MatchAtomTypeData(matchmr) );
}

MatchAtomTypeData SIREDB_EXPORT SireDB::operator&&(const MatchMRData &matchmr, 
                                     const MatchAtomType &matchtyp)
{
    return ( MatchAtomTypeData(matchmr) and MatchAtomTypeData(matchtyp) );
}

MatchAtomTypeData SIREDB_EXPORT SireDB::operator&&(const MatchAtomType &matchtyp, 
                                     const MatchMR &matchmr)
{
    return ( MatchAtomTypeData(matchtyp) and MatchAtomTypeData(matchmr) );
}

MatchAtomTypeData SIREDB_EXPORT SireDB::operator&&(const MatchMR &matchmr, 
                                     const MatchAtomType &matchtyp)
{
    return ( MatchAtomTypeData(matchmr) and MatchAtomTypeData(matchtyp) );
}

MatchAtomTypeData SIREDB_EXPORT SireDB::operator&&(const MatchAtomType &matchtyp, 
                                     const MatchMol &matchmol)
{
    return ( MatchAtomTypeData(matchtyp) and MatchAtomTypeData(matchmol) );
}

MatchAtomTypeData SIREDB_EXPORT SireDB::operator&&(const MatchMol &matchmol, 
                                     const MatchAtomType &matchtyp)
{
    return ( MatchAtomTypeData(matchmol) and MatchAtomTypeData(matchtyp) );
}

MatchAtomTypeData SIREDB_EXPORT SireDB::operator&&(const MatchAtomType &matchtyp, 
                                     const MatchRes &matchres)
{
    return ( MatchAtomTypeData(matchtyp) and MatchAtomTypeData(matchres) );
}

MatchAtomTypeData SIREDB_EXPORT SireDB::operator&&(const MatchRes &matchres, 
                                     const MatchAtomType &matchtyp)
{
    return ( MatchAtomTypeData(matchres) and MatchAtomTypeData(matchtyp) );
}
