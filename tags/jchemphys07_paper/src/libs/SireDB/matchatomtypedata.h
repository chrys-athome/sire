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

#ifndef SIREDB_MATCHATOMTYPEDATA_H
#define SIREDB_MATCHATOMTYPEDATA_H

#include "matchdata.h"
#include "matchmrdata.h"
#include "matchatomtype.h"

#include <boost/tuple/tuple.hpp>

SIRE_BEGIN_HEADER

namespace SireDB
{

using boost::tuple;

/**
This class holds the matching criteria used to match atom types (in addition to the standard molecule and residue matching criteria). This class is very similar to MatchMRAData, and like MatchMRAData is linked to RelateMRADB, so this is linked with RelateAtomTypeDB.

@author Christopher Woods
*/
class SIREDB_EXPORT MatchAtomTypeData : public MatchData
{
public:
    MatchAtomTypeData();
    
    MatchAtomTypeData(const MatchMol &moldata);
    MatchAtomTypeData(const MatchRes &resdata);
    MatchAtomTypeData(const MatchAtomType &atmdata);
    
    MatchAtomTypeData(const MatchMR &molresdata);
    MatchAtomTypeData(const MatchMRData &molresdata);
    
    MatchAtomTypeData(uint atm, const MatchRes &resdata);
    MatchAtomTypeData(uint atm, const MatchAtomType &atmdata);
    
    MatchAtomTypeData(const MatchAtomTypeData &other);
    
    ~MatchAtomTypeData();

    MatchAtomTypeData operator&&(const MatchAtomTypeData &other) const;

    QString toString() const;

    bool isEmpty() const;

    tuple<MatchMR,MatchAtomType> getMatch(uint atm) const;
    int nMatches() const;

    bool hasMoleculeCriteria() const;
    bool hasResidueCriteria() const;
    bool hasAtomCriteria() const;

    RelateID addTo(ParameterDB &db, uint n) const;
    RelateID addMirroredTo(ParameterDB &db, uint n) const;

    void set(uint atm, const tuple<MatchMR,MatchAtomType> &match);
    
    void setAtomType(const AtomType &atomtype);
    void setAtomType(const QString &atomtype);
    void setAtomType(uint atm, const AtomType &atomtype);
    void setAtomType(uint atm, const QString &atomtype);
    
    void setElement(const Element &element);
    void setElement(uint nprotons);
    void setElement(const QString &element);
    void setElement(uint atm, const Element &element);
    void setElement(uint atm, uint nprotons);
    void setElement(uint atm, const QString &element);
    
    void simplify(uint maxatm);

private:
    
    /** The molecule and residue matching criteria */
    MatchMRData mrdata;
    
    /** The atomtype matching criteria */
    MultiMatch<MatchAtomType> atomdata;
};

MatchAtomTypeData operator&&(const MatchAtomTypeData &matchmra, const MatchMRData &matchmr);
MatchAtomTypeData operator&&(const MatchMRData &matchmr, const MatchAtomTypeData &matchmra);

MatchAtomTypeData operator&&(const MatchAtomTypeData &mra, const MatchMR &mr);
MatchAtomTypeData operator&&(const MatchMR &mr, const MatchAtomTypeData &mra);

MatchAtomTypeData operator&&(const MatchAtomTypeData &matchmra, const MatchMol &matchmol);
MatchAtomTypeData operator&&(const MatchMol &matchmol, const MatchAtomTypeData &matchmra);

MatchAtomTypeData operator&&(const MatchAtomTypeData &matchmra, const MatchRes &matchres);
MatchAtomTypeData operator&&(const MatchRes &matchres, const MatchAtomTypeData &matchmra);

MatchAtomTypeData operator&&(const MatchAtomTypeData &matchmra, const MatchAtomType &matchtyp);
MatchAtomTypeData operator&&(const MatchAtomType &matchtyp, const MatchAtomTypeData &matchmra);

MatchAtomTypeData operator&&(const MatchAtomType &matchtyp, const MatchMR &matchmr);
MatchAtomTypeData operator&&(const MatchMR &matchmr, const MatchAtomType &matchtyp);

MatchAtomTypeData operator&&(const MatchAtomType &matchtyp, const MatchMRData &matchmr);
MatchAtomTypeData operator&&(const MatchMRData &matchmr, const MatchAtomType &matchtyp);

MatchAtomTypeData operator&&(const MatchAtomType &matchtyp, const MatchMol &matchmol);
MatchAtomTypeData operator&&(const MatchMol &matchmol, const MatchAtomType &matchtyp);

MatchAtomTypeData operator&&(const MatchAtomType &matchtyp, const MatchRes &matchres);
MatchAtomTypeData operator&&(const MatchRes &matchres, const MatchAtomType &matchtyp);

}

SIRE_END_HEADER

#endif
