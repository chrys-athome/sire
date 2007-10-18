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

#ifndef SIREDB_MATCHHELPERS_H
#define SIREDB_MATCHHELPERS_H

#include "SireMol/atom.h"

#include "matchmra.h"
#include "matchmradata.h"

#include "matchatomtype.h"
#include "matchatomtypedata.h"

SIRE_BEGIN_HEADER

namespace SireMol
{
class Element;
}

namespace SireDB
{

using SireMol::ResNum;
using SireMol::AtomNum;
using SireMol::Element;


/** This file contains small helper classes that are used to help provide a 
    friendly syntax for specifying matching criteria. The classes in
    this file allow for syntax such as;
    
    db.relateCLJ( "5", mol_name == "calix" and res_name == "PHE" and atom_name == "CA" );
    
    db.relateCLJ( "6", mol_group == "protein" and res_name == "ALA" and res_res_bond_via("N") 
                       and no_res_res_bond_via("C") and atom_name == "HA" );
                       
    db.relateCLJ( "7", mol_state == "perturbed" and res_number == 5 and atom_name == "CW1" );
                       
    db.relateBond( "42", res_name == "ALA" and atom0_name == "CA" and atom1_name == "HA" );
    
    db.relateBond( "43", atom0_type == AtomType("CT", "carbon") 
                              and atom1_type == AtomType("HC", "hydrogen") );
                              
    db.relateBond( "42", atom0_element == "carbon" and atom1_type == "HA" );
                              
    Note that this syntax allows for contradictory matching statements
    to be created. The rule is that if there is a contradiction, then the second
    piece of data is used, e.g. mol_name == "p38" and mol_name == "calix" is a contradiction
    (the molecule's name cannot be both "p38" and "calix", so the second piece of 
    data - "calix" - is used).
    
    Note also that you also have control over the case-sensitivity of the matches, 
    e.g. you can tell the database that molecule names are case-sensitive. This will
    mean that "p38" will be different to "P38". It is best to set the case-sensitivity
    before you start loading parameters or making assignments, as changing the 
    case-sensitivity during these processes may lead to unexpected results.
*/

/** This class is used to help specify molecule names */
class SIREDB_EXPORT match_mol_name
{
public:
    match_mol_name();
    ~match_mol_name();
    MatchMol operator==(const QString &name) const;
};

static const match_mol_name mol_name;

/** This class is used to help specify molecule groups */
class SIREDB_EXPORT match_mol_group
{
public:
    match_mol_group();
    ~match_mol_group();
    MatchMol operator==(const QString &group) const;
};

static const match_mol_group mol_group;

/** This class is used to help specify molecule states */
class SIREDB_EXPORT match_mol_state
{
public:
    match_mol_state();
    ~match_mol_state();
    MatchMol operator==(const QString &state) const;
};

static const match_mol_state mol_state;

/** This class is used to help specify atom name matches */
class SIREDB_EXPORT match_atom_name
{
public:
    match_atom_name();
    match_atom_name(uint atmid);
    ~match_atom_name();
    MatchMRAData operator==(const QString &name) const;
    
private:
    int atm;
};

static const match_atom_name atom0_name(0);
static const match_atom_name atom1_name(1);
static const match_atom_name atom2_name(2);
static const match_atom_name atom3_name(3);

/** This class is used to help specify atom number matches */
class SIREDB_EXPORT match_atom_num
{
public:
    match_atom_num();
    match_atom_num(uint atmid);
    ~match_atom_num();
    MatchMRAData operator==(AtomNum num) const;
    
private:
    int atm;
};

static const match_atom_num atom0_num(0);
static const match_atom_num atom1_num(1);
static const match_atom_num atom2_num(2);
static const match_atom_num atom3_num(3);

/** This class is used to help specify single-atom name matches */
class SIREDB_EXPORT match_single_atom_name
{
public:
    match_single_atom_name();
    ~match_single_atom_name();
    MatchAtom operator==(const QString &name) const;
};

static const match_single_atom_name atom_name;

/** This class is used to help specify single-atom number matches */
class SIREDB_EXPORT match_single_atom_num
{
public:
    match_single_atom_num();
    ~match_single_atom_num();
    MatchAtom operator==(AtomNum num) const;
};

static const match_single_atom_num atom_num;

/** This class is used to help specify residue name matches */
class SIREDB_EXPORT match_res_name
{
public:
    match_res_name();
    match_res_name(uint atmid);
    ~match_res_name();
    MatchMRData operator==(const QString &name) const;
    
private:
    int atm;
};

static const match_res_name res0_name(0);
static const match_res_name res1_name(1);
static const match_res_name res2_name(2);
static const match_res_name res3_name(3);

/** This class is used to help specify residue number matches */
class SIREDB_EXPORT match_res_num
{
public:
    match_res_num();
    match_res_num(uint atmid);
    ~match_res_num();
    MatchMRData operator==(ResNum num) const;
    
private:
    int atm;
};

static const match_res_num res0_num(0);
static const match_res_num res1_num(1);
static const match_res_num res2_num(2);
static const match_res_num res3_num(3);

/** This class is used to help specify residue alias matches */
class SIREDB_EXPORT match_res_alias
{
public:
    match_res_alias();
    match_res_alias(uint atmid);
    ~match_res_alias();
    MatchMRData operator==(const QString &alias) const;
    
private:
    int atm;
};

static const match_res_alias res0_alias(0);
static const match_res_alias res1_alias(1);
static const match_res_alias res2_alias(2);
static const match_res_alias res3_alias(3);

/** This class is used to help specify matching inter residue bonds */
class SIREDB_EXPORT match_res_res_bond_via
{
public:
    match_res_res_bond_via();
    match_res_res_bond_via(uint atmid);
    ~match_res_res_bond_via();
    MatchMRData operator==(const QString &atomname) const;

private:
    int atm;
};

static const match_res_res_bond_via res0_res_bond_via(0);
static const match_res_res_bond_via res1_res_bond_via(1);
static const match_res_res_bond_via res2_res_bond_via(2);
static const match_res_res_bond_via res3_res_bond_via(3);

/** This class is used to help specify matching inter residue bonds */
class SIREDB_EXPORT match_no_res_res_bond_via
{
public:
    match_no_res_res_bond_via();
    match_no_res_res_bond_via(uint atmid);
    ~match_no_res_res_bond_via();
    MatchMRData operator==(const QString &atomname) const;

private:
    int atm;
};

static const match_no_res_res_bond_via no_res0_res_bond_via(0);
static const match_no_res_res_bond_via no_res1_res_bond_via(1);
static const match_no_res_res_bond_via no_res2_res_bond_via(2);
static const match_no_res_res_bond_via no_res3_res_bond_via(3);

/** This class is used to help specify single-residue name matches */
class SIREDB_EXPORT match_single_res_name
{
public:
    match_single_res_name();
    ~match_single_res_name();
    MatchRes operator==(const QString &name) const;
};

static const match_single_res_name res_name;

/** This class is used to help specify single-residue number matches */
class SIREDB_EXPORT match_single_res_num
{
public:
    match_single_res_num();
    ~match_single_res_num();
    MatchRes operator==(ResNum num) const;
};

static const match_single_res_num res_num;

/** This class is used to help specify single-residue alias matches */
class SIREDB_EXPORT match_single_res_alias
{
public:
    match_single_res_alias();
    ~match_single_res_alias();
    MatchRes operator==(const QString &alias) const;
};

static const match_single_res_alias res_alias;

/** This class is used to help specify matching single inter residue bonds */
class SIREDB_EXPORT match_single_res_res_bond_via
{
public:
    match_single_res_res_bond_via();
    ~match_single_res_res_bond_via();
    MatchRes operator==(const QString &atomname) const;
};

static const match_single_res_res_bond_via res_res_bond_via;

/** This class is used to help specify matching single inter residue bonds */
class SIREDB_EXPORT match_single_no_res_res_bond_via
{
public:
    match_single_no_res_res_bond_via();
    ~match_single_no_res_res_bond_via();
    MatchRes operator==(const QString &atomname) const;
};

static const match_single_no_res_res_bond_via no_res_res_bond_via;

/** This class is used to help specify atom type matches (full and string matches) */
class SIREDB_EXPORT match_atom_type
{
public:
    match_atom_type();
    match_atom_type(uint atmid);
    ~match_atom_type();
    
    MatchAtomTypeData operator==(const QString &atomtype) const;
    MatchAtomTypeData operator==(const AtomType &atomtype) const;

private:
    int atm;
};

static const match_atom_type atom0_type(0);
static const match_atom_type atom1_type(1);
static const match_atom_type atom2_type(2);
static const match_atom_type atom3_type(3);

/** This class is used to help specify atom element matches (full and string matches) */
class SIREDB_EXPORT match_atom_element
{
public:
    match_atom_element();
    match_atom_element(uint atmid);
    ~match_atom_element();
    
    MatchAtomTypeData operator==(const QString &element) const;
    MatchAtomTypeData operator==(const Element &element) const;
    MatchAtomTypeData operator==(uint nprotons) const;

private:
    int atm;
};

static const match_atom_element atom0_element(0);
static const match_atom_element atom1_element(1);
static const match_atom_element atom2_element(2);
static const match_atom_element atom3_element(3);

/** This class is used to help specify single-atom type matches (full and string matches) */
class SIREDB_EXPORT match_single_atom_type
{
public:
    match_single_atom_type();
    ~match_single_atom_type();
    
    MatchAtomType operator==(const QString &atomtype) const;
    MatchAtomType operator==(const AtomType &atomtype) const;
};

static const match_single_atom_type atom_type;

/** This class is used to help specify single-atom element matches (full and string matches) */
class SIREDB_EXPORT match_single_atom_element
{
public:
    match_single_atom_element();
    ~match_single_atom_element();
    
    MatchAtomType operator==(const QString &element) const;
    MatchAtomType operator==(const Element &element) const;
    MatchAtomType operator==(uint nprotons) const;
};

static const match_single_atom_element atom_element;

}

SIRE_END_HEADER

#endif
