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

#ifndef SIREFF_INTER2B2G3DFF_HPP
#define SIREFF_INTER2B2G3DFF_HPP

#include "ff3d.h"
#include "inter2b2gff.hpp"

SIRE_BEGIN_HEADER

namespace SireFF
{

/** This class provides an intermolecular non-bonded potential
    that can work with an two-body, three-dimensional potential
    (provided by the template class 'Potential') that
    calculates the intermolecular potential between
    two groups of molecules. This is a 3D specific forcefield, 
    so provides functions that can calculate the 3D forces acting on the atoms.
    
    @author Christopher Woods
*/
template<class Potential>
class Inter2B2G3DFF : public SireBase::ConcreteProperty< Inter2B2G3DFF<Potential>,
                                                         Inter2B2GFF<Potential> >, 
                      public FF3D
{
public:
    Inter2B2G3DFF();
    Inter2B2G3DFF(const QString &name);
    
    Inter2B2G3DFF(const Inter2B2G3DFF<Potential> &other);
    
    ~Inter2B2G3DFF();
    
    static const char* typeName()
    {
        return QMetaType::typeName( qMetaTypeId< Inter2B2G3DFF<Potential> >() );
    }
    
    const char* what() const
    {
        return Inter2B2G3DFF<Potential>::typeName();
    }
    
    Inter2B2G3DFF<Potential>& operator=(const Inter2B2G3DFF<Potential> &other);
    
    bool operator==(const Inter2B2G3DFF<Potential> &other) const;
    bool operator!=(const Inter2B2G3DFF<Potential> &other) const;
    
    Inter2B2G3DFF<Potential>* clone() const
    {
        return new Inter2B2G3DFF<Potential>(*this);
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
Inter2B2G3DFF<Potential>::Inter2B2G3DFF()
  : SireBase::ConcreteProperty< Inter2B2G3DFF<Potential>,Inter2B2GFF<Potential> >(), 
    FF3D()
{}

/** Construct, giving this forcefield a name */
template<class Potential>
SIRE_OUTOFLINE_TEMPLATE
Inter2B2G3DFF<Potential>::Inter2B2G3DFF(const QString &name)
  : SireBase::ConcreteProperty< Inter2B2G3DFF<Potential>,Inter2B2GFF<Potential> >(name), 
    FF3D()
{}

/** Copy constructor */
template<class Potential>
SIRE_OUTOFLINE_TEMPLATE
Inter2B2G3DFF<Potential>::Inter2B2G3DFF(const Inter2B2G3DFF<Potential> &other)
  : SireBase::ConcreteProperty< Inter2B2G3DFF<Potential>,Inter2B2GFF<Potential> >(other), 
    FF3D(other)
{}

/** Destructor */
template<class Potential>
SIRE_OUTOFLINE_TEMPLATE
Inter2B2G3DFF<Potential>::~Inter2B2G3DFF()
{}

/** Copy assignment operator */
template<class Potential>
SIRE_OUTOFLINE_TEMPLATE
Inter2B2G3DFF<Potential>& 
Inter2B2G3DFF<Potential>::operator=(const Inter2B2G3DFF<Potential> &other)
{
    Inter2B2GFF<Potential>::operator=(other);
    FF3D::operator=(other);
    
    return *this;
}

/** Comparison operator */
template<class Potential>
SIRE_OUTOFLINE_TEMPLATE
bool Inter2B2G3DFF<Potential>::operator==(const Inter2B2G3DFF<Potential> &other) const
{
    return Inter2B2GFF<Potential>::operator==(other);
}

/** Comparison operator */
template<class Potential>
SIRE_OUTOFLINE_TEMPLATE
bool Inter2B2G3DFF<Potential>::operator!=(const Inter2B2G3DFF<Potential> &other) const
{
    return Inter2B2GFF<Potential>::operator!=(other);
}

/** Private function used to restore the state of this forcefield */
template<class Potential>
SIRE_OUTOFLINE_TEMPLATE
void Inter2B2G3DFF<Potential>::_pvt_restore(const ForceField &ffield)
{
    if (not ffield->isA< Inter2B2G3DFF<Potential> >())
        detail::throwForceFieldRestoreBug(this->what(), ffield->what());
        
    this->operator=( ffield->asA< Inter2B2G3DFF<Potential> >() );
}

/** Calculate the forces acting on the molecules in the passed forcetable
    that arise from this forcefield, and add them onto the forces present
    in the force table, multiplied by the passed (optional) scaling factor */
template<class Potential>
SIRE_OUTOFLINE_TEMPLATE
void Inter2B2G3DFF<Potential>::force(ForceTable &forcetable, double scale_force)
{
    if (scale_force == 0)
        return;

    int nforcemols = forcetable.count();
    int nmols0 = this->mols[0].count();
    int nmols1 = this->mols[1].count();
    
    typename Potential::ForceWorkspace workspace;
    
    MolForceTable *forcetable_array = forcetable.data();
    const typename Potential::Molecule *mols0_array 
                            = this->mols[0].moleculesByIndex().constData();
    const typename Potential::Molecule *mols1_array
                            = this->mols[1].moleculesByIndex().constData();
    
    for (int i=0; i<nforcemols; ++i)
    {
        MolForceTable &moltable = forcetable_array[i];
        
        MolNum molnum = moltable.molNum();
        
        if (this->mols[0].contains(molnum))
        {
            //calculate the forces on this molecule caused by group0
            int imol = this->mols[0].indexOf(molnum);
            const typename Potential::Molecule &mol0 = mols0_array[imol];
            
            for (int j=0; j<nmols1; ++j)
            {
                const typename Potential::Molecule &mol1 = mols1_array[j];
                
                Potential::calculateForce(mol0, mol1, moltable,
                                          workspace, scale_force);
            }
        }
        
        if (this->mols[1].contains(molnum))
        {
            //calculate the forces on this molecule caused by group1
            int imol = this->mols[1].indexOf(molnum);
            const typename Potential::Molecule &mol0 = mols1_array[imol];
            
            for (int j=0; j<nmols0; ++j)
            {
                const typename Potential::Molecule &mol1 = mols0_array[j];
                
                Potential::calculateForce(mol0, mol1, moltable,
                                          workspace, scale_force);
            }
        }
    }
}

/** Calculate the force acting on the molecules in the passed forcetable  
    caused by the component of this forcefield represented by 'symbol',
    adding this force onto the existing forces in the forcetable (optionally
    multiplied by 'scale_force' */
template<class Potential>
SIRE_OUTOFLINE_TEMPLATE
void Inter2B2G3DFF<Potential>::force(ForceTable &forcetable, const Symbol &symbol,
                                   double scale_force)
{
    if (scale_force == 0)
        return;

    int nforcemols = forcetable.count();
    int nmols0 = this->mols[0].count();
    int nmols1 = this->mols[1].count();
    
    typename Potential::ForceWorkspace workspace;
    
    MolForceTable *forcetable_array = forcetable.data();
    const typename Potential::Molecule *mols0_array 
                            = this->mols[0].moleculesByIndex().constData();
    const typename Potential::Molecule *mols1_array
                            = this->mols[1].moleculesByIndex().constData();
    
    for (int i=0; i<nforcemols; ++i)
    {
        MolForceTable &moltable = forcetable_array[i];
        
        MolNum molnum = moltable.molNum();
        
        if (this->mols[0].contains(molnum))
        {
            //calculate the forces on this molecule caused by group0
            int imol = this->mols[0].indexOf(molnum);
            const typename Potential::Molecule &mol0 = mols0_array[imol];
            
            for (int j=0; j<nmols1; ++j)
            {
                const typename Potential::Molecule &mol1 = mols1_array[j];
                
                Potential::calculateForce(mol0, mol1, moltable,
                                          symbol, this->components(),
                                          workspace, scale_force);
            }
        }
        
        if (this->mols[1].contains(molnum))
        {
            //calculate the forces on this molecule caused by group1
            int imol = this->mols[1].indexOf(molnum);
            const typename Potential::Molecule &mol0 = mols1_array[imol];
            
            for (int j=0; j<nmols0; ++j)
            {
                const typename Potential::Molecule &mol1 = mols0_array[j];
                
                Potential::calculateForce(mol0, mol1, moltable,
                                          symbol, this->components(),
                                          workspace, scale_force);
            }
        }
    }
}

} // end of namespace SireFF

SIRE_END_HEADER

#endif
