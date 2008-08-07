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

#ifndef SIREFF_INTER2B3DFF_HPP
#define SIREFF_INTER2B3DFF_HPP

#include "ff3d.h"
#include "inter2bff.hpp"

#include "SireBase/countflops.h"

#include <QDebug>

SIRE_BEGIN_HEADER

namespace SireFF
{

/** This class provides an intermolecular non-bonded potential
    that can work with an two-body, three-dimensional potential
    (provided by the template class 'Potential'). This is a
    3D specific forcefield, so provides functions that can 
    calculate the 3D forces acting on the atoms.
    
    @author Christopher Woods
*/
template<class Potential>
class SIREFF_EXPORT Inter2B3DFF 
                  : public SireBase::ConcreteProperty< Inter2B3DFF<Potential>,
                                                       Inter2BFF<Potential> >, 
                    public FF3D
{
public:
    Inter2B3DFF();
    Inter2B3DFF(const QString &name);
    
    Inter2B3DFF(const Inter2B3DFF<Potential> &other);
    
    ~Inter2B3DFF();
    
    static const char* typeName()
    {
        return QMetaType::typeName( qMetaTypeId< Inter2B3DFF<Potential> >() );
    }
    
    const char* what() const
    {
        return Inter2B3DFF<Potential>::typeName();
    }
    
    Inter2B3DFF<Potential>& operator=(const Inter2B3DFF<Potential> &other);
    
    bool operator==(const Inter2B3DFF<Potential> &other) const;
    bool operator!=(const Inter2B3DFF<Potential> &other) const;
    
    Inter2B3DFF<Potential>* clone() const
    {
        return new Inter2B3DFF<Potential>(*this);
    }
    
    void force(ForceTable &forcetable, double scale_force=1);
    
    void force(ForceTable &forcetable, const Symbol &symbol,
               double scale_force=1);

    void packCoordinates();

protected:
    typedef typename Inter2BFF<Potential>::Energy Energy;
    typedef typename Inter2BFF<Potential>::EnergyWorkspace EnergyWorkspace;

    typedef typename Inter2BFF<Potential>::Molecules Molecules;
    typedef typename Inter2BFF<Potential>::Molecule Molecule;

    typedef typename Inter2BFF<Potential>::ChangedMolecule ChangedMolecule;

    void recalculateEnergy();

    void _pvt_restore(const ForceField &ffield);
};

#ifndef SIRE_SKIP_INLINE_FUNCTIONS

/** Constructor (without giving the forcefield a name!) */
template<class Potential>
SIRE_OUTOFLINE_TEMPLATE
Inter2B3DFF<Potential>::Inter2B3DFF()
  : SireBase::ConcreteProperty< Inter2B3DFF<Potential>,Inter2BFF<Potential> >(), 
    FF3D()
{}

/** Construct, giving this forcefield a name */
template<class Potential>
SIRE_OUTOFLINE_TEMPLATE
Inter2B3DFF<Potential>::Inter2B3DFF(const QString &name)
  : SireBase::ConcreteProperty< Inter2B3DFF<Potential>,Inter2BFF<Potential> >(name), 
    FF3D()
{}

/** Copy constructor */
template<class Potential>
SIRE_OUTOFLINE_TEMPLATE
Inter2B3DFF<Potential>::Inter2B3DFF(const Inter2B3DFF<Potential> &other)
  : SireBase::ConcreteProperty< Inter2B3DFF<Potential>,Inter2BFF<Potential> >(other), 
    FF3D(other)
{}

/** Destructor */
template<class Potential>
SIRE_OUTOFLINE_TEMPLATE
Inter2B3DFF<Potential>::~Inter2B3DFF()
{}

/** Copy assignment operator */
template<class Potential>
SIRE_OUTOFLINE_TEMPLATE
Inter2B3DFF<Potential>& 
Inter2B3DFF<Potential>::operator=(const Inter2B3DFF<Potential> &other)
{
    Inter2BFF<Potential>::operator=(other);
    FF3D::operator=(other);
    
    return *this;
}

/** Comparison operator */
template<class Potential>
SIRE_OUTOFLINE_TEMPLATE
bool Inter2B3DFF<Potential>::operator==(const Inter2B3DFF<Potential> &other) const
{
    return Inter2BFF<Potential>::operator==(other);
}

/** Comparison operator */
template<class Potential>
SIRE_OUTOFLINE_TEMPLATE
bool Inter2B3DFF<Potential>::operator!=(const Inter2B3DFF<Potential> &other) const
{
    return Inter2BFF<Potential>::operator!=(other);
}

/** Private function used to restore the state of this forcefield */
template<class Potential>
SIRE_OUTOFLINE_TEMPLATE
void Inter2B3DFF<Potential>::_pvt_restore(const ForceField &ffield)
{
    if (not ffield->isA< Inter2B3DFF<Potential> >())
        detail::throwForceFieldRestoreBug(this->what(), ffield->what());
        
    this->operator=( ffield->asA< Inter2B3DFF<Potential> >() );
}

/** Pack the coordinates of the molecules so that they all lie 
    contiguously in memory */
template<class Potential>
SIRE_OUTOFLINE_TEMPLATE
void Inter2B3DFF<Potential>::packCoordinates()
{
    this->mols.packCoordinates();
}

/** Recalculate the energy of the current state of this forcefield. This
    will recalculate the energy using the quickest possible route, e.g.
    if will only recalculate the energies of molecules that have changed
    since the last evaluation */
template<class Potential>
SIRE_OUTOFLINE_TEMPLATE
void Inter2B3DFF<Potential>::recalculateEnergy()
{
    int nmols = this->mols.count();
    const typename Potential::Molecule *mols_array 
                            = this->mols.moleculesByIndex().constData();

    if (this->changed_mols.count() == nmols)
        //all of the molecules have changed!
        this->changed_mols.clear();

    //tell the potential that we are starting an evaluation
    Potential::startEvaluation();

    try
    {

    if (this->changed_mols.isEmpty())
    {
        Energy total_nrg;

        #pragma omp parallel
        {
            //we are not recording changes, so we have to assume that
            //everything has changed. Recalculate the total energy from scratch
            EnergyWorkspace workspace;
            Energy my_total_nrg;

            const typename Potential::Molecule *my_mols_array = mols_array;
            const SireVol::AABox *aaboxes_array = this->mols.aaBoxesByIndex().constData();
            const int my_nmols = nmols;

            const SireVol::SpaceBase &spce = this->space();
            const double cutoff = this->switchingFunction().cutoffDistance();

            //loop over all pairs of molecules
            #pragma omp for schedule(dynamic)
            for (int i=0; i<my_nmols-1; ++i)
            {
                const typename Potential::Molecule &mol0 = my_mols_array[i];
                const SireVol::AABox &aabox0 = aaboxes_array[i];
        
                for (int j=i+1; j<my_nmols; ++j)
                {
                    if (not spce.beyond(cutoff, aabox0, aaboxes_array[j]))
                    {
                        const typename Potential::Molecule &mol1 = my_mols_array[j];
                        Potential::calculateEnergy(mol0, mol1, my_total_nrg, workspace);
                    }
                }
            }
              
            #pragma omp critical
            {
                total_nrg += my_total_nrg;
            }
        }
        
        //set the energy
        this->components().setEnergy(*this, total_nrg);
    }
    else
    {
        //just calculate the changes in energy
        Energy old_nrg;
        Energy new_nrg;

        #pragma omp parallel
        {
            EnergyWorkspace old_workspace;
            EnergyWorkspace new_workspace;

            Energy my_old_nrg;
            Energy my_new_nrg;

            const QHash<MolNum,ChangedMolecule> my_changed_mols = this->changed_mols;
            const typename Potential::Molecule *my_mols_array = mols_array;
            const SireVol::AABox *aaboxes_array = this->mols.aaBoxesByIndex().constData();
            const int my_nmols = nmols;

            const SireVol::SpaceBase &spce = this->space();
            const double cutoff = this->switchingFunction().cutoffDistance();
        
            #pragma omp for schedule(static)
            for (int i=0; i<my_nmols; ++i)
            {
                const typename Potential::Molecule &mol0 = my_mols_array[i];
            
                typename QHash<MolNum,ChangedMolecule>::const_iterator it
                                         = my_changed_mols.constFind(mol0.number());
                
                //get the bounding box for this molecule
                const SireVol::AABox &aabox0 = aaboxes_array[i];
                                                                                                             
                if (it == my_changed_mols.constEnd())
                {
                    //this molecule has not changed - just calculate its
                    //energy with all of the changed molecules - as this molecule
                    //hasn't changed, we only need to calculate the change
                    //in energy between this molecule and the changed parts
                    //of the changed molecules
                    for (typename QHash<MolNum,ChangedMolecule>::const_iterator
                                      it2 = my_changed_mols.constBegin();
                         it2 != this->changed_mols.constEnd();
                         ++it2)
                    {
                        if (not spce.beyond(cutoff, aabox0, it2->oldMolecule().aaBox()))
                        {
                            Potential::calculateEnergy(mol0, it2->oldParts(),
                                                       my_old_nrg, old_workspace);
                        }
                    
                        if (not spce.beyond(cutoff, aabox0, it2->newMolecule().aaBox()))
                        {
                            Potential::calculateEnergy(mol0, it2->newParts(),
                                                       my_new_nrg, new_workspace);
                        }
                    }
                }
                else if (this->changed_mols.count() > 1)
                {
                    //this molecule has changed - calculate its energy with all
                    //of the changed molecules that lie after it in the changed_mols
                    //hash (thus ensuring we don't double-count)
                    typename QHash<MolNum,ChangedMolecule>::const_iterator it2 = it;
                
                    bool this_changed_all = it->changedAll();
                
                    if (this_changed_all)
                    {
                        //all of this molecule has changed - so we need to 
                        //calculate the energy of this molecule with *all* of
                        //the parts of the other changed molecules
                        for (++it2; it2 != my_changed_mols.constEnd(); ++it2)
                        {
                            Potential::calculateEnergy(it->oldMolecule(),
                                                       it2->oldMolecule(),
                                                       my_old_nrg, old_workspace);
                                                   
                            Potential::calculateEnergy(it->newMolecule(),
                                                       it2->newMolecule(),
                                                       my_new_nrg, new_workspace);
                        }
                    }
                    else
                    {
                        for (++it2; it2 != my_changed_mols.constEnd(); ++it2)
                        {
                            if (it2->changedAll())
                            {
                                //all of the other molecule has changed - we need
                                //to calculate the energy of the whole molecules
                                //interaction
                                Potential::calculateEnergy(it->oldMolecule(),
                                                           it2->oldMolecule(),
                                                           my_old_nrg, old_workspace);
                            
                                Potential::calculateEnergy(it->newMolecule(),
                                                           it2->newMolecule(),
                                                           my_new_nrg, new_workspace);
                            }
                            else
                            {
                                //both a part of this molecule and a part of the 
                                //other molecule have changed
                           
                                //the change in energy associated with changing 
                                //the first molecule...
                                Potential::calculateEnergy(it->oldParts(),
                                                           it2->oldMolecule(),
                                                           my_old_nrg, old_workspace);
                                                       
                                Potential::calculateEnergy(it->newParts(),
                                                           it2->newMolecule(),
                                                           my_new_nrg, new_workspace);
                                                       
                                //now the change in energy associated with changing
                                //the second molecule...
                                Potential::calculateEnergy(it2->oldParts(),
                                                           it->oldMolecule(),
                                                           my_old_nrg, old_workspace);
                                                     
                                Potential::calculateEnergy(it2->newParts(),
                                                           it->newMolecule(),
                                                           my_new_nrg, new_workspace);
                                                      
                                //now remove double counted changed in mol1 with
                                //change in mol2
                                Potential::calculateEnergy(it->oldParts(),
                                                           it2->oldParts(),
                                                           my_old_nrg, old_workspace, -1);
                                                      
                                Potential::calculateEnergy(it->newParts(),
                                                           it2->newParts(),
                                                           my_new_nrg, new_workspace, -1);
                            }
                        }
                    }
                }
            }

            #pragma omp critical
            {
                old_nrg += my_old_nrg;
                new_nrg += my_new_nrg;
            }

        } // end of parallel block

        if ( not this->removed_mols.isEmpty() )
        {
            //finally, loop over all of the molecules that have been removed - the energy
            //of non-changed molecules with removed molecules has already been calculated,
            //as has the energy of moved molecules that are before the removed molecules
            //in the moved list. We only now have to calculate the energy of the removed
            //molecules with all of the molecules that lie above us in the moved list
            
            EnergyWorkspace workspace;
            
            for (typename QSet<MolNum>::const_iterator 
                                                it = this->removed_mols.constBegin();
                 it != this->removed_mols.constEnd();
                 ++it)
            {
                typename QSet<MolNum>::const_iterator it2 = it;
            
                const ChangedMolecule &mol0 = *(this->changed_mols.constFind(*it));
            
                for (++it2; it2 != this->removed_mols.constEnd(); ++it2)
                {
                    const ChangedMolecule &mol1 = *(this->changed_mols.constFind(*it2));
            
                    Potential::calculateEnergy(mol0.oldMolecule(),
                                               mol1.oldMolecule(),
                                               old_nrg, workspace);
                                           
                    //molecule has been removed, so no new energy
                }
            }
        }
         
        //change the energy
        this->components().changeEnergy(*this, new_nrg - old_nrg);
        
        //clear the changed molecules
        this->changed_mols.clear();
    }

    Potential::finishedEvaluation();
    
    this->setClean();

    }
    catch(...)
    {
        Potential::finishedEvaluation();
        throw;
    }
}

/** Calculate the forces acting on the molecules in the passed forcetable
    that arise from this forcefield, and add them onto the forces present
    in the force table, multiplied by the passed (optional) scaling factor */
template<class Potential>
SIRE_OUTOFLINE_TEMPLATE
void Inter2B3DFF<Potential>::force(ForceTable &forcetable, double scale_force)
{
    if (scale_force == 0)
        return;

    int nforcemols = forcetable.count();
    int nmols = this->mols.count();
    
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
        const typename Potential::Molecule &mol0 = mols_array[imol];
            
        //calculate the force acting on this molecule caused by all of the 
        //other molecules in this forcefield
        for (int j=0; j<nmols; ++j)
        {
            if (j == imol)
                continue;
                
            const typename Potential::Molecule &mol1 = mols_array[j];
            
            Potential::calculateForce(mol0, mol1, moltable, workspace, scale_force);
        }
    }
}

/** Calculate the force acting on the molecules in the passed forcetable  
    caused by the component of this forcefield represented by 'symbol',
    adding this force onto the existing forces in the forcetable (optionally
    multiplied by 'scale_force' */
template<class Potential>
SIRE_OUTOFLINE_TEMPLATE
void Inter2B3DFF<Potential>::force(ForceTable &forcetable, const Symbol &symbol,
                                   double scale_force)
{
    if (scale_force == 0)
        return;

    int nforcemols = forcetable.count();
    int nmols = this->mols.count();
    
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
        const typename Potential::Molecule &mol0 = mols_array[imol];
            
        //calculate the force acting on this molecule caused by all of the 
        //other molecules in this forcefield
        for (int j=0; j<nmols; ++j)
        {
            if (j == imol)
                continue;
                
            const typename Potential::Molecule &mol1 = mols_array[j];
            
            Potential::calculateForce(mol0, mol1, moltable, symbol,
                                      this->components(), workspace, scale_force);
        }
    }
}

#endif //SIRE_SKIP_INLINE_FUNCTIONS

} // end of namespace SireFF

SIRE_END_HEADER

#endif
