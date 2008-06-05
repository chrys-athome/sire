#ifndef SIREDB_MATCHMR_H
#define SIREDB_MATCHMR_H

#include "matchmol.h"
#include "matchres.h"

SIRE_BEGIN_HEADER

namespace SireDB
{

/** This class holds the match criteria that matches molecule/residue, e.g.

molecules called "p38" and residues called "ALA" and molecule state "perturbed"

This class is not much use on its own as it doesn't contain any atom matching
criteria. It is however used by other classes that contain atom matching criteria
(e.g. MatchMRA) so that atoms may be matched through a combination of 
molecule, residue and atom matching criteria.

@author Christopher Woods
*/
class SIREDB_EXPORT MatchMR
{
public:
    MatchMR();
    MatchMR(const MatchMol &matchmol);
    MatchMR(const MatchRes &matchres);
    MatchMR(const MatchMol &matchmol, const MatchRes &matchres);
    
    MatchMR(const MatchMR &other);

    ~MatchMR();

    QString toString() const;

    bool isEmpty() const;

    const MatchMol& molecule() const;
    const MatchRes& residue() const;

    bool hasMoleculeCriteria() const;
    bool hasResidueCriteria() const;

    MatchMR operator&&(const MatchMR &other) const;

private:

    /** Criteria used to match the molecule */
    MatchMol moldata;
    
    /** Criteria used to match the residue */
    MatchRes resdata;
};

MatchMR operator&&(const MatchMR &matchmr, const MatchMol &matchmol);
MatchMR operator&&(const MatchMol &matchmol, const MatchMR &matchmr);

MatchMR operator&&(const MatchMR &matchmr, const MatchRes &matchres);
MatchMR operator&&(const MatchRes &matchres, const MatchMR &matchmr);

MatchMR operator&&(const MatchMol &matchmol, const MatchRes &matchres);
MatchMR operator&&(const MatchRes &matchres, const MatchMol &matchmol);

}

SIRE_END_HEADER

#endif
