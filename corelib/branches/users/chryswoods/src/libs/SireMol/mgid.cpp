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

#include "mgid.h"
#include "mgidx.h"
#include "mgname.h"
#include "mgnum.h"
#include "moleculegroups.h"

#include "SireBase/incremint.h"

#include "SireMol/errors.h"
#include "SireError/errors.h"

#include "tostring.h"

using namespace SireMol;
using namespace SireBase;

////////
//////// Implementation of MGID
////////

MGID::MGID() : SireID::ID()
{}

MGID::MGID(const MGID &other) : SireID::ID(other)
{}

MGID::~MGID()
{}

////////
//////// Implementation of MGIdx
////////

MGIdx::MGIdx() : SireID::Index_T_<MGIdx>(), MGID()
{}

MGIdx::MGIdx(qint32 idx) : SireID::Index_T_<MGIdx>(idx), MGID()
{}

MGIdx::MGIdx(const MGIdx &other) : SireID::Index_T_<MGIdx>(other), MGID(other)
{}

MGIdx::~MGIdx()
{}

QList<MGNum> MGIdx::map(const MolGroupsBase &molgroups) const
{
    return molgroups.map(*this);
}

////////
//////// Implementation of MGName
////////

MGName::MGName() : SireID::Name(), MGID()
{}

MGName::MGName(const QString &name) : SireID::Name(name), MGID()
{}

MGName::MGName(const MGName &other) : SireID::Name(other), MGID(other)
{}

MGName::~MGName()
{}

QList<MGNum> MGName::map(const MolGroupsBase &molgroups) const
{
    return molgroups.map(*this);
}

////////
//////// Implementation of MGNum
////////

MGNum::MGNum() : SireID::Number(), MGID()
{}

MGNum::MGNum(quint32 num) : SireID::Number(num), MGID()
{}

MGNum::MGNum(const MGNum &other) : SireID::Number(other), MGID(other)
{}

MGNum::~MGNum()
{}

QList<MGNum> MGNum::map(const MolGroupsBase &molgroups) const
{
    return molgroups.map(*this);
}

////////
//////// Implementation of MGNumList
////////

/** Null constructor */
MGNumList::MGNumList() : MGID()
{}

/** Construct from a passed list of numbers */
MGNumList::MGNumList(const QList<MGNum> &mgnumbers)
          : MGID(), mgnums(mgnumbers)
{}
  
/** Copy constructor */  
MGNumList::MGNumList(const MGNumList &other)
          : MGID(other), mgnums(other.mgnums)
{}
  
/** Destructor */
MGNumList::~MGNumList()
{}
    
bool MGNumList::isNull() const
{
    return mgnums.isEmpty();
}

uint MGNumList::hash() const
{
    if (mgnums.isEmpty())
        return 0;
    else
        return mgnums.first().hash() * mgnums.count();
}

QString MGNumList::toString() const
{
    return QString("MGNumList( %1 )").arg( Sire::toString(mgnums) );
}

MGNumList& MGNumList::operator=(const MGNumList &other)
{
    mgnums = other.mgnums;
    MGID::operator=(other);
    
    return *this;
}

bool MGNumList::operator==(const MGNumList &other) const
{
    return mgnums == other.mgnums;
}

bool MGNumList::operator!=(const MGNumList &other) const
{
    return mgnums != other.mgnums;
}

/** Ensure that all of the numbers exist in 'molgroups'

    \throw SireMol::missing_group
    \throw SireError::invalid_args
*/
QList<MGNum> MGNumList::map(const MolGroupsBase &molgroups) const
{
    if (mgnums.isEmpty())
        throw SireError::invalid_arg( QObject::tr(
            "You cannot pass an empty set of molecule group numbers!"), 
                CODELOC );

    foreach (MGNum mgnum, mgnums)
    {
        molgroups.assertContains(mgnum);
    }
    
    return mgnums;
}
