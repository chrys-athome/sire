#ifndef SIREDB_MATCHDATA_H
#define SIREDB_MATCHDATA_H

#include "parameterdb.h"

#include "SireDB/errors.h"

SIRE_BEGIN_HEADER

namespace SireDB
{

/** This is the pure virtual base class of the MatchData hierarchy. The MatchData
    hierarchy of classes contain matching criteria that match atoms, or groups
    of atoms. Important child classes are;
    
    MatchMRAData : Contains criteria matching the molecule, residue and atom (MRA)
    
    MatchAtomTypeData : Contains criteria matching atom types
    
    MatchSmilesData : Contains smiles strings which match atom environments
    
    This class is designed to be used with the relationship database classes, with 
    each class mapping to a particular database class;
    
    MatchMRAData -> RelateMRADB
    MatchAtomTypeData -> RelateAtomTypeDB
    MatchSmilesData -> RelateSmilesDB
    
    @author Christopher Woods
*/
class SIREDB_EXPORT MatchData
{
public:
    MatchData();
    
    virtual ~MatchData();
    
    /** Return whether or not this is an empty match (matches everything) */
    virtual bool isEmpty() const=0;
    
    /** Return a string representation of this match */
    virtual QString toString() const=0;
    
    /** Return the number of atoms in the group that is matched (e.g. 1 for a single
        atom, 2 for matching a pair of atoms - e.g. a bond, 3 for matching a triple
        of atoms etc.) */
    virtual int nMatches() const=0;
    
    /** Add this match to the database. 
        
        'n' is the number of atoms in the group, e.g. n == 2 to match a bond,
        n == 4 for a dihedral etc.
        
        \throw SireDB::db_error
    */
    virtual RelateID addTo(ParameterDB &db, uint n) const=0;
    
    /** Add the mirrored form of this match to the database. This is 
        otherwise identical to 'addTo' */
    virtual RelateID addMirroredTo(ParameterDB &db, uint n) const=0;
    
    /** Return whether or not this MatchData is an instance of type 'T' */
    template<class T>
    bool isA() const
    {
        return dynamic_cast<const T*>(this) != 0;
    }
    
    /** Return this MatchData cast as an instance of type 'T' - note that this
        will return undefined results unless isA<T>() returns true. */
    template<class T>
    const T& asA() const
    {
        return dynamic_cast<const T&>(*this);
    }
};

/** This is a small class that is used to check that the passed match
    contains only information for up to 'n' atoms. This is used by 
    the user-facing database classes to restrict the types of matches
    used, e.g. the BondDB restricts matches to two atoms, so
    uses an AssertMatch<2>, while the CLJDB restricts matches to a single
    atom, so uses AssertMatch<1>.
    
    @author Christopher Woods
*/
template<int n>
class AssertMatch
{
public:
    AssertMatch(const MatchData &match);
    ~AssertMatch();
    
    RelateID addTo(ParameterDB &db) const;
    RelateID addMirroredTo(ParameterDB &db) const;
    
private:
    /** Const reference to the MatchData */
    const MatchData &match;
};
    
/** Construct the assertation from the passed match data - this will throw an 
    exception if the number of atoms specified in the match is greater than
    'n'. */
template<int n>
SIRE_OUTOFLINE_TEMPLATE
AssertMatch<n>::AssertMatch(const MatchData &m) : match(m)
{
    if (n < match.nMatches())
        throw SireDB::invalid_match(QObject::tr(
            "The criteria (\"%1\") contains more atoms than required (%2)")
                .arg(match.toString()).arg(n), CODELOC);
}

/** Destructor */
template<int n>
SIRE_OUTOFLINE_TEMPLATE
AssertMatch<n>::~AssertMatch()
{}

/** Add this match to the database, returning its identifying ID number */
template<int n>
SIRE_OUTOFLINE_TEMPLATE
RelateID AssertMatch<n>::addTo(ParameterDB &db) const
{
    return match.addTo(db,n);
}

/** Add the mirrored for of this match to the database, returning its 
    identifying ID number */
template<int n>
SIRE_OUTOFLINE_TEMPLATE
RelateID AssertMatch<n>::addMirroredTo(ParameterDB &db) const
{
    return match.addMirroredTo(db,n);
}

}

SIRE_END_HEADER

#endif
