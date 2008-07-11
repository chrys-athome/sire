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

#include "SireMol/molgroup.h"
#include "SireMol/moleculegroup.h"

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

namespace SireMove
{

using boost::tuple;

using SireBase::PropertyBase;

using SireMaths::RanGenerator;

using SireMol::PartialMolecule;
using SireMol::MolGroup;
using SireMol::MoleculeGroup;

/** This is the base class of all Samplers. A Sampler is used
    to pick a random molecule from a MoleculeGroup

    @author Christopher Woods
*/
class SIREMOVE_EXPORT SamplerBase : public PropertyBase
{

friend QDataStream& ::operator<<(QDataStream&, const SamplerBase&);
friend QDataStream& ::operator>>(QDataStream&, SamplerBase&);

public:
    SamplerBase();
    SamplerBase(const MolGroup &molgroup);

    SamplerBase(const SamplerBase &other);

    virtual ~SamplerBase();

    virtual SamplerBase* clone() const=0;

    static const char* typeName()
    {
        return "SireMove::SamplerBase";
    }

    void setGenerator(const RanGenerator &generator);
    const RanGenerator& generator() const;

    const MolGroup& group() const;
    
    virtual void setGroup(const MolGroup &molgroup);

    virtual tuple<PartialMolecule,double> sample() const=0;

    virtual double probabilityOf(const PartialMolecule &molecule) const=0;

protected:
    SamplerBase& operator=(const SamplerBase &other);
    
    bool operator==(const SamplerBase &other) const;
    bool operator!=(const SamplerBase &other) const;

private:
    /** The molecule group from which molecules are sampled */
    MoleculeGroup molgroup;

    /** The random number generator used by the sampler */
    RanGenerator rangen;
};

/** This is the polymorphic pointer holder for the 
    Sampler hierarchy of classes (objects to randomly
    choose molecules from a molecule group)
    
    Like all Property polymorphic pointer holder classes,
    this class holds the polymorphic Sampler object as 
    an implicitly shared pointer. You can access the 
    const functions of this object by dereferencing this
    pointer, or by using the Sampler::read() function, e.g.;
    
    mol_and_prob = sampler->sample();
    mol_and_prob = sampler.read().sample();
    
    You must use the Sampler::edit() function to
    access the non-const member functions, e.g.;
    
    sampler.edit().setGroup(molgroup);
    
    Because an implicitly shared pointer is held, this
    class can be copied and passed around quickly. A copy
    is only made when the object being pointed to is
    edited via the .edit() function.

    @author Christopher Woods
*/
class SIREMOVE_EXPORT Sampler : public SireBase::Property
{

friend QDataStream& ::operator<<(QDataStream&, const Sampler&);
friend QDataStream& ::operator>>(QDataStream&, Sampler&);

public:
    Sampler();
    Sampler(const SireBase::PropertyBase &property);
    Sampler(const SamplerBase &molgroup);

    Sampler(const Sampler &other);
    
    ~Sampler();
    
    virtual Sampler& operator=(const SireBase::PropertyBase &property);
    virtual Sampler& operator=(const SamplerBase &other);

    const SamplerBase* operator->() const;
    const SamplerBase& operator*() const;
    
    const SamplerBase& read() const;
    SamplerBase& edit();
    
    const SamplerBase* data() const;
    const SamplerBase* constData() const;
    
    SamplerBase* data();
    
    operator const SamplerBase&() const;

    static const Sampler& shared_null();
};

#ifndef SIRE_SKIP_INLINE_FUNCTIONS

/** Return the molecule group from which a molecule will be sampled */
inline const MolGroup& SamplerBase::group() const
{
    return molgroup;
}

/** Internal function used to return a reference to the random
    number generator used by this sampler */
inline const RanGenerator& SamplerBase::generator() const
{
    return rangen;
}

#endif //SIRE_SKIP_INLINE_FUNCTIONS

}

Q_DECLARE_METATYPE(SireMove::Sampler)

SIRE_EXPOSE_CLASS( SireMove::SamplerBase )

SIRE_EXPOSE_PROPERTY( SireMove::Sampler, SireMove::SamplerBase )

SIRE_END_HEADER

#endif
