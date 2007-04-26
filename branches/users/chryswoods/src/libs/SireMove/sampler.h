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

#ifndef SIREMOVE_SAMPLER_H
#define SIREMOVE_SAMPLER_H

#include <boost/tuple/tuple.hpp>

#include "SireBase/sharedpolypointer.hpp"

#include "SireMaths/rangenerator.h"

SIRE_BEGIN_HEADER

namespace SireMove
{
class Sampler;
class SamplerBase;
}

QDataStream& operator<<(QDataStream&, const SireMove::SamplerBase&);
QDataStream& operator>>(QDataStream&, SireMove::SamplerBase&);

QDataStream& operator<<(QDataStream&, const SireMove::Sampler&);
QDataStream& operator>>(QDataStream&, SireMove::Sampler&);

namespace SireMol
{
class Molecule;
class Residue;
class NewAtom;
class MoleculeGroup;
}

namespace SireMove
{

using boost::tuple;

using SireMaths::RanGenerator;

using SireMol::Molecule;
using SireMol::Residue;
using SireMol::NewAtom;
using SireMol::MoleculeGroup;

/** This is the base class of all Samplers. A Sampler is used
    to pick a random molecule from a MoleculeGroup

    @author Christopher Woods
*/
class SIREMOVE_EXPORT SamplerBase : public QSharedData
{

friend QDataStream& ::operator<<(QDataStream&, const SamplerBase&);
friend QDataStream& ::operator>>(QDataStream&, SamplerBase&);

public:
    SamplerBase();
    SamplerBase(const RanGenerator &rangenerator);

    SamplerBase(const SamplerBase &other);

    virtual ~SamplerBase();

    SamplerBase& operator=(const SamplerBase &other);

    static const char* typeName()
    {
        return "SireMove::SamplerBase";
    }

    void setGenerator(const RanGenerator &generator);
    const RanGenerator& generator() const;

    virtual const char* what() const=0;
    virtual SamplerBase* clone() const=0;

    virtual tuple<Molecule,double> randomMolecule(const MoleculeGroup &group)=0;
    virtual tuple<Residue,double> randomResidue(const MoleculeGroup &group)=0;
    virtual tuple<NewAtom,double> randomAtom(const MoleculeGroup &group)=0;

    virtual double probability(const MoleculeGroup &group,
                               const Molecule &molecule)=0;

    virtual double probability(const MoleculeGroup &group,
                               const Residue &residue)=0;

    virtual double probability(const MoleculeGroup &group,
                               const NewAtom &atom)=0;

protected:
    RanGenerator& generator();

private:
    /** The random number generator used by the sampler */
    RanGenerator rangen;
};

/** Internal function used to return a reference to the random
    number generator used by this sampler */
inline RanGenerator& SamplerBase::generator()
{
    return rangen;
}

/** This is a convienient holder for a SharedPolyPointer<SamplerBase>

    @author Christopher Woods
*/
class SIREMOVE_EXPORT Sampler
{

friend QDataStream& ::operator<<(QDataStream&, const Sampler&);
friend QDataStream& ::operator>>(QDataStream&, Sampler&);

public:
    Sampler();

    Sampler(const SamplerBase &sampler);

    Sampler(const Sampler &other);

    ~Sampler();

    Sampler& operator=(const Sampler &other);

    tuple<Molecule,double> randomMolecule(const MoleculeGroup &group);
    tuple<Residue,double> randomResidue(const MoleculeGroup &group);
    tuple<NewAtom,double> randomAtom(const MoleculeGroup &group);

    double probability(const MoleculeGroup &group, const Molecule &molecule);
    double probability(const MoleculeGroup &group, const Residue &residue);
    double probability(const MoleculeGroup &group, const NewAtom &atom);

    void setGenerator(const RanGenerator &generator);
    const RanGenerator& generator() const;

private:
    /** Implicitly shared pointer to the sampler implementation */
    SireBase::SharedPolyPointer<SamplerBase> d;
};

}

Q_DECLARE_METATYPE(SireMove::Sampler);

SIRE_END_HEADER

#endif
