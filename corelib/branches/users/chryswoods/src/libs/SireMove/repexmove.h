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

#ifndef SIREMOVE_REPEXMOVE_H
#define SIREMOVE_REPEXMOVE_H

#include "SireMaths/rangenerator.h"

#include "SireMove/supramove.h"
#include "SireMove/suprasubmove.h"

SIRE_BEGIN_HEADER

namespace SireMove
{
class RepExSubMove;
class RepExMove;
}

QDataStream& operator<<(QDataStream&, const SireMove::RepExMove&);
QDataStream& operator>>(QDataStream&, SireMove::RepExMove&);

QDataStream& operator<<(QDataStream&, const SireMove::RepExSubMove&);
QDataStream& operator>>(QDataStream&, SireMove::RepExSubMove&);

namespace SireMove
{

class RepExReplicas;
class RepExReplica;

using SireMaths::RanGenerator;

/** This is the sub-move that is applied to each replica in the supra-ensemble

    @author Christopher Woods
*/
class SIREMOVE_EXPORT RepExSubMove
           : public SireBase::ConcreteProperty<RepExSubMove,SupraSubMove>
{

friend QDataStream& ::operator<<(QDataStream&, const RepExSubMove&);
friend QDataStream& ::operator>>(QDataStream&, RepExSubMove&);

public:
    RepExSubMove();
    
    RepExSubMove(const RepExSubMove &other);
    
    ~RepExSubMove();
    
    RepExSubMove& operator=(const RepExSubMove &other);
    
    bool operator==(const RepExSubMove &other) const;
    bool operator!=(const RepExSubMove &other) const;
    
    static const char* typeName()
    {
        return QMetaType::typeName( qMetaTypeId<RepExSubMove>() );
    }

    int nAttempted() const;
    int nAccepted() const;
    int nRejected() const;
    
    int nMoves() const;
    
    double acceptanceRatio() const;

    QString toString() const;
    
    void clearStatistics();

    void move(SupraSubSystem &system, int n_supra_moves, bool record_supra_stats);
};

/** This class is used to perform replica exchange moves on a collection
    of Replicas. Each move involves running a block of sampling
    on each of the replicas, and then performing replice exchange swaps
    and tests between pairs.
    
    @author Christopher Woods
*/
class SIREMOVE_EXPORT RepExMove 
        : public SireBase::ConcreteProperty<RepExMove,SupraMove>
{

friend QDataStream& ::operator<<(QDataStream&, const RepExMove&);
friend QDataStream& ::operator>>(QDataStream&, RepExMove&);

public:
    RepExMove();
    
    RepExMove(const RepExMove &other);
    
    ~RepExMove();
    
    RepExMove& operator=(const RepExMove &other);

    bool operator==(const RepExMove &other) const;
    bool operator!=(const RepExMove &other) const;

    static const char* typeName()
    {
        return QMetaType::typeName( qMetaTypeId<RepExMove>() );
    }
    
    int nAttempted() const;
    int nAccepted() const;
    int nRejected() const;
    
    int nMoves() const;
    
    double acceptanceRatio() const;
    
    void clearStatistics();
    
    void setGenerator(const RanGenerator &generator);
    const RanGenerator& generator() const;

    void move(SupraSystem &system, int nmoves, bool record_stats);

private:
    /** The random number generator used to accept or reject the moves */
    RanGenerator rangenerator;
    
    /** The total number of replica exchange supra-moves attempted */
    quint32 nmoves;
    
    /** The number of times a replica exchange move has been accepted */
    quint32 naccept;
    
    /** The number of times a replica exchange move has been rejected */
    quint32 nreject;
};

}

Q_DECLARE_METATYPE( SireMove::RepExMove )
Q_DECLARE_METATYPE( SireMove::RepExSubMove )

SIRE_EXPOSE_CLASS( SireMove::RepExMove )
SIRE_EXPOSE_CLASS( SireMove::RepExSubMove )

SIRE_END_HEADER

#endif
