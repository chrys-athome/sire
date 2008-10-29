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

#include "prefsampler.h"

#include "atomselection.h"

#include "SireStream/datastream.h"
#include "SireStream/shareddatastream.h"

using namespace SireMove;
using namespace SireMol;
using namespace SireBase;
using namespace SireMaths;
using namespace SireStream;

static const RegisterMetaType<PrefSampler> r_prefsampler;

/** Serialise to a binary datastream */
QDataStream SIREMOVE_EXPORT &operator<<(QDataStream &ds, const PrefSampler &prefsampler)
{
    writeHeader(ds, r_prefsampler, 1);
    
    SharedDataStream sds(ds);
    
    sds << prefsampler.focal_molecule << prefsampler.focal_point 
        << prefsampler.coords_property
        << prefsampler.sampling_constant
        << static_cast<const Sampler&>(prefsampler);
        
    return ds;
}

/** Extract from a binary datastream */
QDataStream SIREMOVE_EXPORT &operator>>(QDataStream &ds, PrefSampler &prefsampler)
{
    VersionID v = readHeader(ds, r_prefsampler);
    
    if (v == 1)
    {
        SharedDataStream sds(ds);
        
        sds >> prefsampler.focal_molecule >> prefsampler.focal_point
            >> prefsampler.coords_property
            >> prefsampler.sampling_constant
            >> static_cast<Sampler&>(prefsampler);
            
        prefsampler.is_dirty = true;
    }
    else
        throw version_error( v, "1", r_prefsampler, CODELOC );
        
    return ds;
}

/** Update weights from the passed new molecule group. This new group
    will replace the existing group */
void PrefSampler::updateWeights(const MoleculeGroup &new_group)
{
    if (new_group == this->moleculeGroup())
        //nothing to do
        return;
        
    Sampler::setGroup(new_group);
}

/** Completely recalculate the weights from scratch */
void PrefSampler::recalculateWeights()
{
    if (not is_dirty)
        return;
        
    //recalculate the weights...
    
    
    is_dirty = false;
}

/** Construct a sampler that prefers the molecules near the origin 
    (using the optionally supplied preferential sampling constant, 'k') */
PrefSampler::PrefSampler(double k)
            : ConcreteProperty<PrefSampler,Sampler>(),
              coords_property("coordinates"),
              sampling_constant(k), is_dirty(true)
{}

/** Construct a sampler that prefers the molecules that are closest
    to the point 'point' (using the optionally supplied preferential
    sampling constant 'k') */
PrefSampler::PrefSampler(const Vector &point, double k)
            : ConcreteProperty<PrefSampler,Sampler>(),
              focal_point(point),
              coords_property("coordinates"),
              sampling_constant(k),
              is_dirty(true)
{}
            
/** Construct a sampler that samples molecule views from the group 'molgroup',
    with a preference to sample molecules that are closest to the 
    point 'point' (using the optionally supplied preferential
    sampling constant 'k') */
PrefSampler::PrefSampler(const Vector &point, const MoleculeGroup &molgroup,
                         double k)
            : ConcreteProperty<PrefSampler,Sampler>(molgroup),
              focal_point(point), 
              coords_property("coordinates"),
              sampling_constant(k),
              is_dirty(true)
{}

/** Construct a sampler that prefers the molecules that are closest to 
    the view 'molview' (using the optionally supplied preferential
    sampling constant 'k') */
PrefSampler::PrefSampler(const MoleculeView &molview, double k)
            : ConcreteProperty<PrefSampler,Sampler>(),
              focal_molecule(molview), 
              coords_property("coordinates"),
              sampling_constant(k),
              is_dirty(true)
{}

/** Construct a sampler that samples the molecules in 'molgroup', and
    that prefers molecules that are closest to the view 'molview' 
    (using the optionally supplied preferential sampling constant 'k') */
PrefSampler::PrefSampler(const MoleculeView &molview, const MoleculeGroup &molgroup,
                         double k)
            : ConcreteProperty<PrefSampler,Sampler>(molgroup),
              focal_molecule(molview),
              coords_property("coordinates"),
              sampling_constant(k),
              is_dirty(true)
{}

/** Copy constructor */
PrefSampler::PrefSampler(const PrefSampler &other)
            : ConcreteProperty<PrefSampler,Sampler>(other),
              focal_molecule(other.focal_molecule),
              focal_point(other.focal_point),
              coords_property(other.coords_property),
              sampling_constant(other.sampling_constant),
              is_dirty(other.is_dirty)
{}

/** Destructor */
PrefSampler::~PrefSampler()
{}

/** Copy assignment operator */
PrefSampler& PrefSampler::operator=(const PrefSampler &other)
{
    if (this != &other)
    {
        focal_molecule = other.focal_molecule;
        focal_point = other.focal_point;
        coords_property = other.coords_property;
        sampling_constant = other.sampling_constant;
        is_dirty = other.is_dirty;
        
        Sampler::operator=(other);
    }
    
    return *this;
}

/** Comparison operator */
bool PrefSampler::operator==(const PrefSampler &other) const
{
    return focal_molecule == other.focal_molecule and
           focal_point == other.focal_point and
           coords_property == other.coords_property and
           sampling_constant == other.sampling_constant and
           Sampler::operator==(other);
}

/** Comparison operator */
bool PrefSampler::operator!=(const PrefSampler &other) const
{
    return not this->operator==(other);
}

/** Set the molecule group from which molecule view will be sampled */
void PrefSampler::setGroup(const MoleculeGroup &molgroup)
{
    this->updateWeights(molgroup);
}

/** Update this sampler so that the molecules match the versions
    in the system 'system' */
void PrefSampler::updateFrom(const System &system)
{
    if (this->usingFocalMolecule())
    {
        //has the focal molecule changed?
        MolNum molnum = focal_molecule.data().number();
        
        if (system.contains(molnum))
        {
            if (system[molnum].data().version() != focal_molecule.data().number())
            {
                //yes - the system contains the molecule with a different version
                focal_molecule.update( system[molnum].data() );
                is_dirty = true;
            }
        }
    }
    
    //now update the MoleculeGroup
    Sampler::updateFrom(system);
}

/** Sample a molecule from the group, and return it, together
    with the probability with which it was chosen */
tuple<PartialMolecule,double> PrefSampler::sample() const
{
    this->recalculateWeights();
    
    //sample the molecule

    //use the von Neumann rejection method to choose a random molecule
    //using the owicki weights
    // (weights = 1 / (dist^2 + k), rejection method - choose random
    //  molecule, then choose random number from 0 to max_prob. If
    //  probability of the molecule <= the random number, then accept
    //  this molecule, else go back to the beginning and try again...

    BOOST_ASSERT(max_prob > 0);

    //pick a random molecule...
    while (true)
    {
        //choose a random molecule ID...
        quint32 i = _pvt_generator().randInt(molprobs.count()-1);

        //get the desired probability of choosing this molecule...
        const MolProb &molprob = molprobs.constData()[i];

        //compare the normalised probability against a random number from 0 to max_prob
        if (_pvt_generator().rand(max_prob) <= molprob.probability)
        {
            //test passed - return the molecule and the normalised probability
            //of picking the molecule
            return tuple<PartialMolecule,double>(group.molecule(molprob.ID),
                                                 molprob.probability / sum_of_probs);
        }
    }

}

/** Return the probability with which the molecule 'molecule' was 
    sampled from this sampler */
double PrefSampler::probabilityOf(const PartialMolecule &molecule) const
{
    this->recalculateWeights();
    
    //sample the molecule
}

/** Set the focal molecule of this sampler */
void PrefSampler::setFocalMolecule(const MoleculeView &molview)
{
    focal_molecule = PartialMolecule(molview);
    is_dirty = true;
}

/** Set the focal point of this sampler */
void PrefSampler::setFocalPoint(const Vector &point)
{
    focal_molecule = PartialMolecule();
    focal_point = point;
    is_dirty = true;
}

/** Set the preferential sampling constant - this is the value
    of 'k' in the biasing algorithm */
void PrefSampler::setSamplingConstant(double k)
{
    sampling_constant = k;
}

/** Return the preferential sampling constant (k) */
double PrefSampler::samplingConstant() const
{
    return sampling_constant;
}

/** Return whether we are using a focal molecule (rather
    than a focal point) */
bool PrefSampler::usingFocalMolecule() const
{
    return not focal_molecule.selection().selectedNone();
}

/** Return whether we are using a focal point (rather
    than a focal molecule) */
bool PrefSampler::usingFocalPoint() const
{
    return focal_molecule.selection().selectedNone();
}

/** Return the current focal point (this will be the current center of the 
    molecule view if a focal molecule is being used) */
const Vector& PrefSampler::focalPoint() const
{
    return focal_point;
}

/** Return the current focal molecule (this will be an empty molecule
    if a focal point is being used) */
const PartialMolecule& PrefSampler::focalMolecule() const
{
    return focal_molecule;
}
