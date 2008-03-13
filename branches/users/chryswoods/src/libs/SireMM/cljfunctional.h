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

#ifndef SIREMM_CLJFUNCTIONAL_H
#define SIREMM_CLJFUNCTIONAL_H

#include "SireBase/propertymap.h"
#include "SireBase/pairmatrix.hpp"
#include "SireBase/packedarray2d.hpp"

#include "cljpair.h"
#include "atomicpairs.h"
#include "cljcomponent.h"

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
}

namespace SireFF
{
class ForceTable;
}

namespace SireMM
{

using SireBase::PropertyMap;

using SireMol::PartialMolecule;

using SireFF::ForceTable;

/** This class provides all of the functions and containers  
    necessary to provide an interface to calculate the
    intermolecular interatomic potentials using the Coulomb 
    and Lennard Jones functions.
    
    @author Christopher Woods
*/
class SIREMM_EXPORT InterCLJPotential
{

friend QDataStream& ::operator<<(QDataStream&, const CLJFunctional&);
friend QDataStream& ::operator>>(QDataStream&, CLJFunctional&);

public:

    typedef detail::CLJEnergy Energy;
    typedef Energy::Components Components;

    typedef detail::CLJParameterNames3D ParameterNames;

    typedef detail::CLJParameter Parameter;
    typedef SireFF::detail::AtomicParameters3D<Parameter> Parameters;
        
    typedef SireBase::PairMatrix<double> Workspace;

    typedef SireFF::detail::FFMolecule3D<Parameters> Molecule;

    CLJFunctional();
    
    CLJFunctional(const CLJFunctional &other);
    
    ~CLJFunctional();
    
    CLJFunctional& operator=(const CLJFunctional &other);

    static const char* typeName()
    {
        return "SireMM::CLJFunctional";
    }

    const char* what() const
    {
        return CLJFunctional::typeName();
    }
    
    Molecules parameterise(const MolGroup &molecules);
    Molecules parameteriseForIntermolecular(const MolGroup &molecules);
    Molecules parameteriseForIntramolecular(const MolGroup &molecules);

    void calculateEnergy(const Molecule &mol0, const Molecule &mol1,
                         Energy &energy, Workspace &workspace,
                         double scale_energy=1) const;

    void calculateForce(const Molecule &mol0, const Molecule &mol1,
                        MolForceTable &forces0, 
                        MolForceTable &forces1,
                        Workspace &workspace,
                        double scale_force=1) const;

    const Properties& properties() const;
    
    bool setProperty(const QString &name, const PropertyBase &value);

private:
    void _pvt_calculateEnergy(const Molecule &mol0, const Molecule &mol1,
                              Energy &energy, Workspace &workspace,
                              double scale_energy) const;

    void _pvt_calculateForce(const Molecule &mol0, const Molecule &mol1,
                             MolForceTable &forces0, 
                             MolForceTable &forces1,
                             Workspace &workspace,
                             double scale_force) const;

    /** The current values of the properties of this functional */
    Properties props;
    
    /** The space in which this functional operates */
    Space spce;
    
    /** The nonbonded switching function */
    SwitchingFunction switchfunc;
    
    /** All of the LJ parameters in this forcefield, arranged by ID 
        (index) */
    QVector<LJParameter> ljparams;
    
    /** All possible LJ parameter pair combinations, arranged
        in a symmetric 2D array */
    Array2D<LJPair> ljpairs;
};

/** This class represents a coulomb and LJ potential that is suited
    towards calculations of intramolecular CLJ energies. This is because
    it uses, in addition to coulomb and LJ parameters, an intramolecular
    non-bonded scaling parameter that allows for the interactions between
    arbitrary intramolecular atom pairs to be scaled by arbitrary amounts
    
    @author Christopher Woods
*/
class SIREMM_EXPORT IntraCLJPotential
{
public:

    typedef detail::CLJEnergy Energy;
    typedef Energy::Components Components;
    
    typedef detail::IntraScaledCLJParameterNames3D ParameterNames;

    typedef detail::CLJParameter Parameter;
    typedef detail::IntraScaledAtomicParameters<
                  SireFF::detail::AtomicParameters3D<Parameter>,
                  detail::IntraScaledParameters<CLJScaleFactors> > Parameters;
        
    typedef SireBase::PairMatrix<double> Workspace;

    typedef SireFF::detail::FFMolecule3D<Parameters> Molecule;

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
    
    void calculateEnergy(const Molecule &mol, Energy &energy,
                         Workspace &workspace,
                         double scale_energy=1) const;

    void calculateForce(const Molecule &mol, MolForceTable &forces,
                        Workspace &workspace,
                        double scale_force=1) const;
    
};

/** Calculate the coulomb and LJ energy between the passed pair
    of molecules and add these energies onto 'energy'. This uses
    the passed workspace to perform the calculation */
inline void InterCLJPotential::calculateEnergy(const InterCLJPotential::Molecule &mol0,
                                               const InterCLJPotential::Molecule &mol1,
                                               InterCLJPotential::Energy &energy,
                                               InterCLJPotential::Workspace &workspace,
                                               double scale_energy) const
{
    if (scale_energy != 0 and not space->beyond(switchfunc->cutoffDistance(),
                                                mol0.aaBox(), mol1.aaBox()))
    {
        this->_pvt_calculateEnergy(mol0, mol1, energy, workspace, scale_energy);
    }
}

/** Calculate the coulomb and LJ forces on the atoms between the passed pair
    of molecules and add these energies onto 'forces'. This uses
    the passed workspace to perform the calculation */
void InterCLJPotential::_pvt_calculateForce(const InterCLJPotential::Molecule &mol0, 
                                            const InterCLJPotential::Molecule &mol1,
                                            MolForceTable &forces0, 
                                            MolForceTable &forces1,
                                            InterCLJPotential::Workspace &workspace,
                                            double scale_force) const
{
    if (scale_force != 0 and not space->beyond(switchfunc->cutoffDistance(),
                                               mol0.aaBox(), mol1.aaBox())
    {
        this->_pvt_calculateForce(mol0, mol1, forces0, forces1,
                                  workspace, scale_force);
    }
}

/** This empty class provides the functions that return the parameters
    used by this forcefield
*/
class SIREMM_EXPORT CLJFunctional::Parameters
{
public:
    Parameters()
    {}
    
    ~Parameters()
    {}
    
    const ParameterName& charges() const
    {
        return Parameters::charge_param;
    }
    
    const ParameterName& lj() const
    {
        return Parameters::lj_param;
    }
    
    const ParameterName& nb_scale() const
    {
        return Parameters::nbscl_param;
    }
    
private:
    /** The parameter name used to locate partial charges */
    static ParameterName charge_param;
    
    /** The parameter name used to locate the LJ parameters */
    static ParameterName lj_param;
    
    /** The parameter name used to locate the non-bonded scale factors */
    static ParameterName nbscl_param;
};

}

SIRE_END_HEADER

#endif
