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

#include "wham.h"

#include "SireCAS/values.h"

#include "SireUnits/units.h"

#include "SireStream/datastream.h"
#include "SireStream/shareddatastream.h"

#include "SireError/errors.h"

using namespace Soiree;
using namespace SireMaths;
using namespace SireCAS;
using namespace SireUnits::Dimension;
using namespace SireUnits;
using namespace SireStream;

static const RegisterMetaType<WHAM> r_wham;

WHAM::Trajectory::Trajectory() : sz(0)
{}

WHAM::Trajectory::Trajectory(double value) : sz(0)
{
    this->add(value);
}

WHAM::Trajectory::Trajectory(MolarEnergy value) : sz(0)
{
    this->add(value);
}

WHAM::Trajectory::Trajectory(const QVector<double> &values) : sz(0)
{
    this->add(values);
}

WHAM::Trajectory::Trajectory(const QVector<MolarEnergy> &values) : sz(0)
{
    this->add(values);
}

WHAM::Trajectory::Trajectory(const Trajectory &other) 
     : values(other.values), sz(other.sz)
{}

WHAM::Trajectory::~Trajectory()
{}

WHAM::Trajectory& WHAM::Trajectory::operator=(const Trajectory &other)
{
    if (this != &other)
    {
        values = other.values;
        sz = other.sz;
    }
    
    return *this;
}

bool WHAM::Trajectory::operator==(const Trajectory &other) const
{
    return sz == other.sz and values == other.values;
}

bool WHAM::Trajectory::operator!=(const Trajectory &other) const
{
    return sz != other.sz or values != other.values;
}

void WHAM::Trajectory::add(double value)
{
    if (values.isEmpty() or values.at(values.count()-1).second != value)
    {
        values.append( QPair<quint64,double>(1, value) );
    }
    else
    {
        values[values.count()-1].first += 1;
    }
    
    sz += 1;
}

void WHAM::Trajectory::add(const MolarEnergy &value)
{
    this->add( value.value() );
}

void WHAM::Trajectory::add(const QVector<double> &vals)
{
    for (QVector<double>::const_iterator it = vals.constBegin();
         it != vals.constEnd();
         ++it)
    {
        if (values.isEmpty() or values.at(values.count()-1).second != *it)
        {
            values.append( QPair<quint64,double>(1, *it) );
        }
        else
        {
            values[values.count()-1].first += 1;
        }
    }
    
    sz += vals.count();
}

void WHAM::Trajectory::add(const QVector<MolarEnergy> &vals)
{
    for (QVector<MolarEnergy>::const_iterator it = vals.constBegin();
         it != vals.constEnd();
         ++it)
    {
        if (values.isEmpty() or values.at(values.count()-1).second != it->value())
        {
            values.append( QPair<quint64,double>(1, it->value()) );
        }
        else
        {
            values[values.count()-1].first += 1;
        }
    }
    
    sz += vals.count();
}

QVector<double> WHAM::Trajectory::toVector() const
{
    QVector<double> vec( sz );
    double *vecdata = vec.data();
    
    int n = 0;
    
    for (Trajectory::const_iterator it = values.constBegin();
         it != values.constEnd();
         ++it)
    {
        for (quint64 i=0; i<it->first; ++i)
        {
            vecdata[n] = it->second;
            n += 1;
        }
    }
    
    return vec;
}

QVector<MolarEnergy> WHAM::Trajectory::toEnergyVector() const
{
    QVector<MolarEnergy> vec( sz );
    MolarEnergy *vecdata = vec.data();
    
    int n = 0;
    
    for (Trajectory::const_iterator it = values.constBegin();
         it != values.constEnd();
         ++it)
    {
        for (quint64 i=0; i<it->first; ++i)
        {
            vecdata[n] = MolarEnergy(it->second);
            n += 1;
        }
    }
    
    return vec;
}

quint64 WHAM::Trajectory::count() const
{
    return sz;
}

quint64 WHAM::Trajectory::size() const
{
    return sz;
}

WHAM::Trajectory::const_iterator WHAM::Trajectory::constBegin() const
{
    return values.constBegin();
}

WHAM::Trajectory::const_iterator WHAM::Trajectory::constEnd() const
{
    return values.constEnd();
}

QDataStream& operator<<(QDataStream &ds, const WHAM::Trajectory &traj)
{
    ds << traj.values << traj.sz;
    return ds;
}

QDataStream& operator>>(QDataStream &ds, WHAM::Trajectory &traj)
{
    ds >> traj.values >> traj.sz;
    return ds;
}

QDataStream SIREMATHS_EXPORT &operator<<(QDataStream &ds, const WHAM &wham)
{
    writeHeader(ds, r_wham, 1);
    
    SharedDataStream sds(ds);
    
    sds << wham.coords_values << wham.umbrella_values
        << wham.reaction_coords << wham.temp.to(kelvin);
        
    return ds;
}

QDataStream SIREMATHS_EXPORT &operator>>(QDataStream &ds, WHAM &wham)
{
    VersionID v = readHeader(ds, r_wham);
    
    if (v == 1)
    {
        SharedDataStream sds(ds);
        
        double temp;
        
        sds >> wham.coords_values >> wham.umbrella_values
            >> wham.reaction_coords >> temp;
            
        wham.temp = temp*kelvin;
    }
    else
        throw version_error(v, "1", r_wham, CODELOC);
        
    return ds;
}

/** Null constructor */
WHAM::WHAM() : temp(25 * celsius)
{}

/** Construct to perform a WHAM analysis for a 1 dimensional umbrella simulation
    along the reaction coordinate represented by the symbol 'coordinate'.
    The umbrella simulation will be assumed to have been run at
    a temperature of 25 C */
WHAM::WHAM(const Symbol &coordinate) : temp(25 * celsius)
{
    reaction_coords.append(coordinate);
    reaction_coords.squeeze();
}

/** Construct to perform a WHAM analysis for a 1 dimensional umbrella simulation
    along the reaction coordinate represented by the symbol 'coordinate'.
    The umbrella simulation will have been run at the specified temperature */
WHAM::WHAM(const Symbol &coordinate, Temperature temperature) : temp(temperature)
{
    reaction_coords.append(coordinate);
    reaction_coords.squeeze();
}

static QVector<Symbol> removeDuplicates(const QVector<Symbol> &coordinates)
{
    bool has_duplicates = false;

    for (QVector<Symbol>::const_iterator it = coordinates.constBegin();
         it != coordinates.constEnd();
         ++it)
    {
        if (coordinates.count(*it) > 1)
        {
            has_duplicates = true;
            break;
        }
    }
    
    if (not has_duplicates)
        return coordinates;
        
    QVector<Symbol> pruned_coordinates(coordinates);
    
    QMutableVectorIterator<Symbol> it(pruned_coordinates);
    it.toBack();
    
    while (it.hasPrevious())
    {
        it.previous();
        
        if (pruned_coordinates.count(it.value()) > 1)
        {
           it.remove(); 
        }
    }
    
    return pruned_coordinates;
}

/** Construct to perform a WHAM analysis for an N-dimensional umbrella
    simulation along the reaction coordinates in 'coordinates'.
    The umbrella simulation will be assumed to have been run at
    a temperature of 25 C */
WHAM::WHAM(const QVector<Symbol> &coordinates)
     : reaction_coords( ::removeDuplicates(coordinates) ), temp(25 * celsius)
{}

/** Construct to perform a WHAM analysis for an N-dimensional umbrella
    simulation along the reaction coordinates in 'coordinates'.
    The umbrella simulation will have been run at the specified temperature */
WHAM::WHAM(const QVector<Symbol> &coordinates, Temperature temperature)
     : reaction_coords( ::removeDuplicates(coordinates) ), temp(temperature)
{}

/** Construct to perform a WHAM analysis for an N-dimensional umbrella
    simulation along the reaction coordinates in 'coordinates'.
    The umbrella simulation will be assumed to have been run at
    a temperature of 25 C */
WHAM::WHAM(const QSet<Symbol> &coordinates)
     : reaction_coords( coordinates.toVector() ), temp(25 * celsius)
{}

/** Construct to perform a WHAM analysis for an N-dimensional umbrella
    simulation along the reaction coordinates in 'coordinates'.
    The umbrella simulation will have been run at the specified temperature */
WHAM::WHAM(const QSet<Symbol> &coordinates, Temperature temperature)
     : reaction_coords( coordinates.toVector() ), temp(temperature)
{}

/** Copy constructor */
WHAM::WHAM(const WHAM &other)
     : coords_values(other.coords_values), umbrella_values(other.umbrella_values),
       reaction_coords(other.reaction_coords), temp(other.temp)
{}

/** Destructor */
WHAM::~WHAM()
{}

const char* WHAM::typeName()
{
    return QMetaType::typeName( qMetaTypeId<WHAM>() );
}

const char* WHAM::what() const
{
    return WHAM::typeName();
}

WHAM* WHAM::clone() const
{
    return new WHAM(*this);
}

/** Copy assignment operator */
WHAM& WHAM::operator=(const WHAM &other)
{
    if (this != &other)
    {
        coords_values = other.coords_values;
        umbrella_values = other.umbrella_values;
        reaction_coords = other.reaction_coords;
        temp = other.temp;
    }
    
    return *this;
}

/** Comparison operator */
bool WHAM::operator==(const WHAM &other) const
{
    return (this == &other) or
           (temp == other.temp and
            umbrella_values == other.umbrella_values and
            reaction_coords == other.reaction_coords and 
            coords_values == other.coords_values);
}

/** Comparison operator */
bool WHAM::operator!=(const WHAM &other) const
{
    return not WHAM::operator==(other);
}

/** Return the number of dimensions (reaction coordinates) of the 
    umbrella, and thus output PMF */
int WHAM::nReactionCoordinates() const
{
    return reaction_coords.count();
}

/** If this is a single reaction coordinate umbrella, then return
    the symbol that represents that reaction coordinate 
    
    \throw SireError::incompatible_error
*/
const Symbol& WHAM::reactionCoordinate() const
{
    if (not reaction_coords.count() == 1)
        throw SireError::incompatible_error( QObject::tr(
            "This function is only compatible with an umbrella composed "
            "from a single reaction coordinate. The number of reaction "
            "coordinates for this WHAM analysis is %1 (%2).")
                .arg(reaction_coords.count())
                .arg(Sire::toString(reaction_coords)), CODELOC );
                
    return reaction_coords.at(0);
}

/** Return all of the symbols that represent the reaction coordinates,
    in the same order as the values of the output PMF will be output */
const QVector<Symbol>& WHAM::reactionCoordinates() const
{
    return reaction_coords;
}

/** Return whether or not this is empty (has no trajectories) */
bool WHAM::isEmpty() const
{
    return reaction_coords.isEmpty() or umbrella_values.isEmpty();
}

/** Return the number of trajectories over which a WHAM analysis
    will be performed */
int WHAM::nTrajectories() const
{
    return umbrella_values.count();
}

/** Return the number of steps in the ith trajectory
    
    \throw SireError::invalid_index
*/
quint64 WHAM::nSteps(int i) const
{
    return umbrella_values.at( Index(i).map( umbrella_values.count() ).count();
}

/** Return the temperature of the trajectories (needed for the WHAM analysis) */
Temperature WHAM::temperature() const
{
    return temp;
}

/** Return the values of the reaction coordinates for the last (current)
    trajectory, for the single reaction coordinate for a 1-D umbrella
    
    \throw SireError::incompatible_error
*/
QVector<double> WHAM::coordinateTrajectory() const
{
    if (reaction_coords.count() != 1)
        throw SireError::incompatible_error( QObject::tr(
            "This function is only compatible with an umbrella composed "
            "from a single reaction coordinate. The number of reaction "
            "coordinates for this WHAM analysis is %1 (%2).")
                .arg(reaction_coords.count())
                .arg(Sire::toString(reaction_coords)), CODELOC );

    if (coords_values.isEmpty())
        return QVector<double>();
    else
        return coords_values.constBegin().value().last().toVector();
}

/** Return the values of the reaction coordinate 'coordinate' for the 
    last (current) trajectory 
    
    \throw SireError::invalid_arg
*/
QVector<double> WHAM::coordinateTrajectory(const Symbol &coordinate) const
{
    if (coords_values.isEmpty())
    {
        if (not reaction_coords.contains(coordinate))
            throw SireError::invalid_arg( QObject::tr(
                    "There is no reaction coordinate %1 - available reaction "
                    "coordinates are %2.")
                        .arg(coordinate.toString())
                        .arg(Sire::toString(reaction_coords)), CODELOC );
    
        return QVector<double>();
    }

    return coords_values[coordinate].last().toVector();
}

/** Return the last (current) umbrella potential trajectory */
QVector<MolarEnergy> WHAM::umbrellaTrajectory() const
{
    if (umbrella_values.isEmpty())
        return QVector<MolarEnergy>();
    else
        return umbrella_values.last().toEnergyVector();
}

/** Return the ith coordinate trajectory (this is only valid when there
    is only one reaction coordinate)
    
    \throw SireError::incompatible_error
    \throw SireError::invalid_index
*/
QVector<double> WHAM::coordinateTrajectory(int i) const
{
    if (reaction_coords.count() != 1)
        throw SireError::incompatible_error( QObject::tr(
            "This function is only compatible with an umbrella composed "
            "from a single reaction coordinate. The number of reaction "
            "coordinates for this WHAM analysis is %1 (%2).")
                .arg(reaction_coords.count())
                .arg(Sire::toString(reaction_coords)), CODELOC );

    QVector<Trajectory> traj = coords_values.constBegin().value();

    return traj.at( Index(i).map(traj.count()) ).last().toVector();
}

/** Return the ith coordinate trajectory for the reaction coordinate
    represented by symbol 'coordinate'
    
    \throw SireError::invalid_arg
    \throw SireError::invalid_index
*/
QVector<double> WHAM::coordinateTrajectory(const Symbol &coordinate, int i) const
{
    if (coords_values.isEmpty())
    {
        if (not reaction_coords.contains(coordinate))
            throw SireError::invalid_arg( QObject::tr(
                    "There is no reaction coordinate %1 - available reaction "
                    "coordinates are %2.")
                        .arg(coordinate.toString())
                        .arg(Sire::toString(reaction_coords)), CODELOC );
    }

    const QVector<Trajectory> traj = coords_values.value(coordinate);
    
    return traj.at( Index(i).map(traj.count()) ).last().toVector();
}

/** Return the ith umbrella potential trajectory */
QVector<MolarEnergy> WHAM::umbrellaTrajectory(int i) const
{
    return umbrella_values.at( Index(i).map(umbrella_values.count()) )
                          .last().toEnergyVector();
}

/** Add the passed reaction coordinate trajectory with associated umbrella
    potential trajectory onto the set. This is appended to the current
    trajectory, or is added as a new trajectory if 'new_trajectory'
    is true. This function is only valid if there is just a single reaction
    coordinate
    
    \throw SireError::incompatible_error
*/
WHAM& WHAM::add(const QVector<double> &coords,
                const QVector<MolarEnergy> &umbrella,
                bool new_trajectory)
{
    if (reaction_coords.count() != 1)
        throw SireError::incompatible_error( QObject::tr(
            "This function is only compatible with an umbrella composed "
            "from a single reaction coordinate. The number of reaction "
            "coordinates for this WHAM analysis is %1 (%2).")
                .arg(reaction_coords.count())
                .arg(Sire::toString(reaction_coords)), CODELOC );

    else if (coords.count() != umbrella.count())
        throw SireError::incompatible_error( QObject::tr(
                "You must ensure that the size of the coordinate trajectory (%1) "
                "is the same as the size of the umbrella trajectory (%2).")
                    .arg(coords.count()).arg(umbrella.count()), 
                        CODELOC );

    if (new_trajectory or coords_values.isEmpty())
    {
        coords_values[reaction_coords.at(0)].append( Trajectory(coords) );
        umbrella_values.append( Trajectory(umbrella) );
    }
    else
    {
        coords_values[reaction_coords.at(0)].last().add(coords);
        umbrella_values.last().add(umbrella);
    }
    
    return *this;
}

/** Add the passed reaction coordinate value with corresponding 
    umbrella value, appending it either to the current trajectory,
    or creating a new trajectory if "new_trajectory" is true.

    This function is only valid if there is just a single reaction
    coordinate
    
    \throw SireError::incompatible_error
*/    
WHAM& WHAM::add(double coords_value, MolarEnergy umbrella_value, bool new_trajectory)
{
    if (reaction_coords.count() != 1)
        throw SireError::incompatible_error( QObject::tr(
            "This function is only compatible with an umbrella composed "
            "from a single reaction coordinate. The number of reaction "
            "coordinates for this WHAM analysis is %1 (%2).")
                .arg(reaction_coords.count())
                .arg(Sire::toString(reaction_coords)), CODELOC );
    
    if (new_trajectory or coords_values.isEmpty())
    {
        coords_values[reaction_coords.at(0)].append( Trajectory(coords_value) );
        umbrella_values.append( Trajectory(umbrella_value) );
    }
    else
    {
        coords_values[reaction_coords.at(0)].last().add(coords_value);
        umbrella_values.last().add(umbrella_value);
    }
    
    return *this;
}

/** Add the passed reaction coordinate values together with the 
    associated umbrella potential values. This appends this
    trajectory to the existing trajectory, or will create
    a new trajectory if 'new_trajectory' is true. The size
    of the trajectory for all reaction coordinates and umbrella
    must be the same. This can only add values for reaction
    coordinates that are part of this WHAM analysis, and no
    reaction coordinates should be missing
    
    \throw SireError::invalid_arg
    \throw SireError::incompatible_error
*/
WHAM& WHAM::add(const QHash< Symbol,QVector<double> > &rc,
                const QVector<MolarEnergy> &umbrella,
                bool new_trajectory)
{
    if (rc.isEmpty() and umbrella.isEmpty())
        return *this;
        
    //make sure that the reaction coordinates are compatible
    if (rc.count() != reaction_coords.count())
    {
        if (rc.count() > reaction_coords.count())
            throw SireError::incompatible_error( QObject::tr(
                    "You have passed more reaction coordinates (%1) than "
                    "are supported by this WHAM analysis (%2). You've passed "
                    "%3, when the analysis uses %4.")
                        .arg(rc.count()).arg(reaction_coords.count())
                        .arg(Sire::toString(rc.keys()))
                        .arg(Sire::toString(reaction_coords)), CODELOC );
        else
            throw SireError::incompatible_error( QObject::tr(
                    "You have passed fewer reaction coordinates (%1) than "
                    "are supported by this WHAM analysis (%2). You've passed "
                    "%3, when the analysis uses %4.")
                        .arg(rc.count()).arg(reaction_coords.count())
                        .arg(Sire::toString(rc.keys()))
                        .arg(Sire::toString(reaction_coords)), CODELOC );
        
    }
    
    for (QVector<Symbol>::const_iterator it = reaction_coords.constBegin();
         it != reaction_coords.constEnd();
         ++it)
    {
        if (not rc.contains(*it))
            throw SireError::incompatible_error( QObject::tr(
                    "You have passed in values for reaction coordinates that "
                    "are not used in this WHAM analysis, and are also missing "
                    "reaction coordinates that are needed for this WHAM analysis. "
                    "You passed in %1, but the WHAM analysis needs %2.")
                        .arg(Sire::toString(rc.keys()))
                        .arg(Sire::toString(reaction_coords)), CODELOC );
    }

    //make sure that all trajectories have the same length
    int sz = umbrella.count();
    
    for (QHash< Symbol,QVector<double> >::const_iterator it = rc.constBegin();
         it != rc.constEnd();
         ++it)
    {
        if (it.value().count() != sz)
        {
            QStringList sizes;
            
            for (QHash< Symbol,QVector<double> >::const_iterator it2 = rc.constBegin();
                 it2 != rc.constEnd();
                 ++it2)
            {
                sizes.append( QObject::tr( "%1 == %2 steps").arg(it.key().toString())
                                                            .arg(it.value().count()) );
            }
            
            throw SireError::incompatible_error( QObject::tr(
                    "All of the reaction coordinate and umbrella potential "
                    "trajectories have to be the same length. The umbrella "
                    "potential trajectory has a size of %1 steps, while for the "
                    "reaction coordinates, then trajectory lengths are; %2.")
                        .arg(sz).arg(sizes.join(", ")), CODELOC );
        }
    }
    
    //ok, we can now finally add the values
    if (new_trajectory or coords_values.isEmpty())
    {
        for (QHash< Symbol,QVector<double> >::const_iterator it = rc.constBegin();
             it != rc.constEnd();
             ++it)
        {
            coords_values[it.key()].append( Trajectory(it.value()) );
        }
        
        umbrella_values.append( Trajectory(umbrella) );
    }
    else
    {
        for (QHash< Symbol,QVector<double> >::const_iterator it = rc.constBegin();
             it != rc.constEnd();
             ++it)
        {
            coords_values[it.key()].last().add(it.value());
        }
        
        umbrella_values.last().add(umbrella);
    }
    
    return *this;
}

/** Add the passed reaction coordinate values together with the 
    associated umbrella potential values. This appends this
    trajectory to the existing trajectory, or will create
    a new trajectory if 'new_trajectory' is true. This can 
    only add values for reaction coordinates that are 
    part of this WHAM analysis, and no reaction coordinates 
    should be missing
    
    \throw SireError::invalid_arg
    \throw SireError::incompatible_error
*/
WHAM& WHAM::add(const Values &rc, MolarEnergy umbrella, bool new_trajectory)
{
    //make sure that the reaction coordinates are compatible
    if (rc.count() != reaction_coords.count())
    {
        if (rc.count() > reaction_coords.count())
            throw SireError::incompatible_error( QObject::tr(
                    "You have passed more reaction coordinates (%1) than "
                    "are supported by this WHAM analysis (%2). You've passed "
                    "%3, when the analysis uses %4.")
                        .arg(rc.count()).arg(reaction_coords.count())
                        .arg(Sire::toString(rc.symbols()))
                        .arg(Sire::toString(reaction_coords)), CODELOC );
        else
            throw SireError::incompatible_error( QObject::tr(
                    "You have passed fewer reaction coordinates (%1) than "
                    "are supported by this WHAM analysis (%2). You've passed "
                    "%3, when the analysis uses %4.")
                        .arg(rc.count()).arg(reaction_coords.count())
                        .arg(Sire::toString(rc.symbols()))
                        .arg(Sire::toString(reaction_coords)), CODELOC );
        
    }
    
    for (QVector<Symbol>::const_iterator it = reaction_coords.constBegin();
         it != reaction_coords.constEnd();
         ++it)
    {
        if (not rc.contains(*it))
            throw SireError::incompatible_error( QObject::tr(
                    "You have passed in values for reaction coordinates that "
                    "are not used in this WHAM analysis, and are also missing "
                    "reaction coordinates that are needed for this WHAM analysis. "
                    "You passed in %1, but the WHAM analysis needs %2.")
                        .arg(Sire::toString(rc.symbols()))
                        .arg(Sire::toString(reaction_coords)), CODELOC );
    }

    //ok, we can now finally add the values
    if (new_trajectory or coords_values.isEmpty())
    {
        for (Values::const_iterator it = rc.constBegin();
             it != rc.constEnd();
             ++it)
        {
            coords_values[ Symbol(it.key()) ].append( Trajectory(it.value()) );
        }
        
        umbrella_values.append( Trajectory(umbrella) );
    }
    else
    {
        for (Values::const_iterator it = rc.constBegin();
             it != rc.constEnd();
             ++it)
        {
            coords_values[ Symbol(it.key()) ].last().add(it.value());
        }
        
        umbrella_values.last().add(umbrella);
    }
    
    return *this;
}

/** Add the contents of another WHAM analysis onto this analysis. This
    will either append the trajectories onto the ends of these trajectories
    (in which case, both WHAM analysis must contain the same number of 
    trajectories), or, if 'new_trajectory' is true, it will add
    the trajectories as new trajectories. The reaction coordinates
    for both WHAM analysis must, of course, be the same (although
    not necessarily in the same order)
    
    \throw SireError::incompatible_error
*/
WHAM& WHAM::add(const WHAM &other, bool new_trajectory)
{
    if (other.reaction_coords.isEmpty())
        return *this;
    
    else if (reaction_coords.isEmpty())
    {
        this->operator=(other);
        return *this;
    }
    
    if (temp != other.temp)
        throw SireError::incompatible_error( QObject::tr(
                "You cannot add the contents of a WHAM analysis "
                "at temperature %1 C to a WHAM analysis at "
                "%2 C - the temperatures must be identical.")
                    .arg(other.temp.to(celsius))
                    .arg(temp.to(celsius)), CODELOC );
    
    if (reaction_coords.count() != other.reaction_coords.count())
    {
        throw SireError::incompatible_error( QObject::tr(
                "You cannot add the contents of a WHAM analysis over "
                "reactions coordinates %1 to that over reaction coordinates "
                "%2, as the reaction coordinates must be identical!")
                    .arg( Sire::toString(other.reaction_coords) )
                    .arg( Sire::toString(reaction_coords) ), CODELOC );
    }
    
    for (QVector<Symbol>::const_iterator it = reaction_coords.constBegin();
         it != reaction_coords.constEnd();
         ++it)
    {
        if (not other.reaction_coords.contains(*it))
            throw SireError::incompatible_error( QObject::tr(
                    "You cannot add the contents of a WHAM analysis over "
                    "reactions coordinates %1 to that over reaction coordinates "
                    "%2, as the reaction coordinates must be identical!")
                        .arg( Sire::toString(other.reaction_coords) )
                        .arg( Sire::toString(reaction_coords) ), CODELOC );
    }
    
    if (other.umbrella_values.isEmpty())
    {
        return *this;
    }
    else (umbrella_values.isEmpty())
    {
        umbrella_values = other.umbrella_values;
        coords_values = other.coords_values;
        return *this;
    }
    
    if (new_trajectory)
    {
        //just add new trajectories for each reaction coordinate
        umbrella_values += other.umbrella_values;
        
        for (QHash< Symbol,QVector<Trajectory> >::const_iterator 
                                                    it = other.coords_values.constBegin();
             it != other.coords_values.constEnd();
             ++it)
        {
            coords_values[it.key()] += it.value();
        }
    }
    else
    {
        if (umbrella_values.count() != other.umbrella_values.count())
            throw SireError::incompatible_error( QObject::tr(
                    "You can not add the contents of the trajectories in "
                    "the passed WHAM analysis to the trajectories in this "
                    "WHAM analysis as the number of trajectories are "
                    "different (%1 vs. %2). Either make the number of "
                    "trajectories the same, or add the WHAM analysis "
                    "using 'new_trajectory=True'.")
                        .arg(umbrella_values.count())
                        .arg(other.umbrella_values.count()), CODELOC );
                        
        int ntraj = umbrella_values.count();
        
        for (int i=0; i<ntraj; ++i)
        {
            umbrella_values[i].add(other.umbrella_values[i]);
        }
        
        for (QHash< Symbol,QVector<Trajectory> >::const_iterator
                                            it = other.coords_values.constBegin();
             it != other.coords_values.constEnd();
             ++it)
        {
            QVector<Trajectory> c = coords_values[it.key()];
            const QVector<Trajectory> o = it.value();
        
            for (int i=0; i<ntraj; ++i)
            {
                c[i].add( o[i] );
            }
        }
    }
    
    return *this;
}

/** Solve the (possibly multi-dimensional) WHAM equations, returning
    the potential of mean force along the reaction coordinate(s) according
    to the passed histogram ranges */
QVector< QPair<QVector<double>,double> > WHAM::solve(
                                const QHash<Symbol,HistogramRange> &range) const
{
    return QVector< QPair<QVector<double>,double> >();
}                                

/** Solve the single dimensional WHAM equations, returning the potential
    of mean force along the reaction coordinate according to the passed
    histogram range
    
    \throw SireError::incompatible_error
*/
Histogram WHAM::solve(const HistogramRange &range) const
{
    return Histogram();
}
