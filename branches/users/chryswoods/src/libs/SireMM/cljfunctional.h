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

#include "cljpair.h"

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
    
    typedef PairMatrix<double> Workspace;

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

    /** This is the atom parameter used by this 
        forcefield to calculate the interatomic potential 
        
        In this case, it is the combination of the charge
        divided by $sqrt( 4 \pi \epsilon_0 )$ (so that
        charge*charge*inv_distance is the coulomb energy)
        and it is the index of the LJ parameter (so that
        we can look up the combination with any other 
        LJ parameter via the LJPair array)
    */
    class Parameter
    {
    public:
        Parameter();
        Parameter(double reduced_chg, quint32 ljid);
        Parameter(const Parameter &other);
        
        ~Parameter();
        
        Parameter& operator=(const Parameter &other);
        
        double reduced_charge;
        quint32 ljid;
    };
    
    Molecules parameterise(const MolGroup &molecules);
    Molecules parameteriseForIntermolecular(const MolGroup &molecules);
    Molecules parameteriseForIntramolecular(const MolGroup &molecules);

    //intermolecular energy between a pair of molecules
    void calculateEnergy(const Molecule &mol0, const Molecule &mol1,
                         Energy &energy, Workspace &workspace) const;
                           
    //intramolecular energy with a molecule
    void calculateEnergy(const Molecule &mol, Energy &energy,
                         Workspace &workspace) const;

    void calculateForces(const Molecule &mol0, const Molecule &mol1,
                         ForceTable &forces, Workspace &workspace) const;
                         
    void calculateForces(const Molecule &mol, ForceTable &forces,
                         Workspace &workspace) const;

    const Properties& properties() const;
    
    bool setProperty(const QString &name, const PropertyBase &value);

private:
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

class SIREMM_EXPORT CLJFunctional::MolParams
{
public:
    Parameters();
    
    const QVector<CLJFunctional::Parameter>& clj() const;
    
    const CLJNBPairs& nbScale() const;
    
private:
    /** All of the parameters for this molecule, arranged by 
        CutGroup */
    QVector<CLJFunctional::Parameter> cljparams;
    
    /** The non-bonded pairs scale factors for each pair
        of atoms within this molecule */
    CLJNBPairs nbscl_factors;
    
    /** The name of the property to use to get
        the charges for this molecule */
    ParameterName chg_property;
    
    /** The name of the property to use to get 
        the LJ parameters for this molecule */
    ParameterName lj_property;
    
    /** The name of the property to use to get 
        the NB scale factor for this molecule */
    ParameterName nbscl_property;
    
    /** The overall molecule type - this is an optimisation
        that allows us to skip lots of sqrt calculations if
        this has relatively few charge parameters
        
        0 = completely zero charge (all atoms have 0 charge)
        1 = mainly uncharged (only a few atoms carry a charge)
        2 = mainly charged (most or all atoms carry a charge)
    */
    quint32 mol_charge_class;
};

/** This is a *very* lightweight class that is used
    to return temporary references to the coordinates
    and parameters of a molecule so that they can be
    used together in the energy calculations */
class SIREMM_EXPORT CLJFunctional::Molecule
{
public:
    Molecule(const CoordGroupArray &coords,
             const QVector<CLJFunctional::Parameters> &params);

    Molecule(const Molecule &other);

    const CoordGroupArray& coordinates() const;
    const CLJFunctional::Parameters& parameters() const;

private:
    /** Reference to the coordinates of this molecule */
    const CoordGroupArray &coords;
    
    /** Reference to the parameters of this molecule */
    const CLJFunctional::Parameters &params;
};

/** This class holds the representation of a molecule required
    by the CLJFunctional
*/
class SIREMM_EXPORT CLJFunctional::Molecules
{

public:
    Molecules();
    
    Molecule at(quint32 i) const;
    
private:
    /** All of the coordinates of all of the atoms arranged
        into molecules, then CoordGroups, then coordinates */
    CoordGroupArrayArray coords;
    
    /** Parameters for all of the atoms arranged in the same
        order as the coordinates */
    QVector<CLJFunctional::Parameters> params;
};

}

SIRE_END_HEADER

#endif
