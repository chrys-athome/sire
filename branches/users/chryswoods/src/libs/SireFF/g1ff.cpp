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

#include "g1ff.h"

#include "SireStream/datastream.h"
#include "SireStream/shareddatastream.h"

using namespace SireFF;
using namespace SireMol;
using namespace SireStream;

static const RegisterMetaType r_g1ff( MAGIC_ONLY, "SireFF::G1FF" );

/** Serialise to a binary datastream */
QDataStream SIREFF_EXPORT &operator<<(QDataStream &ds,
                                      const G1FF &g1ff)
{
    writeHeader(ds, r_g1ff, 1);
    
    SharedDataStream sds(ds);
    
    sds << g1ff.molgroup << static_cast<const FF&>(g1ff);
    
    return ds;
}

/** Extract from a binary datastream */
QDataStream SIREFF_EXPORT &operator>>(QDataStream &ds,
                                      G1FF &g1ff)
{
    VersionID v = readHeader(ds, r_g1ff);
    
    if (v == 1)
    {
        SharedDataStream sds(ds);
        sds >> g1ff.molgroup >> static_cast<FF&>(g1ff);
    }
    else
        throw version_error(v, "1", r_g1ff, CODELOC);
        
    return ds;
}

/** Constructor */
G1FF::G1FF() : FF()
{}

/** Copy constructor */
G1FF::G1FF(const G1FF &other) : FF(other), molgroup(other.molgroup)
{}

/** Copy assignment operator */
G1FF& G1FF::operator=(const G1FF &other)
{
    molgroup = other.molgroup;
    FF::operator=(other);
    
    return *this;
}

/** Assert that this forcefield contains the group with number 'mgnum'

    \throw SireMol::missing_group
*/
void G1FF::assertContains(MGNum mgnum) const
{
    if (molgroup.number() != mgnum)
        throw SireMol::missing_group( QObject::tr(
            "The forcefield %1 (%2) does not contain a group with "
            "number %3. The only group it contains has number %4.")
                .arg(this->name()).arg(this->number())
                .arg(mgnum).arg(molgroup.number()), CODELOC );
}

/** Return a reference to the group with number 'mgnum'

    \throw SireMol::missing_group
*/
MolGroup& G1FF::getGroup(MGNum mgnum)
{
    G1FF::assertContains(mgnum);
    return molgroup;
}

/** Return a reference to the group with number 'mgnum'

    \throw SireMol::missing_group
*/
const MolGroup& G1FF::getGroup(MGNum mgnum) const
{
    G1FF::assertContains(mgnum);
    return molgroup;
}

/** Return pointers to the groups with numbers 'mgnums'

    \throw SireMol::missing_group
*/
void G1FF::getGroups(const QList<MGNum>&,
                     QVarLengthArray<MolGroup*,10>&)
{
    throw SireError::program_bug( QObject::tr(
        "I shouldn't be using this function!"), CODELOC );
}

/** Return const pointers to the groups with number 'mgnums'

    \throw SireMol::missing_group
*/
void G1FF::getGroups(const QList<MGNum> &mgnums,
                     QVarLengthArray<const MolGroup*,10> &groups) const
{
    groups.clear();
    
    foreach (MGNum mgnum, mgnums)
    {
        G1FF::assertContains(mgnum);
        groups.append( &molgroup );
    }
}

/** Return pointers to all of the groups in this forcefield */
QHash<MGNum,MolGroup*> G1FF::getGroups()
{
    throw SireError::program_bug( QObject::tr(
        "I shouldn't be using this function!"), CODELOC );

    return QHash<MGNum,MolGroup*>();
}

/** Return pointers to all of the groups in this forcefield */
QHash<MGNum,const MolGroup*> G1FF::getGroups() const
{
    QHash<MGNum,const MolGroup*> groups;
    groups.reserve(1);
    groups.insert( molgroup.number(), &molgroup );
    
    return groups;
}

