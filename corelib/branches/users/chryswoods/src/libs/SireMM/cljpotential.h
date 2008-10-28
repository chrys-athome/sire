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

#include "coulombpotential.h"
#include "ljpotential.h"

SIRE_BEGIN_HEADER

namespace SireMM
{
class InterCLJPotential;
class IntraCLJPotential;
class CLJPotential;

template<class CLJPot>
class CLJPotentialInterface;

namespace detail{ class CLJParameter; }
}

QDataStream& operator<<(QDataStream&, const SireMM::InterCLJPotential&);
QDataStream& operator>>(QDataStream&, SireMM::InterCLJPotential&);

QDataStream& operator<<(QDataStream&, const SireMM::IntraCLJPotential&);
QDataStream& operator>>(QDataStream&, SireMM::IntraCLJPotential&);

QDataStream& operator<<(QDataStream&, const SireMM::CLJPotential&);
QDataStream& operator>>(QDataStream&, SireMM::CLJPotential&);

QDataStream& operator<<(QDataStream&, const SireMM::detail::CLJParameter&);
QDataStream& operator>>(QDataStream&, SireMM::detail::CLJParameter&);

template<class CLJPot>
QDataStream& operator<<(QDataStream&, const SireMM::CLJPotentialInterface<CLJPot>&);
template<class CLJPot>
QDataStream& operator>>(QDataStream&, SireMM::CLJPotentialInterface<CLJPot>&);

namespace SireMol
{
class PartialMolecule;
class MoleculeGroup;
}

namespace SireMM
{

using SireBase::Properties;
using SireBase::Property;
using SireBase::PropertyMap;
using SireBase::PropertyName;
using SireBase::Property;

using SireCAS::Symbol;

using SireMaths::Vector;

using SireVol::Space;
using SireVol::Space;
using SireVol::CoordGroup;

using SireMol::PartialMolecule;
using SireMol::MoleculeGroup;

using SireFF::MolForceTable;

using SireMol::AtomCharges;

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
    CLJParameterNames3D() : CLJParameterNames(),
                            SireFF::detail::Coords3DParameterName()
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

namespace detail
{

/** This class holds the CLJ parameter used by both the Inter- and Intra-
    CLJPotentials. It is just the charge of the atom in reduced units
    (atomic charge divided by sqrt(4 pi eps_0)) and the ID of the LJ
    parameter in the singleton LJParameterDB database
    
    @author Christopher Woods
*/
class SIREMM_EXPORT CLJParameter : public ChargeParameter,
                                   public LJParamID
{
public:
    CLJParameter(double charge=0, quint32 ljid=0)
          : ChargeParameter(charge), LJParamID(ljid)
    {}
    
    CLJParameter(const CLJParameter &other)
          : ChargeParameter(other), LJParamID(other)
    {}
    
    ~CLJParameter()
    {}
    
    bool operator==(const CLJParameter &other) const
    {
        return LJParamID::operator==(other) and
               ChargeParameter::operator==(other);
    }
    
    bool operator!=(const CLJParameter &other) const
    {
        return LJParamID::operator!=(other) or
               ChargeParameter::operator!=(other);
    }
};

} // end of namespace detail

/** This is the common base class of InterCLJPotential and IntraCLJPotential

    @author Christopher Woods
*/
class SIREMM_EXPORT CLJPotential
{

friend QDataStream& ::operator<<(QDataStream&, const CLJPotential&);
friend QDataStream& ::operator>>(QDataStream&, CLJPotential&);

public:
    virtual ~CLJPotential();

    const Properties& properties() const;
    const Property& property(const QString &name) const;
    bool containsProperty(const QString &name) const;
    
    bool setProperty(const QString &name, const Property &value);

    bool setSpace(const Space &new_space);
    bool setSwitchingFunction(const SwitchingFunction &new_switchfunc);
    bool setShiftElectrostatics(bool switchelectro);
    bool setCombiningRules(const QString &combiningrules);
    
    const Space& space() const;
    const SwitchingFunction& switchingFunction() const;
    bool shiftElectrostatics() const;
    const QString& combiningRules() const;

protected:
    CLJPotential();
    CLJPotential(const CLJPotential &other);
    
    CLJPotential& operator=(const CLJPotential &other);

    void startEvaluation();
    void finishedEvaluation();
    
    virtual void changedPotential()=0;
    
    /** All possible LJ parameter pair combinations, arranged
        in a symmetric 2D array */
    LJPairMatrix ljpairs;

    /** The current values of the properties of this functional */
    Properties props;
    
    /** The space in which this functional operates */
    SpacePtr spce;
    
    /** The nonbonded switching function */
    SwitchFuncPtr switchfunc;

    /** The combining rules to use to get mixed LJ parameters */
    LJParameterDB::CombiningRules combining_rules;
    
    /** Whether or not the LJ pair matrix needs to be rebuilt */
    bool need_update_ljpairs;
    
    /** Whether or not electrostatic potential shifting is used
        (this shifts the entire electrostatic potential so that it
        is zero at the cutoff distance) */
    bool use_electrostatic_shifting;
};

/** This class provides all of the functions and containers  
    necessary to provide an interface to calculate the
    intermolecular interatomic potentials using the Coulomb 
    and Lennard Jones functions.
    
    This is a 3D potential class, namely it requires that
    the atoms possess 3D coordinates, thereby allowing this
    potential to also be used to calculate 3D forces on the atoms.

    This potential has the following properties (parameters);
    
    (1) space               : This is the 3D space in which the molecules exist
    (2) switchingFunction   : This is the switching function used to scale the 
                              energies / forces to zero at the cutoff
    (3) shiftElectrostatics : This is a boolean - if it is true then the 
                              group-group electrostatic interactions are scaled 
                              so that they are zero at the cutoff
    (4) combiningRules      : This is a string specifying the LJ combining rules
                              (currently "arithmetic" or "geometric")
                              
    The molecules used with this potential must contain the following 
    properties (defined in parameters(), e.g. parameters().coordinates())
    
    (1) .coordinates()      : These are the 3D coordinates of each atom, must be
                              a property of type AtomCoords
    (2) .charge()           : These are the partial charges of each atom, must
                              be a property of type AtomCharges
    (3) .lj()               : These are the LJ parameters for each atom, must
                              be a property of type AtomLJs
    
    @author Christopher Woods
*/
class SIREMM_EXPORT InterCLJPotential : public CLJPotential
{

friend QDataStream& ::operator<<(QDataStream&, const InterCLJPotential&);
friend QDataStream& ::operator>>(QDataStream&, InterCLJPotential&);

public:

    typedef CLJEnergy Energy;
    typedef Energy::Components Components;

    typedef CLJParameterNames3D ParameterNames;

    typedef detail::CLJParameter Parameter;
    typedef SireFF::detail::AtomicParameters3D<Parameter> Parameters;
    
    typedef SireBase::PairMatrix<double> EnergyWorkspace;
    typedef SireBase::PairMatrix<SireMaths::DistVector> ForceWorkspace;

    typedef SireFF::detail::FFMolecule3D<InterCLJPotential> Molecule;
    typedef SireFF::detail::FFMolecules3D<InterCLJPotential> Molecules;

    typedef SireFF::detail::ChangedMolecule<Molecule> ChangedMolecule;

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
    parameterise(const MoleculeGroup &molecules,
                 const PropertyMap &map = PropertyMap());

    void calculateEnergy(const InterCLJPotential::Molecule &mol0, 
                         const InterCLJPotential::Molecule &mol1,
                         InterCLJPotential::Energy &energy, 
                         InterCLJPotential::EnergyWorkspace &workspace,
                         double scale_energy=1) const;

    void calculateForce(const InterCLJPotential::Molecule &mol0, 
                        const InterCLJPotential::Molecule &mol1,
                        MolForceTable &forces0,
                        InterCLJPotential::ForceWorkspace &workspace,
                        double scale_force=1) const;

    void calculateForce(const InterCLJPotential::Molecule &mol0,
                        const InterCLJPotential::Molecule &mol1,
                        MolForceTable &forces0,
                        const Symbol &symbol,
                        const Components &components,
                        InterCLJPotential::ForceWorkspace &workspace,
                        double scale_force=1) const;

    void calculateCoulombForce(const InterCLJPotential::Molecule &mol0, 
                               const InterCLJPotential::Molecule &mol1,
                               MolForceTable &forces0,
                               InterCLJPotential::ForceWorkspace &workspace,
                               double scale_force=1) const;

    void calculateLJForce(const InterCLJPotential::Molecule &mol0, 
                          const InterCLJPotential::Molecule &mol1,
                          MolForceTable &forces0,
                          InterCLJPotential::ForceWorkspace &workspace,
                          double scale_force=1) const;

private:
    double totalCharge(const InterCLJPotential::Parameters::Array &params) const;

    void throwMissingForceComponent(const Symbol &symbol,
                                    const Components &components) const;

    void _pvt_calculateEnergy(const InterCLJPotential::Molecule &mol0, 
                              const InterCLJPotential::Molecule &mol1,
                              InterCLJPotential::Energy &energy, 
                              InterCLJPotential::EnergyWorkspace &workspace,
                              double scale_energy) const;

    void _pvt_calculateForce(const InterCLJPotential::Molecule &mol0, 
                             const InterCLJPotential::Molecule &mol1,
                             MolForceTable &forces0, 
                             InterCLJPotential::ForceWorkspace &workspace,
                             double scale_force) const;

    void _pvt_calculateCoulombForce(const InterCLJPotential::Molecule &mol0, 
                                    const InterCLJPotential::Molecule &mol1,
                                    MolForceTable &forces0, 
                                    InterCLJPotential::ForceWorkspace &workspace,
                                    double scale_force) const;

    void _pvt_calculateLJForce(const InterCLJPotential::Molecule &mol0, 
                               const InterCLJPotential::Molecule &mol1,
                               MolForceTable &forces0, 
                               InterCLJPotential::ForceWorkspace &workspace,
                               double scale_force) const;
};

/** This class provides all of the functions and containers  
    necessary to provide an interface to calculate the
    intramolecular interatomic potentials using the Coulomb 
    and Lennard Jones functions.
    
    This is a 3D potential class, namely it requires that
    the atoms possess 3D coordinates, thereby allowing this
    potential to also be used to calculate 3D forces on the atoms.

    This potential has the following properties (parameters);
    
    (1) space               : This is the 3D space in which the molecules exist
    (2) switchingFunction   : This is the switching function used to scale the 
                              energies / forces to zero at the cutoff
    (3) shiftElectrostatics : This is a boolean - if it is true then the 
                              group-group electrostatic interactions are scaled 
                              so that they are zero at the cutoff
    (4) combiningRules      : This is a string specifying the LJ combining rules
                              (currently "arithmetic" or "geometric")
                              
    The molecules used with this potential must contain the following 
    properties (defined in parameters(), e.g. parameters().coordinates())
    
    (1) .coordinates()       : These are the 3D coordinates of each atom, must be
                               a property of type AtomCoords
    (2) .charge()            : These are the partial charges of each atom, must
                               be a property of type AtomCharges
    (3) .lj()                : These are the LJ parameters for each atom, must
                               be a property of type AtomLJs
    (4) .intraScaleFactors() : These are the intramolecular atomic scaling factors,
                               used to scale the intramolecular coulomb and LJ
                               energies, must be a property of type CLJNBPairs
    
    @author Christopher Woods
*/
class SIREMM_EXPORT IntraCLJPotential : public CLJPotential
{

friend QDataStream& ::operator<<(QDataStream&, const IntraCLJPotential&);
friend QDataStream& ::operator>>(QDataStream&, IntraCLJPotential&);

public:
    typedef CLJEnergy Energy;
    typedef Energy::Components Components;
    
    typedef ScaledCLJParameterNames3D ParameterNames;

    typedef detail::CLJParameter Parameter;
    
    typedef detail::IntraScaledAtomicParameters<
                  SireFF::detail::AtomicParameters3D<Parameter>,
                  detail::IntraScaledParameters<CLJNBPairs> > Parameters;
        
    typedef SireBase::PairMatrix<double> EnergyWorkspace;
    typedef SireBase::PairMatrix<SireMaths::DistVector> ForceWorkspace;

    typedef SireFF::detail::FFMolecule3D<IntraCLJPotential> Molecule;
    typedef SireFF::detail::FFMolecules3D<IntraCLJPotential> Molecules;

    typedef SireFF::detail::ChangedMolecule<Molecule> ChangedMolecule;

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
    
    static ParameterNames parameters()
    {
        return ParameterNames();
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
    parameterise(const MoleculeGroup &molecules,
                 const PropertyMap &map = PropertyMap());

    void calculateEnergy(const IntraCLJPotential::Molecule &mol, 
                         IntraCLJPotential::Energy &energy,
                         IntraCLJPotential::EnergyWorkspace &workspace,
                         double scale_energy=1) const;

    void calculateEnergy(const IntraCLJPotential::Molecule &mol,
                         const IntraCLJPotential::Molecule &rest_of_mol,
                         IntraCLJPotential::Energy &energy,
                         IntraCLJPotential::EnergyWorkspace &workspace,
                         double scale_energy=1) const;

    void calculateForce(const IntraCLJPotential::Molecule &mol, 
                        MolForceTable &forces,
                        IntraCLJPotential::ForceWorkspace &workspace,
                        double scale_force=1) const;

    void calculateForce(const IntraCLJPotential::Molecule &mol,
                        const IntraCLJPotential::Molecule &rest_of_mol,
                        MolForceTable &forces,
                        IntraCLJPotential::ForceWorkspace &workspace,
                        double scale_force=1) const;

    void calculateForce(const IntraCLJPotential::Molecule &mol, 
                        MolForceTable &forces,
                        const Symbol &symbol,
                        const Components &components,
                        IntraCLJPotential::ForceWorkspace &workspace,
                        double scale_force=1) const;

    void calculateForce(const IntraCLJPotential::Molecule &mol,
                        const IntraCLJPotential::Molecule &rest_of_mol,
                        MolForceTable &forces,
                        const Symbol &symbol,
                        const Components &components,
                        IntraCLJPotential::ForceWorkspace &workspace,
                        double scale_force=1) const;

    void calculateCoulombForce(const IntraCLJPotential::Molecule &mol,
                               MolForceTable &forces,
                               IntraCLJPotential::ForceWorkspace &workspace,
                               double scale_force=1) const;

    void calculateCoulombForce(const IntraCLJPotential::Molecule &mol,
                               const IntraCLJPotential::Molecule &rest_of_mol,
                               MolForceTable &forces,
                               IntraCLJPotential::ForceWorkspace &workspace,
                               double scale_force=1) const;
                               
    void calculateLJForce(const IntraCLJPotential::Molecule &mol,
                          const IntraCLJPotential::Molecule &rest_of_mol,
                          MolForceTable &forces,
                          IntraCLJPotential::ForceWorkspace &workspace,
                          double scale_force=1) const;
                               
    void calculateLJForce(const IntraCLJPotential::Molecule &mol,
                          MolForceTable &forces,
                          IntraCLJPotential::ForceWorkspace &workspace,
                          double scale_force=1) const;
                               
private:
    double totalCharge(const IntraCLJPotential::Parameters::Array &params) const;

    void assertCompatible(const IntraCLJPotential::Molecule &mol,
                          const IntraCLJPotential::Molecule &rest_of_mol) const;

    void calculateEnergy(const CLJNBPairs::CGPairs &group_pairs,
                         IntraCLJPotential::EnergyWorkspace &workspace,
                         const Parameter *params0_array,
                         const Parameter *params1_array,
                         const quint32 nats0, const quint32 nats1,
                         double &icnrg, double &iljnrg) const;

    void calculateEnergy(const CLJNBPairs::CGPairs &group_pairs,
                         const QSet<SireID::Index> &atoms0, 
                         const QSet<SireID::Index> &atoms1,
                         IntraCLJPotential::EnergyWorkspace &workspace,
                         const Parameter *params0_array,
                         const Parameter *params1_array,
                         const quint32 nats0, const quint32 nats1,
                         double &icnrg, double &iljnrg) const;

    void calculateForce(const CLJNBPairs::CGPairs &group_pairs,
                        const CoordGroup &group0, const CoordGroup &group1,
                        const double mindist,
                        IntraCLJPotential::ForceWorkspace &workspace,
                        const IntraCLJPotential::Parameter *params0_array,
                        const IntraCLJPotential::Parameter *params1_array,
                        const quint32 nats0, const quint32 nats1,
                        const double shift_coul,
                        Vector *group_forces0_array,
                        const double scale_force) const;

    void calculateForce(const CLJNBPairs::CGPairs &group_pairs,
                        const QSet<SireID::Index> &atoms0,
                        const QSet<SireID::Index> &atoms1,
                        const CoordGroup &group0, const CoordGroup &group1,
                        const double mindist,
                        IntraCLJPotential::ForceWorkspace &workspace,
                        const IntraCLJPotential::Parameter *params0_array,
                        const IntraCLJPotential::Parameter *params1_array,
                        const double shift_coul,
                        Vector *group_forces0_array,
                        const double scale_force) const;

    void calculateCoulombForce(const CLJNBPairs::CGPairs &group_pairs,
                               const CoordGroup &group0, const CoordGroup &group1,
                               const double mindist,
                               IntraCLJPotential::ForceWorkspace &workspace,
                               const IntraCLJPotential::Parameter *params0_array,
                               const IntraCLJPotential::Parameter *params1_array,
                               const quint32 nats0, const quint32 nats1,
                               const double shift_coul,
                               Vector *group_forces0_array,
                               const double scale_force) const;

    void calculateCoulombForce(const CLJNBPairs::CGPairs &group_pairs,
                               const QSet<SireID::Index> &atoms0,
                               const QSet<SireID::Index> &atoms1,
                               const CoordGroup &group0, const CoordGroup &group1,
                               const double mindist,
                               IntraCLJPotential::ForceWorkspace &workspace,
                               const IntraCLJPotential::Parameter *params0_array,
                               const IntraCLJPotential::Parameter *params1_array,
                               const double shift_coul,
                               Vector *group_forces0_array,
                               const double scale_force) const;

    void calculateLJForce(const CLJNBPairs::CGPairs &group_pairs,
                          const CoordGroup &group0, const CoordGroup &group1,
                          const double mindist,
                          IntraCLJPotential::ForceWorkspace &workspace,
                          const IntraCLJPotential::Parameter *params0_array,
                          const IntraCLJPotential::Parameter *params1_array,
                          const quint32 nats0, const quint32 nats1,
                          Vector *group_forces0_array,
                          const double scale_force) const;

    void calculateLJForce(const CLJNBPairs::CGPairs &group_pairs,
                          const QSet<SireID::Index> &atoms0,
                          const QSet<SireID::Index> &atoms1,
                          const CoordGroup &group0, const CoordGroup &group1,
                          const double mindist,
                          IntraCLJPotential::ForceWorkspace &workspace,
                          const IntraCLJPotential::Parameter *params0_array,
                          const IntraCLJPotential::Parameter *params1_array,
                          Vector *group_forces0_array,
                          const double scale_force) const;

    void throwMissingForceComponent(const Symbol &symbol,
                                    const Components &components) const;
};

/** This small class is used to hide most of the public interfaces of the 
    CLJPotential derived class, so that only the property-related functions
    are publically available. This provides a better interface when deriving
    a full forcefield class from a CLJ potential.
    
    @author Christopher Woods
*/
template<class CLJPot>
class CLJPotentialInterface : protected CLJPot
{

friend QDataStream& ::operator<<<>(QDataStream&, const CLJPotentialInterface<CLJPot>&);
friend QDataStream& ::operator>><>(QDataStream&, CLJPotentialInterface<CLJPot>&);

public:
    CLJPotentialInterface() : CLJPot()
    {}
    
    CLJPotentialInterface(const CLJPotentialInterface &other) : CLJPot(other)
    {}
    
    ~CLJPotentialInterface()
    {}
    
    static typename CLJPot::ParameterNames parameters()
    {
        return CLJPot::parameters();
    }
    
    const Properties& properties() const
    {
        return CLJPot::properties();
    }
    
    const Property& property(const QString &name) const
    {
        return CLJPot::property(name);
    }
    
    bool containsProperty(const QString &name) const
    {
        return CLJPot::containsProperty(name);
    }
    
    bool setProperty(const QString &name, const Property &value)
    {
        return CLJPot::setProperty(name, value);
    }

    bool setSpace(const Space &new_space)
    {
        return CLJPot::setSpace(new_space);
    }
    
    bool setSwitchingFunction(const SwitchingFunction &new_switchfunc)
    {
        return CLJPot::setSwitchingFunction(new_switchfunc);
    }
    
    bool setShiftElectrostatics(bool switchelectro)
    {
        return CLJPot::setShiftElectrostatics(switchelectro);
    }
    
    bool setCombiningRules(const QString &combiningrules)
    {
        return CLJPot::setCombiningRules(combiningrules);
    }
    
    const Space& space() const
    {
        return CLJPot::space();
    }
    
    const SwitchingFunction& switchingFunction() const
    {
        return CLJPot::switchingFunction();
    }

    bool shiftElectrostatics() const
    {
        return CLJPot::shiftElectrostatics();
    }
    
    const QString& combiningRules() const
    {
        return CLJPot::combiningRules();
    }
};

#ifndef SIRE_SKIP_INLINE_FUNCTIONS

//////
////// Inline functions of InterCLJPotential
//////

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
    if (scale_energy != 0)// and 
       // not (mol0.isEmpty() or mol1.isEmpty()))
    {
        this->_pvt_calculateEnergy(mol0, mol1, energy, workspace, scale_energy);
    }
}

/** Calculate the coulomb and LJ forces on the atoms between the passed pair
    of molecules and add the forces on 'mol0' onto 'forces'. This uses
    the passed workspace to perform the calculation. The forces
    are scaled by the optional 'scaled_forces' */
inline void 
InterCLJPotential::calculateForce(const InterCLJPotential::Molecule &mol0, 
                                  const InterCLJPotential::Molecule &mol1,
                                  MolForceTable &forces0, 
                                  InterCLJPotential::ForceWorkspace &workspace,
                                  double scale_force) const
{
    if ( scale_force != 0 and 
         not (mol0.isEmpty() or mol1.isEmpty()) and
         not spce->beyond(switchfunc->cutoffDistance(),
                          mol0.aaBox(), mol1.aaBox()) )
    {
        this->_pvt_calculateForce(mol0, mol1, forces0,
                                  workspace, scale_force);
    }
}

/** Calculate the coulomb forces on the atoms between the passed pair
    of molecules and add the forces on 'mol0' onto 'forces'. This uses
    the passed workspace to perform the calculation. The forces
    are scaled by the optional 'scaled_forces' */
inline void 
InterCLJPotential::calculateCoulombForce(const InterCLJPotential::Molecule &mol0, 
                                         const InterCLJPotential::Molecule &mol1,
                                         MolForceTable &forces0, 
                                         InterCLJPotential::ForceWorkspace &workspace,
                                         double scale_force) const
{
    if ( scale_force != 0 and 
         not (mol0.isEmpty() or mol1.isEmpty()) and
         not spce->beyond(switchfunc->cutoffDistance(),
                          mol0.aaBox(), mol1.aaBox()) )
    {
        this->_pvt_calculateCoulombForce(mol0, mol1, forces0,
                                         workspace, scale_force);
    }
}

/** Calculate the LJ forces on the atoms between the passed pair
    of molecules and add the forces on 'mol0' onto 'forces'. This uses
    the passed workspace to perform the calculation. The forces
    are scaled by the optional 'scaled_forces' */
inline void 
InterCLJPotential::calculateLJForce(const InterCLJPotential::Molecule &mol0, 
                                    const InterCLJPotential::Molecule &mol1,
                                    MolForceTable &forces0, 
                                    InterCLJPotential::ForceWorkspace &workspace,
                                    double scale_force) const
{
    if ( scale_force != 0 and 
         not (mol0.isEmpty() or mol1.isEmpty()) and
         not spce->beyond(switchfunc->cutoffDistance(),
                          mol0.aaBox(), mol1.aaBox()) )
    {
        this->_pvt_calculateLJForce(mol0, mol1, forces0,
                                    workspace, scale_force);
    }
}

/** Calculate the component of the force represented by 'symbol' between the 
    passed pair of molecules, and add the forces on 'mol0' onto 'forces0'.
    This uses the passed workspace to perform the calculation. The forces
    are scaled by the optional 'scaled_forces' */
inline void 
InterCLJPotential::calculateForce(const InterCLJPotential::Molecule &mol0,
                                  const InterCLJPotential::Molecule &mol1,
                                  MolForceTable &forces0,
                                  const Symbol &symbol,
                                  const InterCLJPotential::Components &components,
                                  InterCLJPotential::ForceWorkspace &workspace,
                                  double scale_force) const
{
    if (symbol == components.total())
        this->calculateForce(mol0, mol1, forces0, workspace, scale_force);
       
    else if (symbol == components.coulomb())
        this->calculateCoulombForce(mol0, mol1, forces0, workspace, scale_force);
        
    else if (symbol == components.lj())
        this->calculateLJForce(mol0, mol1, forces0, workspace, scale_force);
        
    else
        throwMissingForceComponent(symbol, components);
}

//////
////// Inline functions of IntraCLJPotential
//////

/** Calculate the forces represented by the symbol 'symbol' between the 
    atoms in the molecule 'mol' and add these forces onto 'forces'. This uses
    the passed workspace to perform the calculation */
inline void 
IntraCLJPotential::calculateForce(const IntraCLJPotential::Molecule &mol, 
                                  MolForceTable &forces,
                                  const Symbol &symbol,
                                  const IntraCLJPotential::Components &components,
                                  IntraCLJPotential::ForceWorkspace &workspace,
                                  double scale_force) const
{
    if (symbol == components.total())
        this->calculateForce(mol, forces, workspace, scale_force);
        
    else if (symbol == components.coulomb())
        this->calculateCoulombForce(mol, forces, workspace, scale_force);
        
    else if (symbol == components.lj())
        this->calculateLJForce(mol, forces, workspace, scale_force);
        
    else
        throwMissingForceComponent(symbol, components);
}

/** Calculate the forces represented by the symbol 'symbol' acting on the 
    atoms in 'mol1' caused by the atoms in the rest of the same molecule 
    in 'rest_of_mol', and add these forces onto 'forces'. This uses the 
    passed workspace to perform the calculation
    
    \throw SireError::incompatible_error
*/
inline void
IntraCLJPotential::calculateForce(const IntraCLJPotential::Molecule &mol,
                                  const IntraCLJPotential::Molecule &rest_of_mol,
                                  MolForceTable &forces,
                                  const Symbol &symbol,
                                  const IntraCLJPotential::Components &components,
                                  IntraCLJPotential::ForceWorkspace &workspace,
                                  double scale_force) const
{
    if (symbol == components.total())
        this->calculateForce(mol, rest_of_mol, forces, workspace, scale_force);
        
    else if (symbol == components.coulomb())
        this->calculateCoulombForce(mol, rest_of_mol, forces, workspace, scale_force);
        
    else if (symbol == components.lj())
        this->calculateLJForce(mol, rest_of_mol, forces, workspace, scale_force);
        
    else
        throwMissingForceComponent(symbol, components);
}

#endif //SIRE_SKIP_INLINE_FUNCTIONS

}

QDataStream& operator<<(QDataStream&, const SireMM::detail::CLJParameter&);
QDataStream& operator>>(QDataStream&, SireMM::detail::CLJParameter&);

template<class CLJPot>
QDataStream& operator<<(QDataStream &ds,
                        const SireMM::CLJPotentialInterface<CLJPot> &cljpot)
{
    ds << static_cast<const CLJPot&>(cljpot);
    return ds;
}

template<class CLJPot>
QDataStream& operator>>(QDataStream &ds,
                        SireMM::CLJPotentialInterface<CLJPot> &cljpot)
{
    ds >> static_cast<CLJPot&>(cljpot);
    return ds;
}

Q_DECLARE_TYPEINFO( SireMM::detail::CLJParameter, Q_MOVABLE_TYPE );

SIRE_EXPOSE_CLASS( SireMM::CLJParameterNames )
SIRE_EXPOSE_CLASS( SireMM::CLJParameterNames3D )
SIRE_EXPOSE_CLASS( SireMM::ScaledCLJParameterNames3D )

SIRE_END_HEADER

#endif
