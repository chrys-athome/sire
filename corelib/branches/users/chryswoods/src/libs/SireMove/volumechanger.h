/********************************************\
  *
  *  Sire - Molecular Simulation Framework
  *
  *  Copyright (C) 2009  Christopher Woods
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

#ifndef SIREVOL_VOLUMECHANGER_H
#define SIREVOL_VOLUMECHANGER_H

#include "SireBase/property.h"
#include "SireBase/propertymap.h"

#include "SireMol/mgnum.h"

SIRE_BEGIN_HEADER

namespace SireMove
{
class VolumeChanger;
class NullVolumeChanger;

class ScaleVolumeFromCenter;
}

QDataStream& operator<<(QDataStream&, const VolumeChanger&);
QDataStream& operator>>(QDataStream&, VolumeChanger&);

QDataStream& operator<<(QDataStream&, const NullVolumeChanger&);
QDataStream& operator>>(QDataStream&, NullVolumeChanger&);

QDataStream& operator<<(QDataStream&, const ScaleVolumeFromCenter&);
QDataStream& operator>>(QDataStream&, ScaleVolumeFromCenter&);

namespace SireSystem
{
class System;
}

namespace SireMove
{

using SireMol::MoleculeGroup;
using SireMol::MGNum;

using SireBase::PropertyMap;

using SireSystem::System;

/** This is the base class of all volume changing function classes.
    These classes are used to change the volume of a system
    i.e. during a VolumeMove
    
    @author Christopher Woods
*/
class SIREMOVE_EXPORT VolumeChanger : public SireBase::Property
{

friend QDataStream& ::operator<<(QDataStream&, const VolumeChanger&);
friend QDataStream& ::operator>>(QDataStream&, VolumeChanger&);

public:
    VolumeChanger();
    VolumeChanger(MGNum mgnum);
    VolumeChanger(const MoleculeGroup &molgroup);
    
    VolumeChanger(const VolumeChanger &other);
    
    virtual ~VolumeChanger();
    
    VolumeChanger* clone() const=0;
    
    static const char* typeName()
    {
        return "SireMove::VolumeChanger";
    }

    void setGenerator(const RanGenerator &generator);
    const RanGenerator& generator() const;
    
    MGNum groupNumber() const;
    
    void setGroup(MGNum mgnum);
    void setGroup(const MoleculeGroup &molgroup);
    
    virtual int setVolume(System &system,
                          const SireUnits::Dimension::Volume &volume,
                          const PropertyMap &map = PropertyMap()) const=0;
    
    virtual int changeVolume(System &system,
                             const SireUnits::Dimension::Volume &delta,
                             const PropertyMap &map = PropertyMap()) const;
    
    virtual int randomChangeVolume(System &system, 
                                   const SireUnits::Dimension::Volume &maxvolchange,
                                   const PropertyMap &map = PropertyMap()) const;
    
    static const NullVolumeChanger& null();
    
protected:
    VolumeChanger& operator=(const VolumeChanger &other);
    
    bool operator==(const VolumeChanger &other) const;
    bool operator!=(const VolumeChanger &other) const;
    
private:
    /** The number of the molecule group that will be 
        moved by this volume changer */
    MGNum mgnum;
};

typedef SireBase::PropertyPtr<VolumeChanger> VolumeChangerPtr;

}

Q_DECLARE_METATYPE( SireMove::NullVolumeChanger )
Q_DECLARE_METATYPE( SireMove::ScaleVolumeFromCenter )

SIRE_EXPOSE_CLASS( SireMove::VolumeChanger )
SIRE_EXPOSE_CLASS( SireMove::NullVolumeChanger )
SIRE_EXPOSE_CLASS( SireMove::ScaleVolumeFromCenter )

SIRE_END_HEADER

#endif
