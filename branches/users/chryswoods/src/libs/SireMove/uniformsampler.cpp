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

#include "uniformsampler.h"

#include "SireMol/partialmolecule.h"

#include "SireMol/moleculegroup.h"
#include "SireMol/moleculegroups.h"

#include "SireMol/moleculeid.h"

#include "SireSystem/querysystem.h"

#include "SireMol/errors.h"

#include "SireStream/datastream.h"

using namespace SireMol;
using namespace SireMove;
using namespace SireSystem;
using namespace SireStream;

static const RegisterMetaType<UniformSampler> r_sampler;

/** Serialise to a binary datastream */
QDataStream SIREMOVE_EXPORT &operator<<(QDataStream &ds,
                                        const UniformSampler &sampler)
{
    writeHeader(ds, r_sampler, 1)
          << sampler.groupid
          << static_cast<const SamplerBase&>(sampler);

    return ds;
}

/** Deserialise from a binary datastream */
QDataStream SIREMOVE_EXPORT &operator>>(QDataStream &ds, UniformSampler &sampler)
{
    VersionID v = readHeader(ds, r_sampler);

    if (v == 1)
    {
        UniformSampler newsampler;
        
        ds >> sampler.groupid
           >> static_cast<SamplerBase&>(newsampler);
        
        sampler = newsampler;
    }
    else
        throw version_error(v, "1", r_sampler, CODELOC);

    return ds;
}

/** Constructor */
UniformSampler::UniformSampler(const RanGenerator &rangenerator) 
               : SamplerBase(rangenerator), groupid(0), majver(0)
{}

/** Update the statistics regarding the current group... */
void UniformSampler::updateFrom(const MoleculeGroup &group)
{
    if (majver != group.version().major())
    {
        //recreate the list of MoleculeIDs
        molids = group.moleculeIDs().toList();
        majver = group.version().major();
    }
}

/** Constructor */
UniformSampler::UniformSampler(const MoleculeGroup &molgroup,
                               const RanGenerator &generator)
               : SamplerBase(generator),
                 groupid(molgroup.ID()), majver(0)
{
    this->updateFrom(molgroup);
}

/** Copy constructor */
UniformSampler::UniformSampler(const UniformSampler &other)
               : SamplerBase(other),
                 molids(other.molids),
                 groupid(other.groupid), majver(other.groupid)
{}

/** Destructor */
UniformSampler::~UniformSampler()
{}

/** Copy assignment */
UniformSampler& UniformSampler::operator=(const UniformSampler &other)
{
    SamplerBase::operator=(other);
    
    molids = other.molids;
    groupid = other.groupid;
    majver = other.majver;

    return *this;
}

/** Set the group from which molecules will be sampled */
void UniformSampler::setGroup(const MoleculeGroup &group)
{
    if (not (group.ID() == groupid and group.version().major() == majver))
    {
        groupid = group.ID();
        majver = 0;
        this->updateFrom(group);
    }
}

/** Comparison operator */
bool UniformSampler::_pvt_isEqual(const PropertyBase &other) const
{
    BOOST_ASSERT( other.isA<UniformSampler>() );
    
    const UniformSampler &sampler = other.asA<UniformSampler>();
    
    return groupid == sampler.groupid and majver == sampler.majver;
}

/** Return a random molecule from the passed System. This
    throws an exception if there are no molecules in the group,
    or the group is not in the system!

    \throw SireMol::missing_molecule
    \throw SireMol::missing_group
*/
tuple<PartialMolecule,double> UniformSampler::sample(const QuerySystem &system)
{
    //get the group from the system
    const MoleculeGroup &group = system.groups().group(groupid);

    if (group.isEmpty())
        throw SireMol::missing_molecule( QObject::tr(
            "The MoleculeGroup is empty, so we can't choose a molecule!"),
                CODELOC );
    
    //update the statistics to mirror 'group'
    this->updateFrom(group);
    
    int nmols = molids.count();
  
    BOOST_ASSERT(nmols != 0);
  
    if (nmols == 1)
        return tuple<PartialMolecule,double>(group.molecule(molids.at(0)),
                                             1.0);
    else
    {
        //choose a random molecule ID...
        quint32 i = _pvt_generator().randInt(molids.count()-1);
    
        //return the matching molecule
        return tuple<PartialMolecule,double>(group.molecule( molids.at(i) ),
                                             1.0 / molids.count());
    }
}

/** Return the probability of selecting the molecule 'molecule' from
    the system 'system'. An exception will be thrown if it in not
    possible to select this molecule from the passed group,
    or if the group is not in the system.
    
    \throw SireMol::missing_molecule
    \throw SireMol::missing_group
*/
double UniformSampler::probabilityOf(const PartialMolecule &molecule,
                                     const QuerySystem &system)
{
    const MoleculeGroup &group = system.groups().group(groupid);
    
    if (not group.refersTo(molecule.ID()))
        throw SireMol::missing_molecule( QObject::tr(
            "The molecule \"%1\" (%2) is not in the group with ID == %3.")
                .arg(molecule.name()).arg(molecule.ID())
                .arg(groupid), CODELOC );
    
    this->updateFrom(group);

    return 1.0 / molids.count();
}
