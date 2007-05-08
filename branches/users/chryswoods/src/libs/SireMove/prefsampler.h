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

#include "SireMaths/vector.h"

#include "SireMol/moleculeid.h"
#include "SireMol/moleculegroupid.h"
#include "SireMol/moleculegroup.h"

#include "SireMol/partialmolecule.h"

#include "SireBase/version.h"

SIRE_BEGIN_HEADER

namespace SireMove
{
class PrefSampler;
}

QDataStream& operator<<(QDataStream&, const SireMove::PrefSampler&);
QDataStream& operator>>(QDataStream&, SireMove::PrefSampler&);

namespace SireMol
{
class MoleculeGroup;
}

namespace SireMove
{

using SireMol::MoleculeGroup;
using SireMol::PartialMolecule;

using SireMol::MoleculeID;
using SireMol::MoleculeGroupID;
using SireMol::PartialMolecule;

using SireBase::Version;

using SireMaths::Vector;
using SireMaths::RanGenerator;

/** This class is used to pick molecules at random
    from a MoleculeGroup, with the molecules closest
    to the

    @author Christopher Woods
*/
class SIREMOVE_EXPORT PrefSampler : public SamplerBase
{

friend QDataStream& ::operator<<(QDataStream&, const PrefSampler&);
friend QDataStream& ::operator>>(QDataStream&, PrefSampler&);

public:
    PrefSampler(const RanGenerator &rangenerator = RanGenerator());

    PrefSampler(const MoleculeGroup &group,
                const RanGenerator &rangenerator = RanGenerator());

    PrefSampler(const PartialMolecule &center,
                const RanGenerator &rangenerator = RanGenerator());

    PrefSampler(const PartialMolecule &center,
                double k,
                const RanGenerator &rangenerator = RanGenerator());

    PrefSampler(const PartialMolecule &center,
                const MoleculeGroup &group,
                const RanGenerator &rangenerator = RanGenerator());

    PrefSampler(const PartialMolecule &center,
                double k,
                const MoleculeGroup &group,
                const RanGenerator &rangenerator = RanGenerator());

    PrefSampler(const PrefSampler &other);

    ~PrefSampler();

    PrefSampler& operator=(const PrefSampler &other);

    void setGroup(const MoleculeGroup &group);
    void setCenterMolecule(const PartialMolecule &molecule);
    void setK(double k);

    const PartialMolecule& centerMolecule() const;
    double k() const;

    static const char* typeName()
    {
        return "SireMove::PrefSampler";
    }

    const char* what() const
    {
        return PrefSampler::typeName();
    }

    PrefSampler* clone() const
    {
        return new PrefSampler(*this);
    }

    tuple<PartialMolecule,double> sample(const QuerySystem &system);

    double probabilityOf(const PartialMolecule &molecule,
                         const QuerySystem &system);

    struct MolProb
    {
        MoleculeID ID;
        Version version;
        float probability;
    };

protected:
    bool _pvt_isEqual(const PropertyBase &other) const;

    float calculateProbability(const PartialMolecule &molecule) const;

    void completeUpdate(const PartialMolecule &new_center,
                        const MoleculeGroup &group);

    void partialUpdate(const MoleculeGroup &group);

private:
    void updateFrom(const QuerySystem &system);

    /** List of IDs, versions and probabilities
        of all of the molecules that could be selected */
    QVector<MolProb> molprobs;

    /** The sum of all of the molecule probabilities */
    float sum_of_probs;

    /** The highest probability value... */
    float max_prob;

    /** The k-value used in the probability calculation */
    double kval;

    /** The molecule from which the probability is calculated
         - the geometric center of the molecule is used - the
           closer a molecule is to the center, the higher
           the probability that it will be chosen */
    PartialMolecule center_mol;

    /** The point from which the probabilities are calculated */
    Vector center_point;

    /** The MoleculeGroup from which the molecule will be sampled */
    MoleculeGroup molgroup;

    /** Whether we need a complete update */
    bool need_complete_update;
};

/** Return the center molecule - molecules closest to this molecule
    will be sampled more frequently. */
inline const PartialMolecule& PrefSampler::centerMolecule() const
{
    return center_mol;
}

/** Return the value of 'k' - this is used to moderate the sampling
    algorithm - large values of k prevent volume expansion */
inline double PrefSampler::k() const
{
    return kval;
}

}

Q_DECLARE_METATYPE(SireMove::PrefSampler);
Q_DECLARE_TYPEINFO(SireMove::PrefSampler::MolProb, Q_MOVABLE_TYPE);

SIRE_END_HEADER

#endif
