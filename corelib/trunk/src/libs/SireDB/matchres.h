#ifndef SIREDB_MATCHRES_H
#define SIREDB_MATCHRES_H

#include <QSet>
#include <QString>
#include <QStringList>

#include "SireMol/atomindex.h"

SIRE_BEGIN_HEADER

namespace SireDB
{

using SireMol::ResNum;

/**
This class is used by MatchData to contain the information that is used to match a residue.

@author Christopher Woods
*/
class SIREDB_EXPORT MatchRes
{
public:
    MatchRes();
    
    MatchRes(const MatchRes &other);
    
    ~MatchRes();
    
    bool operator==(const MatchRes &other) const;
    bool operator!=(const MatchRes &other) const;
    
    QString toString() const;
    QString toString(uint atm) const;
    
    bool isEmpty() const;
    
    bool hasNumberMatch() const;
    bool hasNameMatch() const;
    bool hasResResBonds() const;
    bool hasNoResResBonds() const;
    bool hasAliasMatch() const;
    
    void setName(const QString &name);
    QString name() const;

    void setNumber(ResNum number);
    ResNum number() const;
    
    void addResResBond(const QString &atom);
    void addNoResResBond(const QString &atom);
    
    QStringList resResBonds() const;
    QStringList noResResBonds() const;

    void setAlias(const QString &alias);
    QString alias() const;

    MatchRes operator&&(const MatchRes &other) const;

private:

    /** The matched name of the residue - will be non-null if the residue
        name forms part of the match */
    QString resname;
    
    /** The alias of this residue - aliases are used to allow multiple 
        residues to match the same relationship, e.g. used for matching
        the backbone atoms for protein residues */
    QString resalias;
    
    /** The matched number of the residue - will be non-zero if the residue
        number forms part of the match */
    ResNum resnum;

    /** The atoms that must be involved in inter-residue bonding in the
        matched residue. This will be empty if there are no atoms that 
        have to be in interresidue bonds to match */
    QSet<QString> interresbonds;
    
    /** The atoms that must not be involved in inter-residue bonding in
        the matched residue. This will be empty if there are no atoms that
        must not be in interresidue bonds to match */
    QSet<QString> not_interresbonds;
};

}

SIRE_END_HEADER

#endif
