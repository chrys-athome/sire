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

#ifndef SIREMM_SOFTCLJPOTENTIAL_H
#define SIREMM_SOFTCLJPOTENTIAL_H

#include "cljpotential.h"
#include "softcljcomponent.h"

SIRE_BEGIN_HEADER

namespace SireMM
{
class SoftCLJPotential;
class InterSoftCLJPotential;
}

QDataStream& operator<<(QDataStream&, const SireMM::SoftCLJPotential&);
QDataStream& operator>>(QDataStream&, SireMM::SoftCLJPotential&);

QDataStream& operator<<(QDataStream&, const SireMM::InterSoftCLJPotential&);
QDataStream& operator>>(QDataStream&, SireMM::InterSoftCLJPotential&);

namespace SireMM
{

/** This class provides a Coulomb and Lennard Jones potential that 
    can be softened (using the soft-core function as described in
    Zacharias and McCammon, J. Chem. Phys., 1994, and also,
    Michel et al., JCTC, 2007)
    
    @author Christopher Woods
*/
class SIREMM_EXPORT SoftCLJPotential : public CLJPotential
{

friend QDataStream& ::operator<<(QDataStream&, const SoftCLJPotential&);
friend QDataStream& ::operator>>(QDataStream&, SoftCLJPotential&);

public:
    ~SoftCLJPotential();
    
    bool setShiftDelta(double delta);
    bool setCoulombPower(int power);
    bool setLJPower(int power);
    
    bool setProperty(const QString &name, const Property &value);
    
    double alpha(int i) const;

    bool setAlpha(double alpha);
    bool setAlpha(int i, double alpha);
    
    bool removeAlpha(int i);
    
    void clearAlphas();
    
    double shiftDelta() const;
    int coulombPower() const;
    int ljPower() const;
    
protected:
    SoftCLJPotential();
    SoftCLJPotential(const SoftCLJPotential &other);
    
    SoftCLJPotential& operator=(const SoftCLJPotential &other);
    
    /** The array of unique alpha values - alpha = 0 gives the 
        true Coulomb and LJ potential, while alpha > 0 gives
        the (increasingly) softened potential  */
    QVector<double> alpha_values;
    
    /** The index mapping from the index of the alpha component
        to the actual unique alpha value - this is used to make
        sure that we only internally store unique alpha values */
    QVector<int> alpha_index;
    
    /** The value of delta for the LJ shift function */
    double shift_delta;
    
    /** The coulomb power */
    quint32 coul_power;
    
    /** The LJ power */
    quint32 lj_power;
};

/** This class provides all of the functions and containers  
    necessary to provide an interface to calculate the
    intermolecular interatomic potentials using the soft Coulomb 
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
    (5) alpha               : The alpha scaling parameter used to soften the potential
    (6) coulombPower        : A parameter used to control the softening of coulomb terms
    (7) ljPower             : A parameter used to control the softening of LJ terms
                              
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
class SIREMM_EXPORT InterSoftCLJPotential : public SoftCLJPotential
{

friend QDataStream& ::operator<<(QDataStream&, const InterSoftCLJPotential&);
friend QDataStream& ::operator>>(QDataStream&, InterSoftCLJPotential&);

public:
    typedef SoftCLJEnergy Energy;
    typedef Energy::Components Components;

    typedef CLJParameterNames3D ParameterNames;

    typedef detail::CLJParameter Parameter;
    typedef SireFF::detail::AtomicParameters3D<Parameter> Parameters;
    
    typedef SireBase::PairMatrix<double> EnergyWorkspace;
    typedef SireBase::PairMatrix<SireMaths::DistVector> ForceWorkspace;

    typedef SireFF::detail::FFMolecule3D<InterSoftCLJPotential> Molecule;
    typedef SireFF::detail::FFMolecules3D<InterSoftCLJPotential> Molecules;

    typedef SireFF::detail::ChangedMolecule<Molecule> ChangedMolecule;

    InterSoftCLJPotential();
    
    InterSoftCLJPotential(const InterSoftCLJPotential &other);
    
    ~InterSoftCLJPotential();
    
    InterSoftCLJPotential& operator=(const InterSoftCLJPotential &other);

    static const char* typeName()
    {
        return "SireMM::InterSoftCLJPotential";
    }

    const char* what() const
    {
        return InterCLJPotential::typeName();
    }
    
    static ParameterNames parameters()
    {
        return ParameterNames();
    }
    
    InterSoftCLJPotential::Parameters 
    getParameters(const PartialMolecule &molecule,
                  const PropertyMap &map = PropertyMap());
    
    InterSoftCLJPotential::Parameters
    updateParameters(const InterSoftCLJPotential::Parameters &old_params,
                     const PartialMolecule &old_molecule,
                     const PartialMolecule &new_molecule,
                     const PropertyMap &map = PropertyMap());
                     
    InterSoftCLJPotential::Parameters
    updateParameters(const InterSoftCLJPotential::Parameters &old_params,
                     const PartialMolecule &old_molecule,
                     const PartialMolecule &new_molecule,
                     const PropertyMap &old_map, const PropertyMap &new_map);
    
    InterSoftCLJPotential::Molecule
    parameterise(const PartialMolecule &molecule,
                 const PropertyMap &map = PropertyMap());
    
    InterSoftCLJPotential::Molecules 
    parameterise(const MoleculeGroup &molecules,
                 const PropertyMap &map = PropertyMap());

    void calculateEnergy(const InterSoftCLJPotential::Molecule &mol0, 
                         const InterSoftCLJPotential::Molecule &mol1,
                         InterSoftCLJPotential::Energy &energy, 
                         InterSoftCLJPotential::EnergyWorkspace &workspace,
                         double scale_energy=1) const;

    void calculateForce(const InterSoftCLJPotential::Molecule &mol0, 
                        const InterSoftCLJPotential::Molecule &mol1,
                        MolForceTable &forces0,
                        InterSoftCLJPotential::ForceWorkspace &workspace,
                        double scale_force=1) const;

    void calculateForce(const InterSoftCLJPotential::Molecule &mol0,
                        const InterSoftCLJPotential::Molecule &mol1,
                        MolForceTable &forces0,
                        const Symbol &symbol,
                        const Components &components,
                        InterSoftCLJPotential::ForceWorkspace &workspace,
                        double scale_force=1) const;

    void calculateCoulombForce(const InterSoftCLJPotential::Molecule &mol0, 
                               const InterSoftCLJPotential::Molecule &mol1,
                               MolForceTable &forces0,
                               InterSoftCLJPotential::ForceWorkspace &workspace,
                               double scale_force=1) const;

    void calculateLJForce(const InterSoftCLJPotential::Molecule &mol0, 
                          const InterSoftCLJPotential::Molecule &mol1,
                          MolForceTable &forces0,
                          InterSoftCLJPotential::ForceWorkspace &workspace,
                          double scale_force=1) const;

private:
    double totalCharge(const InterSoftCLJPotential::Parameters::Array &params) const;

    void throwMissingForceComponent(const Symbol &symbol,
                                    const Components &components) const;

    void _pvt_calculateEnergy(const InterSoftCLJPotential::Molecule &mol0, 
                              const InterSoftCLJPotential::Molecule &mol1,
                              InterSoftCLJPotential::Energy &energy, 
                              InterSoftCLJPotential::EnergyWorkspace &workspace,
                              double scale_energy) const;

    void _pvt_calculateForce(const InterSoftCLJPotential::Molecule &mol0, 
                             const InterSoftCLJPotential::Molecule &mol1,
                             MolForceTable &forces0, 
                             InterSoftCLJPotential::ForceWorkspace &workspace,
                             double scale_force) const;

    void _pvt_calculateCoulombForce(const InterSoftCLJPotential::Molecule &mol0, 
                                    const InterSoftCLJPotential::Molecule &mol1,
                                    MolForceTable &forces0, 
                                    InterSoftCLJPotential::ForceWorkspace &workspace,
                                    double scale_force) const;

    void _pvt_calculateLJForce(const InterSoftCLJPotential::Molecule &mol0, 
                               const InterSoftCLJPotential::Molecule &mol1,
                               MolForceTable &forces0, 
                               InterSoftCLJPotential::ForceWorkspace &workspace,
                               double scale_force) const;
};

/** This small class is used to hide most of the public interfaces of the 
    SoftCLJPotential derived class, so that only the property-related functions
    are publically available. This provides a better interface when deriving
    a full forcefield class from a SoftCLJ potential.
    
    @author Christopher Woods
*/
template<class SoftCLJPot>
class SoftCLJPotentialInterface : public CLJPotentialInterface<SoftCLJPot>
{

public:
    SoftCLJPotentialInterface() : CLJPotentialInterface<SoftCLJPot>()
    {}
    
    SoftCLJPotentialInterface(const SoftCLJPotentialInterface<SoftCLJPot> &other) 
                : CLJPotentialInterface<SoftCLJPot>(other)
    {}
    
    ~SoftCLJPotentialInterface()
    {}
    
    bool setAlpha(double alpha)
    {
        return SoftCLJPot::setAlpha(alpha);
    }
    
    bool setShiftDelta(double delta)
    {
        return SoftCLJPot::setShiftDelta(delta);
    }
    
    bool setCoulombPower(int power)
    {
        return SoftCLJPot::setCoulombPower(power);
    }
    
    bool setLJPower(int power)
    {
        return SoftCLJPot::setLJPower(power);
    }
    
    double alpha() const
    {
        return SoftCLJPot::alpha();
    }
    
    double shiftDelta() const
    {
        return SoftCLJPot::shiftDelta();
    }
    
    int coulombPower() const
    {
        return SoftCLJPot::coulombPower();
    }
    
    int ljPower() const
    {
        return SoftCLJPot::ljPower();
    }
};

#ifndef SIRE_SKIP_INLINE_FUNCTIONS

//////
////// Inline functions of InterSoftCLJPotential
//////

/** Calculate the coulomb and LJ energy between the passed pair
    of molecules and add these energies onto 'energy'. This uses
    the passed workspace to perform the calculation */
inline void 
InterSoftCLJPotential::calculateEnergy(const InterSoftCLJPotential::Molecule &mol0,
                                       const InterSoftCLJPotential::Molecule &mol1,
                                       InterSoftCLJPotential::Energy &energy,
                                       InterSoftCLJPotential::EnergyWorkspace &workspace,
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
InterSoftCLJPotential::calculateForce(const InterSoftCLJPotential::Molecule &mol0, 
                                      const InterSoftCLJPotential::Molecule &mol1,
                                      MolForceTable &forces0, 
                                      InterSoftCLJPotential::ForceWorkspace &workspace,
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
InterSoftCLJPotential::calculateCoulombForce(
                                        const InterSoftCLJPotential::Molecule &mol0, 
                                        const InterSoftCLJPotential::Molecule &mol1,
                                        MolForceTable &forces0, 
                                        InterSoftCLJPotential::ForceWorkspace &workspace,
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
InterSoftCLJPotential::calculateLJForce(
                                    const InterSoftCLJPotential::Molecule &mol0, 
                                    const InterSoftCLJPotential::Molecule &mol1,
                                    MolForceTable &forces0, 
                                    InterSoftCLJPotential::ForceWorkspace &workspace,
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
InterSoftCLJPotential::calculateForce(
                                  const InterSoftCLJPotential::Molecule &mol0,
                                  const InterSoftCLJPotential::Molecule &mol1,
                                  MolForceTable &forces0,
                                  const Symbol &symbol,
                                  const InterSoftCLJPotential::Components &components,
                                  InterSoftCLJPotential::ForceWorkspace &workspace,
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

#endif

}

SIRE_END_HEADER

#endif
