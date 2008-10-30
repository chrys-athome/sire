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

#include "cgcgid.h"
#include "cgidx.h"
#include "molinfo.h"

#include "SireMol/errors.h"

#include "SireStream/datastream.h"
#include "SireStream/shareddatastream.h"

using namespace SireMol;
using namespace SireStream;

static const RegisterMetaType<CGCGID> r_cgcgid;

/** Serialise to a binary datastream */
QDataStream SIREMOL_EXPORT &operator<<(QDataStream &ds, const CGCGID &cgcgid)
{
    writeHeader(ds, r_cgcgid, 1);
    
    SharedDataStream sds(ds);
    
    sds << cgcgid.cgid0 << cgcgid.cgid1;
    
    return ds;
}

/** Extract from a binary datastream */
QDataStream SIREMOL_EXPORT &operator>>(QDataStream &ds, CGCGID &cgcgid)
{
    VersionID v = readHeader(ds, r_cgcgid);
    
    if (v == 1)
    {
        SharedDataStream sds(ds);
        
        sds >> cgcgid.cgid0 >> cgcgid.cgid1;
    }
    else
        throw version_error( v, "1", r_cgcgid, CODELOC );
        
    return ds;
}

/** Constructor */
CGCGID::CGCGID() : CGID()
{}

/** Construct from two CGIDs */
CGCGID::CGCGID(const CGID &id0, const CGID &id1)
       : CGID(), cgid0(id0), cgid1(id1)
{}

/** Copy constructor */
CGCGID::CGCGID(const CGCGID &other)
       : CGID(other),
         cgid0(other.cgid0), cgid1(other.cgid1)
{}

/** Destructor */
CGCGID::~CGCGID()
{}

/** Return a string representation of this ID */
QString CGCGID::toString() const
{
    if (this->isNull())
        return "*";
    else if (cgid0.isNull())
        return cgid1.toString();
    else if (cgid1.isNull())
        return cgid0.toString();
    else
        return QString("%1 and %2").arg(cgid0.toString(), cgid1.toString());
}

/** Map this pair of CutGroup IDs to the indicies of matching CutGroups

    \throw SireMol::missing_cutgroup
    \throw SireError::invalid_index
*/
QList<CGIdx> CGCGID::map(const MolInfo &molinfo) const
{
    if (this->isNull())
        return molinfo.getCutGroups();
    else if (cgid0.isNull())
        return cgid1.map(molinfo);
    else if (cgid1.isNull())
        return cgid0.map(molinfo);
    else
    {
        QList<CGIdx> cgidxs = 
                         MolInfo::intersection( cgid0.map(molinfo),
                                                cgid1.map(molinfo) );
                
        if (cgidxs.isEmpty())
            throw SireMol::missing_cutgroup( QObject::tr(
                "There is no CutGroup matching the ID %1.")
                    .arg(this->toString()), CODELOC );
    
        qSort(cgidxs);
        
        return cgidxs;
    }
}
