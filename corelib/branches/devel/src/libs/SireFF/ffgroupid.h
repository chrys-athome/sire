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

#ifndef SIREFF_FFGROUPID_H
#define SIREFF_FFGROUPID_H

#include "ffbase.h"
#include "forcefieldid.h"

#include <boost/tuple/tuple.hpp>

SIRE_BEGIN_HEADER

namespace SireFF
{
class FFGroupID;
}

QDataStream& operator<<(QDataStream&, const SireFF::FFGroupID&);
QDataStream& operator>>(QDataStream&, SireFF::FFGroupID&);

namespace SireFF
{

/** This class is used to identify a particular group within
    a particular forcefield
    
    @author Christopher Woods
*/
class SIREFF_EXPORT FFGroupID
{

friend QDataStream& ::operator<<(QDataStream&, const FFGroupID&);
friend QDataStream& ::operator>>(QDataStream&, FFGroupID&);

public:
    FFGroupID();
    
    FFGroupID(ForceFieldID ffid, const FFBase::Group &group);
    FFGroupID(const boost::tuple<ForceFieldID,FFBase::Group> &id);
    
    FFGroupID(const FFGroupID &other);
    
    ~FFGroupID();

    FFGroupID& operator=(const FFGroupID &other);
    
    bool operator==(const FFGroupID &other) const;
    bool operator!=(const FFGroupID &other) const;
    
    ForceFieldID ID() const;
    
    const FFBase::Group& group() const;

    QString toString() const;

private:
    /** The forcefield's ID */
    ForceFieldID id;
    
    /** The group's ID */
    FFBase::Group groupid;
};

/** Return the ForceField's ID */
inline ForceFieldID FFGroupID::ID() const
{
    return id;
}

/** Return the group's ID */
inline const FFBase::Group& FFGroupID::group() const
{
    return groupid;
}

}

Q_DECLARE_METATYPE(SireFF::FFGroupID);

SIRE_END_HEADER

#endif
