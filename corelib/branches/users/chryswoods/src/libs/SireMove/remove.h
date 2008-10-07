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

#ifndef SIREMOVE_REMOVE_H
#define SIREMOVE_REMOVE_H

#include "reset.h"

SIRE_BEGIN_HEADER

namespace SireMove
{
class REMove;
}

QDataStream& operator<<(QDataStream&, const SireMove::REMove&);
QDataStream& operator>>(QDataStream&, SireMove::REMove&);

namespace SireMove
{

/** This is the base class of all of the (1D) replica exchange moves.
    These perform sampling on a set of replicas (held in a RESet)
    and then perform a replica exchange test the exchange pairs
    of replicas within that set 
    
    @author Christopher Woods
*/
class SIREMOVE_EXPORT REMove : public SireBase::PropertyBase
{

friend QDataStream& ::operator<<(QDataStream&, const REMove&);
friend QDataStream& ::operator>>(QDataStream&, REMove&);

public:
    REMove();

    virtual ~REMove();
    
    static const char* typeName()
    {
        return "SireMove::REMove";
    }
    
    virtual REMove* clone() const=0;
    
    virtual void move(RESet &replicas, int nmoves, bool record_stats=true)=0;
    
    int nAccepted() const;
    int nRejected() const;
    
    double acceptanceRatio() const;
    
    void clearMoveStatistics();
    
protected:
    REMove(const REMove &other);
    
    REMove& operator=(const REMove &other);
    
    bool operator==(const REMove &other) const;
    bool operator!=(const REMove &other) const;

    void moveAccepted();
    void moveRejected();
    
private:
    /** The number of accepted replica exchange moves */
    int naccepted;
    
    /** The number of rejected replica exchange moves */
    int nrejected;
};

}

SIRE_EXPOSE_CLASS( SireMove::REMove )

SIRE_END_HEADER

#endif
