/********************************************\
  *
  *  Sire - Molecular Simulation Framework
  *
  *  Copyright (C) 2010  Christopher Woods
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

#include "patch.h"

#include "SireError/errors.h"

#include "SireStream/datastream.h"
#include "SireStream/shareddatastream.h"

using namespace SireFF;
using namespace SireBase;
using namespace SireStream;

static const RegisterMetaType<Patch> r_patch;

QDataStream SIREFF_EXPORT &operator<<(QDataStream &ds, const Patch &patch)
{
    writeHeader(ds, r_patch, 1);
    
    SharedDataStream sds(ds);
    
    sds << patch.coords << patch.params << patch.idx_to_beadid
        << patch.beadid_to_idx << static_cast<const Property&>(patch);
        
    return ds;
}

QDataStream SIREFF_EXPORT &operator>>(QDataStream &ds, Patch &patch)
{
    VersionID v = readHeader(ds, r_patch);
    
    if (v == 1)
    {
        SharedDataStream sds(ds);
        
        sds >> patch.coords >> patch.params >> patch.idx_to_beadid
            >> patch.beadid_to_idx >> static_cast<Property&>(patch);
            
        patch.aabox = patch.coords.aaBox();
    }
    else
        throw version_error(v, "1", r_patch, CODELOC);
        
    return ds;
}

/** Constructor */
Patch::Patch() : ConcreteProperty<Patch,Property>()
{}

/** Copy constructor */
Patch::Patch(const Patch &other) 
      : ConcreteProperty<Patch,Property>(other),
        coords(other.coords), params(other.params),
        idx_to_beadid(other.idx_to_beadid), beadid_to_idx(other.beadid_to_idx),
        aabox(other.aabox)
{}

/** Destructor */
Patch::~Patch()
{}

const char* Patch::typeName()
{
    return QMetaType::typeName( qMetaTypeId<Patch>() );
}

/** Copy assignment operator */
Patch& Patch::operator=(const Patch &other)
{
    if (this != &other)
    {
        coords = other.coords;
        params = other.params;
        idx_to_beadid = other.idx_to_beadid;
        beadid_to_idx = other.beadid_to_idx;
        Property::operator=(other);
    }
    
    return *this;
}

/** Comparison operator */
bool Patch::operator==(const Patch &other) const
{
    return this == &other or
           (coords == other.coords and params == other.params and
            idx_to_beadid == other.idx_to_beadid and 
            beadid_to_idx == other.beadid_to_idx and
            Property::operator==(other));
}

/** Comparison operator */
bool Patch::operator!=(const Patch &other) const
{
    return not Patch::operator==(other);
}

/** Return whether or not this patch is empty */
bool Patch::isEmpty() const
{
    return coords.count() == 0;
}

/** Return the AABox that completely surrounds this patch */
const AABox& Patch::aaBox() const
{
    return aabox;
}

/** Return the internal IDs of each of the beads in this patch,
    in the order that they appear in the patch */
const QVector<quint32> Patch::beadIDs() const
{
    return idx_to_beadid;
}

/** Return the array of coordinates of the beads in this patch */
const CoordGroupArray& Patch::coordinates() const
{
    return coords;
}

/** Return the array of parameters for each bead in the patch */
const FFParametersArray& Patch::parameters() const
{
    return params.read();
}

/** Internal function used to add the bead with internal ID 'beadid'
    and specified coordinates and parameters to this patch */
void Patch::add(quint32 beadid, const CoordGroup &groupcoords, 
                const FFParameters &groupparams)
{
    if (beadid_to_idx.contains(beadid))
        //this bead already exists in this patch - remove it as it is 
        //an old version
        this->remove(beadid);

    beadid_to_idx.insert(beadid, idx_to_beadid.count());
    idx_to_beadid.append(beadid);
    
    params.edit().append(groupparams);
    coords.append(groupcoords);
    
    aabox = coords.aaBox();
}

/** Internal function used to add the array of beads with the passed
    array of beadids */
void Patch::add(const QVector<quint32> &beadids, const CoordGroupArray &groupcoords,
                const FFParametersArray &groupparams)
{
    for (QVector<quint32>::const_iterator it = beadids.constBegin();
         it != beadids.constEnd();
         ++it)
    {
        if (beadid_to_idx.contains(*it))
            this->remove(*it);
    }
    
    for (int i=0; i<beadids.count(); ++i)
    {
        beadid_to_idx.insert(beadids.constData()[i], beadids.count() + i);
    }
    
    idx_to_beadid += beadids;
    params.edit().append(groupparams);
    coords.append(groupcoords);
}

/*
void Patch::update(quint32 beadid, const CoordGroup &coords);
void Patch::update(quint32 beadid, const FFParameters &params);
void Patch::update(quint32 beadid, const CoordGroup &coords, const FFParameters &params);
            
void Patch::update(const QVector<quint32> &beadids, const CoordGroupArray &coords);
void Patch::update(const QVector<quint32> &beadids, const FFParametersArray &params);
void Patch::update(const QVector<quint32> &beadids, const CoordGroupArray &coords,
            const FFParametersArray &params);
*/
            
void Patch::remove(quint32 beadid)
{
    throw SireError::incomplete_code("TODO", CODELOC);
}

void Patch::remove(const QVector<quint32> &beadids)
{
    throw SireError::incomplete_code("TODO", CODELOC);
}

/*
void Patch::removeAll();
*/