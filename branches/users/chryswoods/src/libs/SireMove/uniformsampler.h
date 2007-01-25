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

#ifndef SIREMOVE_UNIFORMSAMPLER_H
#define SIREMOVE_UNIFORMSAMPLER_H

#include "sampler.h"

SIRE_BEGIN_HEADER

namespace SireMove
{

/** This class is used to pick contained Molecules, Residues
    or Atoms at random (uniformly distributed).
    
    @author Christopher Woods
*/
class SIREMOVE_EXPORT UniformSampler : public SamplerBase
{
public:
    UniformSampler();
    UniformSampler(const RanGenerator &rangenerator);

    UniformSampler(const UniformSampler &other);

    ~UniformSampler();

    UniformSampler& operator=(const UniformSampler &other);

    static const char* typeName()
    {
        return "SireMove::UniformSampler";
    }

    const char* what() const
    {
        return UniformSampler::typeName();
    }

    UniformSampler* clone() const
    {
        return new UniformSampler(*this);
    }

    tuple<Molecule,double> randomMolecule(const MoleculeGroup &group);
    tuple<Residue,double> randomResidue(const MoleculeGroup &group);
    tuple<NewAtom,double> randomAtom(const MoleculeGroup &group);

    double probability(const MoleculeGroup &group, const Molecule &molecule);
    double probability(const MoleculeGroup &group, const Residue &residue);
    double probability(const MoleculeGroup &group, const NewAtom &atom);

private:
    tuple<Molecule,double> _pvt_randomMolecule(const MoleculeGroup &group, 
                                               uint nmols);
};

}

Q_DECLARE_METATYPE(SireMove::UniformSampler);

SIRE_END_HEADER

#endif
