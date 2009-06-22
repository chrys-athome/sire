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

#include "SireID/index.h"

#include "SireStream/datastream.h"
#include "SireStream/shareddatastream.h"

#include "SireCAS/errors.h"

using namespace SireMM;
using namespace SireMol;
using namespace SireFF;
using namespace SireID;
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
    
    sds << distrest.p[0] << distrest.p[1]
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
        
        sds >> distrest.p[0] >> distrest.p[1]
            >> distrest.nrg_expression
            >> distrest.force_expression
            >> static_cast<Restraint3D&>(distrest);

        distrest.intra_molecule_points = Point::intraMoleculePoints(distrest.p[0],
                                                                    distrest.p[1]);
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

/** Return all of the symbols that can be used in this restraint function */
Symbols DistanceRestraint::symbols()
{
    return Symbols(r());
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
                    nrg_expression(restraint)
{
    p[0] = point0;
    p[1] = point1;

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
    
    intra_molecule_points = Point::intraMoleculePoints(p[0], p[1]);
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
                    nrg_expression(nrg_restraint),
                    force_expression(force_restraint)
{
    p[0] = point0;
    p[1] = point1;

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
    
    intra_molecule_points = Point::intraMoleculePoints(p[0], p[1]);
}

/** Copy constructor */
DistanceRestraint::DistanceRestraint(const DistanceRestraint &other)
                  : ConcreteProperty<DistanceRestraint,Restraint3D>(other),
                    nrg_expression(other.nrg_expression),
                    force_expression(other.force_expression),
                    intra_molecule_points(other.intra_molecule_points)
{
    for (int i=0; i<2; ++i)
    {
        p[i] = other.p[i];
    }
}

/** Destructor */
DistanceRestraint::~DistanceRestraint()
{}

/** Copy assignment operator */
DistanceRestraint& DistanceRestraint::operator=(const DistanceRestraint &other)
{
    if (this != &other)
    {
        Restraint3D::operator=(other);

        for (int i=0; i<2; ++i)
        {
            p[i] = other.p[i];
        }
        
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
             p[0] == other.p[0] and p[1] == other.p[1] and
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

/** This restraint involves two points */
int DistanceRestraint::nPoints() const
{
    return 2;
}

/** Return the ith point */
const Point& DistanceRestraint::point(int i) const
{
    i = Index(i).map( this->nPoints() );

    return p[i].read();
}

/** Return the first point */
const Point& DistanceRestraint::point0() const
{
    return p[0].read();
}

/** Return the second point */
const Point& DistanceRestraint::point1() const
{
    return p[1].read();
}

/** Set the space used to evaluate the energy of this restraint

    \throw SireVol::incompatible_space
*/
void DistanceRestraint::setSpace(const Space &new_space)
{
    if (not this->space().equals(new_space))
    {
        PointPtr old_p0 = p[0];
    
        try
        {
            p[0].edit().setSpace(new_space);
            p[1].edit().setSpace(new_space);
            
            Restraint3D::setSpace(new_space);
        }
        catch(...)
        {
            p[0] = old_p0;
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
            distance = Vector::distance( p[0].read().point(), p[1].read().point() );
        else
            distance = this->space().calcDist( p[0].read().point(),
                                               p[1].read().point() );
                                                  
        return MolarEnergy( nrg_expression.evaluate( r() == distance ) );
    }
}

/** Calculate the force acting on the molecule in the forcetable 'forcetable' 
    caused by this restraint, and add it on to the forcetable scaled by 
    'scale_force' */
void DistanceRestraint::force(MolForceTable &forcetable, double scale_force) const
{
    bool in_p0 = p[0].read().contains(forcetable.molNum());
    bool in_p1 = p[1].read().contains(forcetable.molNum());
    
    if (not (in_p0 or in_p1))
        //this molecule is not affected by the restraint
        return;
        
    Vector delta;
    
    if (intra_molecule_points)
        delta = p[1].read().point() - p[0].read().point();
    else
    {
        const Vector &point0 = p[0].read().point();
        Vector point1 = p[1].read().point();
        
        point1 = this->space().getMinimumImage(point1, point0);
        
        delta = point1 - point0;
    }
      
    double distance = delta.length();

    Vector f = (force_expression.evaluate( r() == distance ) / distance) * delta;

    if (not f.isZero())
    {
        if (in_p0)
            p[0].read().addForce(forcetable, -scale_force*f);
    
        if (in_p1)
            p[1].read().addForce(forcetable, scale_force*f);
    }
}

/** Calculate the force acting on the molecules in the forcetable 'forcetable' 
    caused by this restraint, and add it on to the forcetable scaled by 
    'scale_force' */
void DistanceRestraint::force(ForceTable &forcetable, double scale_force) const
{
    bool in_p0 = p[0].read().usesMoleculesIn(forcetable);
    bool in_p1 = p[1].read().usesMoleculesIn(forcetable);
    
    if (not (in_p0 or in_p1))
        //this molecule is not affected by the restraint
        return;

    Vector delta;
    
    if (intra_molecule_points)
        delta = p[1].read().point() - p[0].read().point();
    else
    {
        const Vector &point0 = p[0].read().point();
        Vector point1 = p[1].read().point();
        
        point1 = this->space().getMinimumImage(point1, point0);
        
        delta = point1 - point0;
    }
      
    double distance = delta.length();

    Vector f = (force_expression.evaluate( r() == distance ) / distance) * delta;

    if (not f.isZero())
    {
        if (in_p0)
            p[0].read().addForce(forcetable, -scale_force*f);
    
        if (in_p1)
            p[1].read().addForce(forcetable, scale_force*f);
    }
}

/** Update the points of this restraint using new molecule data from 'moldata'

    \throw SireBase::missing_property
    \throw SireError::invalid_cast
    \throw SireError::incompatible_error
*/
void DistanceRestraint::update(const MoleculeData &moldata, const PropertyMap &map)
{
    bool in_p0 = p[0].read().contains(moldata.number());
    bool in_p1 = p[1].read().contains(moldata.number());
    
    if (in_p0 and in_p1)
    {
        PointPtr old_p0 = p[1];
        
        try
        {
            p[0].edit().update(moldata, map);
            p[1].edit().update(moldata, map);
        }
        catch(...)
        {
            p[0] = old_p0;
            throw;
        }
    }
    else if (in_p0)
    {
        p[0].edit().update(moldata, map);
    }
    else if (in_p1)
    {
        p[1].edit().update(moldata, map);
    }
}
            
/** Update the points of this restraint using new molecule data from 'molecules'

    \throw SireBase::missing_property
    \throw SireError::invalid_cast
    \throw SireError::incompatible_error
*/
void DistanceRestraint::update(const Molecules &molecules, const PropertyMap &map)
{
    bool in_p0 = p[0].read().usesMoleculesIn(molecules);
    bool in_p1 = p[1].read().usesMoleculesIn(molecules);
    
    if (in_p0 and in_p1)
    {
        PointPtr old_p0 = p[0];
        
        try
        {
            p[0].edit().update(molecules, map);
            p[1].edit().update(molecules, map);
        }
        catch(...)
        {
            p[0] = old_p0;
            throw;
        }
    }
    else if (in_p0)
    {
        p[0].edit().update(molecules, map);
    }
    else if (in_p1)
    {
        p[1].edit().update(molecules, map);
    }
}

/** Return the molecules used in this restraint */
Molecules DistanceRestraint::molecules() const
{
    int n0 = p[0].read().nMolecules();
    int n1 = p[1].read().nMolecules();
    
    if (n0 != 0 and n1 != 0)
    {
        Molecules mols = p[0].read().molecules();
        mols += p[1].read().molecules();
        
        return mols;
    }
    else if (n0 != 0)
    {
        return p[0].read().molecules();
    }
    else if (n1 != 0)
    {
        return p[1].read().molecules();
    }
    else
        return Molecules();
}

/** Return whether or not this restraint affects the molecule
    with number 'molnum' */
bool DistanceRestraint::contains(MolNum molnum) const
{
    return p[0].read().contains(molnum) or p[1].read().contains(molnum);
}

/** Return whether or not this restraint affects the molecule
    with ID 'molid' */
bool DistanceRestraint::contains(const MolID &molid) const
{
    return p[0].read().contains(molid) or p[1].read().contains(molid);
}
    
/** Return whether or not this restraint involves any of the molecules
    that are in the forcetable 'forcetable' */
bool DistanceRestraint::usesMoleculesIn(const ForceTable &forcetable) const
{
    return p[0].read().usesMoleculesIn(forcetable) or 
           p[1].read().usesMoleculesIn(forcetable);
}

/** Return whether or not this restraint involves any of the molecules
    in 'molecules' */
bool DistanceRestraint::usesMoleculesIn(const Molecules &molecules) const
{
    return p[0].read().usesMoleculesIn(molecules) or
           p[1].read().usesMoleculesIn(molecules);
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
//////////// Implementation of DoubleDistanceRestraint
////////////

static const RegisterMetaType<DoubleDistanceRestraint> r_doubledistrest;

/** Serialise to a binary datastream */
QDataStream SIREMM_EXPORT &operator<<(QDataStream &ds, 
                                      const DoubleDistanceRestraint &doubledistrest)
{
    writeHeader(ds, r_doubledistrest, 1);
    
    SharedDataStream sds(ds);
    
    sds << doubledistrest.p[0] << doubledistrest.p[1]
        << doubledistrest.p[2] << doubledistrest.p[3]
        << doubledistrest.nrg_expression
        << doubledistrest.force01_expression
        << doubledistrest.force23_expression
        << static_cast<const Restraint3D&>(doubledistrest);
        
    return ds;
}

/** Extract from a binary datastream */
QDataStream SIREMM_EXPORT &operator>>(QDataStream &ds, 
                                      DoubleDistanceRestraint &doubledistrest)
{
    VersionID v = readHeader(ds, r_doubledistrest);
    
    if (v == 1)
    {
        SharedDataStream sds(ds);
        
        sds >> doubledistrest.p[0] >> doubledistrest.p[1]
            >> doubledistrest.p[2] >> doubledistrest.p[3]
            >> doubledistrest.nrg_expression
            >> doubledistrest.force01_expression
            >> doubledistrest.force23_expression
            >> static_cast<Restraint3D&>(doubledistrest);

        doubledistrest.intra_molecule_points01 = Point::intraMoleculePoints(
                                                                    doubledistrest.p[0],
                                                                    doubledistrest.p[1]);
                                                                    
        doubledistrest.intra_molecule_points23 = Point::intraMoleculePoints(
                                                                    doubledistrest.p[2],
                                                                    doubledistrest.p[3]);
    }
    else
        throw version_error( v, "1", r_doubledistrest, CODELOC );
        
    return ds;
}

Q_GLOBAL_STATIC_WITH_ARGS( Symbol, getR01Symbol, ("r01") );
Q_GLOBAL_STATIC_WITH_ARGS( Symbol, getR23Symbol, ("r23") );

/** Return the symbol that represents the distance between the
    points 0 and 1 ("r01") */
const Symbol& DoubleDistanceRestraint::r01()
{
    return *(getR01Symbol());
}

/** Return the symbol that represents the distance between the
    points 2 and 3 ("r23") */
const Symbol& DoubleDistanceRestraint::r23()
{
    return *(getR23Symbol());
}

/** Return all of the symbols that can be used in this restraint function */
Symbols DoubleDistanceRestraint::symbols()
{
    Symbols syms;
    syms.insert( r01() );
    syms.insert( r23() );
    
    return syms;
}

/** Constructor */
DoubleDistanceRestraint::DoubleDistanceRestraint()
                        : ConcreteProperty<DoubleDistanceRestraint,Restraint3D>()
{}

static void validateFunction(const Expression &function,
                             const Symbols &symbols)
{
    if (not function.symbols().intersects(symbols))
        throw SireCAS::missing_symbol( QObject::tr(
            "The expression used for a positional restraint must be "
            "a function of at least one of \"%1\". The passed expression (%2) is not "
            "usable as it is a function of %3.")
                .arg( Sire::toString(symbols) )
                .arg( function.toString() )
                .arg( Sire::toString(function.symbols()) ), CODELOC );
                
    if (not symbols.contains(function.symbols()))
        throw SireError::invalid_arg( QObject::tr(
            "The expression used for a positional restraint must only "
            "be a function of \"%1\". The passed expression (%2) is a "
            "function of %3.")
                .arg( Sire::toString(symbols) )
                .arg(function.toString())
                .arg( Sire::toString(function.symbols()) ), CODELOC );
}

/** Construct a restraint that acts on the two distances defined
    using the passed four points, using the expression 'restraint' */
DoubleDistanceRestraint::DoubleDistanceRestraint(const PointRef &point0, 
                                                 const PointRef &point1,
                                                 const PointRef &point2,
                                                 const PointRef &point3,
                                                 const Expression &restraint)
                  : ConcreteProperty<DoubleDistanceRestraint,Restraint3D>(),
                    nrg_expression(restraint)
{
    p[0] = point0;
    p[1] = point1;
    p[2] = point2;
    p[3] = point3;

    if (nrg_expression.isConstant())
    {
        nrg_expression = nrg_expression.evaluate(Values());
        force01_expression = 0;
        force23_expression = 0;
    }
    else
    {
        ::validateFunction(nrg_expression, symbols());
        force01_expression = nrg_expression.differentiate(r01());
        force23_expression = nrg_expression.differentiate(r23());
        
        if (force01_expression.isConstant())
            force01_expression = force01_expression.evaluate(Values());
        
        if (force23_expression.isConstant())
            force23_expression = force23_expression.evaluate(Values());
    }
    
    intra_molecule_points01 = Point::intraMoleculePoints(p[0], p[1]);
    intra_molecule_points23 = Point::intraMoleculePoints(p[2], p[3]);
}

/** Copy constructor */
DoubleDistanceRestraint::DoubleDistanceRestraint(const DoubleDistanceRestraint &other)
                        : ConcreteProperty<DoubleDistanceRestraint,Restraint3D>(other),
                          nrg_expression(other.nrg_expression),
                          force01_expression(other.force01_expression),
                          force23_expression(other.force23_expression),
                          intra_molecule_points01(other.intra_molecule_points01),
                          intra_molecule_points23(other.intra_molecule_points23)
{
    for (int i=0; i<this->nPoints(); ++i)
    {
        p[i] = other.p[i];
    }
}

/** Destructor */
DoubleDistanceRestraint::~DoubleDistanceRestraint()
{}

/** Copy assignment operator */
DoubleDistanceRestraint& DoubleDistanceRestraint::operator=(
                                                    const DoubleDistanceRestraint &other)
{
    if (this != &other)
    {
        Restraint3D::operator=(other);

        for (int i=0; i<this->nPoints(); ++i)
        {
            p[i] = other.p[i];
        }
        
        nrg_expression = other.nrg_expression;
        force01_expression = other.force01_expression;
        force23_expression = other.force23_expression;
        intra_molecule_points01 = other.intra_molecule_points01;
        intra_molecule_points23 = other.intra_molecule_points23;
    }
    
    return *this;
}

/** Comparison operator */
bool DoubleDistanceRestraint::operator==(const DoubleDistanceRestraint &other) const
{
    return this == &other or
           ( Restraint3D::operator==(other) and
             p[0] == other.p[0] and p[1] == other.p[1] and
             p[2] == other.p[2] and p[3] == other.p[3] and
             nrg_expression == other.nrg_expression and
             force01_expression == other.force01_expression and
             force23_expression == other.force23_expression);
}

/** Comparison operator */
bool DoubleDistanceRestraint::operator!=(const DoubleDistanceRestraint &other) const
{
    return not DoubleDistanceRestraint::operator==(other);
}

const char* DoubleDistanceRestraint::typeName()
{
    return QMetaType::typeName( qMetaTypeId<DoubleDistanceRestraint>() );
}

/** This restraint involves four points */
int DoubleDistanceRestraint::nPoints() const
{
    return 4;
}

/** Return the ith point */
const Point& DoubleDistanceRestraint::point(int i) const
{
    i = Index(i).map( this->nPoints() );

    return p[i].read();
}

/** Return the first point */
const Point& DoubleDistanceRestraint::point0() const
{
    return p[0].read();
}

/** Return the second point */
const Point& DoubleDistanceRestraint::point1() const
{
    return p[1].read();
}

/** Return the third point */
const Point& DoubleDistanceRestraint::point2() const
{
    return p[2].read();
}

/** Return the fourth point */
const Point& DoubleDistanceRestraint::point3() const
{
    return p[3].read();
}

/** Set the space used to evaluate the energy of this restraint

    \throw SireVol::incompatible_space
*/
void DoubleDistanceRestraint::setSpace(const Space &new_space)
{
    if (not this->space().equals(new_space))
    {
        DoubleDistanceRestraint old_state(*this);
    
        try
        {
            for (int i=0; i<this->nPoints(); ++i)
            {
                p[i].edit().setSpace(new_space);
            }
            
            Restraint3D::setSpace(new_space);
        }
        catch(...)
        {
            DoubleDistanceRestraint::operator=(old_state);
            throw;
        }
    }
}

/** Return the function used to calculate the restraint energy */
const Expression& DoubleDistanceRestraint::restraintFunction() const
{
    return nrg_expression;
}

/** Return the function used to calculate the restraint force along the 
    distance r01 */
const Expression& DoubleDistanceRestraint::differentialRestraintFunction01() const
{
    return force01_expression;
}

/** Return the function used to calculate the restraint force along the 
    distance r23 */
const Expression& DoubleDistanceRestraint::differentialRestraintFunction23() const
{
    return force23_expression;
}

/** Return the energy of this restraint */
MolarEnergy DoubleDistanceRestraint::energy() const
{
    if (nrg_expression.isConstant())
        return MolarEnergy( nrg_expression.evaluate(Values()) );
    else
    {
        double d01;
        double d23;
        
        if (intra_molecule_points01)
            //we don't use the space when calculating intra-molecular distances
            d01 = Vector::distance( p[0].read().point(), p[1].read().point() );
        else
            d01 = this->space().calcDist( p[0].read().point(),
                                          p[1].read().point() );
        
        if (intra_molecule_points23)
            d23 = Vector::distance( p[2].read().point(), p[3].read().point() );
        else
            d23 = this->space().calcDist( p[2].read().point(),
                                          p[3].read().point() );
                                                                                                                                      
        return MolarEnergy( nrg_expression.evaluate( (r01() == d01) +
                                                     (r23() == d23) ) );
    }
}

/** Calculate the force acting on the molecule in the forcetable 'forcetable' 
    caused by this restraint, and add it on to the forcetable scaled by 
    'scale_force' */
void DoubleDistanceRestraint::force(MolForceTable &forcetable, double scale_force) const
{
    bool in_p0 = p[0].read().contains(forcetable.molNum());
    bool in_p1 = p[1].read().contains(forcetable.molNum());
    bool in_p2 = p[2].read().contains(forcetable.molNum());
    bool in_p3 = p[3].read().contains(forcetable.molNum());
    
    if (not (in_p0 or in_p1 or in_p2 or in_p3))
        //this molecule is not affected by the restraint
        return;
        
    Vector delta01;
    Vector delta23;
    
    if (intra_molecule_points01)
        delta01 = p[1].read().point() - p[0].read().point();
    else
    {
        const Vector &point0 = p[0].read().point();
        Vector point1 = p[1].read().point();
        
        point1 = this->space().getMinimumImage(point1, point0);
        
        delta01 = point1 - point0;
    }
      
    if (intra_molecule_points23)
        delta23 = p[3].read().point() - p[2].read().point();
    else
    {
        const Vector &point2 = p[2].read().point();
        Vector point3 = p[3].read().point();
        
        point3 = this->space().getMinimumImage(point3, point2);
        
        delta23 = point3 - point2;
    }
      
    double d01 = delta01.length();
    double d23 = delta23.length();

    Values vals = (r01() == d01) + (r23() == d23);

    if (in_p0 or in_p1)
    {
        Vector f01 = (force01_expression.evaluate(vals) / d01) * delta01;
        
        if (not f01.isZero())
        {
            if (in_p0)
                p[0].read().addForce(forcetable, -scale_force*f01);
    
            if (in_p1)
                p[1].read().addForce(forcetable, scale_force*f01);
        }
    }

    if (in_p2 or in_p3)
    {
        Vector f23 = (force23_expression.evaluate(vals) / d23) * delta23;
    
        if (not f23.isZero())
        {
            if (in_p2)
                p[2].read().addForce(forcetable, -scale_force*f23);
    
            if (in_p3)
                p[3].read().addForce(forcetable, scale_force*f23);
        }
    }
}

/** Calculate the force acting on the molecules in the forcetable 'forcetable' 
    caused by this restraint, and add it on to the forcetable scaled by 
    'scale_force' */
void DoubleDistanceRestraint::force(ForceTable &forcetable, double scale_force) const
{
    bool in_p0 = p[0].read().usesMoleculesIn(forcetable);
    bool in_p1 = p[1].read().usesMoleculesIn(forcetable);
    bool in_p2 = p[2].read().usesMoleculesIn(forcetable);
    bool in_p3 = p[3].read().usesMoleculesIn(forcetable);
    
    if (not (in_p0 or in_p1 or in_p2 or in_p3))
        //this molecule is not affected by the restraint
        return;

    Vector delta01;
    Vector delta23;
    
    if (intra_molecule_points01)
        delta01 = p[1].read().point() - p[0].read().point();
    else
    {
        const Vector &point0 = p[0].read().point();
        Vector point1 = p[1].read().point();
        
        point1 = this->space().getMinimumImage(point1, point0);
        
        delta01 = point1 - point0;
    }
    
    if (intra_molecule_points23)
        delta23 = p[3].read().point() - p[2].read().point();
    else
    {
        const Vector &point2 = p[2].read().point();
        Vector point3 = p[3].read().point();
        
        point3 = this->space().getMinimumImage(point3, point2);
        
        delta23 = point3 - point2;
    }
      
    double d01 = delta01.length();
    double d23 = delta23.length();

    Values vals = (r01() == d01) + (r23() == d23);

    if (in_p0 or in_p1)
    {
        Vector f01 = (force01_expression.evaluate(vals) / d01) * delta01;
        
        if (not f01.isZero())
        {
            if (in_p0)
                p[0].read().addForce(forcetable, -scale_force*f01);
    
            if (in_p1)
                p[1].read().addForce(forcetable, scale_force*f01);
        }
    }

    if (in_p2 or in_p3)
    {
        Vector f23 = (force23_expression.evaluate(vals) / d23) * delta23;
    
        if (not f23.isZero())
        {
            if (in_p2)
                p[2].read().addForce(forcetable, -scale_force*f23);
    
            if (in_p3)
                p[3].read().addForce(forcetable, scale_force*f23);
        }
    }
}

/** Update the points of this restraint using new molecule data from 'moldata'

    \throw SireBase::missing_property
    \throw SireError::invalid_cast
    \throw SireError::incompatible_error
*/
void DoubleDistanceRestraint::update(const MoleculeData &moldata, const PropertyMap &map)
{
    if (this->contains(moldata.number()))
    {
        DoubleDistanceRestraint old_state(*this);
        
        try
        {
            for (int i=0; i<this->nPoints(); ++i)
            {
                p[i].edit().update(moldata, map);
            }
        }
        catch(...)
        {
            DoubleDistanceRestraint::operator=(old_state);
            throw;
        }
    }
}
            
/** Update the points of this restraint using new molecule data from 'molecules'

    \throw SireBase::missing_property
    \throw SireError::invalid_cast
    \throw SireError::incompatible_error
*/
void DoubleDistanceRestraint::update(const Molecules &molecules, const PropertyMap &map)
{
    if (this->usesMoleculesIn(molecules))
    {
        DoubleDistanceRestraint old_state(*this);
        
        try
        {
            for (int i=0; i<this->nPoints(); ++i)
            {
                p[i].edit().update(molecules, map);
            }
        }
        catch(...)
        {
            DoubleDistanceRestraint::operator=(old_state);
            throw;
        }
    }
}

/** Return the molecules used in this restraint */
Molecules DoubleDistanceRestraint::molecules() const
{
    Molecules mols;
    
    for (int i=0; i<this->nPoints(); ++i)
    {
        mols += p[i].read().molecules();
    }
    
    return mols;
}

/** Return whether or not this restraint affects the molecule
    with number 'molnum' */
bool DoubleDistanceRestraint::contains(MolNum molnum) const
{
    return p[0].read().contains(molnum) or p[1].read().contains(molnum) or
           p[2].read().contains(molnum) or p[2].read().contains(molnum);
}

/** Return whether or not this restraint affects the molecule
    with ID 'molid' */
bool DoubleDistanceRestraint::contains(const MolID &molid) const
{
    return p[0].read().contains(molid) or p[1].read().contains(molid) or
           p[2].read().contains(molid) or p[3].read().contains(molid);
}
    
/** Return whether or not this restraint involves any of the molecules
    that are in the forcetable 'forcetable' */
bool DoubleDistanceRestraint::usesMoleculesIn(const ForceTable &forcetable) const
{
    return p[0].read().usesMoleculesIn(forcetable) or 
           p[1].read().usesMoleculesIn(forcetable) or
           p[2].read().usesMoleculesIn(forcetable) or
           p[3].read().usesMoleculesIn(forcetable);
}

/** Return whether or not this restraint involves any of the molecules
    in 'molecules' */
bool DoubleDistanceRestraint::usesMoleculesIn(const Molecules &molecules) const
{
    return p[0].read().usesMoleculesIn(molecules) or
           p[1].read().usesMoleculesIn(molecules) or
           p[2].read().usesMoleculesIn(molecules) or
           p[3].read().usesMoleculesIn(molecules);
}

////////////
//////////// Implementation of TripleDistanceRestraint
////////////

static const RegisterMetaType<TripleDistanceRestraint> r_tripledistrest;

/** Serialise to a binary datastream */
QDataStream SIREMM_EXPORT &operator<<(QDataStream &ds, 
                                      const TripleDistanceRestraint &tripledistrest)
{
    writeHeader(ds, r_tripledistrest, 1);
    
    SharedDataStream sds(ds);
    
    sds << tripledistrest.p[0] << tripledistrest.p[1]
        << tripledistrest.p[2] << tripledistrest.p[3]
        << tripledistrest.p[4] << tripledistrest.p[5]
        << tripledistrest.nrg_expression
        << tripledistrest.force01_expression
        << tripledistrest.force23_expression
        << tripledistrest.force45_expression
        << static_cast<const Restraint3D&>(tripledistrest);
        
    return ds;
}

/** Extract from a binary datastream */
QDataStream SIREMM_EXPORT &operator>>(QDataStream &ds, 
                                      TripleDistanceRestraint &tripledistrest)
{
    VersionID v = readHeader(ds, r_tripledistrest);
    
    if (v == 1)
    {
        SharedDataStream sds(ds);
        
        sds >> tripledistrest.p[0] >> tripledistrest.p[1]
            >> tripledistrest.p[2] >> tripledistrest.p[3]
            >> tripledistrest.p[4] >> tripledistrest.p[5]
            >> tripledistrest.nrg_expression
            >> tripledistrest.force01_expression
            >> tripledistrest.force23_expression
            >> tripledistrest.force45_expression
            >> static_cast<Restraint3D&>(tripledistrest);

        tripledistrest.intra_molecule_points01 = Point::intraMoleculePoints(
                                                                    tripledistrest.p[0],
                                                                    tripledistrest.p[1]);
                                                                    
        tripledistrest.intra_molecule_points23 = Point::intraMoleculePoints(
                                                                    tripledistrest.p[2],
                                                                    tripledistrest.p[3]);
                                                                    
        tripledistrest.intra_molecule_points45 = Point::intraMoleculePoints(
                                                                    tripledistrest.p[4],
                                                                    tripledistrest.p[5]);
    }
    else
        throw version_error( v, "1", r_tripledistrest, CODELOC );
        
    return ds;
}

Q_GLOBAL_STATIC_WITH_ARGS( Symbol, getR45Symbol, ("r45") );

/** Return the symbol that represents the distance between the
    points 0 and 1 ("r01") */
const Symbol& TripleDistanceRestraint::r01()
{
    return *(getR01Symbol());
}

/** Return the symbol that represents the distance between the
    points 2 and 3 ("r23") */
const Symbol& TripleDistanceRestraint::r23()
{
    return *(getR23Symbol());
}

/** Return the symbol that represents the distance between the
    points 4 and 5 ("r45") */
const Symbol& TripleDistanceRestraint::r45()
{
    return *(getR45Symbol());
}

/** Return all of the symbols that can be used in this restraint function */
Symbols TripleDistanceRestraint::symbols()
{
    Symbols syms;
    syms.insert( r01() );
    syms.insert( r23() );
    syms.insert( r45() );
    
    return syms;
}

/** Constructor */
TripleDistanceRestraint::TripleDistanceRestraint()
                        : ConcreteProperty<TripleDistanceRestraint,Restraint3D>()
{}

/** Construct a restraint that acts on the three distances defined
    using the passed six points, using the expression 'restraint' */
TripleDistanceRestraint::TripleDistanceRestraint(const PointRef &point0, 
                                                 const PointRef &point1,
                                                 const PointRef &point2,
                                                 const PointRef &point3,
                                                 const PointRef &point4,
                                                 const PointRef &point5,
                                                 const Expression &restraint)
                  : ConcreteProperty<TripleDistanceRestraint,Restraint3D>(),
                    nrg_expression(restraint)
{
    p[0] = point0;
    p[1] = point1;
    p[2] = point2;
    p[3] = point3;
    p[4] = point4;
    p[5] = point5;

    if (nrg_expression.isConstant())
    {
        nrg_expression = nrg_expression.evaluate(Values());
        force01_expression = 0;
        force23_expression = 0;
        force45_expression = 0;
    }
    else
    {
        ::validateFunction(nrg_expression, symbols());
        force01_expression = nrg_expression.differentiate(r01());
        force23_expression = nrg_expression.differentiate(r23());
        force45_expression = nrg_expression.differentiate(r45());
        
        if (force01_expression.isConstant())
            force01_expression = force01_expression.evaluate(Values());
        
        if (force23_expression.isConstant())
            force23_expression = force23_expression.evaluate(Values());

        if (force45_expression.isConstant())
            force45_expression = force45_expression.evaluate(Values());
    }
    
    intra_molecule_points01 = Point::intraMoleculePoints(p[0], p[1]);
    intra_molecule_points23 = Point::intraMoleculePoints(p[2], p[3]);
    intra_molecule_points45 = Point::intraMoleculePoints(p[4], p[5]);
}

/** Copy constructor */
TripleDistanceRestraint::TripleDistanceRestraint(const TripleDistanceRestraint &other)
                        : ConcreteProperty<TripleDistanceRestraint,Restraint3D>(other),
                          nrg_expression(other.nrg_expression),
                          force01_expression(other.force01_expression),
                          force23_expression(other.force23_expression),
                          force45_expression(other.force45_expression),
                          intra_molecule_points01(other.intra_molecule_points01),
                          intra_molecule_points23(other.intra_molecule_points23),
                          intra_molecule_points45(other.intra_molecule_points45)
{
    for (int i=0; i<this->nPoints(); ++i)
    {
        p[i] = other.p[i];
    }
}

/** Destructor */
TripleDistanceRestraint::~TripleDistanceRestraint()
{}

/** Copy assignment operator */
TripleDistanceRestraint& TripleDistanceRestraint::operator=(
                                                const TripleDistanceRestraint &other)
{
    if (this != &other)
    {
        Restraint3D::operator=(other);

        for (int i=0; i<this->nPoints(); ++i)
        {
            p[i] = other.p[i];
        }
        
        nrg_expression = other.nrg_expression;
        force01_expression = other.force01_expression;
        force23_expression = other.force23_expression;
        force45_expression = other.force45_expression;
        intra_molecule_points01 = other.intra_molecule_points01;
        intra_molecule_points23 = other.intra_molecule_points23;
        intra_molecule_points45 = other.intra_molecule_points45;
    }
    
    return *this;
}

/** Comparison operator */
bool TripleDistanceRestraint::operator==(const TripleDistanceRestraint &other) const
{
    return this == &other or
           ( Restraint3D::operator==(other) and
             p[0] == other.p[0] and p[1] == other.p[1] and
             p[2] == other.p[2] and p[3] == other.p[3] and
             p[4] == other.p[4] and p[5] == other.p[5] and
             nrg_expression == other.nrg_expression and
             force01_expression == other.force01_expression and
             force23_expression == other.force23_expression and
             force45_expression == other.force45_expression);
}

/** Comparison operator */
bool TripleDistanceRestraint::operator!=(const TripleDistanceRestraint &other) const
{
    return not TripleDistanceRestraint::operator==(other);
}

const char* TripleDistanceRestraint::typeName()
{
    return QMetaType::typeName( qMetaTypeId<TripleDistanceRestraint>() );
}

/** This restraint involves six points */
int TripleDistanceRestraint::nPoints() const
{
    return 6;
}

/** Return the ith point */
const Point& TripleDistanceRestraint::point(int i) const
{
    i = Index(i).map( this->nPoints() );

    return p[i].read();
}

/** Return the first point */
const Point& TripleDistanceRestraint::point0() const
{
    return p[0].read();
}

/** Return the second point */
const Point& TripleDistanceRestraint::point1() const
{
    return p[1].read();
}

/** Return the third point */
const Point& TripleDistanceRestraint::point2() const
{
    return p[2].read();
}

/** Return the fourth point */
const Point& TripleDistanceRestraint::point3() const
{
    return p[3].read();
}

/** Return the fifth point */
const Point& TripleDistanceRestraint::point4() const
{
    return p[4].read();
}

/** Return the sixth point */
const Point& TripleDistanceRestraint::point5() const
{
    return p[5].read();
}

/** Set the space used to evaluate the energy of this restraint

    \throw SireVol::incompatible_space
*/
void TripleDistanceRestraint::setSpace(const Space &new_space)
{
    if (not this->space().equals(new_space))
    {
        TripleDistanceRestraint old_state(*this);
    
        try
        {
            for (int i=0; i<this->nPoints(); ++i)
            {
                p[i].edit().setSpace(new_space);
            }
            
            Restraint3D::setSpace(new_space);
        }
        catch(...)
        {
            TripleDistanceRestraint::operator=(old_state);
            throw;
        }
    }
}

/** Return the function used to calculate the restraint energy */
const Expression& TripleDistanceRestraint::restraintFunction() const
{
    return nrg_expression;
}

/** Return the function used to calculate the restraint force along the 
    distance r01 */
const Expression& TripleDistanceRestraint::differentialRestraintFunction01() const
{
    return force01_expression;
}

/** Return the function used to calculate the restraint force along the 
    distance r23 */
const Expression& TripleDistanceRestraint::differentialRestraintFunction23() const
{
    return force23_expression;
}

/** Return the function used to calculate the restraint force along the 
    distance r45 */
const Expression& TripleDistanceRestraint::differentialRestraintFunction45() const
{
    return force45_expression;
}

/** Return the energy of this restraint */
MolarEnergy TripleDistanceRestraint::energy() const
{
    if (nrg_expression.isConstant())
        return MolarEnergy( nrg_expression.evaluate(Values()) );
    else
    {
        double d01;
        double d23;
        double d45;
        
        if (intra_molecule_points01)
            //we don't use the space when calculating intra-molecular distances
            d01 = Vector::distance( p[0].read().point(), p[1].read().point() );
        else
            d01 = this->space().calcDist( p[0].read().point(),
                                          p[1].read().point() );
        
        if (intra_molecule_points23)
            d23 = Vector::distance( p[2].read().point(), p[3].read().point() );
        else
            d23 = this->space().calcDist( p[2].read().point(),
                                          p[3].read().point() );
        
        if (intra_molecule_points45)
            d45 = Vector::distance( p[5].read().point(), p[4].read().point() );
        else
            d45 = this->space().calcDist( p[4].read().point(),
                                          p[5].read().point() );
                                                                                                                                      
        return MolarEnergy( nrg_expression.evaluate( (r01() == d01) +
                                                     (r23() == d23) +
                                                     (r45() == d45) ) );
    }
}

/** Calculate the force acting on the molecule in the forcetable 'forcetable' 
    caused by this restraint, and add it on to the forcetable scaled by 
    'scale_force' */
void TripleDistanceRestraint::force(MolForceTable &forcetable, double scale_force) const
{
    bool in_p0 = p[0].read().contains(forcetable.molNum());
    bool in_p1 = p[1].read().contains(forcetable.molNum());
    bool in_p2 = p[2].read().contains(forcetable.molNum());
    bool in_p3 = p[3].read().contains(forcetable.molNum());
    bool in_p4 = p[4].read().contains(forcetable.molNum());
    bool in_p5 = p[5].read().contains(forcetable.molNum());
    
    if (not (in_p0 or in_p1 or in_p2 or in_p3 or in_p4 or in_p5))
        //this molecule is not affected by the restraint
        return;
        
    Vector delta01;
    Vector delta23;
    Vector delta45;
    
    if (intra_molecule_points01)
        delta01 = p[1].read().point() - p[0].read().point();
    else
    {
        const Vector &point0 = p[0].read().point();
        Vector point1 = p[1].read().point();
        
        point1 = this->space().getMinimumImage(point1, point0);
        
        delta01 = point1 - point0;
    }
      
    if (intra_molecule_points23)
        delta23 = p[3].read().point() - p[2].read().point();
    else
    {
        const Vector &point2 = p[2].read().point();
        Vector point3 = p[3].read().point();
        
        point3 = this->space().getMinimumImage(point3, point2);
        
        delta23 = point3 - point2;
    }
      
    if (intra_molecule_points45)
        delta45 = p[5].read().point() - p[4].read().point();
    else
    {
        const Vector &point4 = p[4].read().point();
        Vector point5 = p[5].read().point();
        
        point5 = this->space().getMinimumImage(point5, point4);
        
        delta45 = point5 - point4;
    }
      
    double d01 = delta01.length();
    double d23 = delta23.length();
    double d45 = delta45.length();

    Values vals = (r01() == d01) + (r23() == d23) + (r45() == d45);

    if (in_p0 or in_p1)
    {
        Vector f01 = (force01_expression.evaluate(vals) / d01) * delta01;
        
        if (not f01.isZero())
        {
            if (in_p0)
                p[0].read().addForce(forcetable, -scale_force*f01);
    
            if (in_p1)
                p[1].read().addForce(forcetable, scale_force*f01);
        }
    }

    if (in_p2 or in_p3)
    {
        Vector f23 = (force23_expression.evaluate(vals) / d23) * delta23;
    
        if (not f23.isZero())
        {
            if (in_p2)
                p[2].read().addForce(forcetable, -scale_force*f23);
    
            if (in_p3)
                p[3].read().addForce(forcetable, scale_force*f23);
        }
    }

    if (in_p4 or in_p5)
    {
        Vector f45 = (force45_expression.evaluate(vals) / d45) * delta45;
    
        if (not f45.isZero())
        {
            if (in_p4)
                p[4].read().addForce(forcetable, -scale_force*f45);
    
            if (in_p5)
                p[5].read().addForce(forcetable, scale_force*f45);
        }
    }
}

/** Calculate the force acting on the molecules in the forcetable 'forcetable' 
    caused by this restraint, and add it on to the forcetable scaled by 
    'scale_force' */
void TripleDistanceRestraint::force(ForceTable &forcetable, double scale_force) const
{
    bool in_p0 = p[0].read().usesMoleculesIn(forcetable);
    bool in_p1 = p[1].read().usesMoleculesIn(forcetable);
    bool in_p2 = p[2].read().usesMoleculesIn(forcetable);
    bool in_p3 = p[3].read().usesMoleculesIn(forcetable);
    bool in_p4 = p[4].read().usesMoleculesIn(forcetable);
    bool in_p5 = p[5].read().usesMoleculesIn(forcetable);
    
    if (not (in_p0 or in_p1 or in_p2 or in_p3 or in_p4 or in_p5))
        //this molecule is not affected by the restraint
        return;

    Vector delta01;
    Vector delta23;
    Vector delta45;
    
    if (intra_molecule_points01)
        delta01 = p[1].read().point() - p[0].read().point();
    else
    {
        const Vector &point0 = p[0].read().point();
        Vector point1 = p[1].read().point();
        
        point1 = this->space().getMinimumImage(point1, point0);
        
        delta01 = point1 - point0;
    }
    
    if (intra_molecule_points23)
        delta23 = p[3].read().point() - p[2].read().point();
    else
    {
        const Vector &point2 = p[2].read().point();
        Vector point3 = p[3].read().point();
        
        point3 = this->space().getMinimumImage(point3, point2);
        
        delta23 = point3 - point2;
    }
    
    if (intra_molecule_points45)
        delta45 = p[5].read().point() - p[4].read().point();
    else
    {
        const Vector &point4 = p[4].read().point();
        Vector point5 = p[5].read().point();
        
        point5 = this->space().getMinimumImage(point5, point4);
        
        delta45 = point5 - point4;
    }
      
    double d01 = delta01.length();
    double d23 = delta23.length();
    double d45 = delta45.length();

    Values vals = (r01() == d01) + (r23() == d23) + (r45() == d45);

    if (in_p0 or in_p1)
    {
        Vector f01 = (force01_expression.evaluate(vals) / d01) * delta01;
        
        if (not f01.isZero())
        {
            if (in_p0)
                p[0].read().addForce(forcetable, -scale_force*f01);
    
            if (in_p1)
                p[1].read().addForce(forcetable, scale_force*f01);
        }
    }

    if (in_p2 or in_p3)
    {
        Vector f23 = (force23_expression.evaluate(vals) / d23) * delta23;
    
        if (not f23.isZero())
        {
            if (in_p2)
                p[2].read().addForce(forcetable, -scale_force*f23);
    
            if (in_p3)
                p[3].read().addForce(forcetable, scale_force*f23);
        }
    }

    if (in_p4 or in_p5)
    {
        Vector f45 = (force45_expression.evaluate(vals) / d45) * delta45;
    
        if (not f45.isZero())
        {
            if (in_p4)
                p[4].read().addForce(forcetable, -scale_force*f45);
    
            if (in_p5)
                p[5].read().addForce(forcetable, scale_force*f45);
        }
    }
}

/** Update the points of this restraint using new molecule data from 'moldata'

    \throw SireBase::missing_property
    \throw SireError::invalid_cast
    \throw SireError::incompatible_error
*/
void TripleDistanceRestraint::update(const MoleculeData &moldata, const PropertyMap &map)
{
    if (this->contains(moldata.number()))
    {
        TripleDistanceRestraint old_state(*this);
        
        try
        {
            for (int i=0; i<this->nPoints(); ++i)
            {
                p[i].edit().update(moldata, map);
            }
        }
        catch(...)
        {
            TripleDistanceRestraint::operator=(old_state);
            throw;
        }
    }
}
            
/** Update the points of this restraint using new molecule data from 'molecules'

    \throw SireBase::missing_property
    \throw SireError::invalid_cast
    \throw SireError::incompatible_error
*/
void TripleDistanceRestraint::update(const Molecules &molecules, const PropertyMap &map)
{
    if (this->usesMoleculesIn(molecules))
    {
        TripleDistanceRestraint old_state(*this);
        
        try
        {
            for (int i=0; i<this->nPoints(); ++i)
            {
                p[i].edit().update(molecules, map);
            }
        }
        catch(...)
        {
            TripleDistanceRestraint::operator=(old_state);
            throw;
        }
    }
}

/** Return the molecules used in this restraint */
Molecules TripleDistanceRestraint::molecules() const
{
    Molecules mols;
    
    for (int i=0; i<this->nPoints(); ++i)
    {
        mols += p[i].read().molecules();
    }
    
    return mols;
}

/** Return whether or not this restraint affects the molecule
    with number 'molnum' */
bool TripleDistanceRestraint::contains(MolNum molnum) const
{
    return p[0].read().contains(molnum) or p[1].read().contains(molnum) or
           p[2].read().contains(molnum) or p[2].read().contains(molnum) or
           p[4].read().contains(molnum) or p[5].read().contains(molnum);
}

/** Return whether or not this restraint affects the molecule
    with ID 'molid' */
bool TripleDistanceRestraint::contains(const MolID &molid) const
{
    return p[0].read().contains(molid) or p[1].read().contains(molid) or
           p[2].read().contains(molid) or p[3].read().contains(molid) or
           p[4].read().contains(molid) or p[5].read().contains(molid);
}
    
/** Return whether or not this restraint involves any of the molecules
    that are in the forcetable 'forcetable' */
bool TripleDistanceRestraint::usesMoleculesIn(const ForceTable &forcetable) const
{
    return p[0].read().usesMoleculesIn(forcetable) or 
           p[1].read().usesMoleculesIn(forcetable) or
           p[2].read().usesMoleculesIn(forcetable) or
           p[3].read().usesMoleculesIn(forcetable) or
           p[4].read().usesMoleculesIn(forcetable) or
           p[5].read().usesMoleculesIn(forcetable);
}

/** Return whether or not this restraint involves any of the molecules
    in 'molecules' */
bool TripleDistanceRestraint::usesMoleculesIn(const Molecules &molecules) const
{
    return p[0].read().usesMoleculesIn(molecules) or
           p[1].read().usesMoleculesIn(molecules) or
           p[2].read().usesMoleculesIn(molecules) or
           p[3].read().usesMoleculesIn(molecules) or
           p[4].read().usesMoleculesIn(molecules) or
           p[5].read().usesMoleculesIn(molecules);
}
