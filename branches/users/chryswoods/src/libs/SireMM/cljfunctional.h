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

using SireBase::PropertyName;
using SireBase::PackedArray2D;

using SireFF::detail::FFParameters3D;

/** These are the Coulomb and LJ parameters of all of 
    the atoms in this view. This stores the parameters
    in a format that is optimised for the CLJ forcefield
    
    @author Christopher Woods
*/
class CLJParameters : public FFParameters3D
{
public:

    class Parameter
    {
        Parameter();
        Parameter(double reduced_chg, quint32 ljid);
        Parameter(const Parameter &other);
        
        ~Parameter();
        
        Parameter& operator=(const Parameter &other);
        
        double reduced_charge;
        quint32 ljid;
    };

    CLJParameters();
    
    CLJParameters(const PartialMolecule &molecule,
                  const PropertyName &coords_property);
     
    CLJParameters(const FFParameters3D &params3d,
                  const PackedArray2D<Parameter> &cljparams);
                                            
    template<class T>
    CLJParameters(const PartialMolecule &molecule,
                  const T &parameternames);
    
    CLJParameters(const CLJParameters &other);
    
    ~CLJParameters();
    
    CLJParameters& operator=(const CLJParameters &other);
    
    bool operator==(const CLJParameters &other) const;
    bool operator!=(const CLJParameters &other) const;

    const PackedArray2D<Parameter>& cljParameters() const;

    void setCLJParameters(const PackedArray2D<Parameter> &parameters);

    bool changedAllGroups(const CLJParameters &params) const;
    
    QSet<quint32> getChangedGroups(const CLJParameters &params) const;
    
    void addChangedGroups(const CLJParameters &params,
                          QSet<quint32> &changed_groups) const;
    
    CLJParameters applyMask(const QSet<quint32> &idxs) const;

    void assertCompatible(const PackedArray2D<Parameter> &params) const;

protected:
    /** The CLJ parameters. This holds the reduced charge
        (charge divided by sqrt(4 pi eps0)) and the ID of
        the LJ parameter (so that the LJPair parameter can 
        be located) */
    PackedArray2D<Parameter> cljparams;
};

/** Construct for the passed molecule, using the .coordinates()   
    property to find the correct parameter */
template<class T>
CLJParameters::CLJParameters(const PartialMolecule &molecule,
                             const T &parameternames)
              : FFParameters3D(molecule, parameternames)
{}

/** Return the array of CLJ parameters */
inline const PackedArray2D<CLJParameters::Parameter>& 
CLJParameters::cljParameters() const
{
    return cljparams;
}

} // end of namespace SireMM::detail

/** This class provides all of the functions and containers  
    necessary to provide an interface to calculate interatomic
    potentials using the Coulomb and Lennard Jones functions.
    
    @author Christopher Woods
*/
class SIREMM_EXPORT CLJFunctional
{

friend QDataStream& ::operator<<(QDataStream&, const CLJFunctional&);
friend QDataStream& ::operator>>(QDataStream&, CLJFunctional&);

public:

    class Energy;     // class used to hold the energy components
    class Parameters; // the parameters used by this functional
    class Components; // the energy components available in this functional
    
    typedef SireBase::PairMatrix<double> Workspace;

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
                           
    void calculateEnergy(const Molecule &mol, Energy &energy,
                         Workspace &workspace,
                         double scale_energy=1) const;

    void calculateForce(const Molecule &mol0, const Molecule &mol1,
                        MolForceTable &forces0, 
                        MolForceTable &forces1,
                        Workspace &workspace,
                        double scale_force=1) const;
                         
    void calculateForce(const Molecule &mol, MolForceTable &forces,
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

/** Calculate the coulomb and LJ energy between the passed pair
    of molecules and add these energies onto 'energy'. This uses
    the passed workspace to perform the calculation */
inline void CLJPotential::calculateEnergy(const CLJPotential::Molecule &mol0,
                                          const CLJPotential::Molecule &mol1,
                                          CLJPotential::Energy &energy,
                                          CLJPotential::Workspace &workspace,
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
void CLJPotential::_pvt_calculateForce(const Molecule &mol0, const Molecule &mol1,
                                       MolForceTable &forces0, 
                                       MolForceTable &forces1,
                                       Workspace &workspace,
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

/** This is the class that holds the energy components of 
    the CLJFunctional (Coulomb and LJ) 
*/
class SIREMM_EXPORT CLJFunctional::Energy
{

friend QDataStream& ::operator<<(QDataStream&, const Energy&);
friend QDataStream& ::operator>>(QDataStream&, Energy&);

public:
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
