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

#ifndef SIREDB_MATCHMRDATA_H
#define SIREDB_MATCHMRDATA_H

#include "multimatch.hpp"
#include "matchres.h"
#include "matchmol.h"

SIRE_BEGIN_HEADER

namespace SireDB
{

class MatchMR;

/** This class forms the base of the MatchData classes that contain matching
    information about the molecule and residue (M R), e.g. MatchMRAData contains
    match data for matching molecule, residue and atom information.
    
    The parameter database uses three MatchMRData derived classes;
    
    (1) MatchMRAData : Match molecule, residue and atom names, numbers etc.
    
    (2) MatchAtomTypeData : Match molecule and residue, then atom type
    
    (3) MatchSmilesData : Match molecule and residue, then atom smiles string
    
    This class is used as a base for the above three classes as it allows
    matches to be narrowed down to a specific molecule and residue, and also
    to a group of molecules (e.g. "protein" or "solvent") and to a particular
    molecule state (e.g. "reference" or "perturbed")

    @author Christopher Woods
*/
class SIREDB_EXPORT MatchMRData
{
public:
    MatchMRData();
    MatchMRData(const MatchMol &moldata);
    MatchMRData(const MatchRes &resdata);
    
    MatchMRData(const MatchMol &moldata, const MatchRes &resdata);
    MatchMRData(const MatchMR &matchmr);
    
    MatchMRData(uint atm, const MatchRes &resdata);
    
    MatchMRData(const MatchMRData &other);
    
    ~MatchMRData();

    QString toString() const;

    bool isEmpty() const;

    MatchMR getMatch(uint atm) const;
    int nMatches() const;

    bool hasMoleculeCriteria() const;
    bool hasResidueCriteria() const;

    void set(uint atm, const MatchMR &match);

    void simplify(uint maxatm);

    MatchMRData operator&&(const MatchMRData &other) const;

private:

    /** Molecule matching criteria */
    MatchMol moldata;
    
    /** Residue matching criteria */
    MultiMatch<MatchRes> resdata;
    
};

MatchMRData operator&&(const MatchRes &res, const MatchMRData &mrdata);
MatchMRData operator&&(const MatchMRData &mrdata, const MatchRes &res);

MatchMRData operator&&(const MatchMol &mol, const MatchMRData &mrdata);
MatchMRData operator&&(const MatchMRData &mrdata, const MatchMol &mol);

MatchMRData operator&&(const MatchMRData &mrdata, const MatchMR &mr);
MatchMRData operator&&(const MatchMR &mr, const MatchMRData &mrdata);

}

SIRE_END_HEADER

#endif
