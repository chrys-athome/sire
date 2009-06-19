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

#include "distancerestraint.h"

#include "SireFF/forcetable.h"

#include "SireCAS/symbols.h"
#include "SireCAS/values.h"
#include "SireCAS/conditional.h"
#include "SireCAS/power.h"

#include "SireStream/datastream.h"
#include "SireStream/shareddatastream.h"

#include "SireCAS/errors.h"

using namespace SireMM;
using namespace SireMol;
using namespace SireFF;
using namespace SireBase;
using namespace SireCAS;
using namespace SireMaths;
using namespace SireStream;
using namespace SireUnits::Dimension;

////////////
//////////// Implementation of DistanceRestraint
////////////

static const RegisterMetaType<DistanceRestraint> r_distrest;

/** Serialise to a binary datastream */
QDataStream SIREMM_EXPORT &operator<<(QDataStream &ds, 
                                      const DistanceRestraint &distrest)
{
    writeHeader(ds, r_distrest, 1);
    
    SharedDataStream sds(ds);
    
    sds << distrest.p0 << distrest.p1
        << distrest.nrg_expression
        << distrest.force_expression
        << static_cast<const Restraint3D&>(distrest);
        
    return ds;
}

/** Extract from a binary datastream */
QDataStream SIREMM_EXPORT &operator>>(QDataStream &ds, DistanceRestraint &distrest)
{
    VersionID v = readHeader(ds, r_distrest);
    
    if (v == 1)
    {
        SharedDataStream sds(ds);
        
        sds >> distrest.p0 >> distrest.p1
            >> distrest.nrg_expression
            >> distrest.force_expression
            >> static_cast<Restraint3D&>(distrest);

        distrest.intra_molecule_points = Point::intraMoleculePoints(distrest.p0,
                                                                    distrest.p1);
    }
    else
        throw version_error( v, "1", r_distrest, CODELOC );
        
    return ds;
}

Q_GLOBAL_STATIC_WITH_ARGS( Symbol, getRSymbol, ("r") );

/** Return the symbol that represents the distance between the
    two points ("r") */
const Symbol& DistanceRestraint::r()
{
    return *(getRSymbol());
}

/** Constructor */
DistanceRestraint::DistanceRestraint()
                  : ConcreteProperty<DistanceRestraint,Restraint3D>()
{}

static void validateFunction(const Expression &function,
                             const Symbol &symbol)
{
    if (not function.isFunction(symbol))
        throw SireCAS::missing_symbol( QObject::tr(
            "The expression used for a positional restraint must be "
            "a function of \"%1\". The passed expression (%2) is not "
            "usable as it is a function of %3.")
                .arg(symbol.toString())
                .arg(function.toString())
                .arg( Sire::toString(function.symbols()) ), CODELOC );
                
    if (function.symbols().count() > 1)
        throw SireError::invalid_arg( QObject::tr(
            "The expression used for a positional restraint must only "
            "be a function of \"%1\". The passed expression (%2) is a "
            "function of %3.")
                .arg(symbol.toString())
                .arg(function.toString())
                .arg( Sire::toString(function.symbols()) ), CODELOC );
}

/** Construct a restraint that acts between the two points 'point0' and 'point1',
    restraining the distance between these points using the expression 
    'restraint' */
DistanceRestraint::DistanceRestraint(const PointRef &point0, const PointRef &point1,
                                     const Expression &restraint)
                  : ConcreteProperty<DistanceRestraint,Restraint3D>(),
                    p0(point0), p1(point1), 
                    nrg_expression(restraint)
{
    if (nrg_expression.isConstant())
    {
        nrg_expression = nrg_expression.evaluate(Values());
        force_expression = 0;
    }
    else
    {
        ::validateFunction(nrg_expression, r());
        force_expression = nrg_expression.differentiate(r());
        
        if (force_expression.isConstant())
            force_expression = force_expression.evaluate(Values());
    }
    
    intra_molecule_points = Point::intraMoleculePoints(p0, p1);
}

/** Internal constructor used to construct a restraint between the two 
    points 'point0' and 'point1' that restrains the distance between
    the two points using the function 'nrg_restraint' and
    calculates the force using the function 'force_restraint'. It 
    is a good idea to ensure that 'force_restraint' really is the
    differential of 'nrg_restraint' with respect to r() */
DistanceRestraint::DistanceRestraint(const PointRef &point0, const PointRef &point1,
                                     const Expression &nrg_restraint,
                                     const Expression &force_restraint)
                  : ConcreteProperty<DistanceRestraint,Restraint3D>(),
                    p0(point0), p1(point1),
                    nrg_expression(nrg_restraint),
                    force_expression(force_restraint)
{
    if (nrg_expression.isConstant())
    {
        nrg_expression = nrg_expression.evaluate(Values());
    }
    else
    {
        ::validateFunction(nrg_expression, r());
    }

    if (force_expression.isConstant())
    {
        force_expression = force_expression.evaluate(Values());
    }
    else
    {
        ::validateFunction(force_expression, r());
    }
    
    intra_molecule_points = Point::intraMoleculePoints(p0, p1);
}

/** Copy constructor */
DistanceRestraint::DistanceRestraint(const DistanceRestraint &other)
                  : ConcreteProperty<DistanceRestraint,Restraint3D>(other),
                    p0(other.p0), p1(other.p1),
                    nrg_expression(other.nrg_expression),
                    force_expression(other.force_expression),
                    intra_molecule_points(other.intra_molecule_points)
{}

/** Destructor */
DistanceRestraint::~DistanceRestraint()
{}

/** Copy assignment operator */
DistanceRestraint& DistanceRestraint::operator=(const DistanceRestraint &other)
{
    if (this != &other)
    {
        Restraint3D::operator=(other);
        
        p0 = other.p0;
        p1 = other.p1;
        nrg_expression = other.nrg_expression;
        force_expression = other.force_expression;
        intra_molecule_points = other.intra_molecule_points;
    }
    
    return *this;
}

/** Comparison operator */
bool DistanceRestraint::operator==(const DistanceRestraint &other) const
{
    return this == &other or
           ( Restraint3D::operator==(other) and
             p0 == other.p0 and p1 == other.p1 and
             nrg_expression == other.nrg_expression and
             force_expression == other.force_expression);
}

/** Comparison operator */
bool DistanceRestraint::operator!=(const DistanceRestraint &other) const
{
    return not DistanceRestraint::operator==(other);
}

const char* DistanceRestraint::typeName()
{
    return QMetaType::typeName( qMetaTypeId<DistanceRestraint>() );
}

/** Return the first point */
const Point& DistanceRestraint::point0() const
{
    return p0.read();
}

/** Return the second point */
const Point& DistanceRestraint::point1() const
{
    return p1.read();
}

/** Set the space used to evaluate the energy of this restraint

    \throw SireVol::incompatible_space
*/
void DistanceRestraint::setSpace(const Space &new_space)
{
    if (not this->space().equals(new_space))
    {
        PointPtr old_p0 = p0;
    
        try
        {
            p0.edit().setSpace(new_space);
            p1.edit().setSpace(new_space);
            
            Restraint3D::setSpace(new_space);
        }
        catch(...)
        {
            p0 = old_p0;
            throw;
        }
    }
}

/** Return the function used to calculate the restraint energy */
const Expression& DistanceRestraint::restraintFunction() const
{
    return nrg_expression;
}

/** Return the function used to calculate the restraint force */
const Expression& DistanceRestraint::differentialRestraintFunction() const
{
    return force_expression;
}

/** Return the energy of this restraint */
MolarEnergy DistanceRestraint::energy() const
{
    if (nrg_expression.isConstant())
        return MolarEnergy( nrg_expression.evaluate(Values()) );
    else
    {
        double distance;
        
        if (intra_molecule_points)
            //we don't use the space when calculating intra-molecular distances
            distance = Vector::distance( p0.read().point(), p1.read().point() );
        else
            distance = this->space().calcDist( p0.read().point(),
                                               p1.read().point() );
                                                  
        return MolarEnergy( nrg_expression.evaluate( r() == distance ) );
    }
}

/** Calculate the force acting on the molecule in the forcetable 'forcetable' 
    caused by this restraint, and add it on to the forcetable scaled by 
    'scale_force' */
void DistanceRestraint::force(MolForceTable &forcetable, double scale_force) const
{
    bool in_p0 = p0.read().contains(forcetable.molNum());
    bool in_p1 = p1.read().contains(forcetable.molNum());
    
    if (not (in_p0 or in_p1))
        //this molecule is not affected by the restraint
        return;
        
    Vector delta;
    
    if (intra_molecule_points)
        delta = p1.read().point() - p0.read().point();
    else
    {
        const Vector &point0 = p0.read().point();
        Vector point1 = p1.read().point();
        
        point1 = this->space().getMinimumImage(point1, point0);
        
        delta = point1 - point0;
    }
      
    double distance = delta.length();

    Vector f = (force_expression.evaluate( r() == distance ) / distance) * delta;

    if (in_p0)
        p0.read().addForce(forcetable, -scale_force*f);
    
    if (in_p1)
        p1.read().addForce(forcetable, scale_force*f);
}

/** Calculate the force acting on the molecules in the forcetable 'forcetable' 
    caused by this restraint, and add it on to the forcetable scaled by 
    'scale_force' */
void DistanceRestraint::force(ForceTable &forcetable, double scale_force) const
{
    bool in_p0 = p0.read().usesMoleculesIn(forcetable);
    bool in_p1 = p1.read().usesMoleculesIn(forcetable);
    
    if (not (in_p0 or in_p1))
        //this molecule is not affected by the restraint
        return;

    Vector delta;
    
    if (intra_molecule_points)
        delta = p1.read().point() - p0.read().point();
    else
    {
        const Vector &point0 = p0.read().point();
        Vector point1 = p1.read().point();
        
        point1 = this->space().getMinimumImage(point1, point0);
        
        delta = point1 - point0;
    }
      
    double distance = delta.length();

    Vector f = (force_expression.evaluate( r() == distance ) / distance) * delta;

    if (in_p0)
        p0.read().addForce(forcetable, -scale_force*f);
    
    if (in_p1)
        p1.read().addForce(forcetable, scale_force*f);
}

/** Update the points of this restraint using new molecule data from 'moldata'

    \throw SireBase::missing_property
    \throw SireError::invalid_cast
    \throw SireError::incompatible_error
*/
void DistanceRestraint::update(const MoleculeData &moldata, const PropertyMap &map)
{
    bool in_p0 = p0.read().contains(moldata.number());
    bool in_p1 = p1.read().contains(moldata.number());
    
    if (in_p0 and in_p1)
    {
        PointPtr old_p0 = p0;
        
        try
        {
            p0.edit().update(moldata, map);
            p1.edit().update(moldata, map);
        }
        catch(...)
        {
            p0 = old_p0;
            throw;
        }
    }
    else if (in_p0)
    {
        p0.edit().update(moldata, map);
    }
    else if (in_p1)
    {
        p1.edit().update(moldata, map);
    }
}
            
/** Update the points of this restraint using new molecule data from 'molecules'

    \throw SireBase::missing_property
    \throw SireError::invalid_cast
    \throw SireError::incompatible_error
*/
void DistanceRestraint::update(const Molecules &molecules, const PropertyMap &map)
{
    bool in_p0 = p0.read().usesMoleculesIn(molecules);
    bool in_p1 = p1.read().usesMoleculesIn(molecules);
    
    if (in_p0 and in_p1)
    {
        PointPtr old_p0 = p0;
        
        try
        {
            p0.edit().update(molecules, map);
            p1.edit().update(molecules, map);
        }
        catch(...)
        {
            p0 = old_p0;
            throw;
        }
    }
    else if (in_p0)
    {
        p0.edit().update(molecules, map);
    }
    else if (in_p1)
    {
        p1.edit().update(molecules, map);
    }
}

/** Return the molecules used in this restraint */
Molecules DistanceRestraint::molecules() const
{
    int n0 = p0.read().nMolecules();
    int n1 = p1.read().nMolecules();
    
    if (n0 != 0 and n1 != 0)
    {
        Molecules mols = p0.read().molecules();
        mols += p1.read().molecules();
        
        return mols;
    }
    else if (n0 != 0)
    {
        return p0.read().molecules();
    }
    else if (n1 != 0)
    {
        return p1.read().molecules();
    }
    else
        return Molecules();
}

/** Return whether or not this restraint affects the molecule
    with number 'molnum' */
bool DistanceRestraint::contains(MolNum molnum) const
{
    return p0.read().contains(molnum) or p1.read().contains(molnum);
}

/** Return whether or not this restraint affects the molecule
    with ID 'molid' */
bool DistanceRestraint::contains(const MolID &molid) const
{
    return p0.read().contains(molid) or p1.read().contains(molid);
}

static Expression harmonicFunction(double force_constant)
{
    if (SireMaths::isZero(force_constant))
        return 0;
    else
        return force_constant * pow(DistanceRestraint::r(), 2);
}

static Expression diffHarmonicFunction(double force_constant)
{
    if (SireMaths::isZero(force_constant))
        return 0;
    else
        return (2*force_constant) * DistanceRestraint::r();
}

/** Return a distance restraint that applies a harmonic potential between 
    the points 'point0' and 'point1' using a force constant 'force_constant'
    and using the (optionally supplied) property map to find the 
    required properties */
DistanceRestraint DistanceRestraint::harmonic(
                                  const PointRef &point0,
                                  const PointRef &point1,
                                  const HarmonicDistanceForceConstant &force_constant)
{
    return DistanceRestraint(point0, point1,
                             ::harmonicFunction(force_constant), 
                             ::diffHarmonicFunction(force_constant));
}

static Expression halfHarmonicFunction(double force_constant, double length)
{
    if ( SireMaths::isZero(force_constant) )
        return 0;
        
    else if ( length <= 0 )
        //this is just a harmonic function
        return ::harmonicFunction(force_constant);

    else
    {
        const Symbol &r = DistanceRestraint::r();
        return Conditional( 
                GreaterThan(r, length), force_constant * pow(r-length, 2), 0 );
    }
}

static Expression diffHalfHarmonicFunction(double force_constant, double distance)
{
    if ( SireMaths::isZero(force_constant) )
        return 0;
    
    else if (distance <= 0)
        //this is just a harmonic function
        return ::diffHarmonicFunction(force_constant);
    
    else
    {
        const Symbol &r = DistanceRestraint::r();
        return Conditional( GreaterThan(r, distance), 
                                (2*force_constant) * (r-distance), 0 );
    }
}

/** Return a distance restraint that applied a half-harmonic potential 
    between the points 'point0' and 'point1' above a distance 'distance'
    using a force constant 'force_constant' and using the
    (optionally supplied) property map out find the required properties */
DistanceRestraint DistanceRestraint::halfHarmonic(
                                      const PointRef &point0,
                                      const PointRef &point1,
                                      const Length &distance,
                                      const HarmonicDistanceForceConstant &force_constant)
{
    return DistanceRestraint(point0, point1,
                             ::halfHarmonicFunction(force_constant, distance),
                             ::diffHalfHarmonicFunction(force_constant, distance));
}

////////////
//////////// Implementation of DistanceRestraints
////////////

////////////
//////////// Implementation of DoubleDistanceRestraint
////////////


////////////
//////////// Implementation of TripleDistanceRestraint
////////////
