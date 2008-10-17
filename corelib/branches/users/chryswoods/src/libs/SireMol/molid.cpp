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
#include "molidx.h"
#include "molnum.h"
#include "molname.h"

#include "moleculegroup.h"
#include "moleculegroups.h"

#include "mgidx.h"

#include "mover.hpp"

#include "SireBase/incremint.h"

#include "SireMol/errors.h"
#include "SireError/errors.h"

#include "tostring.h"

using namespace SireMol;
using namespace SireBase;

///////
/////// Implementation of MolID
///////

MolID::MolID() : SireID::ID()
{}

MolID::MolID(const MolID &other) : SireID::ID(other)
{}

MolID::~MolID()
{}

///////
/////// Implementation of MolIdx
///////

MolIdx::MolIdx() : SireID::Index_T_<MolIdx>(), MolID()
{}

MolIdx::MolIdx(qint32 idx) : SireID::Index_T_<MolIdx>(idx), MolID()
{}

MolIdx::MolIdx(const MolIdx &other) : SireID::Index_T_<MolIdx>(other), MolID(other)
{}

MolIdx::~MolIdx()
{}

QList<MolNum> MolIdx::map(const MoleculeGroup &molgroup) const
{
    return molgroup.map(*this);
}

QList<MolNum> MolIdx::map(const MolGroupsBase &molgroups) const
{
    return molgroups.map(*this);
}

///////
/////// Implementation of MolNum
///////

MolNum::MolNum() : SireID::Number(), MolID()
{}

MolNum::MolNum(quint32 num) : SireID::Number(num), MolID()
{}

MolNum::MolNum(const MolNum &other) : SireID::Number(other), MolID(other)
{}

MolNum::~MolNum()
{}

static Incremint last_num(0);

MolNum MolNum::getUniqueNumber()
{
    return MolNum( last_num.increment() );
}

QList<MolNum> MolNum::map(const MoleculeGroup &molgroup) const
{
    return molgroup.map(*this);
}

QList<MolNum> MolNum::map(const MolGroupsBase &molgroups) const
{
    return molgroups.map(*this);
}

///////
/////// Implementation of MolName
///////

MolName::MolName() : SireID::Name(), MolID()
{}

MolName::MolName(const QString &name) : SireID::Name(name), MolID()
{}

MolName::MolName(const MolName &other) : SireID::Name(other), MolID(other)
{}

MolName::~MolName()
{}

QList<MolNum> MolName::map(const MoleculeGroup &molgroup) const
{
    return molgroup.map(*this);
}

QList<MolNum> MolName::map(const MolGroupsBase &molgroups) const
{
    return molgroups.map(*this);
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
QList<MolNum> MolNumList::map(const MoleculeGroup &molgroup) const
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
