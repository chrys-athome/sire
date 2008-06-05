#ifndef SIREDB_MATCHATOM_H
#define SIREDB_MATCHATOM_H

#include <QStringList>

#include "SireMol/atom.h"

#include "matchdata.h"

SIRE_BEGIN_HEADER

namespace SireDB
{

using SireMol::AtomNum;

/**
This class is used by MatchMRA to contain the information that is used to match an atom.

@author Christopher Woods
*/
class SIREDB_EXPORT MatchAtom : public MatchData
{
public:
    MatchAtom();
    
    MatchAtom(const MatchAtom &other);
    
    ~MatchAtom();
    
    bool operator==(const MatchAtom &other) const;
    bool operator!=(const MatchAtom &other) const;
    
    QString toString() const;
    QString toString(uint atm) const;
    
    bool isEmpty() const;
    
    int nMatches() const;
    
    RelateID addTo(ParameterDB &db, uint n) const;
    RelateID addMirroredTo(ParameterDB &db, uint n) const;
    
    bool hasNumberMatch() const;
    bool hasNameMatch() const;

    void setName(const QString &name);
    QString name() const;
    
    void setNumber(AtomNum number);
    AtomNum number() const;
    
    MatchAtom operator&&(const MatchAtom &other) const;

private:

    /** The name of the atom - this will be null if the atom name does not
        form part of the match */
    QString atmname;
    
    /** The number of the atom - this will be zero if the atom number does not   
        form part of the match */
    AtomNum atmnum;
};

}

SIRE_END_HEADER

#endif
