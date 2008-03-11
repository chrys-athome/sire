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
class CLJFunctional;
}

QDataStream& operator<<(QDataStream&, const SireMM::CLJFunctional&);
QDataStream& operator>>(QDataStream&, SireMM::CLJFunctional&);

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

namespace detail
{

using SireFF::detail::AtomicParameters3D;

/** Generic atomic parameters that use a scale factor
    for intermolecular atom pairs
    
    @author Christopher Woods
*/
template<class PARAM, class SCALE_FACTORS>
class IntraScaledAtomicParameters3D : public AtomicParameters3D<PARAM>
{
public:
    IntraScaledAtomicParameters3D();
    
    IntraScaledAtomicParameters3D(const PartialMolecule &molecule,
                                  const PropertyName &coord_property,
                                  const PropertyName &scale_property);
                                  
    IntraScaledAtomicParameters3D(const SireBase::AtomicParameters3D<PARAM> &params,
                                  const SCALE_FACTORS &scale_factors);

    template<class T>
    IntraScaledAtomicParameters3D(const PartialMolecule &molecule,
                                  const T &propertynames);
                                  
    IntraScaledAtomicParameters3D(
        const IntraScaledAtomicParameters3D<PARAM,SCALE_FACTORS> &other);
    
    ~IntraScaledAtomicParameters3D();
    
    IntraScaledAtomicParameters3D<PARAM> operator=(
                    const IntraScaledAtomicParameters3D<PARAM,SCALE_FACTORS> &other);
                                
    bool operator==(
        const IntraScaledAtomicParameters3D<PARAM,SCALE_FACTORS> &other) const;
    bool operator!=(
        const IntraScaledAtomicParameters3D<PARAM,SCALE_FACTORS> &other) const;
    
    const SCALE_FACTORS& scaleFactors() const;
    
    void setScaleFactors(const SCALE_FACTORS &scale_factors);
    
    bool changedAllGroups(
            const IntraScaledAtomicParameters3D<PARAM,SCALE_FACTORS> &other) const;
    
    QSet<quint32> getChangedGroups(
            const IntraScaledAtomicParameters3D<PARAM,SCALE_FACTORS> &params) const;
    
    void addChangedGroups(
            const IntraScaledAtomicParameters3D<PARAM,SCALE_FACTORS> &params,
            QSet<quint32> &changed_groups) const;
    
    IntraScaledAtomicParameters3D<PARAM,SCALE_FACTORS> 
    applyMask(const QSet<quint32> &idxs) const;

protected:
    /** The intramolecular inter-atomic scale factors to apply to 
        the potential between intramolecular pairs of atoms */
    SCALE_FACTORS sclfactors;
};

/////////
///////// Implementation of IntraScaledAtomicParameters3D
/////////

/** Null constructor */
template<class PARAM, class SCALE_FACTORS>
SIRE_OUTOFLINE_TEMPLATE
IntraScaledAtomicParameters3D<PARAM,SCALE_FACTORS>::IntraScaledAtomicParameters3D()
               : AtomicParameters3D<PARAM>()
{}

/** Construct for the molecule 'molecule' using the specified properties
    to find the coordinates and scale factor properties
    
    \throw SireBase::missing_property
    \throw SireError::invalid_cast
*/
template<class PARAM, class SCALE_FACTORS>
SIRE_OUTOFLINE_TEMPLATE
IntraScaledAtomicParameters3D<PARAM,SCALE_FACTORS>::IntraScaledAtomicParameters3D(
                              const PartialMolecule &molecule,
                              const PropertyName &coord_property,
                              const PropertyName &scale_property)
                : AtomicParameters3D<PARAM>(molecule, coord_property)
{
    sclfactors = molecule.property(scale_property).asA<SCALE_FACTORS>();
}

/** Construct by combining some AtomicParameters3D with some scale factors */
template<class PARAM, class SCALE_FACTORS>
SIRE_OUTOFLINE_TEMPLATE
IntraScaledAtomicParameters3D<PARAM>::IntraScaledAtomicParameters3D(
                              const SireBase::AtomicParameters3D<PARAM> &params,
                              const SCALE_FACTORS &scale_factors)
                : AtomicParameters3D<PARAM>(params), sclfactors(scale_factors)
{
    this->assertCompatible(sclfactors);
}

/** Construct the parameters for the passed molecule using 'propertynames' 
    to find the right properties for the parameters
    
    \throw SireBase::missing_property
    \throw SireError::invalid_cast
*/
template<class PARAM, class SCALE_FACTORS>
template<class T>
SIRE_OUTOFLINE_TEMPLATE
IntraScaledAtomicParameters3D<PARAM>::IntraScaledAtomicParameters3D(
                              const PartialMolecule &molecule,
                              const T &propertynames)
                : AtomicParameters3D<PARAM>(molecule, propertynames)
{
    sclfactors = molecule.property( propertynames.nbScaleFactors() )
                         .asA<SCALE_FACTORS>();
}
                              
/** Copy constructor */
template<class PARAM, class SCALE_FACTORS>
SIRE_OUTOFLINE_TEMPLATE
IntraScaledAtomicParameters3D<PARAM>::IntraScaledAtomicParameters3D(
                              const IntraScaledAtomicParameters3D<PARAM> &other)
                : AtomicParameters3D<PARAM>(other),
                  sclfactors(other.sclfactors)
{}

/** Destructor */
template<class PARAM, class SCALE_FACTORS>
SIRE_OUTOFLINE_TEMPLATE
IntraScaledAtomicParameters3D<PARAM>::~IntraScaledAtomicParameters3D()
{}

/** Copy assignment operator */
template<class PARAM, class SCALE_FACTORS>
SIRE_OUTOFLINE_TEMPLATE
IntraScaledAtomicParameters3D<PARAM> IntraScaledAtomicParameters3D<PARAM>::operator=(
                            const IntraScaledAtomicParameters3D<PARAM> &other)
{
    AtomicParameters3D<PARAM>::operator=(other);
    sclfactors = other.sclfactors;
    return *this;
}
                 
/** Comparison operator */           
template<class PARAM, class SCALE_FACTORS>
SIRE_OUTOFLINE_TEMPLATE
bool IntraScaledAtomicParameters3D<PARAM>::operator==(
                            const IntraScaledAtomicParameters3D<PARAM> &other) const
{
    return AtomicParameters3D::operator==(other) and 
           sclfactors == other.sclfactors;
}

/** Comparison operator */
template<class PARAM, class SCALE_FACTORS>
SIRE_OUTOFLINE_TEMPLATE
bool IntraScaledAtomicParameters3D<PARAM>::operator!=(
                            const IntraScaledAtomicParameters3D<PARAM> &other) const
{
    return AtomicParameters3D::operator!=(other) or
           sclfactors != other.sclfactors;
}

/** Return the inter-atomic intramolecular scale factors for the 
    intramolecular atom-atom interactions */
template<class PARAM, class SCALE_FACTORS>
SIRE_OUTOFLINE_TEMPLATE
const SCALE_FACTORS& IntraScaledAtomicParameters3D<PARAM>::scaleFactors() const
{
    return sclfactors;
}

/** Set the inter-atomic intramolecular scale factors for the 
    intramolecular atom-atom interactions 
    
    \throw SireError::incompatible_error
*/
template<class PARAM, class SCALE_FACTORS>
SIRE_OUTOFLINE_TEMPLATE
void IntraScaledAtomicParameters3D<PARAM>::setScaleFactors(
                                            const SCALE_FACTORS &scale_factors)
{
    this->assertCompatible(scale_factors);
    sclfactors = scale_factors;
}

/** Return whether or not all CutGroups in this molecule have changed compared
    to 'other' */
template<class PARAM, class SCALE_FACTORS>
SIRE_OUTOFLINE_TEMPLATE
bool IntraScaledAtomicParameters3D<PARAM>::changedAllGroups(
                        const IntraScaledAtomicParameters3D<PARAM> &other) const
{
    return sclfactors != other.sclfactors or
           AtomicParameters3D<PARAM>::changedAllGroups(other);
}

/** Add to 'changed_groups' the indicies of groups that have changed
    compared to 'other' */
template<class PARAM, class SCALE_FACTORS>
SIRE_OUTOFLINE_TEMPLATE
void IntraScaledAtomicParameters3D<PARAM>::addChangedGroups(
                      const IntraScaledAtomicParameters3D<PARAM> &other,
                      QSet<quint32> &changed_groups) const
{
    if (sclfactors != other.sclfactors)
    {
        //all groups have changed
        for (CGIdx i(0); i<quint32(this->coordinates().count()); ++i)
        {
            changed_groups.insert(i);
        }
        
        return;
    }
    else
        AtomicParameters3D<PARAM>::addChangedGroups(other, changed_groups);
}

/** Return the indicies of CutGroups that have changed compared to 'other' */
template<class PARAM, class SCALE_FACTORS>
SIRE_OUTOFLINE_TEMPLATE
QSet<quint32> IntraScaledAtomicParameters3D<PARAM>::getChangedGroups(
                        const IntraScaledAtomicParameters3D<PARAM> &other) const
{
    QSet<quint32> changed_groups;
    changed_groups.reserve(this->coordinates().count());
    this->addChangedGroups(other, changed_groups);
    
    return changed_groups;
}

/** Mask these parameters so that only the atomic parameters for the 
    CutGroups whose indicies are in 'cgidxs' are present. */
template<class PARAM, class SCALE_FACTORS>
SIRE_OUTOFLINE_TEMPLATE
IntraScaledAtomicParameters3D<PARAM> 
IntraScaledAtomicParameters3D<PARAM>::applyMask(const QSet<quint32> &idxs) const
{
    return IntraScaledAtomicParameters3D<PARAM>(
                        AtomicParameters3D<PARAM>::applyMask(idxs), sclfactors);
}

} // end of namespace SireMM::detail

/** This is the class that holds the energy components of 
    the CLJFunctional (Coulomb and LJ) 
*/
class SIREMM_EXPORT CLJEnergy
{

friend QDataStream& ::operator<<(QDataStream&, const Energy&);
friend QDataStream& ::operator>>(QDataStream&, Energy&);

public:
    typedef CLJComponent Components;

    Energy(double cnrg=0, double ljnrg=0) : icnrg(cnrg), iljnrg(ljnrg)
    {}
    
    Energy(const Energy &other) : icnrg(other.icnrg), ljnrg(other.ljnrg)
    {}
    
    ~Energy()
    {}
    
    static const char* typeName()
    {
        return "SireMM::CLJFunctional::Energy";
    }
    
    const char* what() const
    {
        return Energy::typeName();
    }
    
    Energy& operator+=(const Energy &other)
    {
        icnrg += other.icnrg;
        iljnrg += other.iljnrg;
        return *this;
    }
    
    Energy& operator-=(const Energy &other)
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

private:
    /** The coulomb and LJ components of the energy */
    double icnrg, iljnrg;
};

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

    typedef SireMM::detail::CLJParameter Parameter;
    typedef SireMM::detail::IntraScaledAtomicParameters3D<Parameter,
                                                          CLJScaleFactors> Parameters;

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

} // end of namespace SireMM

QDataStream& operator<<(QDataStream&, const CLJFunctional::Energy&);
QDataStream& operator>>(QDataStream&, CLJFunctional::Energy&);

QDataStream& operator<<(QDataStream&, const CLJFunctional::Components&);
QDataStream& operator>>(QDataStream&, CLJFunctional::Components&);

namespace SireMM
{

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
