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

#include "SireStream/datastream.h"

using namespace SireMol;
using namespace SireStream;

static const RegisterMetaType<Evaluator> r_eval;

/** Serialise to a binary datastream */
QDataStream SIREMOL_EXPORT &operator<<(QDataStream &ds,
                                       const Evaluator &eval
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
Evaluator::Evaluator() : MoleculeView()
{}

/** Construct from the passed molecule view */
Evaluator::Evaluator(const MoleculeView &molecule)
          : MoleculeView(molecule), selected_atoms(molecule.selectedAtoms())
{}

/** Copy constructor */
Evaluator::Evaluator(const Evaluator &other)
          : MoleculeView(other), selected_atoms(other.selected_atoms)
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
    selected_atoms = other.selectedAtoms();
    
    return *this;
}

/** Return the selected atoms over which the properties
    will be evaluated */
AtomSelection Evaluator::selectedAtoms() const
{
    return selected_atoms;
}

/** Update this evaluator with a new version of the molecule */
void Evaluator::update(const MoleculeData &data) const
{
    MoleculeData::update(data);
}

static void getMinMax(const CoordGroup &cgroup, Vector &min, Vector &max)
{
    const Vector *cgroup_array = cgroup.constData();
    int nats = cgroup.count();
            
    for (int i=0; i<nats; ++i)
    {
        const Vector &coords = cgroup_array[i];
    
        min.setMin(coords);
        max.setMax(coords);
    }
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
    if (selected_atoms.selectedNone())
        return Vector(0);

    //get the coordinates of the atoms
    AtomicCoords coords = d->property(map["coordinates"]);
    
    const CoordGroup *coords_array = coords.constData();
    int ncg = coords.count();
    
    //now get the minimum and maximum coordinates...
    Vector mincoords( std::numeric_limits<double>::max() );
    Vector maxcoords( -std::numeric_limits<double>::min() );
    
    if (selected_atoms.selectedAll())
    {
        for (int i=0; i<ncg; ++i)
        {
            setMinMax(coords_array[i], min, max);
        }
    }
    else if (selected_atoms.selectedAllCutGroups())
    {
        for (CGIdx i(0); i<ncg; ++i)
        {
            if (selected_atoms.selectedAll(i))
            {
                setMinMax(coords_array[i], mincoords, maxcoords);
            }
            else
            {
                setMinMax(coords_array[i], selected_atoms.selectedAtoms(i),
                          mincoords, maxcoords);
            }
        }
    }
    else
    {
        QSet<CGIdx> cgidxs = selected_atoms.selectedCutGroups();
        
        foreach (CGIdx cgidx, cgidxs)
        {
            if (selected_atoms.selectedAll(cgidx))
            {
                setMinMax(coords_array[cgidx], mincoords, maxcoords);
            }
            else
            {
                setMinMax(coords_array[cgidx], selected_atoms.selectedAtoms(cgidx),
                          mincoords, maxcoords);
            }
        }
    }
    
    return mincoords + 0.5*(maxcoords - mincoords);
}

tuple<Vector,Vector>...

double calcMass(const QVector<SireUnits::Dimension::Mass> &masses)
{
    const SireUnits::Dimension::Mass *masses_array = masses.constData();
    int nmasses = masses.count();
    
    double m = 0;
    
    for (int i=0; i<nmasses; ++i)
    {
        m += masses_array[i];
    }
    
    return m;
}

double calcMass(const QVector<SireUnits::Dimension::Mass> &masses,
                const QSet<Index> &idxs)
{
    const SireUnits::Dimension::Mass *masses_array = masses.constData();
    
    double m = 0;
    
    foreach (Index i, idxs)
    {
        m += masses_array[i];
    }
    
    return m;
}

double Evaluator::_pvt_mass(const AtomicMasses &masses) const
{
    double m = 0;
    
    const QVector<SireUnits::Dimension::Mass> *masses_array = masses.constData();
    int ncg = masses.count();

    if (selected_atoms.selectedAll())
    {
        for (int i=0; i<ncg; ++i)
        {
            m += calcMass( masses_array[i] );
        }
    }
    else if (selected_atoms.selectedAllCutGroups())
    {
        for (CGIdx i(0); i<ncg; ++i)
        {
            if (selected_atoms.selectedAll(i))
                m += calcMass( masses_array[i] );
            else
                m += calcMass( masses_array[i], selected_atoms.selectedAtoms(i) );
        }
    }
    else
    {
        foreach (CGIdx cgidx, selected_atoms.selectedCutGroups())
        {
            if (selected_atoms.selectedAll(cgidx))
                m += calcMass( masses_array[i] );
            else
                m += calcMass( masses_array[i], selected_atoms.selectedAtoms(i) );
        }
    }
    
    return m;
}

double calcMass(const QVector<Element> &elements)
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

double calcMass(const QVector<Element> &elements, const QSet<Index> &idxs)
{
    const Element *elements_array = elements.constData();
    
    double m = 0;
    
    foreach (Index i, idxs)
    {
        m += elements_array[i].mass();
    }
    
    return m;
}

double Evaluator::_pvt_mass(const AtomicElements &elements) const
{
    double m = 0;
    
    const QVector<Element> *elements_array = elements.constData();
    int ncg = elements.count();

    if (selected_atoms.selectedAll())
    {
        for (int i=0; i<ncg; ++i)
        {
            m += calcMass( elements_array[i] );
        }
    }
    else if (selected_atoms.selectedAllCutGroups())
    {
        for (CGIdx i(0); i<ncg; ++i)
        {
            if (selected_atoms.selectedAll(i))
                m += calcMass( elements_array[i] );
            else
                m += calcMass( elements_array[i], selected_atoms.selectedAtoms(i) );
        }
    }
    else
    {
        foreach (CGIdx cgidx, selected_atoms.selectedCutGroups())
        {
            if (selected_atoms.selectedAll(cgidx))
                m += calcMass( elements_array[i] );
            else
                m += calcMass( elements_array[i], selected_atoms.selectedAtoms(i) );
        }
    }
    
    return m;
}

/** Return the mass of the selected part of this molecule, using 
    the supplied map to find either the mass property, or if that
    does not exist, using the element property
    
    \throw SireBase::missing_property
    \throw SireError::invalid_cast
*/
double Evaluator::mass() const
{
    if (selected_atoms.selectedNone())
        return 0;

    Property p = d->property( map["mass"], map["element"] );
    
    if (p.isA<AtomicMasses>())
    {
        return this->_pvt_mass(p.asA<AtomicMasses>());
    }
    else if (p.isA<AtomicElements>())
    {
        return this->_pvt_mass(p.asA<AtomicElements>());
    }
    else
        throw SireError::invalid_cast( QObject::tr(
            "Cannot cast the property of type %1 into either an "
            "AtomicMasses or AtomicElements property!")
                .arg(p.what()), CODELOC );

    return 0;
}
