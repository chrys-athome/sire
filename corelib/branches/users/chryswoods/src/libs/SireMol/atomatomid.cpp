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

#include "atomatomid.h"
#include "atomidx.h"
#include "molinfo.h"

#include "SireMol/errors.h"

#include "SireStream/datastream.h"
#include "SireStream/shareddatastream.h"

using namespace SireMol;
using namespace SireStream;

static const RegisterMetaType<AtomAtomID> r_atomatomid;

/** Serialise to a binary datastream */
QDataStream SIREMOL_EXPORT &operator<<(QDataStream &ds, const AtomAtomID &atomatomid)
{
    writeHeader(ds, r_atomatomid, 1);
    
    SharedDataStream sds(ds);
    
    sds << atomatomid.atomid0 << atomatomid.atomid1;
    
    return ds;
}

/** Extract from a binary datastream */
QDataStream SIREMOL_EXPORT &operator>>(QDataStream &ds, AtomAtomID &atomatomid)
{
    VersionID v = readHeader(ds, r_atomatomid);
    
    if (v == 1)
    {
        SharedDataStream sds(ds);
        
        sds >> atomatomid.atomid0 >> atomatomid.atomid1;
    }
    else
        throw version_error( v, "1", r_atomatomid, CODELOC );
        
    return ds;
}

/** Constructor */
AtomAtomID::AtomAtomID() : AtomID()
{}

/** Construct from two AtomIDs */
AtomAtomID::AtomAtomID(const AtomID &id0, const AtomID &id1)
           : AtomID(), atomid0(id0), atomid1(id1)
{}

/** Copy constructor */
AtomAtomID::AtomAtomID(const AtomAtomID &other)
           : AtomID(other),
             atomid0(other.atomid0), atomid1(other.atomid1)
{}

/** Destructor */
AtomAtomID::~AtomAtomID()
{}

/** Return a string representation of this ID */
QString AtomAtomID::toString() const
{
    if (this->isNull())
        return "*";
    else if (atomid0.isNull())
        return atomid1.toString();
    else if (atomid1.isNull())
        return atomid0.toString();
    else
        return QString("%1 and %2").arg(atomid0.toString(), atomid1.toString());
}

/** Map this pair of Atom IDs to the indicies of matching atoms 

    \throw SireMol::missing_atom
    \throw SireError::invalid_index
*/
QList<AtomIdx> AtomAtomID::map(const MolInfo &molinfo) const
{
    if (this->isNull())
        return molinfo.getAtoms();
    else if (atomid0.isNull())
        return atomid1.map(molinfo);
    else if (atomid1.isNull())
        return atomid0.map(molinfo);
    else
    {
        QList<AtomIdx> atomidxs = 
                         MolInfo::intersection( atomid0.map(molinfo),
                                                atomid1.map(molinfo) );
                
        if (atomidxs.isEmpty())
            throw SireMol::missing_atom( QObject::tr(
                "There is no atom matching the ID %1.")
                    .arg(this->toString()), CODELOC );
    
        qSort(atomidxs);
        
        return atomidxs;
    }
}
