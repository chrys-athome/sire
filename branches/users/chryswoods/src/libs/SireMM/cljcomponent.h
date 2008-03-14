/********************************************\
  *
  *  Sire - Molecular Simulation Framework
  *
  *  Copyright (C) 2008  Christopher Woods
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

#ifndef SIREMM_CLJCOMPONENT_H
#define SIREMM_CLJCOMPONENT_H

#include "SireFF/ffcomponent.h"

SIRE_BEGIN_HEADER

namespace SireMM
{

namespace detail
{

/** This class represents a Coulomb component of a forcefield */
class SIREMM_EXPORT CoulombComponent : public SireFF::detail::FFComponent
{
public:
    CoulombComponent(quint64 ffuid = 0);
    CoulombComponent(const SireCAS::Symbol &symbol);
    
    CoulombComponent(const CoulombComponent &other);
    
    ~CoulombComponent();
    
    static const char* typeName()
    {
        return "SireMM::CoulombComponent";
    }
    
    const char* what() const
    {
        return CoulombComponent::typeName();
    }
    
    CoulombComponent* clone() const
    {
        return new CoulombComponent(*this);
    }
    
    const CoulombComponent& total() const
    {
        return *this;
    }
};

/** This class represents a LJ component of a forcefield */
class SIREMM_EXPORT LJComponent : public SireFF::detail::FFComponent
{
public:
    LJComponent(quint64 ffuid = 0);
    LJComponent(const SireCAS::Symbol &symbol);
    
    LJComponent(const LJComponent &other);
    
    ~LJComponent();
    
    static const char* typeName()
    {
        return "SireMM::LJComponent";
    }
    
    const char* what() const
    {
        return LJComponent::typeName();
    }
    
    LJComponent* clone() const
    {
        return new LJComponent(*this);
    }
    
    const LJComponent& total() const
    {
        return *this;
    }
};

/** This class represents the sum of the coulomb and LJ components
    of the forcefield */
class SIREMM_EXPORT CLJComponent : public SireFF::detail::FFComponent
{
public:
    CLJComponent(quint64 ffuid = 0);
    CLJComponent(const SireCAS::Symbol &symbol);
    
    CLJComponent(const CLJComponent &other);
    
    ~CLJComponent();
    
    const CoulombComponent& coulomb() const
    {
        return coul_component;
    }
    
    const LJComponent& lj() const
    {
        return lj_component;
    }
    
    static const char* typeName()
    {
        return "SireMM::CLJComponent";
    }
    
    const char* what() const
    {
        return CLJComponent::typeName();
    }
    
    CLJComponent* clone() const
    {
        return new CLJComponent(*this);
    }

    const CLJComponent& total() const
    {
        return *this;
    }

protected:
    /** The coulomb component */
    CoulombComponent coul_component;
    
    /** The LJ component */
    LJComponent lj_component;
};

typedef SireFF::detail::ComponentEnergy<CoulombComponent> CoulombEnergy;
typedef SireFF::detail::ComponentEnergy<LJComponent> LJEnergy;

/** This class holds the coulomb and Lennard-Jones (LJ) components
    of the energy.
*/
class SIREMM_EXPORT CLJEnergy
{
public:
    typedef CLJComponent Components;

    CLJEnergy(double cnrg=0, double ljnrg=0) : icnrg(cnrg), iljnrg(ljnrg)
    {}
    
    CLJEnergy(const CLJEnergy &other) : icnrg(other.icnrg), iljnrg(other.iljnrg)
    {}
    
    ~CLJEnergy()
    {}
    
    static const char* typeName()
    {
        return "SireMM::CLJEnergy";
    }
    
    const char* what() const
    {
        return CLJEnergy::typeName();
    }
    
    CLJEnergy& operator+=(const CLJEnergy &other)
    {
        icnrg += other.icnrg;
        iljnrg += other.iljnrg;
        return *this;
    }
    
    CLJEnergy& operator-=(const CLJEnergy &other)
    {
        icnrg -= other.icnrg;
        iljnrg -= other.iljnrg;
        return *this;
    }
    
    Components components() const
    {
        return Components();
    }
    
    double component(const CoulombComponent&) const
    {
        return icnrg;
    }
    
    double component(const LJComponent&) const
    {
        return iljnrg;
    }
    
    double component(const CLJComponent&) const
    {
        return icnrg + iljnrg;
    }
    
    double coulomb() const
    {
        return icnrg;
    }
    
    double lj() const
    {
        return iljnrg;
    }
    
    operator double() const
    {
        //return the total energy
        return icnrg + iljnrg;
    }
    
    operator CoulombEnergy() const
    {
        return CoulombEnergy(icnrg);
    }

    operator LJEnergy() const
    {
        return LJEnergy(iljnrg);
    }

private:
    /** The coulomb and LJ components of the energy */
    double icnrg, iljnrg;
};

} // end of namespace detail

} // end of namespace SireMM

SIRE_END_HEADER

#endif
