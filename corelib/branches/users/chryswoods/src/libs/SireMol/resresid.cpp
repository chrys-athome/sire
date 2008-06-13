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

#include "resresid.h"
#include "residx.h"
#include "molinfo.h"

#include "SireMol/errors.h"

using namespace SireMol;

/** Constructor */
ResResID::ResResID() : ResID()
{}

/** Construct from two ResIDs */
ResResID::ResResID(const ResID &id0, const ResID &id1)
         : ResID(), resid0(id0), resid1(id1)
{}

/** Copy constructor */
ResResID::ResResID(const ResResID &other)
         : ResID(other),
           resid0(other.resid0), resid1(other.resid1)
{}

/** Destructor */
ResResID::~ResResID()
{}

/** Return a string representation of this ID */
QString ResResID::toString() const
{
    if (this->isNull())
        return "*";
    else if (resid0.isNull())
        return resid1.toString();
    else if (resid1.isNull())
        return resid0.toString();
    else
        return QString("%1 and %2").arg(resid0.toString(), resid1.toString());
}

/** Map this pair of residue IDs to the indicies of matching residue

    \throw SireMol::missing_residue
    \throw SireError::invalid_index
*/
QList<ResIdx> ResResID::map(const MolInfo &molinfo) const
{
    if (this->isNull())
        return molinfo.getResidues();
    else if (resid0.isNull())
        return resid1.map(molinfo);
    else if (resid1.isNull())
        return resid0.map(molinfo);
    else
    {
        QList<ResIdx> residxs = 
                         MolInfo::intersection( resid0.map(molinfo),
                                                resid1.map(molinfo) );
                
        if (residxs.isEmpty())
            throw SireMol::missing_residue( QObject::tr(
                "There is no residue matching the ID %1.")
                    .arg(this->toString()), CODELOC );
    
        qSort(residxs);
        
        return residxs;
    }
}
