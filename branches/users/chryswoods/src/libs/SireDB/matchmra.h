#ifndef SIREDB_MATCHMRA_H
#define SIREDB_MATCHMRA_H

#include "matchdata.h"
#include "matchmr.h"
#include "matchatom.h"

SIRE_BEGIN_HEADER

namespace SireDB
{

/**
This class holds the Match criteria that matches molecule/residue/atom (MRA), e.g.

atoms called "HA1" in residues with number 4 in molecules called "p38"

Molecules may be matched by molecule name, residues by name and/or number
and atoms by name and/or number.

@author Christopher Woods
*/
class SIREDB_EXPORT MatchMRA : public MatchData
{
public:
    MatchMRA();
    
    MatchMRA(const MatchMR &matchmr, const MatchAtom &matchatom);
    
    MatchMRA(const MatchMol &matchmol, const MatchRes &matchres,
             const MatchAtom &matchatom);
    
    MatchMRA(const MatchMol &matchmol);
    MatchMRA(const MatchRes &matchres);
    MatchMRA(const MatchMR &matchmr);
    MatchMRA(const MatchAtom &matchatom);
    
    MatchMRA(const MatchMRA &other);
    
    ~MatchMRA();

    QString toString() const;

    bool isEmpty() const;

    int nMatches() const;
    
    RelateID addTo(ParameterDB &db, uint n) const;
    RelateID addMirroredTo(ParameterDB &db, uint n) const;

    const MatchMol& molecule() const;
    const MatchRes& residue() const;
    const MatchAtom& atom() const;

    const MatchMR& molAndRes() const;

    bool hasMoleculeCriteria() const;
    bool hasResidueCriteria() const;
    bool hasAtomCriteria() const;

    MatchMRA operator&&(const MatchMRA &other) const;

private:

    /** Criteria used to match the molecule and residue */
    MatchMR mrdata;
    
    /** Criteria used to match the atom */
    MatchAtom atomdata;
};

MatchMRA operator&&(const MatchMRA &matchmra, const MatchMR &matchmr);
MatchMRA operator&&(const MatchMR &matchmr, const MatchMRA &matchmra);

MatchMRA operator&&(const MatchMRA &matchmra, const MatchAtom &matchatom);
MatchMRA operator&&(const MatchAtom &matchatom, const MatchMRA &matchmra);

MatchMRA operator&&(const MatchMRA &matchmra, const MatchRes &matchres);
MatchMRA operator&&(const MatchRes &matchres, const MatchMRA &matchmra);

MatchMRA operator&&(const MatchMRA &matchmra, const MatchMol &matchmol);
MatchMRA operator&&(const MatchMol &matchmol, const MatchMRA &matchmra);

MatchMRA operator&&(const MatchMR &matchmr, const MatchAtom &matchatom);
MatchMRA operator&&(const MatchAtom &matchatom, const MatchMR &matchmr);

MatchMRA operator&&(const MatchMol &matchmol, const MatchAtom &matchatom);
MatchMRA operator&&(const MatchAtom &matchatom, const MatchMol &matchmol);

MatchMRA operator&&(const MatchRes &matchres, const MatchAtom &matchatom);
MatchMRA operator&&(const MatchAtom &matchatom, const MatchRes &matchres);

}

SIRE_END_HEADER

#endif
