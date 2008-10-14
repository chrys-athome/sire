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

#ifndef SIREMOVE_HREMOVE_H
#define SIREMOVE_HREMOVE_H

#include "remove.h"

SIRE_BEGIN_HEADER

namespace SireMove
{
class HREMove;
}

QDataStream& operator<<(QDataStream&, const SireMove::HREMove&);
QDataStream& operator>>(QDataStream&, SireMove::HREMove&);

namespace SireMove
{

/** This is a Hamiltonian replica exchange move. This provides
    replica exchange moves that create replicas with different
    Hamiltonians, and then swaps replicas between Hamiltonians.
    This is useful for RETI simulations, or for RE simulations
    between fast and slow potentials
    
    @author Christopher Woods
*/
class SIREMOVE_EXPORT HREMove : public SireBase::ConcreteProperty<HREMove,REMove>
{

friend QDataStream& ::operator<<(QDataStream&, const SireMove::HREMove&);
friend QDataStream& ::operator>>(QDataStream&, SireMove::HREMove&);

public:
    HREMove();
    
    HREMove(const HREMove &other);
    
    ~HREMove();
    
    static const char* typeName()
    {
        return QMetaType::typeName( qMetaTypeId<HREMove>() );
    }
    
    HREMove* clone() const
    {
        return new HREMove(*this);
    }
    
    HREMove& operator=(const HREMove &other);
    
    bool operator==(const HREMove &other) const;
    bool operator!=(const HREMove &other) const;

protected:
    void performRETest(RESet &replicas, bool record_stats);

private:
    /** kT for the temperature at which this replica exchange move takes place */
    
};

}

Q_DECLARE_METATYPE( SireMove::HREMove )

SIRE_EXPOSE_CLASS( SireMove::HREMove )

SIRE_END_HEADER

#endif
