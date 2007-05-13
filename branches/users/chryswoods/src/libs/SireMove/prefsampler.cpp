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

#include "SireMol/partialmolecule.h"
#include "SireMol/molecules.h"
#include "SireMol/moleculegroups.h"
#include "SireMol/propertyextractor.h"

#include "SireSystem/querysystem.h"
#include "SireSystem/systemdata.h"

#include "SireMol/errors.h"
#include "SireError/errors.h"

#include "SireStream/datastream.h"
#include "SireStream/shareddatastream.h"

using namespace SireMove;
using namespace SireSystem;
using namespace SireMol;
using namespace SireMaths;
using namespace SireStream;

static const RegisterMetaType<PrefSampler> r_prefsampler;

/** Serialise to a binary data stream */
QDataStream SIREMOVE_EXPORT &operator<<(QDataStream &ds,
                                        const PrefSampler &prefsampler)
{
    writeHeader(ds, r_prefsampler, 1);

    SharedDataStream sds(ds);

    sds << prefsampler.center_mol << prefsampler.center_point
        << prefsampler.molgroup << prefsampler.kval
        << static_cast<const SamplerBase&>(prefsampler);

    return ds;
}

/** Deserialise from a binary data stream */
QDataStream SIREMOVE_EXPORT &operator>>(QDataStream &ds,
                                        PrefSampler &prefsampler)
{
    VersionID v = readHeader(ds, r_prefsampler);

    if (v == 1)
    {
        SharedDataStream sds(ds);

        PrefSampler sampler;

        sds >> sampler.center_mol >> sampler.center_point
            >> sampler.molgroup >> sampler.kval
            >> static_cast<SamplerBase&>(sampler);

        sampler.need_complete_update = true;

        prefsampler = sampler;
    }
    else
        throw version_error(v, "1", r_prefsampler, CODELOC);

    return ds;
}

/** Construct using the supplied random number generator */
PrefSampler::PrefSampler(const RanGenerator &rangenerator)
            : SamplerBase(rangenerator),
              kval(0), need_complete_update(true)
{}

/** Construct a sampler that samples from the group 'group'
    using the supplied random number generator */
PrefSampler::PrefSampler(const MoleculeGroup &group,
                         const RanGenerator &rangenerator)
            : SamplerBase(rangenerator),
              kval(0), molgroup(group), need_complete_update(true)
{}

/** Construct a sampler that uses 'center' as the central molecule
    for preferential sampling */
PrefSampler::PrefSampler(const PartialMolecule &center,
                         const RanGenerator &rangenerator)
            : SamplerBase(rangenerator),
              kval(0), center_mol(center), need_complete_update(true)
{}

/** Construct a sampler that uses 'center' as the central molecule,
    and that has a k value of 'k' */
PrefSampler::PrefSampler(const PartialMolecule &center,
                         double k, const RanGenerator &rangenerator)
            : SamplerBase(rangenerator),
              kval(k), center_mol(center), need_complete_update(true)
{}

/** Construct a sampler that uses 'center' as the central molecule,
    and that samples from the group 'group' */
PrefSampler::PrefSampler(const PartialMolecule &center,
                         const MoleculeGroup &group,
                         const RanGenerator &rangenerator)
            : SamplerBase(rangenerator),
              kval(0), center_mol(center),
              molgroup(group), need_complete_update(true)
{}

/** Construct a sampler that uses 'center' as the central molecule,
    has a k value of 'k', and that samples from the group 'group' */
PrefSampler::PrefSampler(const PartialMolecule &center,
                         double k, const MoleculeGroup &group,
                         const RanGenerator &rangenerator)
            : SamplerBase(rangenerator),
              kval(k), center_mol(center),
              molgroup(group), need_complete_update(true)
{}

/** Copy constructor */
PrefSampler::PrefSampler(const PrefSampler &other)
            : SamplerBase(other),
              molprobs(other.molprobs),
              sum_of_probs(other.sum_of_probs),
              max_prob(other.max_prob),
              kval(other.kval),
              center_mol(other.center_mol),
              center_point(other.center_point),
              molgroup(other.molgroup),
              need_complete_update(other.need_complete_update)
{}

/** Destructor */
PrefSampler::~PrefSampler()
{}

/** Copy assignment */
PrefSampler& PrefSampler::operator=(const PrefSampler &other)
{
    if (this != &other)
    {
        molprobs = other.molprobs;
        sum_of_probs = other.sum_of_probs;
        max_prob = other.max_prob;
        kval = other.kval;
        center_mol = other.center_mol;
        center_point = other.center_point;
        molgroup = other.molgroup;
        need_complete_update = other.need_complete_update;

        SamplerBase::operator=(other);
    }

    return *this;
}

/** Calculate the (unnormalised) probability of choosing the molecule
    'molecule' */
float PrefSampler::calculateProbability(const PartialMolecule &molecule) const
{
    // p = 1 / (dist^2 + k)
    return 1.0 / (Vector::distance2(center_point,
                                    molecule.extract().geometricCenter())
                  + kval);
}

/** Perform a complete update of all of the statistics */
void PrefSampler::completeUpdate(const PartialMolecule &new_center,
                                 const MoleculeGroup &group)
{
    center_mol = new_center;
    center_point = center_mol.extract().geometricCenter();

    if (group.version().major() != molgroup.version().major())
    {
        //we need to recreate the array of molecule information
        //for the surrounding molecules
        molprobs = QVector<MolProb>( group.count() );
    }

    sum_of_probs = 0;
    max_prob = 0;

    BOOST_ASSERT( molprobs.count() == group.count() );

    MolProb *molprobs_array = molprobs.data();
    int i=0;

    for (Molecules::const_iterator it = group.molecules().constBegin();
         it != group.molecules().constEnd();
         ++it)
    {
        MolProb &molprob = molprobs_array[i];
        ++i;

        molprob.ID = it->ID();
        molprob.version = it->version();

        molprob.probability = calculateProbability(*it);

        sum_of_probs += molprob.probability;
        max_prob = qMax(max_prob, molprob.probability);
    }
}

/** Perform a partial update of the system */
void PrefSampler::partialUpdate(const MoleculeGroup &group)
{
    BOOST_ASSERT( molprobs.count() == group.count() );

    int nmols = molprobs.count();
    MolProb *molprobs_array = molprobs.data();

    //annoying linear search over all surrounding molecules...
    max_prob = 0;

    for (int i=0; i<nmols; ++i)
    {
        MolProb &molprob = molprobs_array[i];

        const PartialMolecule &molecule = group.molecule(molprob.ID);

        if (molecule.version() != molprob.version)
        {
            //this molecule needs to be updated...
            sum_of_probs -= molprob.probability;

            molprob.probability = calculateProbability(molecule);

            molprob.version = molecule.version();

            sum_of_probs += molprob.probability;
        }

        //need to find the largest probability
        max_prob = qMax(max_prob, molprob.probability);
    }
}

/** Update the statistics from the system 'system' */
void PrefSampler::updateFrom(const QuerySystem &system)
{
    //get the current version of the molecule...
    PartialMolecule current_mol = system.molecule(center_mol);
    const MoleculeGroup &group = system.groups().group( molgroup.ID() );

    if (need_complete_update or current_mol.version() != center_mol.version())
    {
        //we need to update everything as the center molecule has moved!
        this->completeUpdate(current_mol, group);
        need_complete_update = false;
    }
    else
    {
        if (group.version().major() != molgroup.version().major())
        {
            //we need to update everything as the number
            //or identity of surrounding molecules may have changed
            this->completeUpdate(center_mol, group);
        }
        else if (group.version().minor() != molgroup.version().minor())
        {
            //some of the surrounding molecules may have moved...
            this->partialUpdate(group);
        }
    }

    center_mol = current_mol;
    molgroup = group;
}

/** Set the MoleculeGroup from which molecules will be sampled */
void PrefSampler::setGroup(const MoleculeGroup &group)
{
    if (molgroup.ID() != group.ID() or
        molgroup.version().major() != group.version().major())
    {
        need_complete_update = true;
    }

    molgroup = group;
}

/** Set the center molecule that is used to calculate the probabilities */
void PrefSampler::setCenterMolecule(const PartialMolecule &molecule)
{
    if (center_mol != molecule)
    {
        center_mol = molecule;
        need_complete_update = true;
    }
}

/** Set the k value used in the probability calculation

    \throw SireError::invalid_arg
*/
void PrefSampler::setK(double k)
{
    if (k < 0)
        throw SireError::invalid_arg( QObject::tr(
            "You can only use values of k that are greater than zero (%1 is invalid!)")
                .arg(k), CODELOC );

    if (k != kval)
    {
        kval = k;
        need_complete_update = true;
    }
}

/** Choose a random molecule from the MoleculeGroup associated with this
    sampler, from the System 'system'. This returns a tuple of the molecule,
    and the probability that the molecule was sampled. */
tuple<PartialMolecule,double> PrefSampler::sample(const QuerySystem &system)
{
    const MoleculeGroup &group = system.groups().group(molgroup.ID());

    if (group.isEmpty())
        throw SireMol::missing_molecule( QObject::tr(
            "The MoleculeGroup is empty, so we can't choose a molecule!"),
                CODELOC );

    //update our statistics... (need to do this to get the most up-to-date
    //center molecule, even if we only have a single molecule to pick
    this->updateFrom(system);

    //do the easy thing if there is only one molecule in the group
    if (group.count() == 1)
        return tuple<PartialMolecule,double>(group.molecule(molprobs.at(0).ID),
                                             1.0);

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
            //test passed - return the molecule and the normalised probability
            //of picking the molecule
            return tuple<PartialMolecule,double>(group.molecule(molprob.ID),
                                                 molprob.probability / sum_of_probs);
    }
}

/** Return the probability of sampling the molecule 'molecule' in the passed
    system */
double PrefSampler::probabilityOf(const PartialMolecule &molecule,
                                  const QuerySystem &system)
{
    const MoleculeGroup &group = system.groups().group(molgroup.ID());

    if (not group.refersTo(molecule.ID()))
        throw SireMol::missing_molecule( QObject::tr(
            "The molecule \"%1\" (%2) is not in the group with ID == %3.")
                .arg(molecule.name()).arg(molecule.ID())
                .arg(molgroup.ID()), CODELOC );

    this->updateFrom(system);

    return this->calculateProbability(molecule) / sum_of_probs;
}

/** Comparison function */
bool PrefSampler::_pvt_isEqual(const PropertyBase &other) const
{
    BOOST_ASSERT(other.isA<PrefSampler>());

    const PrefSampler &sampler = other.asA<PrefSampler>();

    return molgroup == sampler.molgroup and kval == sampler.kval and
           center_mol == sampler.center_mol;
}

/** Assert that this sampler is compatible with the system 'system'

    \throw SireMol::missing_group
*/
void PrefSampler::assertCompatibleWith(const QuerySystem &system) const
{
    system.info().groups().assertContains(molgroup.ID());
}
