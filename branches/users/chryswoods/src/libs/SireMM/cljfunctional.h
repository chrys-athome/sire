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
    class Molecule;   // how the molecule is represented in this functional
    class ChangedMol; // how to change the molecule in this functional
    class Parameters; // the parameters used by this functional
    class Components; // the energy components available in this functional
    class Workspace;  // the temporary workspace used during the calculation

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

    Energy calculateEnergy(const Molecule &mol0, const Molecule &mol1,
                           Workspace &workspace) const;
                           
    Energy calculateEnergy(const Molecule &mol, Workspace &workspace) const;

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
};

} // end of namespace SireMM

QDataStream& operator<<(QDataStream&, const CLJFunctional::Energy&);
QDataStream& operator>>(QDataStream&, CLJFunctional::Energy&);

QDataStream& operator<<(QDataStream&, const CLJFunctional::Molecule&);
QDataStream& operator>>(QDataStream&, CLJFunctional::Molecule&);

QDataStream& operator<<(QDataStream&, const CLJFunctional::ChangedMol&);
QDataStream& operator>>(QDataStream&, CLJFunctional::ChangedMol&);

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

/** This small class provides a temporary workspace that is needed to
    hold intermediate values for the CLJ calculation
*/
class SIREMM_EXPORT CLJFunctional::Workspace
{
public:
    Workspace()
    {}
    
    ~Workspace()
    {}
    
    SireBase::PairMatrix<double> distmatrix;
    SireBase::PairMatrix<CLJPair> cljmatrix;
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
};

namespace detail
{
class CLJFunctional_MolData;
}

/** This class holds the representation of a molecule required
    by the CLJFunctional
*/
class SIREMM_EXPORT CLJFunctional::Molecule
{

friend QDataStream& ::operator<<(QDataStream&, const Molecule&);
friend QDataStream& ::operator>>(QDataStream&, Molecule&);

public:
    Molecule();
    
    Molecule(const PartialMolecule &molecule,
             const ParameterMap &map = ParameterMap());
             
    Molecule(const Molecule &other);
    
    ~Molecule();
    
    Molecule& operator=(const Molecule &other);
    
    static const char* typeName()
    {
        return "SireMM::CLJFunctional::Molecule";
    }

    const char* what() const
    {
        return Molecule::typeName();
    }

private:
    /** Implicitly shared pointer to the raw data for this molecule */
    QSharedDataPointer<detail::CLJFunctional_MolData> d;
};

}

SIRE_END_HEADER

#endif
