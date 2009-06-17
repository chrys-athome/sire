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

#include "positionalrestraint.h"

#include "SireMol/cgatomidx.h"
#include "SireMol/atom.h"
#include "SireMol/moleculeview.h"
#include "SireMol/mover.hpp"
#include "SireMol/selector.hpp"

#include "SireCAS/values.h"
#include "SireCAS/symbol.h"
#include "SireCAS/conditional.h"

#include "SireFF/forcetable.h"

#include "SireStream/datastream.h"
#include "SireStream/shareddatastream.h"

#include "SireError/errors.h"
#include "SireCAS/errors.h"

using namespace SireMM;
using namespace SireFF;
using namespace SireMol;
using namespace SireCAS;
using namespace SireMaths;
using namespace SireBase;
using namespace SireUnits::Dimension;
using namespace SireStream;

//////////////
////////////// Implementation of PositionalRestraint
//////////////

static const RegisterMetaType<PositionalRestraint> r_posrestraint( MAGIC_ONLY,
                                                      PositionalRestraint::typeName() );

/** Serialise to a binary datastream */
QDataStream SIREMM_EXPORT &operator<<(QDataStream &ds,
                                      const PositionalRestraint &posrestraint)
{
    writeHeader(ds, r_posrestraint, 1);
    
    SharedDataStream sds(ds);
    
    sds << posrestraint.restraint_point << posrestraint.molecule_point
        << posrestraint.restraint_function 
        << posrestraint.diff_restraint_function
        << static_cast<const Restraint3D&>(posrestraint);
        
    return ds;
}

/** Extract from a binary datastream */
QDataStream SIREMM_EXPORT &operator>>(QDataStream &ds,
                                      PositionalRestraint &posrestraint)
{
    VersionID v = readHeader(ds, r_posrestraint);
    
    if (v == 1)
    {
        SharedDataStream sds(ds);
        
        sds >> posrestraint.restraint_point >> posrestraint.molecule_point
            >> posrestraint.restraint_function
            >> posrestraint.diff_restraint_function
            >> static_cast<Restraint3D&>(posrestraint);
    }
    else
        throw version_error( v, "1", r_posrestraint, CODELOC );
        
    return ds;
}

Q_GLOBAL_STATIC_WITH_ARGS( Symbol, getRSymbol, ("r") );

/** Return the symbol used to represent the distance between
    the restraint point and the molecule point */
const Symbol& PositionalRestraint::r()
{
    return *(getRSymbol());
}

/** Constructor */
PositionalRestraint::PositionalRestraint() : Restraint3D()
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

/** Construct a restraint from the point 'restraint_point' using
    the function 'restraint_expression' to calculate the energy
    and force with respect to the distance between the molecule
    and this point. The restraint expression must be a function
    of "r" only (as the distance is represented by the symbol "r")
    
    \throw SireCAS::missing_symbol
    \throw SireCAS::unavailable_differential
    \throw SireError::invalid_arg
*/
PositionalRestraint::PositionalRestraint(const Vector &point,
                                         const Expression &func)
                    : Restraint3D(),
                      restraint_point(point),
                      restraint_function(func)
{
    if (func.isConstant())
    {
        restraint_function = func.evaluate(Values());
        diff_restraint_function = 0;
    }
    else
    {
        ::validateFunction(restraint_function, r());
        diff_restraint_function = restraint_function.differentiate(r());
        
        if (diff_restraint_function.isConstant())
            diff_restraint_function = diff_restraint_function.evaluate(Values());
    }
}

/** Construct a restraint from the point 'restraint_point' using
    the function 'restraint_expression' to calculate the energy
    'differential_restraint_expression' to calculate the force 
    with respect to the distance between the molecule
    and this point. Both expressions must be a function
    of "r" only (as the distance is represented by the symbol "r")
    
    (and ideally, 'differential_restraint_expression' *must* be
     the differential of 'restraint_expression', or else 
     dynamics will move away from the true energy surface!)
    
    \throw SireCAS::missing_symbol
    \throw SireCAS::unavailable_differential
    \throw SireError::invalid_arg
*/
PositionalRestraint::PositionalRestraint(const Vector &point,
                                         const Expression &func,
                                         const Expression &diff_func)
                    : Restraint3D(),
                      restraint_point(point),
                      restraint_function(func),
                      diff_restraint_function(diff_func)
{
    if (func.isConstant())
    {
        restraint_function = func.evaluate(Values());
    }
    else
    {
        ::validateFunction(restraint_function, r());
    }
    
    if (diff_func.isConstant())
    {
        diff_restraint_function = diff_func.evaluate(Values());
    }
    else
    {
        ::validateFunction(diff_restraint_function, r());
    }
}

/** Copy constructor */
PositionalRestraint::PositionalRestraint(const PositionalRestraint &other)
                    : Restraint3D(other),
                      restraint_point(other.restraint_point),
                      molecule_point(other.molecule_point),
                      restraint_function(other.restraint_function),
                      diff_restraint_function(other.diff_restraint_function)
{}

/** Destructor */
PositionalRestraint::~PositionalRestraint()
{}

/** Copy assignment operator */
PositionalRestraint& PositionalRestraint::operator=(const PositionalRestraint &other)
{
    if (this != &other)
    {
        restraint_point = other.restraint_point;
        molecule_point = other.molecule_point;
        restraint_function = other.restraint_function;
        diff_restraint_function = other.diff_restraint_function;
        Restraint3D::operator=(other);
    }
    
    return *this;
}

/** Comparison operator */
bool PositionalRestraint::operator==(const PositionalRestraint &other) const
{
    return this == &other or
           (restraint_point == other.restraint_point and
            molecule_point == other.molecule_point and
            restraint_function == other.restraint_function and
            diff_restraint_function == other.diff_restraint_function and
            Restraint3D::operator==(other) );
}

/** Comparison operator */
bool PositionalRestraint::operator!=(const PositionalRestraint &other) const
{
    return not PositionalRestraint::operator==(other);
}

/** Return the function used to calculate the energy of the restraint */
const Expression& PositionalRestraint::restraintFunction() const
{
    return restraint_function;
}

/** Return the differential of the restraint function, used to 
    calculate the force caused by the restraint */
const Expression& PositionalRestraint::differentialRestraintFunction() const
{
    return diff_restraint_function;
}

/** Return the location of the restraint */
const Vector& PositionalRestraint::restraintPoint() const
{
    return restraint_point;
}

/** Return the location of the current point in the molecule that is
    being restrained */
const Vector& PositionalRestraint::moleculePoint() const
{
    return molecule_point;
}

/** Internal function called to update the location of the
    point in the molecule being restrained */
void PositionalRestraint::update(const Vector &new_molecule_point)
{
    molecule_point = new_molecule_point;
}

/** Return the current energy of this restraint */
MolarEnergy PositionalRestraint::energy() const
{
    return MolarEnergy( restraint_function.evaluate(
                                r() == Vector::distance(restraint_point,
                                                        molecule_point) ) );
}

/** Calculate the force acting along the vector from the molecule point
    to the restraint point */
Vector PositionalRestraint::restraintForce() const
{
    Vector delta = restraint_point - molecule_point;
    double distance = delta.length();
    
    return (diff_restraint_function.evaluate( r() == distance ) / distance)
                     * delta;
}

//////////////
////////////// Implementation of AtomicRestraint
//////////////

static const RegisterMetaType<AtomicRestraint> r_atomrestraint;

/** Serialise to a binary datastream */
QDataStream SIREMM_EXPORT &operator<<(QDataStream &ds, 
                                      const AtomicRestraint &atomrestraint)
{
    writeHeader(ds, r_atomrestraint, 1);
    
    ds << atomrestraint.cgatomidx
       << static_cast<const PositionalRestraint&>(atomrestraint);
       
    return ds;
}

/** Extract from a binary datastream */
QDataStream SIREMM_EXPORT &operator>>(QDataStream &ds,
                                      AtomicRestraint &atomrestraint)
{
    VersionID v = readHeader(ds, r_atomrestraint);
    
    if (v == 1)
    {
        ds >> atomrestraint.cgatomidx
           >> static_cast<PositionalRestraint&>(atomrestraint);
    }
    else
        throw version_error( v, "1", r_atomrestraint, CODELOC );
        
    return ds;
}

/** Constructor */
AtomicRestraint::AtomicRestraint()
                : ConcreteProperty<AtomicRestraint,PositionalRestraint>()
{}

/** Construct the restraint that restrains the atom 'atom' to the 
    point 'restraint_point' using the expression 'restraint_expression'
    to calculate the energy and forces, and using the (optionally) supplied
    property map to locate all of the necessary properties
    
    \throw SireBase::missing_property
    \throw SireError::invalid_cast
*/
AtomicRestraint::AtomicRestraint(const Atom &atom, const Vector &restraint_point,
                                 const Expression &restraint_expression,
                                 const PropertyMap &map)
      : ConcreteProperty<AtomicRestraint,PositionalRestraint>(restraint_point,
                                                              restraint_expression),
        cgatomidx( atom.cgAtomIdx() )
{
    AtomicRestraint::update(atom.data(), map);
}

/** Construct the restraint that restrains the atom 'atom' to the 
    point 'restraint_point' using the expression 'restraint_expression'
    to calculate the energy and 'diff_restraint_expression' to calculate
    the forces, and using the (optionally) supplied
    property map to locate all of the necessary properties
    
    \throw SireBase::missing_property
    \throw SireError::invalid_cast
*/
AtomicRestraint::AtomicRestraint(const Atom &atom, const Vector &restraint_point,
                                 const Expression &restraint_expression,
                                 const Expression &diff_restraint_expression,
                                 const PropertyMap &map)
      : ConcreteProperty<AtomicRestraint,PositionalRestraint>(restraint_point,
                                                              restraint_expression,
                                                              diff_restraint_expression),
         cgatomidx( atom.cgAtomIdx() )
{
    AtomicRestraint::update(atom.data(), map);
}
                
/** Copy constructor */
AtomicRestraint::AtomicRestraint(const AtomicRestraint &other)
                : ConcreteProperty<AtomicRestraint,PositionalRestraint>(other),
                  cgatomidx(other.cgatomidx)
{}

/** Destructor */
AtomicRestraint::~AtomicRestraint()
{}

const char* AtomicRestraint::typeName()
{
    return QMetaType::typeName( qMetaTypeId<AtomicRestraint>() );
}

/** Copy assignment operator */
AtomicRestraint& AtomicRestraint::operator=(const AtomicRestraint &other)
{
    cgatomidx = other.cgatomidx;
    PositionalRestraint::operator=(other);
    
    return *this;
}

/** Comparison operator */
bool AtomicRestraint::operator==(const AtomicRestraint &other) const
{
    return cgatomidx == other.cgatomidx and
           PositionalRestraint::operator==(other);
}

/** Comparison operator */
bool AtomicRestraint::operator!=(const AtomicRestraint &other) const
{
    return cgatomidx != other.cgatomidx or
           PositionalRestraint::operator!=(other);
}

static Expression harmonicFunction(double force_constant)
{
    if (SireMaths::isZero(force_constant))
        return 0;
    else
        return force_constant * pow(PositionalRestraint::r(), 2);
}

static Expression diffHarmonicFunction(double force_constant)
{
    if (SireMaths::isZero(force_constant))
        return 0;
    else
        return (2*force_constant) * PositionalRestraint::r();
}

/** Return a restraint that uses a harmonic potential to restrain
    the atom 'atom' to the point 'restraint_point' using a 
    force constant 'force_constant', and using the optionally
    supplied property map to find the necessary properties
    
    \throw SireBase::missing_property
    \throw SireError::invalid_cast
*/
AtomicRestraint AtomicRestraint::harmonic(
                                   const Atom &atom, 
                                   const Vector &restraint_point,
                                   const HarmonicPositionForceConstant &force_constant,
                                   const PropertyMap &map)
{
    return AtomicRestraint( atom, restraint_point,
                            ::harmonicFunction(force_constant),
                            ::diffHarmonicFunction(force_constant),
                            map );
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
        const Symbol &r = PositionalRestraint::r();
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
        const Symbol &r = PositionalRestraint::r();
        return Conditional( GreaterThan(r, distance), 
                                (2*force_constant) * (r-distance), 0 );
    }
}

/** Return a restraint that uses a half-harmonic potential to restrain
    the atom 'atom' to within the distance 'distance' of the point 
    'restraint_point' using a force constant 'force_constant', and using 
    the optionally supplied property map to find the necessary properties
    
    \throw SireBase::missing_property
    \throw SireError::invalid_cast
*/
AtomicRestraint AtomicRestraint::halfHarmonic(
                                   const Atom &atom,
                                   const Vector &restraint_point,
                                   const Length &distance,
                                   const HarmonicPositionForceConstant &force_constant,
                                   const PropertyMap &map)
{
    return AtomicRestraint( atom, restraint_point,
                            ::halfHarmonicFunction(force_constant, distance),
                            ::diffHalfHarmonicFunction(force_constant, distance),
                            map );
}

/** Update the retraint with the new version of the molecule in 'moldata' */
void AtomicRestraint::update(const MoleculeData &moldata, const PropertyMap &map)
{
    const PropertyName &coords_property = map["coordinates"];
    
    const AtomCoords &coords = moldata.property(coords_property).asA<AtomCoords>();
    
    PositionalRestraint::update( coords[cgatomidx] );
}

/** Calculate the force acting on the atom caused by this restraint and 
    add the result (scaled by 'scale_force') into the passed force table */
void AtomicRestraint::force(MolForceTable &forcetable, double scale_force) const
{
    if ( forcetable.selected(cgatomidx.cutGroup()) )
    {
        //calculate the force acting along the vector from the restraint
        //point to the molecule point
        Vector f = PositionalRestraint::restraintForce();
        
        //add this onto the passed forcetable
        forcetable.add( cgatomidx, scale_force*f );
    }
}

//////////////
////////////// Implementation of CenterOfGeometryRestraint
//////////////

static const RegisterMetaType<CenterOfGeometryRestraint> r_cogrestraint;

/** Serialise to a binary datastream */
QDataStream SIREMM_EXPORT &operator<<(QDataStream &ds,
                                      const CenterOfGeometryRestraint &cogrestraint)
{
    writeHeader(ds, r_cogrestraint, 1);
    
    SharedDataStream sds(ds);
    
    sds << cogrestraint.selected_atoms
        << static_cast<const PositionalRestraint&>(cogrestraint);
        
    return ds;
}

/** Extract from a binary datastream */
QDataStream SIREMM_EXPORT &operator>>(QDataStream &ds,
                                      CenterOfGeometryRestraint &cogrestraint)
{
    VersionID v = readHeader(ds, r_cogrestraint);
    
    if (v == 1)
    {
        SharedDataStream sds(ds);
        
        sds >> cogrestraint.selected_atoms
            >> static_cast<PositionalRestraint&>(cogrestraint);
    }
    else
        throw version_error( v, "1", r_cogrestraint, CODELOC );
        
    return ds;
}

/** Constructor */
CenterOfGeometryRestraint::CenterOfGeometryRestraint()
            : ConcreteProperty<CenterOfGeometryRestraint,PositionalRestraint>()
{}

/** Construct to restrain the center of geometry of the atoms in 'molview'
    to the point 'restraint_point' using the expression 'restraint' for the
    energy and force of the restraint, using the optionally supplied 
    property map 'map' to find the necessary properties 
    
    \throw SireBase::missing_property
    \throw SireError::invalid_cast
*/
CenterOfGeometryRestraint::CenterOfGeometryRestraint(const MoleculeView &molview,
                                                     const Vector &restraint_point,
                                                     const Expression &restraint,
                                                     const PropertyMap &map)
  : ConcreteProperty<CenterOfGeometryRestraint,PositionalRestraint>(restraint_point,
                                                                    restraint),
    selected_atoms(molview.selection())
{
    CenterOfGeometryRestraint::update(molview.data(), map);
}

/** Internal function used to specify the restraint energy and force
    functions separately 
    
    \throw SireBase::missing_property
    \throw SireError::invalid_cast
*/
CenterOfGeometryRestraint::CenterOfGeometryRestraint(const MoleculeView &molview,
                                                     const Vector &restraint_point,
                                                     const Expression &restraint,
                                                     const Expression &diff_restraint,
                                                     const PropertyMap &map)
  : ConcreteProperty<CenterOfGeometryRestraint,PositionalRestraint>(restraint_point,
                                                                    restraint,
                                                                    diff_restraint),
    selected_atoms(molview.selection())
{
    CenterOfGeometryRestraint::update(molview.data(), map);
}

/** Copy constructor */                          
CenterOfGeometryRestraint::CenterOfGeometryRestraint(
                                        const CenterOfGeometryRestraint &other)
        : ConcreteProperty<CenterOfGeometryRestraint,PositionalRestraint>(other),
          selected_atoms(other.selected_atoms)
{}

/** Destructor */
CenterOfGeometryRestraint::~CenterOfGeometryRestraint()
{}

const char* CenterOfGeometryRestraint::typeName()
{
    return QMetaType::typeName( qMetaTypeId<CenterOfGeometryRestraint>() );
}

/** Copy assignment operator */
CenterOfGeometryRestraint& CenterOfGeometryRestraint::operator=(
                                        const CenterOfGeometryRestraint &other)
{
    PositionalRestraint::operator=(other);
    selected_atoms = other.selected_atoms;
    
    return *this;
}

/** Comparison operator */
bool CenterOfGeometryRestraint::operator==(const CenterOfGeometryRestraint &other) const
{
    return selected_atoms == other.selected_atoms and
           PositionalRestraint::operator==(other);
}

/** Comparison operator */
bool CenterOfGeometryRestraint::operator!=(const CenterOfGeometryRestraint &other) const
{
    return selected_atoms != other.selected_atoms or
           PositionalRestraint::operator!=(other);
}

/** Return a restraint that restrains the center of geometry of 'molview' 
    to the point 'restraint_point' using a harmonic potential with a force
    constant of 'force_constant' */
CenterOfGeometryRestraint CenterOfGeometryRestraint::harmonic(
                                 const MoleculeView &molview,
                                 const Vector &restraint_point,
                                 const HarmonicPositionForceConstant &force_constant,
                                 const PropertyMap &map)
{
    return CenterOfGeometryRestraint(molview, restraint_point,
                                     ::harmonicFunction(force_constant),
                                     ::diffHarmonicFunction(force_constant),
                                     map);
}

/** Return a restraint that restrains the center of geometry of 'molview' 
    to the point 'restraint_point' using a half-harmonic potential
    that starts beyond the distance 'distance' using the force constant
    'force_constant' */
CenterOfGeometryRestraint CenterOfGeometryRestraint::halfHarmonic(
                                const MoleculeView &molview,
                                const Vector &restraint_point,
                                const Length &distance,
                                const HarmonicPositionForceConstant &force_constant,
                                const PropertyMap &map)
{
    return CenterOfGeometryRestraint(molview, restraint_point, 
                                     ::halfHarmonicFunction(force_constant,distance),
                                     ::diffHalfHarmonicFunction(force_constant,distance),
                                     map);
}

/** Update this restraint */
void CenterOfGeometryRestraint::update(const MoleculeData &moldata, 
                                       const PropertyMap &map)
{
    Evaluator evaluator(moldata, selected_atoms);
    PositionalRestraint::update( evaluator.centerOfGeometry(map) );
}

/** Calculate the force acting on all of the atoms caused by this restraint */
void CenterOfGeometryRestraint::force(MolForceTable &forcetable,
                                      double scale_force) const
{
    //calculate the force acting along the vector from the restraint
    //point to the molecule point
    Vector f = PositionalRestraint::restraintForce();
       
    //add this onto the passed forcetable
    forcetable.add( selected_atoms, scale_force*f );
}

//////////////
////////////// Implementation of CenterOfMassRestraint
//////////////

static const RegisterMetaType<CenterOfMassRestraint> r_comrestraint;

/** Serialise to a binary datastream */
QDataStream SIREMM_EXPORT &operator<<(QDataStream &ds,
                                      const CenterOfMassRestraint &comrestraint)
{
    writeHeader(ds, r_comrestraint, 1);
    
    SharedDataStream sds(ds);
    
    sds << comrestraint.selected_atoms
        << static_cast<const PositionalRestraint&>(comrestraint);
        
    return ds;
}

/** Extract from a binary datastream */
QDataStream SIREMM_EXPORT &operator>>(QDataStream &ds,
                                      CenterOfMassRestraint &comrestraint)
{
    VersionID v = readHeader(ds, r_comrestraint);
    
    if (v == 1)
    {
        SharedDataStream sds(ds);
        
        sds >> comrestraint.selected_atoms
            >> static_cast<PositionalRestraint&>(comrestraint);
    }
    else
        throw version_error( v, "1", r_comrestraint, CODELOC );
        
    return ds;
}

/** Constructor */
CenterOfMassRestraint::CenterOfMassRestraint()
            : ConcreteProperty<CenterOfMassRestraint,PositionalRestraint>()
{}

/** Construct to restrain the center of mass of the atoms in 'molview'
    to the point 'restraint_point' using the expression 'restraint' for the
    energy and force of the restraint, using the optionally supplied 
    property map 'map' to find the necessary properties 
    
    \throw SireBase::missing_property
    \throw SireError::invalid_cast
*/
CenterOfMassRestraint::CenterOfMassRestraint(const MoleculeView &molview,
                                             const Vector &restraint_point,
                                             const Expression &restraint,
                                             const PropertyMap &map)
  : ConcreteProperty<CenterOfMassRestraint,PositionalRestraint>(restraint_point,
                                                                restraint),
    selected_atoms(molview.selection())
{
    CenterOfMassRestraint::update(molview.data(), map);
}

/** Internal function used to specify the restraint energy and force
    functions separately 
    
    \throw SireBase::missing_property
    \throw SireError::invalid_cast
*/
CenterOfMassRestraint::CenterOfMassRestraint(const MoleculeView &molview,
                                             const Vector &restraint_point,
                                             const Expression &restraint,
                                             const Expression &diff_restraint,
                                             const PropertyMap &map)
  : ConcreteProperty<CenterOfMassRestraint,PositionalRestraint>(restraint_point,
                                                                restraint,
                                                                diff_restraint),
    selected_atoms(molview.selection())
{
    CenterOfMassRestraint::update(molview.data(), map);
}

/** Copy constructor */                          
CenterOfMassRestraint::CenterOfMassRestraint(
                                        const CenterOfMassRestraint &other)
        : ConcreteProperty<CenterOfMassRestraint,PositionalRestraint>(other),
          selected_atoms(other.selected_atoms)
{}

/** Destructor */
CenterOfMassRestraint::~CenterOfMassRestraint()
{}

const char* CenterOfMassRestraint::typeName()
{
    return QMetaType::typeName( qMetaTypeId<CenterOfMassRestraint>() );
}

/** Copy assignment operator */
CenterOfMassRestraint& CenterOfMassRestraint::operator=(
                                        const CenterOfMassRestraint &other)
{
    PositionalRestraint::operator=(other);
    selected_atoms = other.selected_atoms;
    
    return *this;
}

/** Comparison operator */
bool CenterOfMassRestraint::operator==(const CenterOfMassRestraint &other) const
{
    return selected_atoms == other.selected_atoms and
           PositionalRestraint::operator==(other);
}

/** Comparison operator */
bool CenterOfMassRestraint::operator!=(const CenterOfMassRestraint &other) const
{
    return selected_atoms != other.selected_atoms or
           PositionalRestraint::operator!=(other);
}

/** Return a restraint that restrains the center of mass of 'molview' 
    to the point 'restraint_point' using a harmonic potential with a force
    constant of 'force_constant' */
CenterOfMassRestraint CenterOfMassRestraint::harmonic(
                                 const MoleculeView &molview,
                                 const Vector &restraint_point,
                                 const HarmonicPositionForceConstant &force_constant,
                                 const PropertyMap &map)
{
    return CenterOfMassRestraint(molview, restraint_point,
                                 ::harmonicFunction(force_constant),
                                 ::diffHarmonicFunction(force_constant),
                                 map);
}

/** Return a restraint that restrains the center of mass of 'molview' 
    to the point 'restraint_point' using a half-harmonic potential
    that starts beyond the distance 'distance' using the force constant
    'force_constant' */
CenterOfMassRestraint CenterOfMassRestraint::halfHarmonic(
                                const MoleculeView &molview,
                                const Vector &restraint_point,
                                const Length &distance,
                                const HarmonicPositionForceConstant &force_constant,
                                const PropertyMap &map)
{
    return CenterOfMassRestraint(molview, restraint_point, 
                                 ::halfHarmonicFunction(force_constant,distance),
                                 ::diffHalfHarmonicFunction(force_constant,distance),
                                 map);
}

/** Update this restraint */
void CenterOfMassRestraint::update(const MoleculeData &moldata, 
                                   const PropertyMap &map)
{
    Evaluator evaluator(moldata, selected_atoms);
    PositionalRestraint::update( evaluator.centerOfMass(map) );
}

/** Calculate the force acting on all of the atoms caused by this restraint */
void CenterOfMassRestraint::force(MolForceTable &forcetable,
                                  double scale_force) const
{
    //calculate the force acting along the vector from the restraint
    //point to the molecule point
    Vector f = PositionalRestraint::restraintForce();
       
    //add this onto the passed forcetable
    forcetable.add( selected_atoms, scale_force*f );
}
