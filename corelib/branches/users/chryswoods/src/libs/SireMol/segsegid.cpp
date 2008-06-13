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

#include <QSet>

#include "segsegid.h"
#include "segidx.h"
#include "molinfo.h"

#include "SireMol/errors.h"

using namespace SireMol;

/** Constructor */
SegSegID::SegSegID() : SegID()
{}

/** Construct from two SegIDs */
SegSegID::SegSegID(const SegID &id0, const SegID &id1)
         : SegID(), segid0(id0), segid1(id1)
{}

/** Copy constructor */
SegSegID::SegSegID(const SegSegID &other)
         : SegID(other),
           segid0(other.segid0), segid1(other.segid1)
{}

/** Destructor */
SegSegID::~SegSegID()
{}

/** Return a string representation of this ID */
QString SegSegID::toString() const
{
    if (this->isNull())
        return "*";
    else if (segid0.isNull())
        return segid1.toString();
    else if (segid1.isNull())
        return segid0.toString();
    else
        return QString("%1 and %2").arg(segid0.toString(), segid1.toString());
}

/** Map this pair of segment IDs to the indicies of matching segments

    \throw SireMol::missing_segment
    \throw SireError::invalid_index
*/
QList<SegIdx> SegSegID::map(const MolInfo &molinfo) const
{
    if (this->isNull())
        return molinfo.getSegments();
    else if (segid0.isNull())
        return segid1.map(molinfo);
    else if (segid1.isNull())
        return segid0.map(molinfo);
    else
    {
        QList<SegIdx> segidxs = 
                         MolInfo::intersection( segid0.map(molinfo),
                                                segid1.map(molinfo) );
                
        if (segidxs.isEmpty())
            throw SireMol::missing_segment( QObject::tr(
                "There is no segment matching the ID %1.")
                    .arg(this->toString()), CODELOC );
    
        qSort(segidxs);
        
        return segidxs;
    }
}
