/********************************************\
  *
  *  Sire - Molecular Simulation Framework
  *
  *  Copyright (C) 2008  Christopher Woods
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

#include "molid.h"

#include "molgroup.h"
#include "molgroups.h"

#include "mgidx.h"

#include "mover.hpp"

#include "SireMol/errors.h"
#include "SireError/errors.h"

#include "tostring.h"

using namespace SireMol;

///////
/////// Implementation of MolID
///////

/** Map this ID to the matching molecules in the groups held in 
    'molgroupsbase'
    
    \throw SireMol::missing_molecule
*/
QList<MolNum> MolID::map(const MolGroupsBase &molgroupsbase) const
{
    QList<MolNum> molnums;
    
    int ngroups = molgroupsbase.nGroups();
    
    //loop over all groups in the set
    for (MGIdx i(0); i<ngroups; ++i)
    {
        try
        {
            //add on the matching molecules in this group
            molnums += this->map( molgroupsbase[i] );
        }
        catch(...)
        {
            //there were no matching molecules
        }
    }
    
    if (molnums.isEmpty())
        throw SireMol::missing_molecule( QObject::tr(
            "There were no molecules in the group that matched the ID "
            "%1.").arg(this->toString()), CODELOC );
            
    //now remove duplicates - need to preserve the order
    //in which the molecules were added to this list...
    if (molnums.count() == 1)
        return molnums;
        
    QSet<MolNum> added_molnums;
    
    QMutableListIterator<MolNum> it(molnums);
    
    while (it.hasNext())
    {
        if (added_molnums.contains(it.next()))
            it.remove();
            
        else
            added_molnums.insert(it.value());
    }
    
    return molnums;
}

///////
/////// Implementation of MolNumList
///////

/** Null constructor */
MolNumList::MolNumList() : MolID()
{}

/** Construct from a passed list of numbers */
MolNumList::MolNumList(const QList<MolNum> &molnumbers)
          : MolID(), molnums(molnumbers)
{}
  
/** Copy constructor */  
MolNumList::MolNumList(const MolNumList &other)
          : MolID(other), molnums(other.molnums)
{}
  
/** Destructor */
MolNumList::~MolNumList()
{}
    
bool MolNumList::isNull() const
{
    return molnums.isEmpty();
}

uint MolNumList::hash() const
{
    if (molnums.isEmpty())
        return 0;
    else
        return molnums.first().hash() * molnums.count();
}

QString MolNumList::toString() const
{
    return QString("MolNumList( %1 )").arg( Sire::toString(molnums) );
}

MolNumList& MolNumList::operator=(const MolNumList &other)
{
    molnums = other.molnums;
    MolID::operator=(other);
    
    return *this;
}

bool MolNumList::operator==(const MolNumList &other) const
{
    return molnums == other.molnums;
}

bool MolNumList::operator!=(const MolNumList &other) const
{
    return molnums != other.molnums;
}

/** Ensure that all of the numbers exist in 'molgroup'

    \throw SireMol::missing_molecule
    \throw SireError::invalid_arg
*/
QList<MolNum> MolNumList::map(const MolGroup &molgroup) const
{
    if (molnums.isEmpty())
        throw SireError::invalid_arg( QObject::tr(
            "You cannot pass an empty set of molecule numbers!"), 
                CODELOC );

    foreach (MolNum molnum, molnums)
    {
        molgroup.assertContains(molnum);
    }
    
    return molnums;
}

/** Ensure that all of the numbers exist in 'molgroups'

    \throw SireMol::missing_group
    \throw SireError::invalid_arg
*/
QList<MolNum> MolNumList::map(const MolGroupsBase &molgroups) const
{
    if (molnums.isEmpty())
        throw SireError::invalid_arg( QObject::tr(
            "You cannot pass an empty set of molecule numbers!"), 
                CODELOC );

    foreach (MolNum molnum, molnums)
    {
        molgroups.assertContains(molnum);
    }
    
    return molnums;
}
