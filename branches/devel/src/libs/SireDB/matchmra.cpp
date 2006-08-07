
#include "matchmra.h"
#include "relatemradb.h"

using namespace SireDB;

/** Null constructor */
MatchMRA::MatchMRA() : MatchData()
{}

/** Construct a match from the passed molecule, residue and atom match criteria */
MatchMRA::MatchMRA(const MatchMol &mol, const MatchRes &res, const MatchAtom &atom)
         : MatchData(), mrdata(mol,res), atomdata(atom)
{}

/** Construct a match from the passed mol/res and atom match criteria */
MatchMRA::MatchMRA(const MatchMR &mr, const MatchAtom &atom)
         : MatchData(), mrdata(mr), atomdata(atom)
{}

/** Construct a match from the passsed atom matching criteria */
MatchMRA::MatchMRA(const MatchAtom &atom)
         : MatchData(), atomdata(atom)
{}

/** Construct a match from the passsed molecule matching criteria */
MatchMRA::MatchMRA(const MatchMol &matchmol)
         : MatchData(), mrdata(matchmol)
{}

/** Construct a match from the passsed residue matching criteria */
MatchMRA::MatchMRA(const MatchRes &matchres)
         : MatchData(), mrdata(matchres)
{}

/** Construct a match from the passsed molecule/residue matching criteria */
MatchMRA::MatchMRA(const MatchMR &matchmr)
         : MatchData(), mrdata(matchmr)
{}

/** Copy constructor */
MatchMRA::MatchMRA(const MatchMRA &other)
         : MatchData(), mrdata(other.mrdata), atomdata(other.atomdata)
{}

/** Destructor */
MatchMRA::~MatchMRA()
{}

/** Return a string representation of this match */
QString MatchMRA::toString() const
{
    QStringList parts;
    
    if (not mrdata.isEmpty())
        parts.append(mrdata.toString());
        
    if (not atomdata.isEmpty())
        parts.append(atomdata.toString());
        
    if (not parts.isEmpty())
        return parts.join(" and ");
    else
        return QObject::tr("*");
}

/** Return whether this is an empty match (matches everything) */
bool MatchMRA::isEmpty() const
{
    return mrdata.isEmpty() and atomdata.isEmpty();
}

/** Return a const reference to the molecule criteria - this will be empty
    if there are no molecule criteria */
const MatchMol& MatchMRA::molecule() const
{
    return mrdata.molecule();
}

/** Return a const reference to the residue criteria - this will be empty
    if there are no residue criteria */
const MatchRes& MatchMRA::residue() const
{
    return mrdata.residue();
}

/** Return a const reference to the MatchMR which holds the molecule and residue 
    matching criteria - this will be empty if there are no molecule or residue 
    matching criteria */
const MatchMR& MatchMRA::molAndRes() const
{
    return mrdata;
}

/** Return a const reference to the atom criteria - this will be empty
    if there are no atom criteria */
const MatchAtom& MatchMRA::atom() const
{
    return atomdata;
}

/** Return whether this contains molecule matching criteria */
bool MatchMRA::hasMoleculeCriteria() const
{
    return mrdata.hasMoleculeCriteria();
}

/** Return whether this contains residue matching criteria */
bool MatchMRA::hasResidueCriteria() const
{
    return mrdata.hasResidueCriteria();
}

/** Return whether this contains atom matching criteria */
bool MatchMRA::hasAtomCriteria() const
{
    return not atomdata.isEmpty();
}

/** This contains only a single match */
int MatchMRA::nMatches() const
{
    return 1;
}
    
/** Add this to the database 'db' for matching 'n' atoms, and return 
    the ID number of this matching criteria */
RelateID MatchMRA::addTo(ParameterDB &db, uint n) const
{
    //n must be 1 as we can't match a repeated atom!
    if (n != 1)
        return 0;
    else
    {
        //ensure that the database has the right component
        db.addComponent<RelateMRADB>();
        
        return db.asA<RelateMRADB>().add(*this);
    }
}
    
/** Add this to the database 'db' for matching 'n' atoms, and return 
    the ID number of this matching criteria */
RelateID MatchMRA::addMirroredTo(ParameterDB &db, uint n) const
{
    return this->addTo(db,n);
}

/** 'and' combine two MatchMRA objects together. If there is any contradiction 
    then the data from the second object is used, e.g. mol_name == "p38" 
    and mol_name == "calix" will combine together to form mol_name == "calix" */
MatchMRA MatchMRA::operator&&(const MatchMRA &other) const
{
    MatchMRA ret;
    
    ret.mrdata = mrdata and other.mrdata;
    ret.atomdata = atomdata and other.atomdata;
    
    return ret;
}

MatchMRA SIREDB_EXPORT SireDB::operator&&(const MatchMRA &matchmra, 
                                          const MatchMR &matchmr)
{
    return ( matchmra and MatchMRA(matchmr) );
}

MatchMRA SIREDB_EXPORT SireDB::operator&&(const MatchMR &matchmr, 
                                          const MatchMRA &matchmra)
{
    return ( MatchMRA(matchmr) and matchmra );
}

MatchMRA SIREDB_EXPORT SireDB::operator&&(const MatchMR &matchmr, 
                                          const MatchAtom &matchatom)
{
    return ( MatchMRA(matchmr) and MatchMRA(matchatom) );
}

MatchMRA SIREDB_EXPORT SireDB::operator&&(const MatchAtom &matchatom, 
                                          const MatchMR &matchmr)
{
    return ( MatchMRA(matchatom) and MatchMRA(matchmr) );
}

MatchMRA SIREDB_EXPORT SireDB::operator&&(const MatchMol &matchmol, 
                                          const MatchAtom &matchatom)
{
    return ( MatchMRA(matchmol) and MatchMRA(matchatom) );
}

MatchMRA SIREDB_EXPORT SireDB::operator&&(const MatchAtom &matchatom, 
                                          const MatchMol &matchmol)
{
    return ( MatchMRA(matchatom) and MatchMRA(matchmol) );
}

MatchMRA SIREDB_EXPORT SireDB::operator&&(const MatchRes &matchres, 
                                          const MatchAtom &matchatom)
{
    return ( MatchMRA(matchres) and MatchMRA(matchatom) );
}

MatchMRA SIREDB_EXPORT SireDB::operator&&(const MatchAtom &matchatom, 
                                          const MatchRes &matchres)
{
    return ( MatchMRA(matchatom) and MatchMRA(matchres) );
}

MatchMRA SIREDB_EXPORT SireDB::operator&&(const MatchMRA &matchmra, 
                                          const MatchAtom &matchatom)
{
    return ( matchmra and MatchMRA(matchatom) );
}

MatchMRA SIREDB_EXPORT SireDB::operator&&(const MatchAtom &matchatom, 
                                          const MatchMRA &matchmra)
{
    return ( MatchMRA(matchatom) and matchmra );
}

MatchMRA SIREDB_EXPORT SireDB::operator&&(const MatchMRA &matchmra, 
                                          const MatchRes &matchres)
{
    return ( matchmra and MatchMRA(matchres) );
}

MatchMRA SIREDB_EXPORT SireDB::operator&&(const MatchRes &matchres, 
                                          const MatchMRA &matchmra)
{
    return ( MatchMRA(matchres) and matchmra );
}

MatchMRA SIREDB_EXPORT SireDB::operator&&(const MatchMRA &matchmra, 
                                          const MatchMol &matchmol)
{
    return ( matchmra and MatchMRA(matchmol) );
}

MatchMRA SIREDB_EXPORT SireDB::operator&&(const MatchMol &matchmol, 
                                          const MatchMRA &matchmra)
{
    return ( MatchMRA(matchmol) and matchmra );
}
