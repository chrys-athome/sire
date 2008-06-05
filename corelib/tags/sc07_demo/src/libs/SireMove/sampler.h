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
#include "SireBase/property.h"

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
class PartialMolecule;
}

namespace SireSystem
{
class QuerySystem;
}

namespace SireMove
{

using boost::tuple;

using SireBase::PropertyBase;
using SireBase::SharedPolyPointer;

using SireMaths::RanGenerator;

using SireMol::PartialMolecule;

using SireSystem::QuerySystem;

/** This is the base class of all Samplers. A Sampler is used
    to pick a random molecule from a MoleculeGroup

    @author Christopher Woods
*/
class SIREMOVE_EXPORT SamplerBase : public PropertyBase
{

friend QDataStream& ::operator<<(QDataStream&, const SamplerBase&);
friend QDataStream& ::operator>>(QDataStream&, SamplerBase&);

public:
    SamplerBase(const RanGenerator &rangenerator);

    SamplerBase(const SamplerBase &other);

    ~SamplerBase();

    SamplerBase& operator=(const SamplerBase &other);

    virtual SamplerBase* clone() const=0;

    static const char* typeName()
    {
        return "SireMove::SamplerBase";
    }

    static SharedPolyPointer<SamplerBase> null_sampler();

    void setGenerator(const RanGenerator &generator);
    const RanGenerator& generator() const;

    virtual tuple<PartialMolecule,double> sample(const QuerySystem &system)=0;

    virtual double probabilityOf(const PartialMolecule &molecule,
                                 const QuerySystem &system)=0;

    virtual void assertCompatibleWith(const QuerySystem &system) const=0;

protected:
    RanGenerator& _pvt_generator();

private:
    /** The random number generator used by the sampler */
    RanGenerator rangen;
};

/** Internal function used to return a reference to the random
    number generator used by this sampler */
inline RanGenerator& SamplerBase::_pvt_generator()
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

    tuple<PartialMolecule,double> sample(const QuerySystem &system);

    double probabilityOf(const PartialMolecule &molecule,
                         const QuerySystem &system);

    void setGenerator(const RanGenerator &generator);
    const RanGenerator& generator() const;

    void assertCompatibleWith(const QuerySystem &system) const;

private:
    /** Implicitly shared pointer to the sampler implementation */
    SireBase::SharedPolyPointer<SamplerBase> d;
};

}

Q_DECLARE_METATYPE(SireMove::Sampler);

SIRE_END_HEADER

#endif
