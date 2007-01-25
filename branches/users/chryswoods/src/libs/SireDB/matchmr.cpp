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

#include "matchmr.h"

using namespace SireDB;

/** Null constructor */
MatchMR::MatchMR()
{}

/** Construct a Match containing molecule matching criteria */
MatchMR::MatchMR(const MatchMol &matchmol)
        : moldata(matchmol)
{}

/** Construct a match containing residue matching criteria */
MatchMR::MatchMR(const MatchRes &matchres)
        : resdata(matchres)
{}

/** Construct a match containing both molecule and residue matching criteria */
MatchMR::MatchMR(const MatchMol &matchmol, const MatchRes &matchres)
        : moldata(matchmol), resdata(matchres)
{}

/** Copy constructor */
MatchMR::MatchMR(const MatchMR &other)
        : moldata(other.moldata), resdata(other.resdata)
{}

/** Destructor */
MatchMR::~MatchMR()
{}

/** Return a string representation of the matching criteria */
QString MatchMR::toString() const
{
    QStringList parts;
    
    if (not moldata.isEmpty())
        parts.append( moldata.toString() );
        
    if (not resdata.isEmpty())
        parts.append( resdata.toString() );
        
    if (parts.isEmpty())
        return "*";
    else
        return parts.join( QObject::tr(" and ") );
}

/** Return whether or not this is an empty match (matches everything) */
bool MatchMR::isEmpty() const
{
    return moldata.isEmpty() and resdata.isEmpty();
}

/** Return the molecule matching criteria */
const MatchMol& MatchMR::molecule() const
{
    return moldata;
}

/** Return the residue matching criteria */
const MatchRes& MatchMR::residue() const
{
    return resdata;
}

/** Return whether this has molecule matching criteria */
bool MatchMR::hasMoleculeCriteria() const
{
    return not moldata.isEmpty();
}

/** Return whether or not this has residue matching criteria */
bool MatchMR::hasResidueCriteria() const
{
    return not resdata.isEmpty();
}

/** 'and' combine two MatchMR objects together. If there is any contradiction 
    then the data from the second object is used, e.g. mol_name == "p38"
    and mol_name == "calix" will return mol_name == "calix" */
MatchMR MatchMR::operator&&(const MatchMR &other) const
{
    MatchMR ret;
    ret.moldata = moldata and other.moldata;
    ret.resdata = resdata and other.resdata;
    return ret;
}

MatchMR SIREDB_EXPORT SireDB::operator&&(const MatchMR &matchmr, const MatchMol &matchmol)
{
    return ( matchmr and MatchMR(matchmol) );
}

MatchMR SIREDB_EXPORT SireDB::operator&&(const MatchMol &matchmol, const MatchMR &matchmr)
{
    return ( MatchMR(matchmol) and matchmr );
}

MatchMR SIREDB_EXPORT SireDB::operator&&(const MatchMR &matchmr, const MatchRes &matchres)
{
    return ( matchmr and MatchMR(matchres) );
}

MatchMR SIREDB_EXPORT SireDB::operator&&(const MatchRes &matchres, const MatchMR &matchmr)
{
    return ( MatchMR(matchres) and matchmr );
}

MatchMR SIREDB_EXPORT SireDB::operator&&(const MatchMol &matchmol, const MatchRes &matchres)
{
    return ( MatchMR(matchmol) and MatchMR(matchres) );
}

MatchMR SIREDB_EXPORT SireDB::operator&&(const MatchRes &matchres, const MatchMol &matchmol)
{
    return ( MatchMR(matchres) and MatchMR(matchmol) );
}
