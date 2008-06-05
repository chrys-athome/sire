/********************************************\
  *
  *  Sire - Molecular Simulation Framework
  *
  *  Copyright (C) 2007  Christopher Woods
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

#include "matchhelpers.h"
#include "matchmrdata.h"
#include "matchmradata.h"
#include "matchatomtype.h"
#include "matchatomtypedata.h"

using namespace SireDB;

//////////////
////////////// Implementation of match_mol_name
//////////////

match_mol_name::match_mol_name()
{}

match_mol_name::~match_mol_name()
{}

MatchMol match_mol_name::operator==(const QString &name) const
{
    if (name.isNull())
        return MatchMol();
    else
    {
        MatchMol moldata;
        moldata.setName(name);
        return moldata;
    }
}

//////////////
////////////// Implementation of match_mol_group
//////////////

match_mol_group::match_mol_group()
{}

match_mol_group::~match_mol_group()
{}

MatchMol match_mol_group::operator==(const QString &group) const
{
    if (group.isNull())
        return MatchMol();
    else
    {
        MatchMol moldata;
        moldata.setGroup(group);
        return moldata;
    }
}

//////////////
////////////// Implementation of match_mol_state
//////////////

match_mol_state::match_mol_state()
{}

match_mol_state::~match_mol_state()
{}

MatchMol match_mol_state::operator==(const QString &state) const
{
    if (state.isNull())
        return MatchMol();
    else
    {
        MatchMol moldata;
        moldata.setState(state);
        return moldata;
    }
}

//////////////
////////////// Implementation of match_atom_name
//////////////

match_atom_name::match_atom_name() : atm(-1)
{}

match_atom_name::match_atom_name(uint atmid) : atm(atmid)
{}

match_atom_name::~match_atom_name()
{}

MatchMRAData match_atom_name::operator==(const QString &name) const
{
    if (name.isNull())
        return MatchMRAData();
    else
    {
        MatchAtom atomdata;
        atomdata.setName(name);

        if (atm < 0)
            return MatchMRAData(atomdata);
        else
            return MatchMRAData(atm, atomdata);
    }
}

//////////////
////////////// Implementation of match_atom_num
//////////////

match_atom_num::match_atom_num() : atm(-1)
{}

match_atom_num::match_atom_num(uint atmid) : atm(atmid)
{}

match_atom_num::~match_atom_num()
{}

MatchMRAData match_atom_num::operator==(AtomNum num) const
{
    if (num == 0)
        return MatchMRAData();
    else
    {
        MatchAtom atomdata;
        atomdata.setNumber(num);

        if (atm < 0)
            return MatchMRAData(atomdata);
        else
            return MatchMRAData(atm, atomdata);
    }
}

//////////////
////////////// Implementation of match_single_atom_name
//////////////

match_single_atom_name::match_single_atom_name()
{}

match_single_atom_name::~match_single_atom_name()
{}

MatchAtom match_single_atom_name::operator==(const QString &name) const
{
    if (name.isNull())
        return MatchAtom();
    else
    {
        MatchAtom atomdata;
        atomdata.setName(name);
        return atomdata;
    }
}

//////////////
////////////// Implementation of match_single_atom_num
//////////////

match_single_atom_num::match_single_atom_num()
{}

match_single_atom_num::~match_single_atom_num()
{}

MatchAtom match_single_atom_num::operator==(AtomNum num) const
{
    if (num == 0)
        return MatchAtom();
    else
    {
        MatchAtom atomdata;
        atomdata.setNumber(num);
        return atomdata;
    }
}

//////////////
////////////// Implementation of match_res_name
//////////////

match_res_name::match_res_name() : atm(-1)
{}

match_res_name::match_res_name(uint atmid) : atm(atmid)
{}

match_res_name::~match_res_name()
{}

MatchMRData match_res_name::operator==(const QString &name) const
{
    if (name.isNull())
        return MatchMRData();
    else
    {
        MatchRes resdata;
        resdata.setName(name);

        if (atm < 0)
            return MatchMRData(resdata);
        else
            return MatchMRData(atm, resdata);
    }
}

//////////////
////////////// Implementation of match_res_num
//////////////

match_res_num::match_res_num() : atm(-1)
{}

match_res_num::match_res_num(uint atmid) : atm(atmid)
{}

match_res_num::~match_res_num()
{}

MatchMRData match_res_num::operator==(ResNum num) const
{
    if (num == 0)
        return MatchMRData();
    else
    {
        MatchRes resdata;
        resdata.setNumber(num);

        if (atm < 0)
            return MatchMRData(resdata);
        else
            return MatchMRData(atm, resdata);
    }
}

//////////////
////////////// Implementation of match_res_alias
//////////////

match_res_alias::match_res_alias() : atm(-1)
{}

match_res_alias::match_res_alias(uint atmid) : atm(atmid)
{}

match_res_alias::~match_res_alias()
{}

MatchMRData match_res_alias::operator==(const QString &alias) const
{
    if (alias.isEmpty())
        return MatchMRData();
    else
    {
        MatchRes resdata;
        resdata.setAlias(alias);

        if (atm < 0)
            return MatchMRData(resdata);
        else
            return MatchMRData(atm, resdata);
    }
}

//////////////
////////////// Implementation of match_res_res_bond_via
//////////////

match_res_res_bond_via::match_res_res_bond_via() : atm(-1)
{}

match_res_res_bond_via::match_res_res_bond_via(uint atmid) : atm(atmid)
{}

match_res_res_bond_via::~match_res_res_bond_via()
{}

MatchMRData match_res_res_bond_via::operator==(const QString &atomname) const
{
    if (atomname.isNull())
      return MatchMRData();
    else
    {
        MatchRes resdata;
        resdata.addResResBond(atomname);

        if (atm < 0)
            return MatchMRData(resdata);
        else
            return MatchMRData(atm, resdata);
    }
}

//////////////
////////////// Implementation of no_res_res_bond_via
//////////////

match_no_res_res_bond_via::match_no_res_res_bond_via() : atm(-1)
{}

match_no_res_res_bond_via::match_no_res_res_bond_via(uint atmid) : atm(atmid)
{}

match_no_res_res_bond_via::~match_no_res_res_bond_via()
{}

MatchMRData match_no_res_res_bond_via::operator==(const QString &atomname) const
{
    if (atomname.isNull())
        return MatchMRData();
    else
    {
        MatchRes resdata;
        resdata.addNoResResBond(atomname);

        if (atm < 0)
            return MatchMRData(resdata);
        else
            return MatchMRData(atm, resdata);
    }
}

//////////////
////////////// Implementation of match_single_res_name
//////////////

match_single_res_name::match_single_res_name()
{}

match_single_res_name::~match_single_res_name()
{}

MatchRes match_single_res_name::operator==(const QString &name) const
{
    if (name.isNull())
        return MatchRes();
    else
    {
        MatchRes resdata;
        resdata.setName(name);
        return resdata;
    }
}

//////////////
////////////// Implementation of match_single_res_num
//////////////

match_single_res_num::match_single_res_num()
{}

match_single_res_num::~match_single_res_num()
{}

MatchRes match_single_res_num::operator==(ResNum num) const
{
    if (num == 0)
        return MatchRes();
    else
    {
        MatchRes resdata;
        resdata.setNumber(num);
        return resdata;
    }
}

//////////////
////////////// Implementation of match_single_res_alias
//////////////

match_single_res_alias::match_single_res_alias()
{}

match_single_res_alias::~match_single_res_alias()
{}

MatchRes match_single_res_alias::operator==(const QString &alias) const
{
    if (alias.isEmpty())
        return MatchRes();
    else
    {
        MatchRes resdata;
        resdata.setAlias(alias);
        return resdata;
    }
}

//////////////
////////////// Implementation of match_single_res_res_bond_via
//////////////

match_single_res_res_bond_via::match_single_res_res_bond_via()
{}

match_single_res_res_bond_via::~match_single_res_res_bond_via()
{}

MatchRes match_single_res_res_bond_via::operator==(const QString &atomname) const
{
    if (atomname.isNull())
      return MatchRes();
    else
    {
        MatchRes resdata;
        resdata.addResResBond(atomname);
        return resdata;
    }
}

//////////////
////////////// Implementation of no_single_res_res_bond_via
//////////////

match_single_no_res_res_bond_via::match_single_no_res_res_bond_via()
{}

match_single_no_res_res_bond_via::~match_single_no_res_res_bond_via()
{}

MatchRes match_single_no_res_res_bond_via::operator==(const QString &atomname) const
{
    if (atomname.isNull())
        return MatchRes();
    else
    {
        MatchRes resdata;
        resdata.addNoResResBond(atomname);
        return resdata;
    }
}

//////////////
////////////// Implementation of match_atom_type
//////////////

match_atom_type::match_atom_type() : atm(-1)
{}

match_atom_type::match_atom_type(uint atmid) : atm(atmid)
{}

match_atom_type::~match_atom_type()
{}

MatchAtomTypeData match_atom_type::operator==(const QString &atomtype) const
{
    if (atomtype.isNull())
        return MatchAtomTypeData();
    else
    {
        MatchAtomTypeData atomdata;
        
        if (atm < 0)
            atomdata.setAtomType(atomtype);
        else
            atomdata.setAtomType(atm, atomtype);
        
        return atomdata;
    }
}

MatchAtomTypeData match_atom_type::operator==(const AtomType &atomtype) const
{
    if (atomtype.isWild())
        return MatchAtomTypeData();
    else
    {
        MatchAtomTypeData atomdata;
        
        if (atm < 0)
            atomdata.setAtomType(atomtype);
        else
            atomdata.setAtomType(atm, atomtype);
        
        return atomdata;
    }
}

//////////////
////////////// Implementation of match_atom_element
//////////////

match_atom_element::match_atom_element() : atm(-1)
{}

match_atom_element::match_atom_element(uint atmid) : atm(atmid)
{}

match_atom_element::~match_atom_element()
{}

MatchAtomTypeData match_atom_element::operator==(const QString &element) const
{
    if (element.isNull())
        return MatchAtomTypeData();
    else
    {
        MatchAtomTypeData atomdata;
        
        if (atm < 0)
            atomdata.setElement(element);
        else
            atomdata.setElement(atm, element);
        
        return atomdata;
    }
}

MatchAtomTypeData match_atom_element::operator==(const Element &element) const
{
    if (element.nProtons() == 0)
        return MatchAtomTypeData();
    else
    {
        MatchAtomTypeData atomdata;
        
        if (atm < 0)
            atomdata.setElement(element);
        else
            atomdata.setElement(atm, element);
        
        return atomdata;
    }
}

MatchAtomTypeData match_atom_element::operator==(uint nprotons) const
{
    if (nprotons == 0)
        return MatchAtomTypeData();
    else
    {
        MatchAtomType atomdata;
        atomdata.setElement(nprotons);
        return atomdata;
    }
}

//////////////
////////////// Implementation of match_single_atom_type
//////////////

match_single_atom_type::match_single_atom_type()
{}

match_single_atom_type::~match_single_atom_type()
{}

MatchAtomType match_single_atom_type::operator==(const QString &atomtype) const
{
    if (atomtype.isNull())
        return MatchAtomType();
    else
    {
        MatchAtomType atomdata;
        atomdata.setType(atomtype);
        return atomdata;
    }
}

MatchAtomType match_single_atom_type::operator==(const AtomType &atomtype) const
{
    if (atomtype.isWild())
        return MatchAtomType();
    else
    {
        MatchAtomType atomdata;
        atomdata.setType(atomtype);
        return atomdata;
    }
}

//////////////
////////////// Implementation of match_single_atom_element
//////////////

match_single_atom_element::match_single_atom_element()
{}

match_single_atom_element::~match_single_atom_element()
{}

MatchAtomType match_single_atom_element::operator==(const QString &element) const
{
    if (element.isNull())
        return MatchAtomType();
    else
    {
        MatchAtomType atomdata;
        atomdata.setElement(element);
        return atomdata;
    }
}

MatchAtomType match_single_atom_element::operator==(const Element &element) const
{
    if (element.nProtons() == 0)
        return MatchAtomType();
    else
    {
        MatchAtomType atomdata;
        atomdata.setElement(element);
        return atomdata;
    }
}

MatchAtomType match_single_atom_element::operator==(uint nprotons) const
{
    if (nprotons == 0)
        return MatchAtomType();
    else
    {
        MatchAtomType atomdata;
        atomdata.setElement(nprotons);
        return atomdata;
    }
}
