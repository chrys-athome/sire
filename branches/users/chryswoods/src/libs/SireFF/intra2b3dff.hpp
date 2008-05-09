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

#ifndef SIREFF_INTRA2B3DFF_HPP
#define SIREFF_INTRA2B3DFF_HPP

#include "ff3d.h"
#include "intra2bff.hpp"

SIRE_BEGIN_HEADER

namespace SireFF
{

/** This class provides an intramolecular non-bonded potential
    that can work with an two-body, three-dimensional potential
    (provided by the template class 'Potential'). This is a
    3D specific forcefield, so provides functions that can 
    calculate the 3D forces acting on the atoms.
    
    @author Christopher Woods
*/
template<class Potential>
class SIREFF_EXPORT Intra2B3DFF 
                  : public SireBase::ConcreteProperty< Intra2B3DFF<Potential>,
                                                       Intra2BFF<Potential> >, 
                    public FF3D
{
public:
    typedef typename Potential::Components Components;

    Intra2B3DFF();
    Intra2B3DFF(const QString &name);
    
    Intra2B3DFF(const Intra2B3DFF<Potential> &other);
    
    ~Intra2B3DFF();
    
    static const char* typeName()
    {
        return QMetaType::typeName( qMetaTypeId< Intra2B3DFF<Potential> >() );
    }
    
    const char* what() const
    {
        return Intra2B3DFF<Potential>::typeName();
    }
    
    Intra2B3DFF<Potential>& operator=(const Intra2B3DFF<Potential> &other);
    
    bool operator==(const Intra2B3DFF<Potential> &other) const;
    bool operator!=(const Intra2B3DFF<Potential> &other) const;
    
    Intra2B3DFF<Potential>* clone() const
    {
        return new Intra2B3DFF<Potential>(*this);
    }
    
    void force(ForceTable &forcetable, double scale_force=1);
    
    void force(ForceTable &forcetable, const Symbol &symbol,
               double scale_force=1);

protected:
    void _pvt_restore(const ForceField &ffield);
};

/** Constructor (without giving the forcefield a name!) */
template<class Potential>
SIRE_OUTOFLINE_TEMPLATE
Intra2B3DFF<Potential>::Intra2B3DFF()
  : SireBase::ConcreteProperty< Intra2B3DFF<Potential>,Intra2BFF<Potential> >(), 
    FF3D()
{}

/** Construct, giving this forcefield a name */
template<class Potential>
SIRE_OUTOFLINE_TEMPLATE
Intra2B3DFF<Potential>::Intra2B3DFF(const QString &name)
  : SireBase::ConcreteProperty< Intra2B3DFF<Potential>,Intra2BFF<Potential> >(name), 
    FF3D()
{}

/** Copy constructor */
template<class Potential>
SIRE_OUTOFLINE_TEMPLATE
Intra2B3DFF<Potential>::Intra2B3DFF(const Intra2B3DFF<Potential> &other)
  : SireBase::ConcreteProperty< Intra2B3DFF<Potential>,Intra2BFF<Potential> >(other), 
    FF3D(other)
{}

/** Destructor */
template<class Potential>
SIRE_OUTOFLINE_TEMPLATE
Intra2B3DFF<Potential>::~Intra2B3DFF()
{}

/** Copy assignment operator */
template<class Potential>
SIRE_OUTOFLINE_TEMPLATE
Intra2B3DFF<Potential>& 
Intra2B3DFF<Potential>::operator=(const Intra2B3DFF<Potential> &other)
{
    Intra2BFF<Potential>::operator=(other);
    FF3D::operator=(other);
    
    return *this;
}

/** Comparison operator */
template<class Potential>
SIRE_OUTOFLINE_TEMPLATE
bool Intra2B3DFF<Potential>::operator==(const Intra2B3DFF<Potential> &other) const
{
    return Intra2BFF<Potential>::operator==(other);
}

/** Comparison operator */
template<class Potential>
SIRE_OUTOFLINE_TEMPLATE
bool Intra2B3DFF<Potential>::operator!=(const Intra2B3DFF<Potential> &other) const
{
    return Intra2BFF<Potential>::operator!=(other);
}

/** Private function used to restore the state of this forcefield */
template<class Potential>
SIRE_OUTOFLINE_TEMPLATE
void Intra2B3DFF<Potential>::_pvt_restore(const ForceField &ffield)
{
    if (not ffield->isA< Intra2B3DFF<Potential> >())
        detail::throwForceFieldRestoreBug(this->what(), ffield->what());
        
    this->operator=( ffield->asA< Intra2B3DFF<Potential> >() );
}

/** Calculate the forces acting on the molecules in the passed forcetable
    that arise from this forcefield, and add them onto the forces present
    in the force table, multiplied by the passed (optional) scaling factor */
template<class Potential>
SIRE_OUTOFLINE_TEMPLATE
void Intra2B3DFF<Potential>::force(ForceTable &forcetable, double scale_force)
{
    if (scale_force == 0)
        return;

    int nforcemols = forcetable.count();
    
    typename Potential::ForceWorkspace workspace;
    
    MolForceTable *forcetable_array = forcetable.data();
    const typename Potential::Molecule *mols_array 
                            = this->mols.moleculesByIndex().constData();
    
    for (int i=0; i<nforcemols; ++i)
    {
        MolForceTable &moltable = forcetable_array[i];
        
        MolNum molnum = moltable.molNum();
        
        if (not this->mols.contains(molnum))
            //we don't contain this molecule, so no point
            //calculating the force
            continue;
            
        //get the copy of this molecule from this forcefield
        int imol = this->mols.indexOf(molnum);
        const typename Potential::Molecule &mol = mols_array[imol];
            
        //calculate the intramolecular forces acting on this molecule
        Potential::calculateForce(mol, moltable, workspace, scale_force);
    }
}

/** Calculate the force acting on the molecules in the passed forcetable  
    caused by the component of this forcefield represented by 'symbol',
    adding this force onto the existing forces in the forcetable (optionally
    multiplied by 'scale_force' */
template<class Potential>
SIRE_OUTOFLINE_TEMPLATE
void Intra2B3DFF<Potential>::force(ForceTable &forcetable, const Symbol &symbol,
                                   double scale_force)
{
    if (scale_force == 0)
        return;

    int nforcemols = forcetable.count();
    
    typename Potential::ForceWorkspace workspace;
    
    MolForceTable *forcetable_array = forcetable.data();
    const typename Potential::Molecule *mols_array 
                            = this->mols.moleculesByIndex().constData();
    
    for (int i=0; i<nforcemols; ++i)
    {
        MolForceTable &moltable = forcetable_array[i];
        
        MolNum molnum = moltable.molNum();
        
        if (not this->mols.contains(molnum))
            //we don't contain this molecule, so no point
            //calculating the force
            continue;
            
        //get the copy of this molecule from this forcefield
        int imol = this->mols.indexOf(molnum);
        const typename Potential::Molecule &mol = mols_array[imol];
            
        //calculate the intramolecular force acting on this molecule
        Potential::calculateForce(mol, moltable, symbol,
                                  this->components(), workspace, scale_force);
    }
}

} // end of namespace SireFF

SIRE_END_HEADER

#endif
