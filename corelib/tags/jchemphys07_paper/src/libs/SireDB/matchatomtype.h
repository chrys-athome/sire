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

#ifndef SIREDB_MATCHATOMTYPE_H
#define SIREDB_MATCHATOMTYPE_H

#include "atomtype.h"
#include "matchdata.h"

SIRE_BEGIN_HEADER

namespace SireDB
{

/**
This class is used to match parameters based on atom type. This also allows matching based on wild types (e.g. any carbon, or indeed any atom). This type of matching is predominantly used by the bond, angle and dihedral terms of AMBER/OPLS like forcefields.

@author Christopher Woods
*/
class SIREDB_EXPORT MatchAtomType : public MatchData
{
public:
    MatchAtomType();
    MatchAtomType(const QString &id, const Element &element);
    MatchAtomType(const AtomType &atomtype);
    MatchAtomType(const Element &element);

    MatchAtomType(const MatchAtomType &other);

    ~MatchAtomType();

    bool operator==(const MatchAtomType &other) const;
    bool operator!=(const MatchAtomType &other) const;
    
    QString toString() const;
    QString toString(uint atm) const;
    
    int nMatches() const;
    
    RelateID addTo(ParameterDB &db, uint n) const;
    RelateID addMirroredTo(ParameterDB &db, uint n) const;
    
    bool isWild() const;
    bool isEmpty() const;
    
    bool isWildType() const;
    bool isWildElement() const;

    void setType(const QString &atomtype);
    void setType(const AtomType &atomtype);
    
    const AtomType& type() const;
    
    void setElement(const Element &element);
    const Element& element() const;
    
    MatchAtomType operator&&(const MatchAtomType &other) const;
    
private:
    /** The AtomType being matched */
    AtomType atomtype;

};

}

SIRE_END_HEADER

#endif
