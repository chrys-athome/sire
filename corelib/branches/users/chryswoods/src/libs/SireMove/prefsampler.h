/********************************************\
  *
  *  Sire - Molecular Simulation Framework
  *
  *  Copyright (C) 2006  Christopher Woods
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

#ifndef SIREMOVE_PREFSAMPLER_H
#define SIREMOVE_PREFSAMPLER_H

#include "sampler.h"

#include "SireMol/partialmolecule.h"

#include "SireBase/propertymap.h"

#include "SireMaths/vector.h"

SIRE_BEGIN_HEADER

namespace SireMove
{
class PrefSampler;
}

QDataStream& operator<<(QDataStream&, const SireMove::PrefSampler&);
QDataStream& operator>>(QDataStream&, SireMove::PrefSampler&);

namespace SireMove
{

using SireMaths::Vector;

using SireBase::PropertyName;

/** This is a sampler that is used to select molecules
    using the distance-based preferential sampling algorithm.
    
    This sampler can be used to bias the choice of molecules
    such that those closest to a target molecule are chosen
    with a higher probability than those that are further
    away
    
    @author Christopher Woods
*/
class SIREMOVE_EXPORT PrefSampler
          : public ConcreteProperty<PrefSampler,Sampler>
{

friend QDataStream& ::operator<<(QDataStream&, const PrefSampler&);
friend QDataStream& ::operator>>(QDataStream&, PrefSampler&);

public:
    PrefSampler(double k=0);
    
    PrefSampler(const Vector &point, double k=0);
    PrefSampler(const Vector &point, const MoleculeGroup &molgroup,
                double k=0);
    
    PrefSampler(const MoleculeView &molview, double k=0);
    PrefSampler(const MoleculeView &molview, const MoleculeGroup &molgroup,
                double k=0);
    
    PrefSampler(const PrefSampler &other);
    
    ~PrefSampler();
    
    PrefSampler& operator=(const PrefSampler &other);
    
    static const char* typeName()
    {
        return QMetaType::typeName( qMetaTypeId<PrefSampler>() );
    }
    
    PrefSampler* clone() const
    {
        return new PrefSampler(*this);
    }
    
    bool operator==(const PrefSampler &other) const;
    bool operator!=(const PrefSampler &other) const;
    
    void setGroup(const MoleculeGroup &molgroup);

    void updateFrom(const System &system);
    
    tuple<PartialMolecule,double> sample() const;

    double probabilityOf(const PartialMolecule &molecule) const;

    void setFocalMolecule(const MoleculeView &molview);
    void setFocalPoint(const Vector &point);
    
    void setCoordinatesProperty(const PropertyName &coords_property);
    
    void setSamplingConstant(double k);
    
    bool usingFocalMolecule() const;
    bool usingFocalPoint() const;
    
    const Vector& focalPoint() const;
    const PartialMolecule& focalMolecule() const;

    double samplingConstant() const;
    
    const PropertyName& coordinatesProperty() const;

private:
    void updateWeights(const MoleculeGroup &new_group);
    void recalculateWeights();

    /** The view of the molecule, the center of which is used
        as the focal point for the preferential sampling algorithm */
    PartialMolecule focal_molecule;
    
    /** The actual focal point of the preferential sampling algorithm */
    Vector focal_point;
    
    /** The coordinates property used to find the coordinates
        of the molecules */
    PropertyName coords_property;
    
    /** The preferential sampling constant */
    double sampling_constant;
    
    /** Whether or not this is dirty (requires a complete recalculation
        of the weights) */
    bool is_dirty;
};

}

Q_DECLARE_METATYPE( SireMove::PrefSampler )

SIRE_EXPOSE_CLASS( SireMove::PrefSampler )

SIRE_END_HEADER

#endif
