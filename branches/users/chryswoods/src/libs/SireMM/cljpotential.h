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

#ifndef SIREMM_CLJPOTENTIAL_H
#define SIREMM_CLJPOTENTIAL_H

#include "SireBase/properties.h"
#include "SireBase/propertymap.h"
#include "SireBase/pairmatrix.hpp"
#include "SireBase/packedarray2d.hpp"

#include "SireVol/space.h"

#include "cljcomponent.h"
#include "cljnbpairs.h"

#include "detail/intrascaledatomicparameters.hpp"

#include "ljparameterdb.h"
#include "switchingfunction.h"

#include "SireFF/detail/ffmolecules3d.h"

SIRE_BEGIN_HEADER

namespace SireMM
{
class InterCLJPotential;
class IntraCLJPotential;
}

QDataStream& operator<<(QDataStream&, const SireMM::InterCLJPotential&);
QDataStream& operator>>(QDataStream&, SireMM::InterCLJPotential&);

QDataStream& operator<<(QDataStream&, const SireMM::IntraCLJPotential&);
QDataStream& operator>>(QDataStream&, SireMM::IntraCLJPotential&);

namespace SireMol
{
class PartialMolecule;
class MolGroup;
}

namespace SireFF
{
typedef SireBase::PackedArray2D<SireMaths::Vector> MolForceTable;
}

namespace SireMM
{

using SireBase::Properties;
using SireBase::PropertyBase;
using SireBase::PropertyMap;
using SireBase::PropertyName;

using SireVol::Space;

using SireMol::PartialMolecule;
using SireMol::MolGroup;

using SireFF::MolForceTable;

namespace detail
{

/** This class provides the default name of the 
    property that contains the charge parameters */
class SIREMM_EXPORT ChargeParameterName
{
public:
    ChargeParameterName()
    {}
    
    ~ChargeParameterName()
    {}
    
    const PropertyName& charge() const
    {
        return chg_param;
    }
    
private:
    static PropertyName chg_param;
};

/** This class provides the default name of the 
    property that contains the LJ parameters */
class SIREMM_EXPORT LJParameterName
{
public:
    LJParameterName()
    {}
    
    ~LJParameterName()
    {}
    
    const PropertyName& lj() const
    {
        return lj_param;
    }
    
private:
    static PropertyName lj_param;
};

/** This class provides the default name of the properties
    that contain the charge and LJ parameters */
class SIREMM_EXPORT CLJParameterNames : public ChargeParameterName,
                                        public LJParameterName
{
public:
    CLJParameterNames() : ChargeParameterName(), LJParameterName()
    {}
    
    ~CLJParameterNames()
    {}
};

/** This class provides the default name of the properties
    that contain the charge, LJ and 3D coordinates properties */
class SIREMM_EXPORT CLJParameterNames3D : public CLJParameterNames,
                                          public SireFF::detail::Coords3DParameterName
{
public:
    CLJParameterNames3D()
    {}
    
    ~CLJParameterNames3D()
    {}
};

/** This class provides the default name of the properties 
    that contain the charge, LJ, intramolecular NB scale parameters and
    3D coordinates properties */
class SIREMM_EXPORT ScaledCLJParameterNames3D : public CLJParameterNames3D,
                                                public detail::IntraScaleParameterName
{
public:
    ScaledCLJParameterNames3D() : CLJParameterNames3D(),
                                  detail::IntraScaleParameterName()
    {}
    
    ~ScaledCLJParameterNames3D()
    {}
};

/** This class holds the CLJ parameter used by both the Inter- and Intra-
    CLJPotentials. It is just the charge of the atom in reduced units
    (atomic charge divided by sqrt(4 pi eps_0)) and the ID of the LJ
    parameter in the singleton LJParameterDB database
    
    @author Christopher Woods
*/
class SIREMM_EXPORT CLJParameter
{
public:
    CLJParameter(double charge=0, quint32 lj_id=0)
          : reduced_charge(charge), ljid(lj_id)
    {}
    
    CLJParameter(const CLJParameter &other)
          : reduced_charge(other.reduced_charge), ljid(other.ljid)
    {}
    
    ~CLJParameter()
    {}
    
    bool operator==(const CLJParameter &other) const
    {
        return reduced_charge == other.reduced_charge and
               ljid == other.ljid;
    }
    
    bool operator!=(const CLJParameter &other) const
    {
        return reduced_charge != other.reduced_charge or
               ljid != other.ljid;
    }
    
    double reduced_charge;
    quint32 ljid;
};

} // end of namespace detail

/** This class provides all of the functions and containers  
    necessary to provide an interface to calculate the
    intermolecular interatomic potentials using the Coulomb 
    and Lennard Jones functions.
    
    This is a 3D potential class, namely it requires that
    the atoms possess 3D coordinates, thereby allowing this
    potential to also be used to calculate 3D forces on the atoms.
=    
    @author Christopher Woods
*/
class SIREMM_EXPORT InterCLJPotential
{

friend QDataStream& ::operator<<(QDataStream&, const InterCLJPotential&);
friend QDataStream& ::operator>>(QDataStream&, InterCLJPotential&);

public:

    typedef detail::CLJEnergy Energy;
    typedef Energy::Components Components;

    typedef detail::CLJParameterNames3D ParameterNames;

    typedef detail::CLJParameter Parameter;
    typedef SireFF::detail::AtomicParameters3D<Parameter> Parameters;
    
    typedef SireBase::PairMatrix<double> EnergyWorkspace;
    typedef SireBase::PairMatrix<SireMaths::DistVector> ForceWorkspace;

    typedef SireFF::detail::FFMolecule3D<InterCLJPotential> Molecule;
    typedef SireFF::detail::FFMolecules3D<InterCLJPotential> Molecules;

    InterCLJPotential();
    
    InterCLJPotential(const InterCLJPotential &other);
    
    ~InterCLJPotential();
    
    InterCLJPotential& operator=(const InterCLJPotential &other);

    static const char* typeName()
    {
        return "SireMM::InterCLJPotential";
    }

    const char* what() const
    {
        return InterCLJPotential::typeName();
    }
    
    static ParameterNames parameters()
    {
        return ParameterNames();
    }
    
    InterCLJPotential::Parameters 
    getParameters(const PartialMolecule &molecule,
                  const PropertyMap &map = PropertyMap());
    
    InterCLJPotential::Parameters
    updateParameters(const InterCLJPotential::Parameters &old_params,
                     const PartialMolecule &old_molecule,
                     const PartialMolecule &new_molecule,
                     const PropertyMap &map = PropertyMap());
                     
    InterCLJPotential::Parameters
    updateParameters(const InterCLJPotential::Parameters &old_params,
                     const PartialMolecule &old_molecule,
                     const PartialMolecule &new_molecule,
                     const PropertyMap &old_map, const PropertyMap &new_map);
    
    InterCLJPotential::Molecule
    parameterise(const PartialMolecule &molecule,
                 const PropertyMap &map = PropertyMap());
    
    InterCLJPotential::Molecules 
    parameterise(const MolGroup &molecules,
                 const PropertyMap &map = PropertyMap());

    void calculateEnergy(const InterCLJPotential::Molecule &mol0, 
                         const InterCLJPotential::Molecule &mol1,
                         InterCLJPotential::Energy &energy, 
                         InterCLJPotential::EnergyWorkspace &workspace,
                         double scale_energy=1) const;

    void calculateForce(const InterCLJPotential::Molecule &mol0, 
                        const InterCLJPotential::Molecule &mol1,
                        MolForceTable &forces0, 
                        MolForceTable &forces1,
                        InterCLJPotential::ForceWorkspace &workspace,
                        double scale_force=1) const;

    const Properties& properties() const;
    
    bool setProperty(const QString &name, const PropertyBase &value);

private:
    double totalCharge(const InterCLJPotential::Parameters::Array &params) const;

    void _pvt_calculateEnergy(const InterCLJPotential::Molecule &mol0, 
                              const InterCLJPotential::Molecule &mol1,
                              InterCLJPotential::Energy &energy, 
                              InterCLJPotential::EnergyWorkspace &workspace,
                              double scale_energy) const;

    void _pvt_calculateForce(const InterCLJPotential::Molecule &mol0, 
                             const InterCLJPotential::Molecule &mol1,
                             MolForceTable &forces0, 
                             MolForceTable &forces1,
                             InterCLJPotential::ForceWorkspace &workspace,
                             double scale_force) const;

    /** The current values of the properties of this functional */
    Properties props;
    
    /** The space in which this functional operates */
    Space spce;
    
    /** The nonbonded switching function */
    SwitchingFunction switchfunc;
    
    /** All possible LJ parameter pair combinations, arranged
        in a symmetric 2D array */
    LJPairMatrix ljpairs;
    
    /** Whether or not electrostatic potential shifting is used
        (this shifts the entire electrostatic potential so that it
        is zero at the cutoff distance) */
    bool use_electrostatic_shifting;
};

/** This class provides all of the functions and containers  
    necessary to provide an interface to calculate the
    intramolecular interatomic potentials using the Coulomb 
    and Lennard Jones functions.
    
    This is a 3D potential class, namely it requires that
    the atoms possess 3D coordinates, thereby allowing this
    potential to also be used to calculate 3D forces on the atoms.
    
    @author Christopher Woods
*/
class SIREMM_EXPORT IntraCLJPotential
{

friend QDataStream& ::operator<<(QDataStream&, const IntraCLJPotential&);
friend QDataStream& ::operator>>(QDataStream&, IntraCLJPotential&);

public:
    typedef detail::CLJEnergy Energy;
    typedef Energy::Components Components;
    
    typedef detail::ScaledCLJParameterNames3D ParameterNames;

    typedef detail::CLJParameter Parameter;
    
    typedef detail::IntraScaledAtomicParameters<
                  SireFF::detail::AtomicParameters3D<Parameter>,
                  detail::IntraScaledParameters<CLJNBPairs> > Parameters;
        
    typedef SireBase::PairMatrix<double> EnergyWorkspace;
    typedef SireBase::PairMatrix<SireMaths::DistVector> ForceWorkspace;

    typedef SireFF::detail::FFMolecule3D<IntraCLJPotential> Molecule;
    typedef SireFF::detail::FFMolecules3D<IntraCLJPotential> Molecules;

    IntraCLJPotential();
    
    IntraCLJPotential(const IntraCLJPotential &other);
    
    ~IntraCLJPotential();
    
    IntraCLJPotential& operator=(const IntraCLJPotential &other);
    
    static const char* typeName()
    {
        return "SireMM::IntraCLJPotential";
    }
    
    const char* what() const
    {
        return IntraCLJPotential::typeName();
    }
    
    IntraCLJPotential::Parameters 
    getParameters(const PartialMolecule &molecule,
                  const PropertyMap &map = PropertyMap());
    
    IntraCLJPotential::Parameters
    updateParameters(const IntraCLJPotential::Parameters &old_params,
                     const PartialMolecule &old_molecule,
                     const PartialMolecule &new_molecule,
                     const PropertyMap &map = PropertyMap());
                     
    IntraCLJPotential::Parameters
    updateParameters(const IntraCLJPotential::Parameters &old_params,
                     const PartialMolecule &old_molecule,
                     const PartialMolecule &new_molecule,
                     const PropertyMap &old_map, const PropertyMap &new_map);
    
    IntraCLJPotential::Molecule
    parameterise(const PartialMolecule &molecule,
                 const PropertyMap &map = PropertyMap());
    
    IntraCLJPotential::Molecules 
    parameterise(const MolGroup &molecules,
                 const PropertyMap &map = PropertyMap());

    void calculateEnergy(const IntraCLJPotential::Molecule &mol, 
                         IntraCLJPotential::Energy &energy,
                         IntraCLJPotential::EnergyWorkspace &workspace,
                         double scale_energy=1) const;

    void calculateForce(const IntraCLJPotential::Molecule &mol, 
                        MolForceTable &forces,
                        IntraCLJPotential::ForceWorkspace &workspace,
                        double scale_force=1) const;

private:
    double totalCharge(const IntraCLJPotential::Parameters::Array &params) const;

    /** The current values of the properties of this functional */
    Properties props;
    
    /** The space in which this functional operates */
    Space spce;
    
    /** The nonbonded switching function */
    SwitchingFunction switchfunc;
    
    /** All possible LJ parameter pair combinations, arranged
        in a symmetric 2D array */
    LJPairMatrix ljpairs;
    
    /** Whether or not electrostatic potential shifting is used
        (this shifts the entire electrostatic potential so that it
        is zero at the cutoff distance) */
    bool use_electrostatic_shifting;
};

/** Calculate the coulomb and LJ energy between the passed pair
    of molecules and add these energies onto 'energy'. This uses
    the passed workspace to perform the calculation */
inline void 
InterCLJPotential::calculateEnergy(const InterCLJPotential::Molecule &mol0,
                                   const InterCLJPotential::Molecule &mol1,
                                   InterCLJPotential::Energy &energy,
                                   InterCLJPotential::EnergyWorkspace &workspace,
                                   double scale_energy) const
{
    if (scale_energy != 0 and not spce->beyond(switchfunc->cutoffDistance(),
                                               mol0.aaBox(), mol1.aaBox()))
    {
        this->_pvt_calculateEnergy(mol0, mol1, energy, workspace, scale_energy);
    }
}

/** Calculate the coulomb and LJ forces on the atoms between the passed pair
    of molecules and add these energies onto 'forces'. This uses
    the passed workspace to perform the calculation */
inline void 
InterCLJPotential::calculateForce(const InterCLJPotential::Molecule &mol0, 
                                  const InterCLJPotential::Molecule &mol1,
                                  MolForceTable &forces0, 
                                  MolForceTable &forces1,
                                  InterCLJPotential::ForceWorkspace &workspace,
                                  double scale_force) const
{
    if ( scale_force != 0 and not spce->beyond(switchfunc->cutoffDistance(),
                                               mol0.aaBox(), mol1.aaBox()) )
    {
        this->_pvt_calculateForce(mol0, mol1, forces0, forces1,
                                  workspace, scale_force);
    }
}

}

Q_DECLARE_TYPEINFO( SireMM::detail::CLJParameter, Q_MOVABLE_TYPE );

SIRE_END_HEADER

#endif
