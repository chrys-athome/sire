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
#include "move.h"

#include "SireUnits/units.h"
#include "SireUnits/temperature.h"

#include "SireSystem/system.h"

#include "SireStream/datastream.h"
#include "SireStream/shareddatastream.h"

using namespace SireMove;
using namespace SireSystem;
using namespace SireBase;
using namespace SireUnits;
using namespace SireUnits::Dimension;
using namespace SireStream;

using boost::tuples::tuple;

static const RegisterMetaType<WeightedMoves> r_weightedmoves;

/** Serialise to a binary datastream */
QDataStream SIREMOVE_EXPORT &operator<<(QDataStream &ds, 
                                        const WeightedMoves &weightedmoves)
{
    writeHeader(ds, r_weightedmoves, 1);
    
    SharedDataStream sds(ds);
    
    qint32 nmoves = weightedmoves.mvs.count();
    const tuple<MovePtr,double> *mvs_array = weightedmoves.mvs.constData();
    
    sds << nmoves;
    
    for (int i=0; i<nmoves; ++i)
    {
        sds << mvs_array[i].get<0>() << mvs_array[i].get<1>();
    }
    
    sds << weightedmoves.rangenerator << static_cast<const Moves&>(weightedmoves);
    
    return ds;
}

/** Extract from a binary datastream */
QDataStream SIREMOVE_EXPORT &operator>>(QDataStream &ds, WeightedMoves &weightedmoves)
{
    VersionID v = readHeader(ds, r_weightedmoves);
    
    if (v == 1)
    {
        SharedDataStream sds(ds);
        
        qint32 nmoves;
        
        sds >> nmoves;
        
        QVector< tuple<MovePtr,double> > mvs(nmoves);
        tuple<MovePtr,double> *mvs_array = mvs.data();
        
        for (int i=0; i<nmoves; ++i)
        {
            MovePtr mv; double weight;
            
            sds >> mv >> weight;
            mvs_array[i] = tuple<MovePtr,double>(mv, weight);
        }
        
        weightedmoves.mvs = mvs;
        
        sds >> weightedmoves.rangenerator >> static_cast<Moves&>(weightedmoves);
        
        weightedmoves.recalculateWeights();
    }
    else
        throw version_error(v, "1", r_weightedmoves, CODELOC);

    return ds;
}

/** Constructor */
WeightedMoves::WeightedMoves()
              : ConcreteProperty<WeightedMoves,Moves>(), maxweight(0)
{}

/** Copy constructor */
WeightedMoves::WeightedMoves(const WeightedMoves &other)
              : ConcreteProperty<WeightedMoves,Moves>(other),
                mvs(other.mvs), rangenerator(other.rangenerator), 
                maxweight(other.maxweight)
{}

/** Destructor */
WeightedMoves::~WeightedMoves()
{}

/** Copy assignment operator */
WeightedMoves& WeightedMoves::operator=(const WeightedMoves &other)
{
    mvs = other.mvs;
    rangenerator = other.rangenerator;
    maxweight = other.maxweight;
    
    Moves::operator=(other);
    
    return *this;
}

static bool operator==(const tuple<MovePtr,double> &t0, 
                       const tuple<MovePtr,double> &t1)
{
    return t0.get<0>() == t1.get<0>() and
           t0.get<1>() == t1.get<1>();
}

/** Comparison operator */
bool WeightedMoves::operator==(const WeightedMoves &other) const
{
    return mvs == other.mvs;
}

/** Comparison operator */
bool WeightedMoves::operator!=(const WeightedMoves &other) const
{
    return mvs != other.mvs;
}

/** Recalculate all of the weights */
void WeightedMoves::recalculateWeights()
{
    maxweight = 0;

    int nmoves = mvs.count();

    if (nmoves == 0)
        return;

    const tuple<MovePtr,double> *mvs_array = mvs.constData();
    
    for (int i=0; i<nmoves; ++i)
    {
        maxweight = qMax( maxweight, mvs_array[i].get<1>() );
    }
}

/** Set the random number generator used to pick moves */
void WeightedMoves::setGenerator(const RanGenerator &rangen)
{
    rangenerator = rangen;
}

/** Return the random number generator used to pick moves */
const RanGenerator& WeightedMoves::generator() const
{
    return rangenerator;
}

/** Add the move 'move' to the list of moves, with the weight 'weight' */
void WeightedMoves::add(const Move &move, double weight)
{
    if (weight <= 0)
        return;

    int nmoves = mvs.count();
    
    tuple<MovePtr,double> *mvs_array = mvs.data();
    
    for (int i=0; i<nmoves; ++i)
    {
        if (mvs_array[i].get<0>()->equals(move))
        {
            mvs_array[i].get<1>() += weight;
            this->recalculateWeights();
            return;
        }
    }
    
    mvs.append( tuple<MovePtr,double>(move, weight) );
    this->recalculateWeights();
}

/** Completely clear all of the move statistics */
void WeightedMoves::clearStatistics()
{
    tuple<MovePtr,double> *mvs_array = mvs.data();
    int nmoves = mvs.count();
    
    for (int i=0; i<nmoves; ++i)
    {
        mvs_array[i].get<0>().edit().clearStatistics();
    }
}

/** Perform 'nmoves' moves on the system 'system' and return the result */
System WeightedMoves::move(const System &system, int nmoves, bool record_stats)
{
    if (mvs.isEmpty())
        return system;

    WeightedMoves old_state(*this);
    System run_system(system);

    try
    {
        int n = mvs.count();
        tuple<MovePtr,double> *mvs_array = mvs.data();

        if (n == 1)
        {
            mvs_array[0].get<0>().edit().move(run_system, nmoves, record_stats);
            return run_system;
        }

        for (int i=0; i<nmoves; ++i)
        {
            //use the von Neumann rejection method to choose a random move
            //using the supplied weights
            //  rejection method - choose random
            //  move, then choose random number from 0 to maxweight. If
            //  probability of the move <= the random number, then accept
            //  this move, else go back to the beginning and try again...
            while (true)
            {
                quint32 i = generator().randInt(n-1);
    
                tuple<MovePtr,double> &move = mvs_array[i];
    
                if ( generator().rand(maxweight) <= move.get<1>() )
                {
                    //use this move
                    move.get<0>().edit().move(run_system, 1, record_stats);
                    break;
                }
            }
        }
    }
    catch(...)
    {
        this->operator=(old_state);
        throw;
    }

    return run_system;
}

/** Set the energy component of all of the moves to 'component' */
void WeightedMoves::setEnergyComponent(const Symbol &component)
{
    int nmoves = mvs.count();
    
    if (nmoves > 0)
    {
        tuple<MovePtr,double> *mvs_array = mvs.data();
        
        for (int i=0; i<nmoves; ++i)
        {
            if (mvs_array[i].get<0>().read().energyComponent() != component)
            {
                mvs_array[i].get<0>().edit().setEnergyComponent(component);
            }
        }
    }
}

/** Set the name of the property that all of the moves will use to 
    find the simulation space (simulation box) to 'spaceproperty' */
void WeightedMoves::setSpaceProperty(const PropertyName &spaceproperty)
{
    int nmoves = mvs.count();
    
    if (nmoves > 0)
    {
        tuple<MovePtr,double> *mvs_array = mvs.data();
        
        for (int i=0; i<nmoves; ++i)
        {
            if (mvs_array[i].get<0>().read().spaceProperty() != spaceproperty)
            {
                mvs_array[i].get<0>().edit().setSpaceProperty(spaceproperty);
            }
        }
    }
}

/** Set the temperature for all moves that have a constant temperature
    to 'temperature'. It has already been checked that these moves
    between them sample at constant temperature */
void WeightedMoves::_pvt_setTemperature(const Temperature &temperature)
{
    int nmoves = mvs.count();
    
    if (nmoves > 0)
    {
        tuple<MovePtr,double> *mvs_array = mvs.data();
        
        for (int i=0; i<nmoves; ++i)
        {
            if (mvs_array[i].get<0>().read().isConstantTemperature())
            {
                if (mvs_array[i].get<0>().read().temperature() != temperature)
                {
                    mvs_array[i].get<0>().edit().setTemperature(temperature);
                }
            }
        }
    }
}

/** Set the pressure for all moves that have a constant pressure
    to 'pressure'. It has already been checked that these moves
    between them sample at constant pressure */
void WeightedMoves::_pvt_setPressure(const Pressure &pressure)
{
    int nmoves = mvs.count();
    
    if (nmoves > 0)
    {
        tuple<MovePtr,double> *mvs_array = mvs.data();
        
        for (int i=0; i<nmoves; ++i)
        {
            if (mvs_array[i].get<0>().read().isConstantPressure())
            {
                if (mvs_array[i].get<0>().read().pressure() != pressure)
                {
                    mvs_array[i].get<0>().edit().setPressure(pressure);
                }
            }
        }
    }
}

/** Set the fugacity for all moves that have a constant fugacity
    to 'fugacity'. It has already been checked that these moves
    between them sample at constant fugacity */
void WeightedMoves::_pvt_setFugacity(const Pressure &fugacity)
{
    int nmoves = mvs.count();
    
    if (nmoves > 0)
    {
        tuple<MovePtr,double> *mvs_array = mvs.data();
        
        for (int i=0; i<nmoves; ++i)
        {
            if (mvs_array[i].get<0>().read().isConstantFugacity())
            {
                if (mvs_array[i].get<0>().read().fugacity() != fugacity)
                {
                    mvs_array[i].get<0>().edit().setFugacity(fugacity);
                }
            }
        }
    }
}

/** Return the moves available in this set */
QList<MovePtr> WeightedMoves::moves() const
{
    int nmoves = mvs.count();
    const tuple<MovePtr,double> *mvs_array = mvs.constData();
    
    QList<MovePtr> moves;
    
    for (int i=0; i<nmoves; ++i)
    {
        moves.append( mvs_array[i].get<0>() );
    }
    
    return moves;
}
