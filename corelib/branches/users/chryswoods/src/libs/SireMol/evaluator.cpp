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

#include "evaluator.h"
#include "atomcoords.h"
#include "atommasses.h"
#include "atomcharges.h"
#include "atomelements.h"
#include "atommatcher.h"

#include "SireVol/coordgroup.h"

#include "SireMaths/sphere.h"
#include "SireMaths/axisset.h"

#include "SireBase/errors.h"

#include "SireUnits/dimensions.h"

#include "SireStream/datastream.h"

#include <QDebug>

using namespace SireMol;
using namespace SireMaths;
using namespace SireVol;
using namespace SireUnits::Dimension;
using namespace SireStream;

static const RegisterMetaType<Evaluator> r_eval;

/** Serialise to a binary datastream */
QDataStream SIREMOL_EXPORT &operator<<(QDataStream &ds,
                                       const Evaluator &eval)
{
    writeHeader(ds, r_eval, 1);
    
    SharedDataStream sds(ds);
    
    sds << eval.selected_atoms << static_cast<const MoleculeView&>(eval);

    return ds;
}

/** Deserialise from a binary datastream */
QDataStream SIREMOL_EXPORT &operator>>(QDataStream &ds, Evaluator &eval)
{
    VersionID v = readHeader(ds, r_eval);

    if (v == 1)
    {
        SharedDataStream sds(ds);
        
        sds >> eval.selected_atoms >> static_cast<MoleculeView&>(eval);
    }
    else
        throw version_error(v, "1", r_eval, CODELOC);

    return ds;
}

/** Null constructor */
Evaluator::Evaluator() : ConcreteProperty<Evaluator,MoleculeView>()
{}

/** Construct from the passed molecule view */
Evaluator::Evaluator(const MoleculeView &molecule)
          : ConcreteProperty<Evaluator,MoleculeView>(molecule), 
            selected_atoms(molecule.selection())
{}

/** Construct to evaluate for the entire molecule in 'moldata' */
Evaluator::Evaluator(const MoleculeData &moldata)
          : ConcreteProperty<Evaluator,MoleculeView>(moldata),
            selected_atoms(moldata)
{}

/** Construct to evaluate properties of the passed selected atoms
    of the molecule viewed in 'molecule' */
Evaluator::Evaluator(const MoleculeView &molecule,
                     const AtomSelection &atoms)
          : ConcreteProperty<Evaluator,MoleculeView>(molecule), selected_atoms(atoms)
{
    selected_atoms.assertCompatibleWith(this->data());
}

/** Construct to evaluate properties of the selected atoms of the 
    passed molecule */
Evaluator::Evaluator(const MoleculeData &moldata,
                     const AtomSelection &atoms)
          : ConcreteProperty<Evaluator,MoleculeView>(moldata), selected_atoms(atoms)
{
    selected_atoms.assertCompatibleWith(this->data());
}

/** Copy constructor */
Evaluator::Evaluator(const Evaluator &other)
          : ConcreteProperty<Evaluator,MoleculeView>(other), 
            selected_atoms(other.selected_atoms)
{}

/** Destructor */
Evaluator::~Evaluator()
{}

/** Copy assignment from another evaluator */
Evaluator& Evaluator::operator=(const Evaluator &other)
{
    if (this != &other)
    {
        MoleculeView::operator=(other);
        selected_atoms = other.selected_atoms;
    }
    
    return *this;
}

/** Copy assignment from another molecule */
Evaluator& Evaluator::operator=(const MoleculeView &other)
{
    MoleculeView::operator=(other);
    selected_atoms = other.selection();
    
    return *this;
}

/** Return a string representation of this evaluator */
QString Evaluator::toString() const
{
    return QObject::tr( "Evaluator( nAtoms() == %1 )" )
            .arg( selected_atoms.nSelected() );
}

/** Return whether or not this is empty */
bool Evaluator::isEmpty() const
{
    return selected_atoms.selectedNone();
}

/** Return whether or not this contains the whole molecule */
bool Evaluator::selectedAll() const
{
    return selected_atoms.selectedAll();
}

/** Return the selected atoms over which the properties
    will be evaluated */
AtomSelection Evaluator::selection() const
{
    return selected_atoms;
}

static void getMinMax(const CoordGroup &cgroup, Vector &min, Vector &max)
{
    //we can cheat by using the CoordGroup's aabox!
    min.setMin(cgroup.aaBox().minCoords());
    max.setMax(cgroup.aaBox().maxCoords());
}

static void getMinMax(const CoordGroup &cgroup, const QSet<Index> &idxs,
                      Vector &min, Vector &max)
{
    const Vector *cgroup_array = cgroup.constData();
            
    foreach (Index i, idxs)
    {
        const Vector &coords = cgroup_array[i];
    
        min.setMin(coords);
        max.setMax(coords);
    }
}

/** Return the axis-aligned box that just contains all of the 
    atoms in this view 
    
    \throw SireBase::missing_property
    \throw SireError::invalid_cast
*/
AABox Evaluator::aaBox(const PropertyMap &map) const
{
    if (selected_atoms.selectedNone())
        return AABox();

    //get the coordinates of the atoms
    const Property &prop = d->property(map["coordinates"]);
    const AtomCoords &coords = prop.asA<AtomCoords>();
    
    const CoordGroup *coords_array = coords.constData();
    int ncg = coords.count();
    
    //now get the minimum and maximum coordinates...
    Vector mincoords( std::numeric_limits<double>::max() );
    Vector maxcoords( -std::numeric_limits<double>::max() );
    
    if (selected_atoms.selectedAll())
    {
        for (int i=0; i<ncg; ++i)
        {
            getMinMax(coords_array[i], mincoords, maxcoords);
        }
    }
    else if (selected_atoms.selectedAllCutGroups())
    {
        for (CGIdx i(0); i<ncg; ++i)
        {
            if (selected_atoms.selectedAll(i))
            {
                getMinMax(coords_array[i], mincoords, maxcoords);
            }
            else
            {
                getMinMax(coords_array[i], selected_atoms.selectedAtoms(i),
                          mincoords, maxcoords);
            }
        }
    }
    else
    {
        foreach (CGIdx cgidx, selected_atoms.selectedCutGroups())
        {
            if (selected_atoms.selectedAll(cgidx))
            {
                getMinMax(coords_array[cgidx], mincoords, maxcoords);
            }
            else
            {
                getMinMax(coords_array[cgidx], selected_atoms.selectedAtoms(cgidx),
                          mincoords, maxcoords);
            }
        }
    }
    
    return AABox::from(mincoords, maxcoords);
}

/** Return the center of the selected atoms,
    using the passed property map to find the coordinates
    property of the molecule (the center is the point
    that is exactly in the middle of the atoms - i.e.
    halfway between the maximum and minimum coordinates

    \throw SireBase::missing_property
    \throw SireError::invalid_cast
*/
Vector Evaluator::center(const PropertyMap &map) const
{
    return this->aaBox(map).center();
}

/** Return the sphere that just encloses all of the atoms in this view 

    \throw SireBase::missing_property
    \throw SireError::invalid_cast
*/
Sphere Evaluator::boundingSphere(const PropertyMap &map) const
{
    return this->aaBox(map).boundingSphere();
}

double calc_mass(const PackedArray2D<SireUnits::Dimension::MolarMass>::Array &masses)
{
    const SireUnits::Dimension::MolarMass *masses_array = masses.constData();
    int nmasses = masses.count();
    
    double m = 0;
    
    for (int i=0; i<nmasses; ++i)
    {
        m += masses_array[i];
    }
    
    return m;
}

static double 
calc_mass(const PackedArray2D<SireUnits::Dimension::MolarMass>::Array &masses,
          const QSet<Index> &idxs)
{
    const SireUnits::Dimension::MolarMass *masses_array = masses.constData();
    
    double m = 0;
    
    foreach (Index i, idxs)
    {
        m += masses_array[i];
    }
    
    return m;
}

static double get_mass(const AtomMasses &masses,
                       const AtomSelection &selected_atoms)
{
    double m = 0;
    
    const PackedArray2D<SireUnits::Dimension::MolarMass>::Array *masses_array 
                                            = masses.constData();
    int ncg = masses.count();

    if (selected_atoms.selectedAll())
    {
        for (int i=0; i<ncg; ++i)
        {
            m += calc_mass( masses_array[i] );
        }
    }
    else if (selected_atoms.selectedAllCutGroups())
    {
        for (CGIdx i(0); i<ncg; ++i)
        {
            if (selected_atoms.selectedAll(i))
                m += calc_mass( masses_array[i] );
            else
                m += calc_mass( masses_array[i], selected_atoms.selectedAtoms(i) );
        }
    }
    else
    {
        foreach (CGIdx i, selected_atoms.selectedCutGroups())
        {
            if (selected_atoms.selectedAll(i))
                m += calc_mass( masses_array[i] );
            else
                m += calc_mass( masses_array[i], selected_atoms.selectedAtoms(i) );
        }
    }
    
    return m;
}

static double calc_mass(const PackedArray2D<Element>::Array &elements)
{
    const Element *elements_array = elements.constData();
    int nelements = elements.count();
    
    double m = 0;
    
    for (int i=0; i<nelements; ++i)
    {
        m += elements_array[i].mass();
    }
    
    return m;
}

static double calc_mass(const PackedArray2D<Element>::Array &elements, 
                        const QSet<Index> &idxs)
{
    const Element *elements_array = elements.constData();
    
    double m = 0;
    
    foreach (Index i, idxs)
    {
        m += elements_array[i].mass();
    }
    
    return m;
}

static double get_mass(const AtomElements &elements,
                       const AtomSelection &selected_atoms)
{
    double m = 0;
    
    const PackedArray2D<Element>::Array *elements_array = elements.constData();
    int ncg = elements.count();

    if (selected_atoms.selectedAll())
    {
        for (int i=0; i<ncg; ++i)
        {
            m += calc_mass( elements_array[i] );
        }
    }
    else if (selected_atoms.selectedAllCutGroups())
    {
        for (CGIdx i(0); i<ncg; ++i)
        {
            if (selected_atoms.selectedAll(i))
                m += calc_mass( elements_array[i] );
            else
                m += calc_mass( elements_array[i], selected_atoms.selectedAtoms(i) );
        }
    }
    else
    {
        foreach (CGIdx i, selected_atoms.selectedCutGroups())
        {
            if (selected_atoms.selectedAll(i))
                m += calc_mass( elements_array[i] );
            else
                m += calc_mass( elements_array[i], selected_atoms.selectedAtoms(i) );
        }
    }
    
    return m;
}

static const Property& get_mass_property(const MoleculeData &moldata,
                                         const PropertyMap &map)
{
    try
    {
        return moldata.property( map["mass"] );
    }
    catch(const SireBase::missing_property&)
    {
        return moldata.property( map["element"] );
    }
}
                                    

/** Return the mass of the selected part of this molecule, using 
    the supplied map to find either the mass property, or if that
    does not exist, using the element property
    
    \throw SireBase::missing_property
    \throw SireError::invalid_cast
*/
MolarMass Evaluator::mass(const PropertyMap &map) const
{
    if (selected_atoms.selectedNone())
        return MolarMass(0);

    const Property &p = get_mass_property(*d, map);
    
    if (p.isA<AtomMasses>())
    {
        return MolarMass( get_mass(p.asA<AtomMasses>(), selected_atoms) );
    }
    else if (p.isA<AtomElements>())
    {
        return MolarMass( get_mass(p.asA<AtomElements>(), selected_atoms) );
    }
    else
        throw SireError::invalid_cast( QObject::tr(
            "Cannot cast the property of type %1 into either an "
            "AtomMasses or AtomElements property!")
                .arg(p.what()), CODELOC );

    return MolarMass(0);
}

static double 
calc_charge(const PackedArray2D<SireUnits::Dimension::Charge>::Array &charges)
{
    const SireUnits::Dimension::Charge *charges_array = charges.constData();
    int ncharges = charges.count();
    
    double c = 0;
    
    for (int i=0; i<ncharges; ++i)
    {
        c += charges_array[i];
    }
    
    return c;
}

static double 
calc_charge(const PackedArray2D<SireUnits::Dimension::Charge>::Array &charges,
            const QSet<Index> &idxs)
{
    const SireUnits::Dimension::Charge *charges_array = charges.constData();
    
    double c = 0;
    
    foreach (Index i, idxs)
    {
        c += charges_array[i];
    }
    
    return c;
}

/** Return the total charge of the selected part of the molecule, using
    the supplied map to find the "charge" property
    
    \throw SireBase::missing_property
    \throw SireError::invalid_cast
*/
Charge Evaluator::charge(const PropertyMap &map) const
{
    const AtomCharges &charges = d->property( map["charge"] )
                                        .asA<AtomCharges>();
                                        
    if (selected_atoms.selectedNone())
        return Charge(0);

    double c = 0;
    
    const PackedArray2D<SireUnits::Dimension::Charge>::Array *charges_array 
                                            = charges.constData();
    int ncg = charges.count();

    if (selected_atoms.selectedAll())
    {
        for (int i=0; i<ncg; ++i)
        {
            c += calc_charge( charges_array[i] );
        }
    }
    else if (selected_atoms.selectedAllCutGroups())
    {
        for (CGIdx i(0); i<ncg; ++i)
        {
            if (selected_atoms.selectedAll(i))
                c += calc_charge( charges_array[i] );
            else
                c += calc_charge( charges_array[i], selected_atoms.selectedAtoms(i) );
        }
    }
    else
    {
        foreach (CGIdx i, selected_atoms.selectedCutGroups())
        {
            if (selected_atoms.selectedAll(i))
                c += calc_charge( charges_array[i] );
            else
                c += calc_charge( charges_array[i], selected_atoms.selectedAtoms(i) );
        }
    }
    
    return Charge(c);
}

int addToAvg(const CoordGroup &coords, Vector &avg)
{
    const Vector *coords_array = coords.constData();
    int nats = coords.count();
    
    for (int i=0; i<nats; ++i)
    {
        avg += coords_array[i];
    }
    
    return nats;
}

int addToAvg(const CoordGroup &coords, const QSet<Index> &indicies,
             Vector &avg)
{
    const Vector *coords_array = coords.constData();

    foreach (Index i, indicies)
    {
       avg += coords_array[i];
    }
    
    return indicies.count();
} 

/** Return the center of geometry of this part of the molecule

    \throw SireBase::missing_property
    \throw SireError::invalid_cast
*/
Vector Evaluator::centerOfGeometry(const PropertyMap &map) const
{
    if (selected_atoms.selectedNone())
        return Vector(0);
        
    const Property &prop = d->property( map["coordinates"] );
    const AtomCoords &coords = prop.asA<AtomCoords>();
    
    const CoordGroup *coords_array = coords.constData();
    int ncg = coords.count();
    
    //calculate the average coordinates
    Vector avg(0);
    
    int navg = 0;
    
    if (selected_atoms.selectedAll())
    {
        for (int i=0; i<ncg; ++i)
        { 
            navg += addToAvg(coords_array[i], avg);
        }
    }
    else if (selected_atoms.selectedAllCutGroups())
    {
        for (CGIdx i(0); i<ncg; ++i)
        {
            if (selected_atoms.selectedAll(i))
            {
                navg += addToAvg(coords_array[i], avg);
            }
            else
            {
                navg += addToAvg(coords_array[i], selected_atoms.selectedAtoms(i),
                                 avg);
            }
        }
    }
    else
    {
        foreach (CGIdx i, selected_atoms.selectedCutGroups())
        {
            if (selected_atoms.selectedAll(i))
            {
                navg += addToAvg(coords_array[i], avg);
            }
            else
            {
                navg += addToAvg(coords_array[i], selected_atoms.selectedAtoms(i),
                                 avg);
            }
        }
    }
    
    //form the average
    return avg / navg;
}

static double 
addToAvg(const CoordGroup &coords, 
         const PackedArray2D<SireUnits::Dimension::MolarMass>::Array &masses, 
         Vector &avg)
{
    const Vector *coords_array = coords.constData();
    const SireUnits::Dimension::MolarMass *masses_array = masses.constData();
    
    int nats = coords.count();
    
    double mass = 0;
    
    for (int i=0; i<nats; ++i)
    {
        avg += masses_array[i] * coords_array[i];
        mass += masses_array[i];
    }
    
    return mass;
}

double addToAvg(const CoordGroup &coords, 
                const PackedArray2D<SireUnits::Dimension::MolarMass>::Array &masses,
                const QSet<Index> &indicies, Vector &avg)
{
    const Vector *coords_array = coords.constData();
    const SireUnits::Dimension::MolarMass *masses_array = masses.constData();

    double mass = 0;

    foreach (Index i, indicies)
    {
       avg += masses_array[i] * coords_array[i];
       mass += masses_array[i];
    }
    
    return mass;
}

static Vector get_com(const AtomCoords &coords,
                      const AtomMasses &masses,
                      const AtomSelection &selected_atoms)
{
    const CoordGroup *coords_array = coords.constData();
    const PackedArray2D<SireUnits::Dimension::MolarMass>::Array *masses_array 
                                                            = masses.constData();
    
    int ncg = coords.count();
    
    //calculate the average mass position
    Vector avg(0);
    
    double total_mass = 0;
    
    if (selected_atoms.selectedAll())
    {
        for (int i=0; i<ncg; ++i)
        { 
            total_mass += addToAvg(coords_array[i], masses_array[i], avg);
        }
    }
    else if (selected_atoms.selectedAllCutGroups())
    {
        for (CGIdx i(0); i<ncg; ++i)
        {
            if (selected_atoms.selectedAll(i))
            {
                total_mass += addToAvg(coords_array[i], masses_array[i], avg);
            }
            else
            {
                total_mass += addToAvg(coords_array[i], masses_array[i],
                                       selected_atoms.selectedAtoms(i), avg);
            }
        }
    }
    else
    {
        foreach (CGIdx i, selected_atoms.selectedCutGroups())
        {
            if (selected_atoms.selectedAll(i))
            {
                total_mass += addToAvg(coords_array[i], masses_array[i], avg);
            }
            else
            {
                total_mass += addToAvg(coords_array[i], masses_array[i],
                                       selected_atoms.selectedAtoms(i), avg);
            }
        }
    }
    
    //form the average
    return avg / total_mass;
}

double addToAvg(const CoordGroup &coords, 
                const PackedArray2D<Element>::Array &elements, 
                Vector &avg)
{
    const Vector *coords_array = coords.constData();
    const Element *elements_array = elements.constData();
    
    int nats = coords.count();
    
    double mass = 0;
    
    for (int i=0; i<nats; ++i)
    {
        avg += elements_array[i].mass() * coords_array[i];
        mass += elements_array[i].mass();
    }
    
    return mass;
}

double addToAvg(const CoordGroup &coords, 
                const PackedArray2D<Element>::Array &elements,
                const QSet<Index> &indicies, Vector &avg)
{
    const Vector *coords_array = coords.constData();
    const Element *elements_array = elements.constData();

    double mass = 0;

    foreach (Index i, indicies)
    {
       avg += elements_array[i].mass() * coords_array[i];
       mass += elements_array[i].mass();
    }
    
    return mass;
}

static Vector get_com(const AtomCoords &coords,
                      const AtomElements &elements,
                      const AtomSelection &selected_atoms)
{
    const CoordGroup *coords_array = coords.constData();
    const PackedArray2D<Element>::Array *elements_array = elements.constData();
    
    int ncg = coords.count();
    
    //calculate the average mass position
    Vector avg(0);
    
    double total_mass = 0;
    
    if (selected_atoms.selectedAll())
    {
        for (int i=0; i<ncg; ++i)
        { 
            total_mass += addToAvg(coords_array[i], elements_array[i], avg);
        }
    }
    else if (selected_atoms.selectedAllCutGroups())
    {
        for (CGIdx i(0); i<ncg; ++i)
        {
            if (selected_atoms.selectedAll(i))
            {
                total_mass += addToAvg(coords_array[i], elements_array[i], avg);
            }
            else
            {
                total_mass += addToAvg(coords_array[i], elements_array[i],
                                       selected_atoms.selectedAtoms(i), avg);
            }
        }
    }
    else
    {
        foreach (CGIdx i, selected_atoms.selectedCutGroups())
        {
            if (selected_atoms.selectedAll(i))
            {
                total_mass += addToAvg(coords_array[i], elements_array[i], avg);
            }
            else
            {
                total_mass += addToAvg(coords_array[i], elements_array[i],
                                       selected_atoms.selectedAtoms(i), avg);
            }
        }
    }
    
    //form the average
    return avg / total_mass;
}

/** Return the center of mass of this part of the molecule

    \throw SireBase::missing_property
    \throw SireError::invalid_cast
*/
Vector Evaluator::centerOfMass(const PropertyMap &map) const
{
    if (selected_atoms.selectedNone())
        return Vector(0);
        
    const Property &prop = d->property( map["coordinates"] );
    const AtomCoords &coords = prop.asA<AtomCoords>();

    const Property &p = get_mass_property(*d,map);
    
    if (p.isA<AtomMasses>())
    {
        return get_com(coords, p.asA<AtomMasses>(), selected_atoms);
    }
    else if (p.isA<AtomElements>())
    {
        return get_com(coords, p.asA<AtomElements>(), selected_atoms);
    }
    else
        throw SireError::invalid_cast( QObject::tr(
            "Cannot cast the property of type %1 into either an "
            "AtomMasses or AtomElements property!")
                .arg(p.what()), CODELOC );
}

AxisSet Evaluator::alignmentAxes(const MoleculeView &other, 
                                 const AtomMatcher &matcher,
                                 const PropertyMap &map) const
{
    throw SireError::incomplete_code(CODELOC);
    return AxisSet();
}
                      
AxisSet Evaluator::alignmentAxes(const MoleculeView &other,
                                 const AtomMatcher &matcher,
                                 const PropertyMap &map0,
                                 const PropertyMap &map1) const
{
    throw SireError::incomplete_code(CODELOC);
    return AxisSet();
}

const char* Evaluator::typeName()
{
    return QMetaType::typeName( qMetaTypeId<Evaluator>() );
}

