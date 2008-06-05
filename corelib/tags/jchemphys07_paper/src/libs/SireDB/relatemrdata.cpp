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

#include "relatemrdata.h"

using namespace SireDB;

/** Null constructor */
RelateMRData::RelateMRData()
{}

/** Construct the object containing the passed data */
RelateMRData::RelateMRData(NameID mname, NameID mgroup, NameID mstate, 
                           NameID ralias, NameID rname, ResNum rnum, 
                           const NameIDSet &rbonds, const NameIDSet &rnobonds)
             : molname(mname), molgroup(mgroup), molstate(mstate), resalias(ralias),
               resname(rname), resnum(rnum), resbonds(rbonds), resnobonds(rnobonds)
{}
    
/** Copy constructor */
RelateMRData::RelateMRData(const RelateMRData &other)
             : molname(other.molname), molgroup(other.molgroup), molstate(other.molstate),
               resalias(other.resalias), resname(other.resname), resnum(other.resnum), 
               resbonds(other.resbonds), resnobonds(other.resnobonds)
{}

/** Destructor */
RelateMRData::~RelateMRData()
{}

/** Return whether or not this is empty (matches everything) */
bool RelateMRData::isEmpty() const
{
    return molname == 0 and molgroup == 0 and molstate == 0 and 
           resalias == 0 and resname == 0 and resnum == 0 and
           resbonds.isEmpty() and resnobonds.isEmpty();
}

/** Return this data formatted to form part of an SQL query string for atom 'n'
    that will find the exact match of this data */
QString RelateMRData::toQueryString() const
{
    return QString("MolName = %1 and MolGroup = %2 and MolState = %3 and "
                   "ResAlias = %4 and ResName = %5 and ResNum = %6 and "
                   "ResBonds = \"%7\" and ResNoBonds = \"%8\"")
            .arg(molname).arg(molgroup).arg(molstate).arg(resalias).arg(resname)
            .arg(resnum.toString(), convertToString(resbonds), convertToString(resnobonds) );
}

/** Return this data formatted to form part of an SQL query string for atom 'n'
    that will search for all matches that are like this match */
QString RelateMRData::toSearchString(NameIDSet resaliases) const
{
    if (resalias != 0 and not resaliases.contains(resalias))
        resaliases.insert(resalias);
    
    if (resaliases.isEmpty())
    {
        return QString("(MolName = %1 or MolName = 0) and "
                       "(MolGroup = %2 or MolGroup = 0) and "
                       "(MolState = %3 or MolState = 0) and "
                       "(ResAlias = 0) and "
                       "(ResName = %4 or ResName = 0) and "
                       "(ResNum = %5 or ResNum = 0)")
                  .arg(molname).arg(molgroup).arg(molstate)
                  .arg(resname).arg(resnum.toString());
    }
    else
    {
        QStringList parts;
        for (NameIDSet::const_iterator it = resaliases.begin();
             it != resaliases.end();
             ++it)
        {
            if (*it != 0)
                parts.append( QString("ResAlias = %1").arg(*it)  );
        }
        
        return QString("(MolName = %1 or MolName = 0) and "
                       "(MolGroup = %2 or MolGroup = 0) and "
                       "(MolState = %3 or MolState = 0) and "
                       "( (%4) or ( (ResName = %5 or ResName = 0) and "
                                   "(ResNum = %6 or ResNum = 0) ) )")
                  .arg(molname).arg(molgroup).arg(molstate)
                  .arg( parts.join(" or ") ).arg(resname).arg(resnum.toString());
    }
}

/** Return this data formatted to form part of an SQL insert string for atom 'n' */
QString RelateMRData::toInsertString() const
{
    return QString("%1, %2, %3, %4, %5, %6, \"%7\", \"%8\"")
              .arg(molname).arg(molgroup).arg(molstate).arg(resalias).arg(resname)
              .arg(resnum.toString(), convertToString(resbonds), convertToString(resnobonds) );
}

/** Return the score value of this match - this is used to rank matches according to 
    how specific they are. The score is based on the following desired ordering;
    
    All matches with molecule name are more specific than matches without the molecule name
    
    then
    
    All matches with a molecule group are more specific than matches without a molecule group
    
    then
    
    All matches with a molecule state are more specific than matches without a molecule state
    
    then
    
    All matches with residue number are more specific than anything else
    
    then
    
    All matches with residue name are more specific than anything else
    
    then 
    
    bonus points for specifying atoms involved or not in interresidue bonds
    
    then
    
    subtract points if this is an alias (so that an exact match overrides an 
    alias match)
*/
int RelateMRData::score() const
{
    return (65536 * int(molname != 0)) + (32768 * int(molgroup != 0)) +
           (16384 * int(molstate != 0)) + (4096 * int(resnum != 0)) + 
           (1024 * int(resname != 0)) + resbonds.count() + resnobonds.count() +
           (-2 * int(resalias == 0));
}

/** Return whether or not this RelateMRAData has compatible bonding with 
    'bondatoms' (e.g. we contain all of the atoms specified in 'bondatoms')
    and 'nobondatoms' (e.g. we don't contain any bonds specified in 'nobondatoms') */
bool RelateMRData::hasCompatibleBonds(const NameIDSet &bondatoms, 
                                      const NameIDSet &nobondatoms) const
{
    for (NameIDSet::const_iterator it = bondatoms.begin();
         it != bondatoms.end();
         ++it)
    {
        if (not resbonds.contains(*it))
            return false;
    }
    
    for (NameIDSet::const_iterator it = nobondatoms.begin();
         it != nobondatoms.end();
         ++it)
    {
        if (resbonds.contains(*it))
            return false;
    }
    
    //everything is ok
    return true;
}

/** Return the NameID of the molecule's name */
NameID RelateMRData::moleculeName() const
{
    return molname;
}

/** Return the NameID of the molecule's group */
NameID RelateMRData::moleculeGroup() const
{
    return molgroup;
}

/** Return the NameID of the molecule's state */
NameID RelateMRData::moleculeState() const
{
    return molstate;
}
    
/** Return the NameID of the residues' alias */
NameID RelateMRData::residueAlias() const
{
    return resalias;
}
    
/** Return the NameID of the residue's name */
NameID RelateMRData::residueName() const
{
    return resname;
}

/** Return the residue's number */
ResNum RelateMRData::residueNumber() const
{
    return resnum;
}

/** Return the set of NameIDs of the atoms involved in inter-residue bonding */
const NameIDSet& RelateMRData::bondedAtoms() const
{
    return resbonds;
}

/** Return the set of NameIDs of the atoms not involved in inter-residue bonding */
const NameIDSet& RelateMRData::nonBondedAtoms() const
{
    return resnobonds;
}
