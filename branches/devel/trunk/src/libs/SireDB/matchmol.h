#ifndef SIREDB_MATCHMOL_H
#define SIREDB_MATCHMOL_H

#include <QString>

#include "sireglobal.h"

SIRE_BEGIN_HEADER

namespace SireDB
{

class MatchRes;

/**
This class is used by MatchData to contain the information that is used to match a molecule.
This information can include the group that the molecule is in (e.g. "solute", "protein",
"solute" etc.) and also the state of the molecule (e.g. "reference", "perturbed"). The
group and state are really just extra strings that can be attached in addition to the 
name of the molecule.

@author Christopher Woods
*/
class SIREDB_EXPORT MatchMol
{
public:
    MatchMol();
    
    MatchMol(const MatchMol &other);
    
    ~MatchMol();
    
    bool operator==(const MatchMol &other) const;
    bool operator!=(const MatchMol &other) const;
    
    QString toString() const;
    
    bool isEmpty() const;
    
    bool hasNameMatch() const;
    bool hasGroupMatch() const;
    bool hasStateMatch() const;
    
    void setName(const QString &name);
    QString name() const;

    void setGroup(const QString &group);
    QString group() const;
    
    void setState(const QString &state);
    QString state() const;

    MatchMol operator&&(const MatchMol &other) const;

private:

    /** The name of the molecule */
    QString molname;
    
    /** The group that the molecule is in */
    QString molgroup;
    
    /** The state of the molecule */
    QString molstate;
};

}

SIRE_END_HEADER

#endif
