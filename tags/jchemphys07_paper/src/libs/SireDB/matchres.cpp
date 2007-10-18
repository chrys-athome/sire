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

#include "matchres.h"

using namespace SireDB;

/** Null constructor */
MatchRes::MatchRes() : resnum(0)
{}

/** Copy constructor */
MatchRes::MatchRes(const MatchRes &other)
         : resname(other.resname), resalias(other.resalias), resnum(other.resnum),
           interresbonds(other.interresbonds),
           not_interresbonds(other.not_interresbonds)
{}

/** Destructor */
MatchRes::~MatchRes()
{}

/** Comparison operator */
bool MatchRes::operator==(const MatchRes &other) const
{
    return resnum == other.resnum and resname == other.resname and
           resalias == other.resalias and
           interresbonds == other.interresbonds and
           not_interresbonds == other.not_interresbonds;
}

/** Comparison operator */
bool MatchRes::operator!=(const MatchRes &other) const
{
    return resnum != other.resnum or resname != other.resname or
           resalias != other.resalias or
           interresbonds != other.interresbonds or
           not_interresbonds != other.not_interresbonds;
}

/** Return a string representation of the criteria */
QString MatchRes::toString() const
{
    QStringList parts;

    if (hasNameMatch())
        parts.append( QObject::tr("res_name == %1").arg(name()) );

    if (hasNumberMatch())
        parts.append( QObject::tr("res_num == %1").arg(number().toString()) );

    if (hasResResBonds())
        parts.append( QObject::tr("res_res_bonds_via == [%1]")
                          .arg(resResBonds().join(",")) );

    if (hasNoResResBonds())
        parts.append( QObject::tr("no_res_res_bonds_via == [%1]")
                          .arg(noResResBonds().join(",")) );

    if (hasAliasMatch())
        parts.append( QObject::tr("res_alias == %1").arg(alias()) );

    if (parts.isEmpty())
        return QObject::tr("res == *");
    else
        return parts.join(QObject::tr(" and "));
}

/** Return a string representation of the criteria, using the id number 'atm' */
QString MatchRes::toString(uint atm) const
{
    QStringList parts;

    if (hasNameMatch())
        parts.append( QObject::tr("res%1_name == %2").arg(atm).arg(name()) );

    if (hasNumberMatch())
        parts.append( QObject::tr("res%1_num == %2").arg(atm).arg(number().toString()) );

    if (hasResResBonds())
        parts.append( QObject::tr("res%1_res_bonds_via == [%2]")
                        .arg(atm).arg(resResBonds().join(",")) );

    if (hasNoResResBonds())
        parts.append( QObject::tr("no_res%1_res_bonds_via == [%2]")
                        .arg(atm).arg(noResResBonds().join(",")) );

    if (hasAliasMatch())
        parts.append( QObject::tr("res%1_alias == %2").arg(atm).arg(alias()) );

    if (parts.isEmpty())
        return QObject::tr("res%1 == *").arg(atm);
    else
        return parts.join(QObject::tr(" and "));
}

/** Does this contain any matching criteria? This will match everything if it is empty */
bool MatchRes::isEmpty() const
{
    return resname.isEmpty() and resnum == 0 and resalias.isEmpty() and
           interresbonds.isEmpty() and not_interresbonds.isEmpty();
}

/** Return whether or not this specifies the number of the residue */
bool MatchRes::hasNumberMatch() const
{
    return resnum != 0;
}

/** Return whether or not this specifies the name of the residue */
bool MatchRes::hasNameMatch() const
{
    return not resname.isEmpty();
}

/** Return whether or not this specifies the atoms that must be involved in
    inter-residue bonds */
bool MatchRes::hasResResBonds() const
{
    return not interresbonds.isEmpty();
}

/** Return whether or not this specifies the atoms that must not be involved in
    inter-residue bonds. */
bool MatchRes::hasNoResResBonds() const
{
    return not not_interresbonds.isEmpty();
}

/** Set the name of the residue that must be matched */
void MatchRes::setName(const QString &name)
{
    resname = name;
}

/** Return the name of the residue that must be matched - an empty
    string is returned if this matches any residue name. */
QString MatchRes::name() const
{
    return resname;
}

/** Set the number of the residue that must be matched */
void MatchRes::setNumber(ResNum number)
{
    resnum = number;
}

/** Return the number of the residue that must be matched - this returns
    0 if this matches any residue number */
ResNum MatchRes::number() const
{
    return resnum;
}

/** Add an atom that must be involved in an inter-residue bond. Note that
    this will remove the atom from the set that must not form inter-residue
    bonds, if it exists in that set. */
void MatchRes::addResResBond(const QString &atom)
{
    not_interresbonds.remove(atom);
    interresbonds.insert(atom);
}

/** Add an atom that must not be involved in an inter-residue bond. Note that
    this will remove the atom from the set that must form inter-residue bonds
    if it exists in that set. */
void MatchRes::addNoResResBond(const QString &atom)
{
    interresbonds.remove(atom);
    not_interresbonds.insert(atom);
}

/** Add a whole load of atoms that are known to be involved in inter-residue
    bonds. */
void MatchRes::addResResBonds(const QSet<QString> &atoms)
{
    not_interresbonds.subtract(atoms);
    interresbonds.unite(atoms);
}

/** Add a whole load of atoms that must not be involved in an inter-residue
    bond. */
void MatchRes::addNoResResBonds(const QSet<QString> &atoms)
{
    interresbonds.subtract(atoms);
    not_interresbonds.unite(atoms);
}

/** Return the list of atoms that must be involved in inter-residue bonds. Return
    an empty list if there are no atoms specified. */
QStringList MatchRes::resResBonds() const
{
    return interresbonds.toList();
}

/** Return the list of atoms that must not be involved in inter-residue bonds.
    Return an empty list if there are no atoms specified. */
QStringList MatchRes::noResResBonds() const
{
    return not_interresbonds.toList();
}

/** Return whether or not this has an alias match */
bool MatchRes::hasAliasMatch() const
{
    return not resalias.isEmpty();
}

/** Set the residue alias */
void MatchRes::setAlias(const QString &alias)
{
    resalias = alias;
}

/** Return the residue alias matching criterium */
QString MatchRes::alias() const
{
    return resalias;
}

/** and combine two sets of MatchRes criteria together. Note that
    the second match is taken if there is any contradiction, e.g.
    res_name == "ALA" and res_name == "ASP" would return a MatchRes
    with res_name == "ASP" */
MatchRes MatchRes::operator&&(const MatchRes &other) const
{
    MatchRes ret(*this);

    if (other.hasNameMatch())
        ret.setName( other.name() );

    if (other.hasNumberMatch())
        ret.setNumber( other.number() );

    if (other.hasAliasMatch())
        ret.setAlias( other.alias() );

    if (other.hasResResBonds())
    {
        for (QSet<QString>::const_iterator it = other.interresbonds.begin();
             it != other.interresbonds.end();
             ++it)
        {
            ret.addResResBond( *it );
        }
    }

    if (other.hasNoResResBonds())
    {
        for (QSet<QString>::const_iterator it = other.not_interresbonds.begin();
             it != other.not_interresbonds.end();
             ++it)
        {
            ret.addNoResResBond( *it );
        }
    }

    return ret;
}
