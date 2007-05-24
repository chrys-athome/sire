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

#include "weightedmoves.h"

#include "SireSystem/querysystem.h"

#include "SireError/errors.h"

#include "SireStream/datastream.h"
#include "SireStream/shareddatastream.h"

using namespace SireMove;
using namespace SireSystem;
using namespace SireStream;

static const RegisterMetaType<WeightedMoves> r_weightmoves;

/** Serialise to a binary data stream */
QDataStream SIREMOVE_EXPORT &operator<<(QDataStream &ds, 
                                        const WeightedMoves &weightmoves)
{
    writeHeader(ds, r_weightmoves, 1);
    
    SharedDataStream sds(ds);
    
    sds << weightmoves._generator;
    
    sds << quint32( weightmoves.weighted_moves.count() );
    
    for (QVector< tuple<Move,double> >::const_iterator 
                        it = weightmoves.weighted_moves.constBegin();
         it != weightmoves.weighted_moves.constEnd();
         ++it)
    {
        sds << it->get<0>() << it->get<1>();
    }
    
    return ds;
}

/** Deserialise from a binary data stream */
QDataStream SIREMOVE_EXPORT &operator>>(QDataStream &ds, 
                                        WeightedMoves &weightmoves)
{
    VersionID v = readHeader(ds, r_weightmoves);
    
    if (v == 1)
    {
        SharedDataStream sds(ds);
        
        sds >> weightmoves._generator;
        
        quint32 nmoves;
        sds >> nmoves;
        
        QVector< tuple<Move,double> > weighted_moves;
    
        for (quint32 i=0; i<nmoves; ++i)
        {
            Move move;
            double weight;
            
            sds >> move >> weight;
            
            weighted_moves.append( tuple<Move,double>(move,weight) );
        }
        
        weightmoves.weighted_moves = weighted_moves;
        weightmoves.recalculateWeights();
    
    }
    else
        throw version_error(v, "1", r_weightmoves, CODELOC);
    
    return ds;
}

/** Construct an empty set of moves, using the optionally passed
    random number generator to select which move should be used next */
WeightedMoves::WeightedMoves(const RanGenerator &generator)
              : MovesBase(),
                _generator(generator), maxweight(0), sum_of_weights(0)
{}

void WeightedMoves::recalculateWeights()
{
    sum_of_weights = 0;
    maxweight = 0;
    
    //calculate the sum and get the maximum weight
    for (QVector< tuple<Move,double> >::const_iterator it = weighted_moves.constBegin();
         it != weighted_moves.constEnd();
         ++it)
    {
        double weight = it->get<1>();
        
        if (weight < 0)
            throw SireError::invalid_arg( QObject::tr(
                "Cannot give a move a negative weight! (%1 for %2)")
                    .arg(weight).arg(it->get<0>().what()),
                        CODELOC );
    
        sum_of_weights += weight;
        maxweight = qMax(maxweight, weight);
    }
}

/** Construct a set of moves from the passed list of moves with weights */
WeightedMoves::WeightedMoves(const QVector< tuple<Move,double> > &moves,
                             const RanGenerator &generator)
              : MovesBase(),
                weighted_moves(moves),
                _generator(generator), maxweight(0), sum_of_weights(0)
{
    recalculateWeights();
}

/** Copy constructor */
WeightedMoves::WeightedMoves(const WeightedMoves &other)
              : MovesBase(other),
                weighted_moves(other.weighted_moves),
                _generator(other._generator),
                maxweight(other.maxweight), sum_of_weights(other.sum_of_weights)
{}

/** Destructor */
WeightedMoves::~WeightedMoves()
{}

/** Return the random number generator used to pick the next move */
const RanGenerator& WeightedMoves::generator() const
{
    return _generator;
}

/** Set the random number generator used to pick the next move */
void WeightedMoves::setGenerator(const RanGenerator &generator)
{
    _generator = generator;
}

/** Add another move to the set, with the optionally supplied weight */
void WeightedMoves::add(const Move &move, double weight)
{
    if (weight < 0)
        throw SireError::invalid_arg( QObject::tr(
            "Cannot give a move a negative weight! (%1 for %2)")
                .arg(weight).arg(move.what()),
                    CODELOC );
    
    weighted_moves.append( tuple<Move,double>(move,weight) );
    
    sum_of_weights += weight;
    maxweight = qMax(maxweight, weight);
}

/** Return the number of different moves in this set */
int WeightedMoves::count() const
{
    return weighted_moves.count();
}

/** Return a reference to the next move in the sequence */
Move& WeightedMoves::nextMove()
{
    if (weighted_moves.isEmpty() or maxweight == 0)
        throw SireError::invalid_state( QObject::tr(
            "You cannot ask for the next move when there aren't any!"),
                CODELOC );
                
    //use the von Neumann rejection method to choose a random move
    //using the supplied weights
    //  rejection method - choose random
    //  move, then choose random number from 0 to maxweight. If
    //  probability of the move <= the random number, then accept
    //  this move, else go back to the beginning and try again...
    
    int nmoves = weighted_moves.count();
    
    tuple<Move,double> *moves_array = weighted_moves.data();
    
    if (nmoves == 1)
        return moves_array[0].get<0>();
    
    //normalise the probabilities
    double norm_maxweight = maxweight / sum_of_weights;
    
    while (true)
    {
        quint32 i = _generator.randInt(nmoves-1);
        
        tuple<Move,double> &move = moves_array[i];
        
        if ( _generator.rand(norm_maxweight) <= (move.get<1>() / sum_of_weights) )
        {
            //use this move
            return move.get<0>();
        }
    }
}

/** Set the energy component on which all of the moves must sample */
void WeightedMoves::setEnergyComponent(const Symbol &symbol)
{
    for (QVector< tuple<Move,double> >::iterator it = weighted_moves.begin();
         it != weighted_moves.end();
         ++it)
    {
        it->get<0>().setEnergyComponent(symbol);
    }
}

/** Return a list of all of the moves in this set */
QList<Move> WeightedMoves::moves() const
{
    QList<Move> all_moves;
    
    for (QVector< tuple<Move,double> >::const_iterator it = weighted_moves.begin();
         it != weighted_moves.end();
         ++it)
    {
        all_moves.append( it->get<0>() );
    }
    
    return all_moves;
}

/** Assert that all of the moves in this set are compatible with the 
    system 'system' - this is to ensure that errors are caught now,
    rather than well into the running of the simulation. */
void WeightedMoves::assertCompatibleWith(QuerySystem &system) const
{
    for (QVector< tuple<Move,double> >::const_iterator it = weighted_moves.begin();
         it != weighted_moves.end();
         ++it)
    {
        it->get<0>().assertCompatibleWith(system);
    }
}
