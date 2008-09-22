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

#ifndef SIREMOVE_WEIGHTEDMOVES_H
#define SIREMOVE_WEIGHTEDMOVES_H

#include "moves.h"

#include "SireMaths/rangenerator.h"

#include <boost/tuple/tuple.hpp>

SIRE_BEGIN_HEADER

namespace SireMove
{
class WeightedMoves;
}

QDataStream& operator<<(QDataStream&, const SireMove::WeightedMoves&);
QDataStream& operator>>(QDataStream&, SireMove::WeightedMoves&);

namespace SireMove
{

using SireMaths::RanGenerator;

/** This is a collection of moves, with each move in the collection
    chosen at random according to its weight

    @author Christopher Woods
*/
class SIREMOVE_EXPORT WeightedMoves 
          : public SireBase::ConcreteProperty<WeightedMoves,MovesBase>
{

friend QDataStream& ::operator<<(QDataStream&, const WeightedMoves&);
friend QDataStream& ::operator>>(QDataStream&, WeightedMoves&);

public:
    WeightedMoves();
    
    WeightedMoves(const WeightedMoves &other);
    
    ~WeightedMoves();
    
    WeightedMoves& operator=(const WeightedMoves &other);
    
    bool operator==(const WeightedMoves &other) const;
    bool operator!=(const WeightedMoves &other) const;
    
    static const char* typeName()
    {
        return QMetaType::typeName( qMetaTypeId<WeightedMoves>() );
    }
    
    WeightedMoves* clone() const
    {
        return new WeightedMoves(*this);
    }
    
    void add(const MoveBase &move, double weight=1);
    
    using MovesBase::move;
    
    System move(const System &system, int nmoves, bool record_stats);
    
    QList<Move> moves() const;

    void setGenerator(const RanGenerator &rangenerator);
    
    const RanGenerator& generator() const;

private:
    void recalculateWeights();
    
    /** The list of moves, together with their associated weights */
    QVector< boost::tuple<Move,double> > mvs;
    
    /** The random number generator used to pick moves */
    RanGenerator rangenerator;
    
    /** The value of the maximum weight */
    double maxweight;
};

}

Q_DECLARE_METATYPE( SireMove::WeightedMoves )

SIRE_EXPOSE_CLASS( SireMove::WeightedMoves )

SIRE_END_HEADER

#endif
