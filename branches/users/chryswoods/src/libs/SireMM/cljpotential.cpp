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

#include "SireBase/sparsematrix.hpp"

#include "cljpotential.h"
#include "ljparameter.h"

#include "SireMol/mover.hpp"
#include "SireMol/atomcoords.h"

#include "SireMaths/maths.h"

#include "SireUnits/units.h"

#include "SireError/errors.h"

using namespace SireMM;
using namespace SireMM::detail;

using namespace SireFF;
using namespace SireFF::detail;

using namespace SireMol;

using namespace SireMaths;

using namespace SireBase;

///////
/////// Implementation of CoulombComponent
///////

/** Constructor */
CoulombComponent::CoulombComponent(quint64 ffuid)
                 : FFComponent(ffuid, QLatin1String("coulomb"))
{}

/** Destructor */
CoulombComponent::~CoulombComponent()
{}

///////
/////// Implementation of LJComponent
///////

/** Constructor */
LJComponent::LJComponent(quint64 ffuid)
            : FFComponent(ffuid, QLatin1String("LJ"))
{}

/** Destructor */
LJComponent::~LJComponent()
{}

///////
/////// Implementation of CLJComponent
///////

/** Constructor */
CLJComponent::CLJComponent(quint64 ffuid)
             : FFComponent(ffuid, QLatin1String("CLJ")), 
               coul_component(ffuid), lj_component(ffuid)
{}

/** Destructor */
CLJComponent::~CLJComponent()
{}

///////
/////// Completely instantiate the CLJPotential ancillary classes
///////

template
class FFMolecule3D<InterCLJPotential>;

template
class ChangedMolecule<InterCLJPotential::Molecule>;

template
class FFMolecule3D<IntraCLJPotential>;

template
class ChangedMolecule<IntraCLJPotential::Molecule>;

/** Internal function used to get the charge and LJ parameters from a molecule
    and convert them into a PackedArray of reduced charges and LJ parameter IDs */
static PackedArray2D<CLJParameter> getCLJParameters(const PartialMolecule &molecule,
                                                    const PropertyName &charge_property,
                                                    const PropertyName &lj_property)
{
    const AtomCharges &chgs = molecule.property(charge_property)->asA<AtomCharges>();
    const AtomLJs &ljs = molecule.property(lj_property)->asA<AtomLJs>();
    
    const AtomSelection &selected_atoms = molecule.selection();
    
    if (selected_atoms.selectedNone())
        return PackedArray2D<CLJParameter>();
    
    //create space for the parameters - only need space for CutGroups
    //that contain at least one selected atom
    QVector< QVector<CLJParameter> > cljparams( selected_atoms.nSelectedCutGroups() );
    QVector<CLJParameter>* cljparams_array = cljparams.data();

    const double sqrt_4pieps0 = std::sqrt(SireUnits::one_over_four_pi_eps0);

    try
    {

    LJParameterDB::lock();

    if (selected_atoms.selectedAllCutGroups())
    {
        const int ncg = molecule.data().info().nCutGroups();
    
        for (CGIdx i(0); i<ncg; ++i)
        {
            const int nats = molecule.data().info().nAtoms(i);
            
            QVector<CLJParameter> group_cljs(nats);
            CLJParameter *group_cljs_array = group_cljs.data();
            
            //get the arrays containing the charge and LJ parameters
            //for this CutGroup
            const SireUnits::Dimension::Charge *group_chgs = chgs.constData(i);
            const LJParameter *group_ljs = ljs.constData(i);
            
            if (selected_atoms.selectedAll(i))
            {
                for (Index j(0); j<nats; ++j)
                {
                    group_cljs_array[j].reduced_charge = group_chgs[j] * sqrt_4pieps0;
                    group_cljs_array[j].ljid = 
                            LJParameterDB::_locked_addLJParameter(group_ljs[j]);
                }
            }
            else
            {
                foreach (Index j, selected_atoms.selectedAtoms(i))
                {
                    group_cljs_array[j].reduced_charge = group_chgs[j] * sqrt_4pieps0;
                    group_cljs_array[j].ljid =
                            LJParameterDB::_locked_addLJParameter(group_ljs[j]);
                }
            }
            
            cljparams_array[i] = group_cljs;
        }
    }
    else
    {
        foreach (CGIdx i, selected_atoms.selectedCutGroups())
        {
            const int nats = molecule.data().info().nAtoms(i);
            
            QVector<CLJParameter> group_cljs(nats);
            CLJParameter *group_cljs_array = group_cljs.data();
            
            //get the arrays containing the charge and LJ parameters
            //for this CutGroup
            const SireUnits::Dimension::Charge *group_chgs = chgs.constData(i);
            const LJParameter *group_ljs = ljs.constData(i);
            
            if (selected_atoms.selectedAll(i))
            {
                for (Index j(0); j<nats; ++j)
                {
                    group_cljs_array[j].reduced_charge = group_chgs[j] * sqrt_4pieps0;
                    group_cljs_array[j].ljid = 
                            LJParameterDB::_locked_addLJParameter(group_ljs[j]);
                }
            }
            else
            {
                foreach (Index j, selected_atoms.selectedAtoms(i))
                {
                    group_cljs_array[j].reduced_charge = group_chgs[j] * sqrt_4pieps0;
                    group_cljs_array[j].ljid =
                            LJParameterDB::_locked_addLJParameter(group_ljs[j]);
                }
            }
            
            cljparams_array[i] = group_cljs;
        }
    }
    
    LJParameterDB::unlock();
    
    }
    catch(...)
    {
        LJParameterDB::unlock();
        throw;
    }
    
    return PackedArray2D<CLJParameter>( cljparams );
}

/////////////
///////////// Implementation of InterCLJPotential
/////////////

/** Return all of the parameters needed by this potential for 
    the molecule 'molecule', using the supplied property map to
    find the properties that contain those parameters
    
    \throw SireBase::missing_property
    \throw SireError::invalid_cast
    \throw SireError::incompatible_error
*/
InterCLJPotential::Parameters 
InterCLJPotential::getParameters(const PartialMolecule &molecule,
                                 const PropertyMap &map)
{
    return Parameters( molecule, map[parameters().coordinates()],
                       getCLJParameters(molecule, map[parameters().charge()],
                                        map[parameters().lj()]) );
}

/** Update the parameters for the molecule going from 'old_molecule' to 
    'new_molecule', with the parameters found using the property map 'map'
    
    \throw SireBase::missing_property
    \throw SireError::invalid_cast
    \throw SireError::incompatible_error
*/
InterCLJPotential::Parameters
InterCLJPotential::updateParameters(const InterCLJPotential::Parameters &old_params,
                                    const PartialMolecule &old_molecule,
                                    const PartialMolecule &new_molecule,
                                    const PropertyMap &map)
{
    if (old_molecule.selection() != new_molecule.selection())
        //the selection has changed - just get completely new parameters
        return this->getParameters(new_molecule, map);

    //get the property names
    const PropertyName &coords_property = map[parameters().coordinates()];
    const PropertyName &chg_property = map[parameters().charge()];
    const PropertyName &lj_property = map[parameters().lj()];
    
    //get what has changed
    bool new_coords = old_molecule.version(coords_property) !=
                         new_molecule.version(coords_property);
                             
    bool new_clj = ( old_molecule.version(chg_property) !=
                         new_molecule.version(chg_property) ) or
                   ( old_molecule.version(lj_property) !=
                         new_molecule.version(lj_property) );

    if (new_coords and new_clj)
    {
        //everything has changed
        return Parameters( new_molecule, coords_property,
                           getCLJParameters(new_molecule, chg_property, lj_property) );
    }
    else if (new_coords)
    {
        //only the coordinates have changed
        return Parameters( new_molecule, coords_property,
                           old_params.atomicParameters() );
    }
    else if (new_clj)
    {
        //only the CLJ parameters have changed
        return Parameters( old_params,
                           getCLJParameters(new_molecule, chg_property, lj_property) );
    }
    else
    {
        //nothing has changed
        return old_params;
    }
}
                 
/** Update the parameters for the molecule going from 'old_molecule' to 
    'new_molecule', also while the parameters of 'old_molecule'
    where found in 'old_map', now get the parameters using 'new_map'
    
    \throw SireBase::missing_property
    \throw SireError::invalid_cast
    \throw SireError::incompatible_error
*/
InterCLJPotential::Parameters
InterCLJPotential::updateParameters(const InterCLJPotential::Parameters &old_params,
                                    const PartialMolecule &old_molecule,
                                    const PartialMolecule &new_molecule,
                                    const PropertyMap &old_map, 
                                    const PropertyMap &new_map)
{
    if (old_molecule.selection() != new_molecule.selection())
        //the selection has changed - just get completely new parameters
        return this->getParameters(new_molecule, new_map);

    //get the property names
    const PropertyName &old_coords = old_map[parameters().coordinates()];
    const PropertyName &old_chg = old_map[parameters().charge()];
    const PropertyName &old_lj = old_map[parameters().lj()];
    
    const PropertyName &new_coords = new_map[parameters().coordinates()];
    const PropertyName &new_chg = new_map[parameters().charge()];
    const PropertyName &new_lj = new_map[parameters().lj()];
    
    //get what has changed
    bool changed_coords = (new_coords != old_coords) or
                           old_molecule.version(old_coords) !=
                           new_molecule.version(old_coords);
                             
    bool changed_clj = (new_chg != old_chg or new_lj != old_lj) or
                       ( old_molecule.version(old_chg) !=
                         new_molecule.version(old_chg) ) or
                       ( old_molecule.version(old_lj) !=
                         new_molecule.version(old_lj) );

    if (changed_coords and changed_clj)
    {
        //everything has changed
        return Parameters( new_molecule, new_coords,
                           getCLJParameters(new_molecule, new_chg, new_lj) );
    }
    else if (changed_coords)
    {
        //only the coordinates have changed
        return Parameters( new_molecule, new_coords,
                           old_params.atomicParameters() );
    }
    else if (changed_clj)
    {
        //only the CLJ parameters have changed
        return Parameters( old_params,
                           getCLJParameters(new_molecule, new_chg, new_lj) );
    }
    else
    {
        //nothing has changed
        return old_params;
    }
}

/** Return the InterCLJPotential::Molecule representation of 'molecule',
    using the supplied PropertyMap to find the properties that contain
    the necessary forcefield parameters
    
    \throw SireBase::missing_property
    \throw SireError::invalid_cast
    \throw SireError::incompatible_error
*/
InterCLJPotential::Molecule
InterCLJPotential::parameterise(const PartialMolecule &molecule,
                                const PropertyMap &map)
{
    return InterCLJPotential::Molecule(molecule, *this, map);
}

/** Concert the passed group of molecules into InterCLJPotential::Molecules,
    using the supplied PropertyMap to find the properties that contain
    the necessary forcefield parameters in each molecule
    
    \throw SireBase::missing_property
    \throw SireError::invalid_cast
    \throw SireError::incompatible_error
*/
InterCLJPotential::Molecules 
InterCLJPotential::parameterise(const MolGroup &molecules,
                                const PropertyMap &map)
{
    return InterCLJPotential::Molecules(molecules, *this, map);
}

/** Return the total charge of the parameters for the group in 'params' */
double InterCLJPotential::totalCharge(
                        const InterCLJPotential::Parameters::Array &params) const
{
    int n = params.count();
    const Parameter *params_array = params.constData();
    
    double chg = 0;
    
    for (int i=0; i<n; ++i)
    {
        chg += params_array[i].reduced_charge;
    }
    
    return chg;
}

/** Calculate the coulomb and LJ energy between the passed pair
    of molecules and add these energies onto 'energy'. This uses
    the passed workspace to perform the calculation */
void InterCLJPotential::_pvt_calculateEnergy(const InterCLJPotential::Molecule &mol0,
                                             const InterCLJPotential::Molecule &mol1,
                                             InterCLJPotential::Energy &energy,
                                             InterCLJPotential::EnergyWorkspace &distmat,
                                             double scale_energy) const
{
    const quint32 ngroups0 = mol0.nCutGroups();
    const quint32 ngroups1 = mol1.nCutGroups();
    
    const CoordGroup *groups0_array = mol0.coordinates().constData();
    const CoordGroup *groups1_array = mol1.coordinates().constData();
    
    BOOST_ASSERT( mol0.parameters().atomicParameters().count() == int(ngroups0) );
    BOOST_ASSERT( mol1.parameters().atomicParameters().count() == int(ngroups1) );
    
    const Parameters::Array *molparams0_array 
                                = mol0.parameters().atomicParameters().constData();
    const Parameters::Array *molparams1_array 
                                = mol1.parameters().atomicParameters().constData();
    
    double cnrg = 0;
    double ljnrg = 0;
    
    //loop over all pairs of CutGroups in the two molecules
    for (quint32 igroup=0; igroup<ngroups0; ++igroup)
    {
        const Parameters::Array &params0 = molparams0_array[igroup];

        const CoordGroup &group0 = groups0_array[igroup];
        const AABox &aabox0 = group0.aaBox();
        const quint32 nats0 = group0.count();
        const Parameter *params0_array = params0.constData();
    
        for (quint32 jgroup=0; jgroup<ngroups1; ++jgroup)
        {
            const CoordGroup &group1 = groups1_array[jgroup];
            const Parameters::Array &params1 = molparams1_array[jgroup];

            //check first that these two CoordGroups could be within cutoff
            //(if there is only one CutGroup in both molecules then this
            //test has already been performed and passed)
            const bool within_cutoff = (ngroups0 == 1 and ngroups1 == 1) or not
                                        spce->beyond(switchfunc->cutoffDistance(), 
                                                     aabox0, group1.aaBox());
            
            if (not within_cutoff)
                //this CutGroup is either the cutoff distance
                continue;
            
            //calculate all of the interatomic distances
            const double mindist = spce->calcDist(group0, group1, distmat);
            
            if (mindist > switchfunc->cutoffDistance())
                //all of the atoms are definitely beyond cutoff
                continue;
                
            double icnrg = 0;
            double iljnrg = 0;
            
            //loop over all interatomic pairs and calculate the energies
            const quint32 nats1 = group1.count();
            const Parameter *params1_array = params1.constData();
            
            for (quint32 i=0; i<nats0; ++i)
            {
                distmat.setOuterIndex(i);
                const Parameter &param0 = params0_array[i];
                
                if (param0.ljid == 0)
                {
                    //null LJ parameter - only add on the coulomb energy
                    for (quint32 j=0; j<nats1; ++j)
                    {
                        icnrg += param0.reduced_charge * 
                                 params1_array[j].reduced_charge / distmat[j];
                    }
                }
                else
                {
                    for (quint32 j=0; j<nats1; ++j)
                    {
                        //do both coulomb and LJ
                        const Parameter &param1 = params1_array[j];
                        
                        const double invdist = double(1) / distmat[j];
                        
                        icnrg += param0.reduced_charge * param1.reduced_charge 
                                      * invdist;
                              
                        if (param1.ljid != 0)
                        {
                            const LJPair &ljpair = ljpairs.constData()[
                                                      ljpairs.map(param0.ljid,
                                                                  param1.ljid)];
                        
                            double sig_over_dist6 = pow_6(ljpair.sigma()*invdist);
                            double sig_over_dist12 = pow_2(sig_over_dist6);

                            iljnrg += ljpair.epsilon() * (sig_over_dist12 - 
                                                          sig_over_dist6);
                        }
                    }
                }
            }
            
            //are we shifting the electrostatic potential?
            if (use_electrostatic_shifting)
                icnrg -= this->totalCharge(params0) * this->totalCharge(params1)
                              / switchfunc->electrostaticCutoffDistance();
            
            //now add these energies onto the total for the molecule,
            //scaled by any non-bonded feather factor
            if (mindist > switchfunc->featherDistance())
            {
                cnrg += switchfunc->electrostaticScaleFactor(mindist) * icnrg;
                ljnrg += switchfunc->vdwScaleFactor(mindist) * iljnrg;
            }
            else
            {
                cnrg += icnrg;
                ljnrg += iljnrg;
            }
        }
    }
    
    //add this molecule pair's energy onto the total
    energy += Energy(scale_energy * cnrg, scale_energy * ljnrg);
}

/** Calculate the coulomb and LJ forces on the atoms between the passed pair
    of molecules and add these energies onto 'forces'. This uses
    the passed workspace to perform the calculation */
void InterCLJPotential::_pvt_calculateForce(const InterCLJPotential::Molecule &mol0, 
                                            const InterCLJPotential::Molecule &mol1,
                                            MolForceTable &forces0, 
                                            MolForceTable &forces1,
                                            InterCLJPotential::ForceWorkspace &distmat,
                                            double scale_force) const
{
    const quint32 ngroups0 = mol0.nCutGroups();
    const quint32 ngroups1 = mol1.nCutGroups();
    
    const CoordGroup *groups0_array = mol0.coordinates().constData();
    const CoordGroup *groups1_array = mol1.coordinates().constData();
    
    BOOST_ASSERT(mol0.parameters().atomicParameters().count() == int(ngroups0));
    BOOST_ASSERT(mol1.parameters().atomicParameters().count() == int(ngroups1));
    
    const Parameters::Array *molparams0_array 
                                    = mol0.parameters().atomicParameters().constData();
    const Parameters::Array *molparams1_array
                                    = mol1.parameters().atomicParameters().constData();
    
    BOOST_ASSERT(forces0.count() == int(ngroups0));
    BOOST_ASSERT(forces1.count() == int(ngroups1));
    
    const MolForceTable::Array *forces0_array = forces0.constData();
    const MolForceTable::Array *forces1_array = forces1.constData();
    
    //loop over all pairs of CutGroups in the two molecules
    for (quint32 igroup=0; igroup<ngroups0; ++igroup)
    {
        const Parameters::Array &params0 = molparams0_array[igroup];

        const CoordGroup &group0 = groups0_array[igroup];
        const AABox &aabox0 = group0.aaBox();
        const quint32 nats0 = group0.count();
        const Parameter *params0_array = params0.constData();
    
        //get the table that holds the forces acting on all of the
        //atoms of this CutGroup
        BOOST_ASSERT(forces0_array[igroup].count() == int(nats0));
    
        Vector *group_forces0_array = forces0.data(igroup);
    
        for (quint32 jgroup=0; jgroup<ngroups1; ++jgroup)
        {
            const CoordGroup &group1 = groups1_array[jgroup];
            const Parameters::Array &params1 = molparams1_array[jgroup];

            //check first that these two CoordGroups could be within cutoff
            //(if there is only one CutGroup in both molecules then this
            //test has already been performed and passed)
            const bool within_cutoff = (ngroups0 == 1 and ngroups1 == 1) or not
                                        spce->beyond(switchfunc->cutoffDistance(), 
                                                     aabox0, group1.aaBox());
            
            if (not within_cutoff)
                //this CutGroup is beyond the cutoff distance
                continue;
            
            //calculate all of the interatomic distances
            const double mindist = spce->calcDistVectors(group0, group1, distmat);
            
            if (mindist > switchfunc->cutoffDistance())
                //all of the atoms are definitely beyond cutoff
                continue;

            const quint32 nats1 = group1.count();

            //get the table that holds all of the forces acting on the
            //atoms of this CutGroup
            BOOST_ASSERT(forces1_array[jgroup].count() == int(nats1));
            
            Vector *group_forces1_array = forces1.data(jgroup);
            
            //loop over all interatomic pairs and calculate the energies
            const Parameter *params1_array = params1.constData();

            if (mindist > switchfunc->featherDistance())
            {
                //we need to calculate the forces taking into account
                //the derivative of the switching function!
                
                //calculate the switching scale factors and their 
                //derivatives
                const double scl_coul = switchfunc->electrostaticScaleFactor(mindist);
                const double scl_lj = switchfunc->vdwScaleFactor(mindist);
                
                Vector group_sep = (group1.aaBox().center() - aabox0.center())
                                        .normalise();
                
                Vector dscl_coul = switchfunc->dElectrostaticScaleFactor(mindist) 
                                     * group_sep;
                                     
                Vector dscl_lj = switchfunc->dVDWScaleFactor(mindist)
                                     * group_sep;
                
                double shift_coul = 0;

                if (use_electrostatic_shifting)
                    shift_coul = this->totalCharge(params0) * this->totalCharge(params1)
                                    / switchfunc->electrostaticCutoffDistance();
                
                for (quint32 i=0; i<nats0; ++i)
                {
                    distmat.setOuterIndex(i);
                    const Parameter &param0 = params0_array[i];
                
                    if (param0.ljid == 0)
                    {
                        //null LJ parameter - only add on the coulomb energy
                        for (quint32 j=0; j<nats1; ++j)
                        {
                            const double q2 = param0.reduced_charge *
                                              params1_array[j].reduced_charge;
                            
                            if (q2 != 0)
                            {
                                //calculate the coulomb energy
                                const double cnrg = q2 / distmat[j].length();
                                               
                                //calculate the coulomb force
                                Vector cforce = (scl_coul * -cnrg / distmat[j].length() *
                                                 distmat[j].direction()) +
                                             
                                                ((cnrg-shift_coul) * dscl_coul);

                                //add this force onto the tables for the two atoms
                                cforce *= scale_force;
                        
                                group_forces0_array[i] -= cforce;
                                group_forces1_array[j] += cforce;
                            }
                        }
                    }
                    else
                    {
                        for (quint32 j=0; j<nats1; ++j)
                        {
                            //do both coulomb and LJ
                            const Parameter &param1 = params1_array[j];
                        
                            const double invdist = double(1) / distmat[j].length();
                        
                            Vector force;
                            
                            const double q2 = param0.reduced_charge *
                                              param1.reduced_charge;
                        
                            if (q2 != 0)
                            {
                                //calculate the energy
                                const double cnrg = q2 * invdist;
                        
                                //calculate the force
                                force = (scl_coul * -cnrg / distmat[j].length() *
                                         distmat[j].direction()) +
                                             
                                         ((cnrg-shift_coul) * dscl_coul);
                            }
                              
                            if (param1.ljid != 0)
                            {
                                const LJPair &ljpair = ljpairs.constData()[
                                                        ljpairs.map(param0.ljid,
                                                                    param1.ljid)];
                        
                                double sig_over_dist6 = pow_6(ljpair.sigma()*invdist);
                                double sig_over_dist12 = pow_2(sig_over_dist6);

                                //calculate the energy
                                const double ljnrg = ljpair.epsilon() *
                                                      (sig_over_dist12 - sig_over_dist6);

                                // dU/dr requires an extra power of r
                                sig_over_dist6 *= invdist;
                                sig_over_dist12 *= invdist;

                                force += ((scl_lj * ljpair.epsilon() * 
                                            (6.0*sig_over_dist6 - 12.0*sig_over_dist12))
                                            * distmat[j].direction())
                                            
                                          + (ljnrg * dscl_lj);
                            }
                        
                            //add the force onto the tables
                            force *= scale_force;
                        
                            group_forces0_array[i] -= force;
                            group_forces1_array[j] += force;
                        }
                    }
                }
            }
            else
            {
                //not in the feather region, so can calculate the forces
                //directly (also, no need to calculate shift, as 
                //the shifting function is constant, so does not
                //affect the gradient)
                for (quint32 i=0; i<nats0; ++i)
                {
                    distmat.setOuterIndex(i);
                    const Parameter &param0 = params0_array[i];
                
                    if (param0.ljid == 0)
                    {
                        //null LJ parameter - only add on the coulomb energy
                        for (quint32 j=0; j<nats1; ++j)
                        {
                            //calculate the coulomb force
                            Vector cforce = -(param0.reduced_charge *
                                              params1_array[j].reduced_charge / 
                                              distmat[j].length2()) *
                                             
                                              distmat[j].direction();

                            //add this force onto the tables for the two atoms
                            cforce *= scale_force;
                        
                            group_forces0_array[i] -= cforce;
                            group_forces1_array[j] += cforce;
                        }
                    }
                    else
                    {
                        for (quint32 j=0; j<nats1; ++j)
                        {
                            //do both coulomb and LJ
                            const Parameter &param1 = params1_array[j];
                        
                            const double invdist = double(1) / distmat[j].length();
                            const double invdist2 = pow_2(invdist);
                        
                            //calculate the force
                            Vector force = -(param0.reduced_charge * 
                                             param1.reduced_charge * invdist2) 
                                            
                                            * distmat[j].direction();
                              
                            if (param1.ljid != 0)
                            {
                                const LJPair &ljpair = ljpairs.constData()[
                                                        ljpairs.map(param0.ljid,
                                                                    param1.ljid)];
                        
                                double sig_over_dist6 = pow_6(ljpair.sigma()*invdist);
                                double sig_over_dist12 = pow_2(sig_over_dist6);

                                // dU/dr requires an extra power of r
                                sig_over_dist6 *= invdist;
                                sig_over_dist12 *= invdist;

                                force += (ljpair.epsilon() * (6.0*sig_over_dist6 - 
                                                              12.0*sig_over_dist12))
                                        * distmat[j].direction();
                            }
                        
                            //add the force onto the tables
                            force *= scale_force;
                        
                            group_forces0_array[i] -= force;
                            group_forces1_array[j] += force;
                        }
                    }
                }
            }
        }
    }
}

/////////////
///////////// Implementation of IntraCLJPotential
/////////////

/** Return the total charge of the parameters for the group in 'params' */
double IntraCLJPotential::totalCharge(
                        const IntraCLJPotential::Parameters::Array &params) const
{
    int n = params.count();
    const Parameter *params_array = params.constData();
    
    double chg = 0;
    
    for (int i=0; i<n; ++i)
    {
        chg += params_array[i].reduced_charge;
    }
    
    return chg;
}

/** Calculate the intramolecular CLJ energy of the passed molecule, and
    add this onto 'energy'. This uses the passed workspace when
    performing the calculation */
void IntraCLJPotential::calculateEnergy(const IntraCLJPotential::Molecule &mol,
                                        IntraCLJPotential::Energy &energy,
                                        IntraCLJPotential::EnergyWorkspace &distmat,
                                        double scale_energy) const
{
    if (scale_energy == 0)
        return;

    const quint32 ngroups = mol.nCutGroups();
    
    const CoordGroup *groups_array = mol.coordinates().constData();
    
    BOOST_ASSERT( mol.parameters().atomicParameters().count() == int(ngroups) );
    const Parameters::Array *molparams_array 
                            = mol.parameters().atomicParameters().constData();

    const CLJNBPairs &nbpairs = mol.parameters().scaleFactors();
    
    double cnrg = 0;
    double ljnrg = 0;
      
    //loop over all pairs of CutGroups in the molecule
    for (quint32 igroup=0; igroup<ngroups; ++igroup)
    {
        const Parameters::Array &params0 = molparams_array[igroup];

        const CoordGroup &group0 = groups_array[igroup];
        const AABox &aabox0 = group0.aaBox();
        const quint32 nats0 = group0.count();
        const Parameter *params0_array = params0.constData();
    
        for (quint32 jgroup=igroup; jgroup<ngroups; ++jgroup)
        {
            const CoordGroup &group1 = groups_array[jgroup];
            const Parameters::Array &params1 = molparams_array[jgroup];

            //check first that these two CoordGroups could be within cutoff
            //(don't test igroup==jgroup as this is the same CutGroup
            // and definitely within cutoff!)
            const bool within_cutoff = (igroup == jgroup) or not
                                        spce->beyond(switchfunc->cutoffDistance(), 
                                                     aabox0, group1.aaBox());
            
            if (not within_cutoff)
                //this CutGroup is beyond the cutoff distance
                continue;
            
            //calculate all of the interatomic distances
            const double mindist = spce->calcDist(group0, group1, distmat);
            
            if (mindist > switchfunc->cutoffDistance())
                //all of the atoms are definitely beyond cutoff
                continue;
                
            //get the non-bonded scale factors for all pairs of atoms
            //between these groups (or within this group, if igroup == jgroup)
            const CLJNBPairs::CGPairs &group_pairs = nbpairs(CGIdx(igroup),
                                                             CGIdx(jgroup));

            double icnrg = 0;
            double iljnrg = 0;
            
            //loop over all intraatomic pairs and calculate the energies
            const quint32 nats1 = group1.count();
            const Parameter *params1_array = params1.constData();
            
            if (group_pairs.isEmpty())
            {
                //there are no scale factors between groups...
                for (quint32 i=0; i<nats0; ++i)
                {
                    distmat.setOuterIndex(i);
                    const Parameter &param0 = params0_array[i];
                
                    if (param0.ljid == 0)
                    {
                        //null LJ parameter - only add on the coulomb energy
                        for (quint32 j=0; j<nats1; ++j)
                        {
                            icnrg += param0.reduced_charge * 
                                     params1_array[j].reduced_charge / distmat[j];
                        }
                    }
                    else
                    {
                        for (quint32 j=0; j<nats1; ++j)
                        {
                            //do both coulomb and LJ
                            const Parameter &param1 = params1_array[j];
                        
                            const double invdist = double(1) / distmat[j];
                        
                            icnrg += param0.reduced_charge * param1.reduced_charge
                                            * invdist;
                              
                            if (param1.ljid != 0)
                            {
                                const LJPair &ljpair = ljpairs.constData()[
                                                       ljpairs.map(param0.ljid,
                                                                   param1.ljid)];
                        
                                double sig_over_dist6 = pow_6(ljpair.sigma()*invdist);
                                double sig_over_dist12 = pow_2(sig_over_dist6);

                                iljnrg += ljpair.epsilon() * (sig_over_dist12 - 
                                                              sig_over_dist6);
                            }
                        }
                    }
                }
            }
            else
            {
                //there are different nb scale factors between
                //the atoms. We need to calculate the energies using
                //them...
                for (quint32 i=0; i<nats0; ++i)
                {
                    distmat.setOuterIndex(i);
                    const Parameter &param0 = params0_array[i];
                
                    if (param0.ljid == 0)
                    {
                        //null LJ parameter - only add on the coulomb energy
                        for (quint32 j=0; j<nats1; ++j)
                        {
                            const CLJScaleFactor &cljscl = group_pairs(i,j);
                            
                            if (cljscl.coulomb() != 0)
                                icnrg += cljscl.coulomb() * 
                                         param0.reduced_charge * 
                                         params1_array[j].reduced_charge / distmat[j];
                        }
                    }
                    else
                    {
                        for (quint32 j=0; j<nats1; ++j)
                        {
                            //do both coulomb and LJ
                            const CLJScaleFactor &cljscl = group_pairs(i,j);

                            if (cljscl.coulomb() != 0 or cljscl.lj() != 0)
                            {
                                const Parameter &param1 = params1_array[j];
                        
                                const double invdist = double(1) / distmat[j];
                        
                                icnrg += cljscl.coulomb() *  
                                         param0.reduced_charge * 
                                         param1.reduced_charge * invdist;
                              
                                if (cljscl.lj() != 0 and param1.ljid != 0)
                                {
                                    const LJPair &ljpair = ljpairs.constData()[
                                                        ljpairs.map(param0.ljid,
                                                                    param1.ljid)];
                        
                                    double sig_over_dist6 = pow_6(ljpair.sigma()*invdist);
                                    double sig_over_dist12 = pow_2(sig_over_dist6);

                                    iljnrg += cljscl.lj() * ljpair.epsilon() * 
                                                (sig_over_dist12 - sig_over_dist6);
                                }
                            }
                        }
                    }
                }
            }
            
            //if this is the same group then half the energies to 
            //correct for double-counting
            if (igroup == jgroup)
            {
                icnrg *= 0.5;
                iljnrg *= 0.5;
            }

            //are we shifting the electrostatic potential?
            if (use_electrostatic_shifting)
                icnrg -= this->totalCharge(params0) * this->totalCharge(params1)
                              / switchfunc->electrostaticCutoffDistance();

            //now add these energies onto the total for the molecule,
            //scaled by any non-bonded feather factor
            if (mindist > switchfunc->featherDistance())
            {
                cnrg += switchfunc->electrostaticScaleFactor(mindist) * icnrg;
                ljnrg += switchfunc->vdwScaleFactor(mindist) * iljnrg;
            }
            else
            {
                cnrg += icnrg;
                ljnrg += iljnrg;
            }
        }
    }
    
    //add this molecule pair's energy onto the total
    energy += Energy(scale_energy * cnrg, scale_energy * ljnrg);
}


/** Calculate the coulomb and LJ forces between the atoms in the molecule 'mol'
    and add these forces onto 'forces'. This uses
    the passed workspace to perform the calculation */
void IntraCLJPotential::calculateForce(const IntraCLJPotential::Molecule &mol,
                                       MolForceTable &forces, 
                                       IntraCLJPotential::ForceWorkspace &distmat,
                                       double scale_force) const
{
    if (scale_force == 0)
        return;
    
    const quint32 ngroups = mol.nCutGroups();
    
    const CoordGroup *groups_array = mol.coordinates().constData();
    
    const Parameters::Array *molparams_array 
                            = mol.parameters().atomicParameters().constData();
    
    BOOST_ASSERT(forces.count() == int(ngroups));
    
    const MolForceTable::Array *forces_array = forces.constData();

    const CLJNBPairs &nbpairs = mol.parameters().scaleFactors();
    
    //loop over all pairs of CutGroups in the molecule
    for (quint32 igroup=0; igroup<ngroups; ++igroup)
    {
        const Parameters::Array &params0 = molparams_array[igroup];

        const CoordGroup &group0 = groups_array[igroup];
        const AABox &aabox0 = group0.aaBox();
        const quint32 nats0 = group0.count();
        const Parameter *params0_array = params0.constData();
    
        //get the table that holds the forces acting on all of the
        //atoms of this CutGroup
        BOOST_ASSERT(forces_array[igroup].count() == int(nats0));
        Vector *group_forces0_array = forces.data(igroup);
    
        for (quint32 jgroup=igroup; jgroup<ngroups; ++jgroup)
        {
            const CoordGroup &group1 = groups_array[jgroup];
            const Parameters::Array &params1 = molparams_array[jgroup];

            //check first that these two CoordGroups could be within cutoff
            const bool outside_cutoff = spce->beyond(switchfunc->cutoffDistance(), 
                                                     aabox0, group1.aaBox());
            
            if (outside_cutoff)
                //this CutGroup is beyond the cutoff distance
                continue;
            
            //calculate all of the interatomic distances
            const double mindist = spce->calcDistVectors(group0, group1, distmat);
            
            if (mindist > switchfunc->cutoffDistance())
                //all of the atoms are definitely beyond cutoff
                continue;

            const quint32 nats1 = group1.count();

            //get the table that holds all of the forces acting on the
            //atoms of this CutGroup
            BOOST_ASSERT(forces_array[jgroup].count() == int(nats1));
            Vector *group_forces1_array = forces.data(jgroup);
            
            //loop over all interatomic pairs and calculate the energies
            const Parameter *params1_array = params1.constData();

            //get the non-bonded scale factors for all pairs of atoms
            //between these two groups (or within this group, if igroup == jgroup)
            const CLJNBPairs::CGPairs &group_pairs = nbpairs(CGIdx(igroup),
                                                             CGIdx(jgroup));

            if (mindist > switchfunc->featherDistance())
            {
                //we need to calculate the forces taking into account
                //the derivative of the switching function!
                
                //calculate the switching scale factors and their 
                //derivatives
                const double scl_coul = switchfunc->electrostaticScaleFactor(mindist);
                const double scl_lj = switchfunc->vdwScaleFactor(mindist);
                
                Vector group_sep = (group1.aaBox().center() -
                                    group0.aaBox().center()).normalise(); 
                
                Vector dscl_coul = switchfunc->dElectrostaticScaleFactor(mindist) 
                                     * group_sep;
                                     
                Vector dscl_lj = switchfunc->dVDWScaleFactor(mindist)
                                     * group_sep;
                
                double shift_coul = 0;

                if (use_electrostatic_shifting and igroup != jgroup)
                    shift_coul = this->totalCharge(params0) * this->totalCharge(params1)
                              / switchfunc->electrostaticCutoffDistance();

                if (group_pairs.isEmpty())
                {
                    //there are no scale factors between atoms in these groups
                    for (quint32 i=0; i<nats0; ++i)
                    {
                        distmat.setOuterIndex(i);
                        const Parameter &param0 = params0_array[i];
                
                        if (param0.ljid == 0)
                        {
                            //null LJ parameter - only add on the coulomb energy
                            for (quint32 j=0; j<nats1; ++j)
                            {
                                const double q2 = param0.reduced_charge *
                                                  params1_array[j].reduced_charge;
                                
                                if (q2 != 0)
                                {
                                    //calculate the coulomb energy
                                    const double cnrg = scl_coul * q2 /
                                                        distmat[j].length();

                                    //calculate the coulomb force
                                    Vector cforce = (-cnrg / distmat[j].length() *
                                                     distmat[j].direction()) +
                                             
                                                    ((cnrg-shift_coul) * dscl_coul);

                                    //add this force onto the tables for the two atoms
                                    cforce *= scale_force;
                            
                                    //if this is the same group, then must scale the 
                                    //force by half to prevent double-counting
                                    if (igroup == jgroup)
                                        cforce *= 0.5;
                            
                                    group_forces0_array[i] -= cforce;
                                    group_forces1_array[j] += cforce;
                                }
                            }
                        }
                        else
                        {
                            for (quint32 j=0; j<nats1; ++j)
                            {
                                //do both coulomb and LJ
                                const Parameter &param1 = params1_array[j];
                        
                                const double invdist = double(1) / distmat[j].length();
                        
                                Vector force;

                                const double q2 = param0.reduced_charge *
                                                  param1.reduced_charge;
                                
                                if (q2 != 0)
                                {
                                    //calculate the energy
                                    const double cnrg = scl_coul * q2 * invdist;

                                    //calculate the force
                                    force = (-cnrg / distmat[j].length() *
                                              distmat[j].direction()) +
                                             
                                              ((cnrg-shift_coul) * dscl_coul);
                                }
                              
                                if (param1.ljid != 0)
                                {
                                    const LJPair &ljpair = ljpairs.constData()[
                                                           ljpairs.map(param0.ljid,
                                                                        param1.ljid)];
                        
                                    double sig_over_dist6 = pow_6(ljpair.sigma()*invdist);
                                    double sig_over_dist12 = pow_2(sig_over_dist6);

                                    //calculate the energy
                                    const double ljnrg = ljpair.epsilon() *
                                                      (sig_over_dist12 - sig_over_dist6);

                                    // dU/dr requires an extra power of r
                                    sig_over_dist6 *= invdist;
                                    sig_over_dist12 *= invdist;

                                    force += ((scl_lj * ljpair.epsilon() * 
                                            (6.0*sig_over_dist6 - 12.0*sig_over_dist12))
                                            * distmat[j].direction())
                                            
                                            + (ljnrg * dscl_lj);
                                }
                        
                                //add the force onto the tables
                                force *= scale_force;
                        
                                //if this is the same group then half the force
                                //to prevent double counting
                                if (igroup == jgroup)
                                    force *= 0.5;
                            
                                group_forces0_array[i] -= force;
                                group_forces1_array[j] += force;
                            }
                        }
                    }
                }
                else
                {
                    //there are different nb scale factors between
                    //the atoms. We need to calculate the forces
                    //using them...
                    for (quint32 i=0; i<nats0; ++i)
                    {
                        distmat.setOuterIndex(i);
                        const Parameter &param0 = params0_array[i];
                
                        if (param0.ljid == 0)
                        {
                            //null LJ parameter - only add on the coulomb energy
                            for (quint32 j=0; j<nats1; ++j)
                            {
                                const CLJScaleFactor &cljscl = group_pairs(i,j);
                            
                                if (cljscl.coulomb() != 0)
                                {
                                    const double q2 = param0.reduced_charge * 
                                                      params1_array[j].reduced_charge;
                                                      
                                    if (q2 != 0)
                                    {
                                        //calculate the coulomb energy
                                        const double cnrg = cljscl.coulomb() *
                                                            scl_coul * q2
                                                            / distmat[j].length();
                                               
                                        //calculate the coulomb force
                                        Vector cforce = (-cnrg 
                                                        / distmat[j].length() *
                                                        distmat[j].direction()) +
                                             
                                                        ((cnrg-shift_coul) * dscl_coul);

                                        //add this force onto the tables 
                                        //for the two atoms
                                        cforce *= scale_force;
                            
                                        //if this is the same group, then must scale the 
                                        //force by half to prevent double-counting
                                        if (igroup == jgroup)
                                            cforce *= 0.5;
                            
                                        group_forces0_array[i] -= cforce;
                                        group_forces1_array[j] += cforce;
                                    }
                                }
                            }
                        }
                        else
                        {
                            for (quint32 j=0; j<nats1; ++j)
                            {
                                //do both coulomb and LJ
                                const CLJScaleFactor &cljscl = group_pairs(i,j);
                                
                                if (cljscl.coulomb() != 0 or cljscl.lj() != 0)
                                {
                                    const Parameter &param1 = params1_array[j];
                        
                                    const double invdist = double(1) 
                                                               / distmat[j].length();
                        
                                    Vector force;
                                    
                                    const double q2 = param0.reduced_charge *
                                                      param1.reduced_charge;

                                    if (q2 != 0)
                                    {
                                        //calculate the energy
                                        const double cnrg = cljscl.coulomb() *
                                                            scl_coul * q2 * invdist;
                        
                                        //calculate the force
                                        force = (scl_coul * -cnrg 
                                                    / distmat[j].length() *
                                                 distmat[j].direction()) +
                                             
                                                 ((cnrg-shift_coul) * dscl_coul);
                                    }
                              
                                    if (cljscl.lj() != 0 and param1.ljid != 0)
                                    {
                                        const LJPair &ljpair = ljpairs.constData()[
                                                            ljpairs.map(param0.ljid,
                                                                        param1.ljid)];
                        
                                        double sig_over_dist6 = pow_6(ljpair.sigma()*invdist);
                                        double sig_over_dist12 = pow_2(sig_over_dist6);

                                        //calculate the energy
                                        const double ljnrg = cljscl.lj() *
                                                             ljpair.epsilon() *
                                                      (sig_over_dist12 - sig_over_dist6);

                                        // dU/dr requires an extra power of r
                                        sig_over_dist6 *= invdist;
                                        sig_over_dist12 *= invdist;

                                        force += ((scl_lj * ljpair.epsilon() * 
                                            (6.0*sig_over_dist6 - 12.0*sig_over_dist12))
                                            * distmat[j].direction())
                                            
                                            + (ljnrg * dscl_lj);
                                    }
                        
                                    //add the force onto the tables
                                    force *= scale_force;
                        
                                    //if this is the same group then half the force
                                    //to prevent double counting
                                    if (igroup == jgroup)
                                        force *= 0.5;
                            
                                    group_forces0_array[i] -= force;
                                    group_forces1_array[j] += force;
                                }
                            }
                        }
                    }
                }
            }
            else
            {
                //not in the feather region, so can calculate the forces
                //directly (also, no need to calculate shift, as 
                //the shifting function is constant, so does not
                //affect the gradient)
                
                if (group_pairs.isEmpty())
                {
                    //no nb scale factors to worry about
                    for (quint32 i=0; i<nats0; ++i)
                    {
                        distmat.setOuterIndex(i);
                        const Parameter &param0 = params0_array[i];
                
                        if (param0.ljid == 0)
                        {
                            //null LJ parameter - only add on the coulomb energy
                            for (quint32 j=0; j<nats1; ++j)
                            {
                                //calculate the coulomb force
                                Vector cforce = -(param0.reduced_charge *
                                                  params1_array[j].reduced_charge / 
                                                  distmat[j].length2()) *
                                             
                                                  distmat[j].direction();

                                //add this force onto the tables for the two atoms
                                cforce *= scale_force;
                        
                                //if this is the same group then half the force
                                //to prevent double counting
                                if (igroup == jgroup)
                                    cforce *= 0.5;

                                group_forces0_array[i] -= cforce;
                                group_forces1_array[j] += cforce;
                            }
                        }
                        else
                        {
                            for (quint32 j=0; j<nats1; ++j)
                            {
                                //do both coulomb and LJ
                                const Parameter &param1 = params1_array[j];
                            
                                const double invdist = double(1) / distmat[j].length();
                                const double invdist2 = pow_2(invdist);
                        
                                //calculate the force
                                Vector force = -(param0.reduced_charge * 
                                                 param1.reduced_charge * invdist2) 
                                            
                                                * distmat[j].direction();
                              
                                if (param1.ljid != 0)
                                {
                                    const LJPair &ljpair = ljpairs.constData()[
                                                           ljpairs.map(param0.ljid,
                                                                    param1.ljid)];
                        
                                    double sig_over_dist6 = pow_6(ljpair.sigma()*invdist);
                                    double sig_over_dist12 = pow_2(sig_over_dist6);

                                    // dU/dr requires an extra power of r
                                    sig_over_dist6 *= invdist;
                                    sig_over_dist12 *= invdist;

                                    force += (ljpair.epsilon() * (6.0*sig_over_dist6 - 
                                                                  12.0*sig_over_dist12))
                                            * distmat[j].direction();
                                }
                        
                                //add the force onto the tables
                                force *= scale_force;

                                //if this is the same group then half the force
                                //to prevent double counting
                                if (igroup == jgroup)
                                    force *= 0.5;

                                group_forces0_array[i] -= force;
                                group_forces1_array[j] += force;
                            }
                        }
                    }
                }
                else
                {
                    //there are different nb scale factors between
                    //different atoms...
                    for (quint32 i=0; i<nats0; ++i)
                    {
                        distmat.setOuterIndex(i);
                        const Parameter &param0 = params0_array[i];
                
                        if (param0.ljid == 0)
                        {
                            //null LJ parameter - only add on the coulomb energy
                            for (quint32 j=0; j<nats1; ++j)
                            {
                                const CLJScaleFactor &cljscl = group_pairs(i,j);
                            
                                if (cljscl.coulomb() != 0)
                                {
                                    //calculate the coulomb force
                                    Vector cforce = -(cljscl.coulomb() *
                                                      param0.reduced_charge *
                                                      params1_array[j].reduced_charge / 
                                                      distmat[j].length2()) *
                                             
                                                      distmat[j].direction();

                                    //add this force onto the tables for the two atoms
                                    cforce *= scale_force;
                        
                                    //if this is the same group then half the force
                                    //to prevent double counting
                                    if (igroup == jgroup)
                                        cforce *= 0.5;

                                    group_forces0_array[i] -= cforce;
                                    group_forces1_array[j] += cforce;
                                }
                            }
                        }
                        else
                        {
                            for (quint32 j=0; j<nats1; ++j)
                            {
                                //do both coulomb and LJ
                                const CLJScaleFactor &cljscl = group_pairs(i,j);
                                
                                if (cljscl.coulomb() != 0 or cljscl.lj() != 0)
                                {
                                    const Parameter &param1 = params1_array[j];
                            
                                    const double invdist = double(1)    
                                                            / distmat[j].length();
                                    const double invdist2 = pow_2(invdist);
                        
                                    //calculate the force
                                    Vector force = -(cljscl.coulomb() *
                                                     param0.reduced_charge * 
                                                     param1.reduced_charge * invdist2) 
                                            
                                                    * distmat[j].direction();
                              
                                    if (cljscl.lj() != 0 and param1.ljid != 0)
                                    {
                                        const LJPair &ljpair = ljpairs.constData()[
                                                               ljpairs.map(param0.ljid,
                                                                    param1.ljid)];
                        
                                        double sig_over_dist6 = pow_6(ljpair.sigma()*invdist);
                                        double sig_over_dist12 = pow_2(sig_over_dist6);

                                        // dU/dr requires an extra power of r
                                        sig_over_dist6 *= invdist;
                                        sig_over_dist12 *= invdist;

                                        force += (cljscl.lj() *
                                                  ljpair.epsilon() * (6.0*sig_over_dist6 - 
                                                                      12.0*sig_over_dist12))
                                                * distmat[j].direction();
                                    }
                        
                                    //add the force onto the tables
                                    force *= scale_force;

                                    //if this is the same group then half the force
                                    //to prevent double counting
                                    if (igroup == jgroup)
                                        force *= 0.5;

                                    group_forces0_array[i] -= force;
                                    group_forces1_array[j] += force;
                                }
                            }
                        }
                    }
                }
            }
        }
    }
}
