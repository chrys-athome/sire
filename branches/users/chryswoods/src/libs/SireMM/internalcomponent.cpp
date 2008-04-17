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

#include "internalcomponent.h"

using namespace SireMM;
using namespace SireFF;

using namespace SireCAS;

//////
////// Implementation of BondComponent
//////

/** Constructor */
BondComponent::BondComponent(const FFName &ffname)
              : FFComponent(ffname, QLatin1String("bond"))
{}

/** Construct from a symbol

    \throw SireError::incompatible_error
*/
BondComponent::BondComponent(const SireCAS::Symbol &symbol)
              : FFComponent(symbol, QLatin1String("bond"))
{}

/** Copy constructor */  
BondComponent::BondComponent(const BondComponent &other)
              : FFComponent(other)
{}
  
/** Destructor */  
BondComponent::~BondComponent()
{}

/** Set the component of the energy in the forcefield 'ff'
    to be equal to the passed energy */
void BondComponent::setEnergy(FF &ff, const BondEnergy &nrg) const
{
    FFComponent::setEnergy(ff, this->total(), nrg);
}

/** Change the component of the energy in the forcefield 'ff'
    by 'delta' */
void BondComponent::changeEnergy(FF &ff, const BondEnergy &delta) const
{
    FFComponent::changeEnergy(ff, this->total(), delta);
}

//////
////// Implementation of AngleComponent
//////

/** Constructor */
AngleComponent::AngleComponent(const FFName &ffname)
               : FFComponent(ffname, QLatin1String("angle"))
{}

/** Construct from a symbol

    \throw SireError::incompatible_error
*/
AngleComponent::AngleComponent(const SireCAS::Symbol &symbol)
               : FFComponent(symbol, QLatin1String("angle"))
{}

/** Copy constructor */  
AngleComponent::AngleComponent(const AngleComponent &other)
              : FFComponent(other)
{}
  
/** Destructor */  
AngleComponent::~AngleComponent()
{}

/** Set the component of the energy in the forcefield 'ff'
    to be equal to the passed energy */
void AngleComponent::setEnergy(FF &ff, const AngleEnergy &nrg) const
{
    FFComponent::setEnergy(ff, this->total(), nrg);
}

/** Change the component of the energy in the forcefield 'ff'
    by 'delta' */
void AngleComponent::changeEnergy(FF &ff, const AngleEnergy &delta) const
{
    FFComponent::changeEnergy(ff, this->total(), delta);
}

//////
////// Implementation of DihedralComponent
//////

/** Constructor */
DihedralComponent::DihedralComponent(const FFName &ffname)
                  : FFComponent(ffname, QLatin1String("dihedral"))
{}

/** Construct from a symbol

    \throw SireError::incompatible_error
*/
DihedralComponent::DihedralComponent(const SireCAS::Symbol &symbol)
                  : FFComponent(symbol, QLatin1String("dihedral"))
{}

/** Copy constructor */  
DihedralComponent::DihedralComponent(const DihedralComponent &other)
              : FFComponent(other)
{}
  
/** Destructor */  
DihedralComponent::~DihedralComponent()
{}

/** Set the component of the energy in the forcefield 'ff'
    to be equal to the passed energy */
void DihedralComponent::setEnergy(FF &ff, const DihedralEnergy &nrg) const
{
    FFComponent::setEnergy(ff, this->total(), nrg);
}

/** Change the component of the energy in the forcefield 'ff'
    by 'delta' */
void DihedralComponent::changeEnergy(FF &ff, const DihedralEnergy &delta) const
{
    FFComponent::changeEnergy(ff, this->total(), delta);
}

//////
////// Implementation of ImproperComponent
//////

/** Constructor */
ImproperComponent::ImproperComponent(const FFName &ffname)
                  : FFComponent(ffname, QLatin1String("improper"))
{}

/** Construct from a symbol

    \throw SireError::incompatible_error
*/
ImproperComponent::ImproperComponent(const SireCAS::Symbol &symbol)
                  : FFComponent(symbol, QLatin1String("improper"))
{}

/** Copy constructor */  
ImproperComponent::ImproperComponent(const ImproperComponent &other)
                  : FFComponent(other)
{}
  
/** Destructor */  
ImproperComponent::~ImproperComponent()
{}

/** Set the component of the energy in the forcefield 'ff'
    to be equal to the passed energy */
void ImproperComponent::setEnergy(FF &ff, const ImproperEnergy &nrg) const
{
    FFComponent::setEnergy(ff, this->total(), nrg);
}

/** Change the component of the energy in the forcefield 'ff'
    by 'delta' */
void ImproperComponent::changeEnergy(FF &ff, const ImproperEnergy &delta) const
{
    FFComponent::changeEnergy(ff, this->total(), delta);
}

//////
////// Implementation of UreyBradleyComponent
//////

/** Constructor */
UreyBradleyComponent::UreyBradleyComponent(const FFName &ffname)
                     : FFComponent(ffname, QLatin1String("urey-bradley"))
{}

/** Construct from a symbol

    \throw SireError::incompatible_error
*/
UreyBradleyComponent::UreyBradleyComponent(const SireCAS::Symbol &symbol)
                     : FFComponent(symbol, QLatin1String("urey-bradley"))
{}

/** Copy constructor */  
UreyBradleyComponent::UreyBradleyComponent(const UreyBradleyComponent &other)
                     : FFComponent(other)
{}
  
/** Destructor */  
UreyBradleyComponent::~UreyBradleyComponent()
{}

/** Set the component of the energy in the forcefield 'ff'
    to be equal to the passed energy */
void UreyBradleyComponent::setEnergy(FF &ff, const UreyBradleyEnergy &nrg) const
{
    FFComponent::setEnergy(ff, this->total(), nrg);
}

/** Change the component of the energy in the forcefield 'ff'
    by 'delta' */
void UreyBradleyComponent::changeEnergy(FF &ff, const UreyBradleyEnergy &delta) const
{
    FFComponent::changeEnergy(ff, this->total(), delta);
}

//////
////// Implementation of StretchStretchComponent
//////

/** Constructor */
StretchStretchComponent::StretchStretchComponent(const FFName &ffname)
                        : FFComponent(ffname, QLatin1String("stretch-stretch"))
{}

/** Construct from a symbol

    \throw SireError::incompatible_error
*/
StretchStretchComponent::StretchStretchComponent(const SireCAS::Symbol &symbol)
                        : FFComponent(symbol, QLatin1String("stretch-stretch"))
{}

/** Copy constructor */  
StretchStretchComponent::StretchStretchComponent(const StretchStretchComponent &other)
                        : FFComponent(other)
{}
  
/** Destructor */  
StretchStretchComponent::~StretchStretchComponent()
{}

/** Set the component of the energy in the forcefield 'ff'
    to be equal to the passed energy */
void StretchStretchComponent::setEnergy(FF &ff, const StretchStretchEnergy &nrg) const
{
    FFComponent::setEnergy(ff, this->total(), nrg);
}

/** Change the component of the energy in the forcefield 'ff'
    by 'delta' */
void StretchStretchComponent::changeEnergy(FF &ff, 
                                           const StretchStretchEnergy &delta) const
{
    FFComponent::changeEnergy(ff, this->total(), delta);
}

//////
////// Implementation of StretchBendComponent
//////

/** Constructor */
StretchBendComponent::StretchBendComponent(const FFName &ffname)
                     : FFComponent(ffname, QLatin1String("stretch-bend"))
{}

/** Construct from a symbol

    \throw SireError::incompatible_error
*/
StretchBendComponent::StretchBendComponent(const SireCAS::Symbol &symbol)
                     : FFComponent(symbol, QLatin1String("stretch-bend"))
{}

/** Copy constructor */  
StretchBendComponent::StretchBendComponent(const StretchBendComponent &other)
                     : FFComponent(other)
{}
  
/** Destructor */  
StretchBendComponent::~StretchBendComponent()
{}

/** Set the component of the energy in the forcefield 'ff'
    to be equal to the passed energy */
void StretchBendComponent::setEnergy(FF &ff, const StretchBendEnergy &nrg) const
{
    FFComponent::setEnergy(ff, this->total(), nrg);
}

/** Change the component of the energy in the forcefield 'ff'
    by 'delta' */
void StretchBendComponent::changeEnergy(FF &ff, const StretchBendEnergy &delta) const
{
    FFComponent::changeEnergy(ff, this->total(), delta);
}

//////
////// Implementation of BendBendComponent
//////

/** Constructor */
BendBendComponent::BendBendComponent(const FFName &ffname)
                  : FFComponent(ffname, QLatin1String("bend-bend"))
{}

/** Construct from a symbol

    \throw SireError::incompatible_error
*/
BendBendComponent::BendBendComponent(const SireCAS::Symbol &symbol)
                  : FFComponent(symbol, QLatin1String("bend-bend"))
{}

/** Copy constructor */  
BendBendComponent::BendBendComponent(const BendBendComponent &other)
                  : FFComponent(other)
{}
  
/** Destructor */  
BendBendComponent::~BendBendComponent()
{}

/** Set the component of the energy in the forcefield 'ff'
    to be equal to the passed energy */
void BendBendComponent::setEnergy(FF &ff, const BendBendEnergy &nrg) const
{
    FFComponent::setEnergy(ff, this->total(), nrg);
}

/** Change the component of the energy in the forcefield 'ff'
    by 'delta' */
void BendBendComponent::changeEnergy(FF &ff, const BendBendEnergy &delta) const
{
    FFComponent::changeEnergy(ff, this->total(), delta);
}

//////
////// Implementation of StretchBendTorsionComponent
//////

/** Constructor */
StretchBendTorsionComponent::StretchBendTorsionComponent(const FFName &ffname)
                  : FFComponent(ffname, QLatin1String("stretch-bend-torsion"))
{}

/** Construct from a symbol

    \throw SireError::incompatible_error
*/
StretchBendTorsionComponent::StretchBendTorsionComponent(const SireCAS::Symbol &symbol)
                  : FFComponent(symbol, QLatin1String("stretch-bend-torsion"))
{}

/** Copy constructor */  
StretchBendTorsionComponent::StretchBendTorsionComponent(
                                        const StretchBendTorsionComponent &other)
              : FFComponent(other)
{}
  
/** Destructor */  
StretchBendTorsionComponent::~StretchBendTorsionComponent()
{}

/** Set the component of the energy in the forcefield 'ff'
    to be equal to the passed energy */
void StretchBendTorsionComponent::setEnergy(FF &ff, 
                                        const StretchBendTorsionEnergy &nrg) const
{
    FFComponent::setEnergy(ff, this->total(), nrg);
}

/** Change the component of the energy in the forcefield 'ff'
    by 'delta' */
void StretchBendTorsionComponent::changeEnergy(FF &ff, 
                                        const StretchBendTorsionEnergy &delta) const
{
    FFComponent::changeEnergy(ff, this->total(), delta);
}

//////
////// Implementation of InternalComponent
//////

/** Constructor */
InternalComponent::InternalComponent(const FFName &ffname)
                  : FFComponent(ffname, QLatin1String("internal")),
                    bond_component(ffname), angle_component(ffname),
                    dihedral_component(ffname), 
                    improper_component(ffname), ub_component(ffname),
                    ss_component(ffname), sb_component(ffname),
                    bb_component(ffname), sbt_component(ffname)
{}

/** Construct from a symbol

    \throw SireError::incompatible_error
*/
InternalComponent::InternalComponent(const SireCAS::Symbol &symbol)
                  : FFComponent(symbol, QLatin1String("internal"))
{
    bond_component = BondComponent( this->forceFieldName() );
    angle_component = AngleComponent( this->forceFieldName() );
    dihedral_component = DihedralComponent( this->forceFieldName() );
    
    improper_component = ImproperComponent( this->forceFieldName() );
    ub_component = UreyBradleyComponent( this->forceFieldName() );
    
    ss_component = StretchStretchComponent( this->forceFieldName() );
    sb_component = StretchBendComponent( this->forceFieldName() );
    bb_component = BendBendComponent( this->forceFieldName() );
    sbt_component = StretchBendTorsionComponent( this->forceFieldName() );
}

/** Copy constructor */  
InternalComponent::InternalComponent(const InternalComponent &other)
                  : FFComponent(other), 
                    bond_component(other.bond_component),
                    angle_component(other.angle_component),
                    dihedral_component(other.dihedral_component),
                    improper_component(other.improper_component),
                    ub_component(other.ub_component),
                    ss_component(other.ss_component),
                    sb_component(other.sb_component),
                    bb_component(other.bb_component),
                    sbt_component(other.sbt_component)
{}
  
/** Destructor */  
InternalComponent::~InternalComponent()
{}

/** Set the internal components of the forcefield 'ff' to the passed values */
void InternalComponent::setEnergy(FF &ff, const InternalEnergy &value) const
{
    FFComponent::setEnergy(ff, this->total(), value.total());
    FFComponent::setEnergy(ff, this->bond(), value.bond());
    FFComponent::setEnergy(ff, this->angle(), value.angle());
    FFComponent::setEnergy(ff, this->dihedral(), value.dihedral());
    
    FFComponent::setEnergy(ff, this->improper(), value.improper());
    FFComponent::setEnergy(ff, this->ureyBradley(), value.ureyBradley());
    
    FFComponent::setEnergy(ff, this->stretchStretch(), value.stretchStretch());
    FFComponent::setEnergy(ff, this->stretchBend(), value.stretchBend());
    FFComponent::setEnergy(ff, this->bendBend(), value.bendBend());
    FFComponent::setEnergy(ff, this->stretchBendTorsion(), value.stretchBendTorsion());
}

/** Change the internal components of the forcefield 'ff' by 'delta' */
void InternalComponent::changeEnergy(FF &ff, const InternalEnergy &delta) const
{
    FFComponent::changeEnergy(ff, this->total(), delta.total());
    FFComponent::changeEnergy(ff, this->bond(), delta.bond());
    FFComponent::changeEnergy(ff, this->angle(), delta.angle());
    FFComponent::changeEnergy(ff, this->dihedral(), delta.dihedral());
    
    FFComponent::changeEnergy(ff, this->improper(), delta.improper());
    FFComponent::changeEnergy(ff, this->ureyBradley(), delta.ureyBradley());
    
    FFComponent::changeEnergy(ff, this->stretchStretch(), delta.stretchStretch());
    FFComponent::changeEnergy(ff, this->stretchBend(), delta.stretchBend());
    FFComponent::changeEnergy(ff, this->bendBend(), delta.bendBend());
    FFComponent::changeEnergy(ff, this->stretchBendTorsion(), 
                              delta.stretchBendTorsion());
}

/////////
///////// Implementation of InternalEnergy
/////////

/** Constructor */
InternalEnergy::InternalEnergy(double bondnrg, double anglenrg,
                               double dihedralnrg, 
                               double impropernrg, double ubnrg,
                               double ssnrg, double sbnrg,
                               double bbnrg, double sbtnrg)
               : ibndnrg(bondnrg), iangnrg(anglenrg),
                 idihnrg(dihedralnrg), 
                 iimpnrg(impropernrg), iubnrg(ubnrg),
                 issnrg(ssnrg), isbnrg(sbnrg),
                 ibbnrg(bbnrg), isbtnrg(sbtnrg)
{}
  
/** Copy constructor */  
InternalEnergy::InternalEnergy(const InternalEnergy &other)
               : ibndnrg(other.ibndnrg), iangnrg(other.iangnrg),
                 idihnrg(other.idihnrg), 
                 iimpnrg(other.iimpnrg), iubnrg(other.iubnrg),
                 issnrg(other.issnrg), isbnrg(other.isbnrg),
                 ibbnrg(other.ibbnrg), isbtnrg(other.isbtnrg)
{}
  
/** Destructor */  
InternalEnergy::~InternalEnergy()
{}
