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

#include "cljcomponent.h"

using namespace SireMM;
using namespace SireFF;

using namespace SireCAS;

//////
////// Implementation of CoulombComponent
//////

/** Constructor */
CoulombComponent::CoulombComponent(const FFName &ffname)
                 : FFComponent(ffname, QLatin1String("coulomb"))
{}

/** Construct from a symbol

    \throw SireError::incompatible_error
*/
CoulombComponent::CoulombComponent(const SireCAS::Symbol &symbol)
                 : FFComponent(symbol, QLatin1String("coulomb"))
{}

/** Copy constructor */  
CoulombComponent::CoulombComponent(const CoulombComponent &other)
                 : FFComponent(other)
{}
  
/** Destructor */  
CoulombComponent::~CoulombComponent()
{}

/** Set the coulomb component of the energy in the forcefield 'ff'
    to equal to the passed CoulombEnergy */
void CoulombComponent::setEnergy(FF &ff, const CoulombEnergy &cnrg) const
{
    FFComponent::setEnergy(ff, this->total(), cnrg);
}

/** Change the coulomb component of the energy in the forcefield 'ff'
    by 'delta' */
void CoulombComponent::changeEnergy(FF &ff, const CoulombEnergy &delta) const
{
    FFComponent::changeEnergy(ff, this->total(), delta);
}

//////
////// Implementation of LJComponent
//////

/** Constructor */
LJComponent::LJComponent(const FFName &ffname)
            : FFComponent(ffname, QLatin1String("LJ"))
{}

/** Construct from a symbol

    \throw SireError::incompatible_error
*/
LJComponent::LJComponent(const SireCAS::Symbol &symbol)
            : FFComponent(symbol, QLatin1String("LJ"))
{}

/** Copy constructor */  
LJComponent::LJComponent(const LJComponent &other)
            : FFComponent(other)
{}
  
/** Destructor */  
LJComponent::~LJComponent()
{}

/** Set the LJ component of the energy in the forcefield 'ff'
    to equal to the passed LJEnergy */
void LJComponent::setEnergy(FF &ff, const LJEnergy &ljnrg) const
{
    FFComponent::setEnergy(ff, this->total(), ljnrg);
}

/** Change the LJ component of the energy in the forcefield 'ff'
    by 'delta' */
void LJComponent::changeEnergy(FF &ff, const LJEnergy &delta) const
{
    FFComponent::changeEnergy(ff, this->total(), delta);
}

//////
////// Implementation of CLJComponent
//////

/** Constructor */
CLJComponent::CLJComponent(const FFName &ffname)
            : FFComponent(ffname, QLatin1String("CLJ")),
               coul_component(ffname), lj_component(ffname)
{}

/** Construct from a symbol

    \throw SireError::incompatible_error
*/
CLJComponent::CLJComponent(const SireCAS::Symbol &symbol)
            : FFComponent(symbol, QLatin1String("CLJ"))
{
    coul_component = CoulombComponent( this->forceFieldName() );
    lj_component = LJComponent( this->forceFieldName() );
}

/** Copy constructor */  
CLJComponent::CLJComponent(const CLJComponent &other)
            : FFComponent(other), coul_component(other.coul_component), 
              lj_component(other.lj_component)
{}
  
/** Destructor */  
CLJComponent::~CLJComponent()
{}

/** Set the CLJ components of the forcefield 'ff' to the passed values */
void CLJComponent::setEnergy(FF &ff, const CLJEnergy &value) const
{
    FFComponent::setEnergy(ff, this->total(), value.total());
    FFComponent::setEnergy(ff, this->coulomb(), value.coulomb());
    FFComponent::setEnergy(ff, this->lj(), value.lj());
}

/** Change the CLJ components of the forcefield 'ff' by 'delta' */
void CLJComponent::changeEnergy(FF &ff, const CLJEnergy &delta) const
{
    FFComponent::changeEnergy(ff, this->total(), delta.total());
    FFComponent::changeEnergy(ff, this->coulomb(), delta.coulomb());
    FFComponent::changeEnergy(ff, this->lj(), delta.lj());
}
