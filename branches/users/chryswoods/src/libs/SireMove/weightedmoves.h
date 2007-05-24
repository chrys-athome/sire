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

#include <QVector>

#include <boost/tuple/tuple.hpp>

#include "SireSystem/move.h"
#include "SireSystem/moves.h"

#include "SireMaths/rangenerator.h"

SIRE_BEGIN_HEADER

namespace SireMove
{
class WeightedMoves;
}

QDataStream& operator<<(QDataStream&, const SireMove::WeightedMoves&);
QDataStream& operator>>(QDataStream&, SireMove::WeightedMoves&);

namespace SireMove
{

using SireSystem::MovesBase;
using SireSystem::Move;
using SireSystem::QuerySystem;

using SireCAS::Symbol;

using SireMaths::RanGenerator;

using boost::tuple;

/** This is a collection of moves, with each move in the collection 
    chosen at random according to its weight
    
    @author Christopher Woods
*/
class SIREMOVE_EXPORT WeightedMoves : public MovesBase
{

friend QDataStream& ::operator<<(QDataStream&, const WeightedMoves&);
friend QDataStream& ::operator>>(QDataStream&, WeightedMoves&);

public:
    WeightedMoves(const RanGenerator &generator = RanGenerator());
    
    WeightedMoves(const QVector< tuple<Move,double> > &moves,
                  const RanGenerator &generator = RanGenerator());
    
    WeightedMoves(const WeightedMoves &other);
    
    ~WeightedMoves();
    
    static const char* typeName()
    {
        return "SireMove::WeightedMoves";
    }

    const char* what() const
    {
        return WeightedMoves::typeName();
    }

    WeightedMoves* clone() const
    {
        return new WeightedMoves(*this);
    }

    const RanGenerator& generator() const;
    void setGenerator(const RanGenerator &generator);

    void add(const Move &move, double weight=1);

    int count() const;

    Move& nextMove();

    void setEnergyComponent(const Symbol &symbol);

    QList<Move> moves() const;

    void assertCompatibleWith(QuerySystem &system) const;

private:
    void recalculateWeights();
    
    /** The list of all moves in this set, together with thier weights */
    QVector< tuple<Move,double> > weighted_moves;
    
    /** The random number generator used to select the moves */
    RanGenerator _generator;
    
    /** The maximum weight */
    double maxweight;
    
    /** The sum of all weights */
    double sum_of_weights;
};

}

Q_DECLARE_METATYPE(SireMove::WeightedMoves);

SIRE_END_HEADER

#endif
