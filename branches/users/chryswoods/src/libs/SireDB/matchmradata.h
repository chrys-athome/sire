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

#ifndef SIREDB_MATCHMRADATA_H
#define SIREDB_MATCHMRADATA_H

#include "matchdata.h"

#include "matchmrdata.h"
#include "matchatom.h"

SIRE_BEGIN_HEADER

namespace SireDB
{

class MatchMRA;

/**
This is the class that holds all of the match data that is accumulated during the evaluation of a user-expression. This class merely aggregates the data into a single object that allows for easy processing by the more specialised match classes.

This class holds match data for matches using criteria specifiying the molecule, residue and atom (MRA)

This class is used to provide a nice syntax for the user to specify matching criteria for an atom, e.g.

\code
//match atom "CA1" in a residue called "ALA" in a molecule called "p38"
mol_name == "p38" and atom_name == "CA1" and res_name == "ALA";

//match an atom called "HA1" in a residue called "ASP" in any molecule
res_name == "ASP" and atom_name == "HA1";

//match an atom called "C" in residue 3 of a molecule called "calix"
mol_name == "calix" and res_num == 3 and atom_name == "C";

//match an atom called "CA" in a residue that has an intermolecular bond to its "C" atom
//and no intermolecular bonds to its "N" atom
atom_name == "CA" and res_res_bond_via("C") and no_res_res_bond_via("N");

//these sorts of expressions can be used to relate atom relationships to 
//specific database parameters

db.relateCLJ( "42", mol_name == "p38" and res_res_bond_via("N") and 
                    res_res_bond_via("C") and atom_name == "CA" );
                    
db.relateCLJ( "6", mol_name == "p38" and res_name == "ALA" and atom_name == "CA1" );

db.relateCLJ( "5", res_name == "ASP" and atom_name == "HA1" );

db.relateCLJ( "4", mol_name == "calix" and res_num == 3 and atom_name == "C" );

db.relateCLJ( "3", res_res_bond_via("C") and no_res_res_bond_via("N") and atom_name == "CA" );

db.relateBond( "5", mol_name == "calix" and res_name == "PHE" and 
                    atom0_name == "CA" and atom1_name == "HA" );

db.relateBond( "6", res0_name == "ALA" and atom0_name == "C" and
                    res1_name == "ASP" and atom1_name == "N" );
                    
db.relateBond( "7", res0_res_bond_via("C") and no_res1_res_bond_via("N") 
                    and atom0_name == "C" and atom1_name == "N" );
                    
\endcode

@author Christopher Woods
*/
class SIREDB_EXPORT MatchMRAData : public MatchData
{
public:
    MatchMRAData();
    
    MatchMRAData(const MatchMol &moldata);
    MatchMRAData(const MatchRes &resdata);
    MatchMRAData(const MatchAtom &atmdata);
    
    MatchMRAData(const MatchMR &matchmr);
    MatchMRAData(const MatchMRData &molresdata);
    
    MatchMRAData(const MatchMRA &match);
    
    MatchMRAData(uint atm, const MatchRes &resdata);
    MatchMRAData(uint atm, const MatchAtom &atmdata);
    
    MatchMRAData(const MatchMRAData &other);
    
    ~MatchMRAData();

    MatchMRAData operator&&(const MatchMRAData &other) const;

    QString toString() const;

    bool isEmpty() const;

    MatchMRA getMatch(uint atm) const;
    int nMatches() const;

    bool hasMoleculeCriteria() const;
    bool hasResidueCriteria() const;
    bool hasAtomCriteria() const;

    RelateID addTo(ParameterDB &db, uint n) const;
    RelateID addMirroredTo(ParameterDB &db, uint n) const;

    void set(uint atm, const MatchMRA &match);
    
    void simplify(uint maxatm);

private:

    /** Molecule/residue matching criteria */
    MatchMRData mrdata;
    
    /** Atom matching criteria */
    MultiMatch<MatchAtom> atomdata;
};

MatchMRAData operator&&(const MatchMRAData &matchmra, const MatchMRData &matchmr);
MatchMRAData operator&&(const MatchMRData &matchmr, const MatchMRAData &matchmra);

MatchMRAData operator&&(const MatchMRAData &matchmra, const MatchAtom &matchatom);
MatchMRAData operator&&(const MatchAtom &matchatom, const MatchMRAData &matchmra);

MatchMRAData operator&&(const MatchMRData &matchmra, const MatchAtom &matchatom);
MatchMRAData operator&&(const MatchAtom &matchatom, const MatchMRData &matchmra);

MatchMRAData operator&&(const MatchMRAData &matchmra, const MatchRes &matchres);
MatchMRAData operator&&(const MatchRes &matchres, const MatchMRAData &matchmra);

MatchMRAData operator&&(const MatchMRAData &matchmra, const MatchMol &matchmol);
MatchMRAData operator&&(const MatchMol &matchmol, const MatchMRAData &matchmra);

MatchMRAData operator&&(const MatchMRData &matchmr, const MatchAtom &matchatom);
MatchMRAData operator&&(const MatchAtom &matchatom, const MatchMRData &matchmr);

MatchMRAData operator&&(const MatchMRAData &matchmra, const MatchMR &matchmr);
MatchMRAData operator&&(const MatchMR &matchmr, const MatchMRAData &matchmra);

MatchMRAData operator&&(const MatchMRAData &mradata, const MatchMRA &mra);
MatchMRAData operator&&(const MatchMRA &mra, const MatchMRAData &mradata);

MatchMRAData operator&&(const MatchMRA &mra, const MatchMRData &mr);
MatchMRAData operator&&(const MatchMRData &mr, const MatchMRA &mra);

}

SIRE_END_HEADER

#endif
