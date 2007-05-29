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

#ifndef SIREMOVE_VOLUMEMOVE_H
#define SIREMOVE_VOLUMEMOVE_H

#include "montecarlo.h"

SIRE_BEGIN_HEADER

namespace SireMove
{
class VolumeMove;
}

QDataStream& operator<<(QDataStream&, const SireMove::VolumeMove&);
QDataStream& operator>>(QDataStream&, SireMove::VolumeMove&);

namespace SireMove
{

/** This is a Monte Carlo move that changes the volume of the system

    @author Christopher Woods
*/
class SIREMOVE_EXPORT VolumeMove : public MonteCarlo
{

friend QDataStream& ::operator<<(QDataStream&, const VolumeMove&);
friend QDataStream& ::operator>>(QDataStream&, VolumeMove&);

public:
    VolumeMove(const RanGenerator &generator = RanGenerator());

    VolumeMove(const MoleculeGroup &group,
               const RanGenerator &generator = RanGenerator());

    VolumeMove(const MolMappingFunction &mapfunc,
               const RanGenerator &generator = RanGenerator());

    VolumeMove(const MoleculeGroup &group,
               const VolChangingFunction &changefunc,
               const RanGenerator &generator = RanGenerator());

    VolumeMove(const MolMappingFunction &mapfunc,
               const VolChangingFunction &changefunc,
               const RanGenerator &generator = RanGenerator());

    VolumeMove(const VolumeMove &other);

    ~VolumeMove();

    VolumeMove& operator=(const VolumeMove &other);

    static const char* typeName()
    {
        return "SireMove::VolumeMove";
    }

    const char* what() const
    {
        return VolumeMove::typeName();
    }

    VolumeMove* clone() const
    {
        return new VolumeMove(*this);
    }

    void setMappingFunction(const MolMappingFunction &mapfunc);
    const MolMappingFunction& mappingFunction() const;

    void setVolumeChangingFunction(const VolChangingFunction &changefunc);
    const VolChangingFunction& volumeChangingFunction() const;

    void move(SimSystem &system);

    void assertCompatibleWith(QuerySystem &system) const;

private:
    /** The mapping function which is used to map the supplied
        molecules from one space into another */
    MolMappingFunction mapfunc;

    /** The function used to change the volume of the space
        from move to move */
    VolChangingFunction volchanger;
};

}

Q_DECLARE_METATYPE(SireMove::VolumeMove);

SIRE_END_HEADER

#endif
