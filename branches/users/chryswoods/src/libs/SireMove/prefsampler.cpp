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
#include "SireMol/moleculegroup.h"
#include "SireMol/moleculegroups.h"
#include "SireMol/propertyextractor.h"

#include "SireSystem/querysystem.h"

#include "SireMol/errors.h"

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
        << prefsampler.groupid
        << static_cast<const SamplerBase&>(prefsampler)
    
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
            >> sampler.groupid
            >> static_cast<SamplerBase&>(sampler);
            
        prefsampler = sampler;
    }
    else
        throw version_error(v, "1", r_prefsampler, CODELOC);
    
    return ds;
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
    
    if (group.version().major() != group_version.major())
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
    
    if (current_mol.version() != center_mol.version())
    {
        //we need to update everything as the center molecule has moved!
        this->completeUpdate(current_mol, system.groups().group(groupid));
    }
    else
    {
        const MoleculeGroup &group = system.groups().group(groupid);
        
        if (group.version().major() != group_version.major())
        {
            //we need to update everything as the number 
            //or identity of surrounding molecules may have changed
            this->completeUpdate(center_mol, group);
        }
        else if (group.version().minor() != group_version.minor())
        {
            //some of the surrounding molecules may have moved...
            this->partialUpdate(group);
        }
    }
}

PrefSampler::PrefSampler(const RanGenerator &rangenerator = RanGenerator());

PrefSampler::PrefSampler(const MoleculeGroup &group,
            const RanGenerator &rangenerator = RanGenerator());

PrefSampler::PrefSampler(const PartialMolecule &center_mol,
            const RanGenerator &rangenerator = RanGenerator());

PrefSampler::PrefSampler(const PartialMolecule &center_mol,
            const MoleculeGroup &group,
            const RanGenerator &rangenerator = RanGenerator());

PrefSampler::PrefSampler(const PrefSampler &other);

PrefSampler::~PrefSampler();

PrefSampler& PrefSampler::operator=(const PrefSampler &other);

void PrefSampler::setGroup(const MoleculeGroup &group);
void PrefSampler::setCenterMolecule(const PartialMolecule &molecule);

tuple<PartialMolecule,double> PrefSampler::sample(const QuerySystem &system)
{
    const MoleculeGroup &group = system.groups().group(groupid);
    
    if (group.isEmpty())
        throw SireMol::missing_molecule( QObject::tr(
            "The MoleculeGroup is empty, so we can't choose a molecule!"),
                CODELOC );
    
    this->updateFrom(system);
    
    if (group.count() == 1)
        return tuple<PartialMolecule,double>(group.molecule(molprobs.at(0).ID),
                                             1.0);
    
    //use the von Neumann rejection method to choose a random molecule 
    //using the owicki weights
    // (weights = 1 / (dist^2 + k), rejection method - choose random
    //  molecule, then choose random number from 0 to max_prob. If 
    //  probability of the molecule <= the random number, then accept
    //  this molecule, else go back to the beginning and try again...
    
    //pick a random molecule...
    while (true)
    {
        //choose a random molecule ID...
        quint32 i = _pvt_generator().randInt(molprobs.count()-1);
        
        const MolProb &molprob = molprobs.constData()[i];
        
        //compare the normalised probability against a random number from 0 to max_prob
        if (_pvt_generator().rand(max_prob) <= molprob.probability)
            return tuple<PartialMolecule,double>(group.molecule(molprob.ID),
                                                 molprob.probability / sum_of_probs);
    }
}

double PrefSampler::probabilityOf(const PartialMolecule &molecule,
                                  const QuerySystem &system)
{
    const MoleculeGroup &group = system.groups().group(groupid);
    
    if (not group.refersTo(molecule.ID()))
        throw SireMol::missing_molecule( QObject::tr(
            "The molecule \"%1\" (%2) is not in the group with ID == %3.")
                .arg(molecule.name()).arg(molecule.ID())
                .arg(groupid), CODELOC );
    
    this->updateFrom(system);
    
    return this->calculateProbability(molecule) / sum_of_probs;
}

bool PrefSampler::_pvt_isEqual(const PropertyBase &other) const;
