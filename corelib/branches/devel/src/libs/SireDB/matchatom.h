/********************************************\
  *
  *  Sire - Molecular Simulation Framework
  *
  *  Copyright (C) 2006  Christopher Woods
  *
  *  This program is free software; you can redistribute it and/or modify
  *  it under the terms of the GNU General Public License as published by
  *  the Free Software Foundation; either version 2 of the License, or
  *  (at your option) any later version.
  *
  *  This program is distributed in the hope that it will be useful,
  *  but WITHOUT ANY WARRANTY; without even the implied warranty of
  *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
  *  GNU General Public License for more details.
  *
  *  You should have received a copy of the GNU General Public License
  *  along with this program; if not, write to the Free Software
  *  Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307  USA
  *
  *  For full details of the license please see the COPYING file
  *  that should have come with this distribution.
  *
  *  You can contact the authors via the developer's mailing list
  *  at http://siremol.org
  *
\*********************************************/

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
