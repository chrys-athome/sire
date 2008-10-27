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

#include <QSet>

#include "molmolid.h"
#include "molnum.h"

#include "molecules.h"
#include "moleculegroup.h"
#include "moleculegroups.h"

using boost::tuple;

using namespace SireMol;

/** Constructor */
MolMolID::MolMolID() : MolID()
{}

/** Construct from the passed pair of IDs */
MolMolID::MolMolID(const MolID &mol0, const MolID &mol1)
         : MolID(), molid0(mol0), molid1(mol1)
{}

/** Construct from the passed pair of IDs */
MolMolID::MolMolID(const boost::tuple<MolIdentifier,MolIdentifier> &molids)
         : MolID(), molid0(molids.get<0>()), molid1(molids.get<1>())
{}

/** Copy constructor */
MolMolID::MolMolID(const MolMolID &other)
         : MolID(), molid0(other.molid0), molid1(other.molid1)
{}

/** Destructor */
MolMolID::~MolMolID()
{}

/** Copy assignment operator */
MolMolID& MolMolID::operator=(const MolMolID &other)
{
    molid0 = other.molid0;
    molid1 = other.molid1;
    MolID::operator=(other);
    
    return *this;
}

/** Comparison operator */
bool MolMolID::operator==(const MolMolID &other) const
{
    return ( molid0 == other.molid0 and
             molid1 == other.molid1 ) or
           
           ( molid0 == other.molid1 and
             molid1 == other.molid0 );
}

/** Comparison operator */
bool MolMolID::operator!=(const MolMolID &other) const
{
    return not this->operator==(other);
}

/** Comparison operator */
bool MolMolID::operator==(const SireID::ID &other) const
{
    return SireID::ID::compare<MolMolID>(*this, other);
}

/** Comparison operator */
bool MolMolID::operator!=(const SireID::ID &other) const
{
    return not this->operator==(other);
}

/** Hash this ID */
uint MolMolID::hash() const
{
    return molid0.hash() + molid1.hash();
}

/** Return string representation of this ID */
QString MolMolID::toString() const
{
    if (molid0.isNull())
        return molid1.toString();
    
    else if (molid1.isNull())
        return molid0.toString();
        
    else
        return QObject::tr("%1 and %2").arg(molid0.toString(), molid1.toString());
}

/** Return whether or not this is null */
bool MolMolID::isNull() const
{
    return molid0.isNull() and molid1.isNull();
}

static QList<MolNum> combine(QList<MolNum> molnums0,
                             const QList<MolNum> &molnums1)
{
    QSet<MolNum> mols = molnums0.toSet();
    
    foreach (MolNum molnum, molnums1)
    {
        if (not mols.contains(molnum))
            molnums0.append(molnum);
    }
    
    return molnums0;
}

/** Map this ID to a list of matching molecule numbers */
QList<MolNum> MolMolID::map(const Molecules &molecules) const
{
    if (molid0.isNull())
        return molid1.map(molecules);
    
    else if (molid1.isNull())
        return molid0.map(molecules);
        
    else
        return ::combine( molid0.map(molecules), molid1.map(molecules) );
}

/** Map this ID to a list of matching molecule numbers */
QList<MolNum> MolMolID::map(const MoleculeGroup &molgroup) const
{
    if (molid0.isNull())
        return molid1.map(molgroup);
    
    else if (molid1.isNull())
        return molid0.map(molgroup);
        
    else
        return ::combine( molid0.map(molgroup), molid1.map(molgroup) );
}

/** Map this ID to a list of matching molecule numbers */
QList<MolNum> MolMolID::map(const MolGroupsBase &molgroups) const
{
    if (molid0.isNull())
        return molid1.map(molgroups);
    
    else if (molid1.isNull())
        return molid0.map(molgroups);
        
    else
        return ::combine( molid0.map(molgroups), molid1.map(molgroups) );
}
