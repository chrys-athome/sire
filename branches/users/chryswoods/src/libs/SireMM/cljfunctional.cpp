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

/** Calculate the coulomb and LJ energy between the passed pair
    of molecules and add these energies onto 'energy'. This uses
    the passed workspace to perform the calculation */
void CLJPotential::_pvt_calculateEnergy(const CLJPotential::Molecule &mol0,
                                        const CLJPotential::Molecule &mol1,
                                        CLJPotential::Energy &energy,
                                        CLJPotential::Workspace &distmat,
                                        double scale_energy) const
{
    const quint32 ngroups0 = mol0.nCutGroups();
    const quint32 ngroups1 = mol1.nCutGroups();
    
    const CoordGroup *groups0_array = mol0.coordGroups().constData();
    const CoordGroup *groups1_array = mol1.coordGroups().constData();
    
    const CGParams *molparams0_array = mol0.parameters().constData();
    const CGParams *molparams1_array = mol1.parameters().constData();
    
    double cnrg = 0;
    double ljnrg = 0;
    
    //loop over all pairs of CutGroups in the two molecules
    for (quint32 igroup=0; igroup<ngroups0; ++igroup)
    {
        const CGParams &params0 = molparams0_array[igroup];

        if (params0.isNull())
            //all of the atoms in this CutGroup are dummies
            continue;

        const CoordGroup &group0 = groups0_array[igroup];
        const AABox &aabox0 = group0.aaBox();
        const quint32 nats0 = group0.count();
        const Parameter *params0_array = params0.constData();
    
        for (quint32 jgroup=0; jgroup<ngroups1; ++jgroup)
        {
            const CoordGroup &group1 = groups1_array[jgroup];
            const CGParams &params1 = molparams1_array[jgroup];

            //check first that these two CoordGroups could be within cutoff
            //(if there is only one CutGroup in both molecules then this
            //test has already been performed and passed)
            const bool within_cutoff = (ngroups0 == 1 and ngroups1 == 1) or not
                                        space->beyond(switchfunc->cutoffDistance(), 
                                                      aabox0, group1.aaBox());
            
            if (not within_cutoff or params1.isNull())
                //this CutGroup is either beyond the cutoff distance
                //or all of the atoms are dummies
                continue;
            
            //calculate all of the interatomic distances
            const double mindist = space->calcDist(group0, group1, distmat);
            
            if (mindist > switchfunc->cutoffDistance())
                //all of the atoms are definitely beyond cutoff
                continue;
                
            double icnrg = 0;
            double iljnrg = 0;
            
            //loop over all interatomic pairs and calculate the energies
            const quint32 nats1 = group1.count();
            const Parameter *params1_array = params1_constData();
            
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
                                 params1_array.reduced_charge / distmat[j];
                    }
                }
                else
                {
                    for (quint32 j=0; j<nats2; ++j)
                    {
                        //do both coulomb and LJ
                        const Parameter &param1 = params1_array[j];
                        
                        const double invdist = double(1) / distmat[j];
                        
                        icnrg += param0.reduced_charge * param1.reduced_chg 
                                      * invdist;
                              
                        if (param1.ljid != 0)
                        {
                            const LJPair &ljpair = ljpairs.constData()[
                                                      ljpairs.map(param0.ljid,
                                                                  param1.ljid)];
                        
                            double sig_over_dist6 = pow_6(ljpair.sigma*invdist);
                            double sig_over_dist12 = pow_2(sig_over_dist6);

                            iljnrg += ljpair.epsilon * (sig_over_dist12 - 
                                                        sig_over_dist6);
                        }
                    }
                }
            }
            
            //are we shifting the electrostatic potential?
            if (use_electrostatic_shifting)
                icnrg -= params0.totalCharge() * params1.totalCharge() * 
                              switchfunc->invCutoffDistance();
            
            //now add these energies onto the total for the molecule,
            //scaled by any non-bonded feather factor
            if (mindist > switchfunc->featherDistance())
            {
                cnrg += switchfunc->coulombScaleFactor(mindist) * icnrg;
                ljnrg += switchfunc->ljScaleFactor(mindist) * iljnrg;
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

/** Calculate the intramolecular CLJ energy of the passed molecule, and
    add this onto 'energy'. This uses the passed workspace when
    performing the calculation */
void CLJPotential::calculateEnergy(const CLJPotential::Molecule &mol,
                                   CLJPotential::Energy &energy,
                                   CLJPotential::Workspace &distmat,
                                   double scale_energy) const
{
    if (scale_energy == 0)
        return;

    const quint32 ngroups = mol.nCutGroups();
    
    const CoordGroup *groups_array = mol.coordGroups().constData();
    const CGParams *molparams_array = mol.parameters().constData();

    const CLJNBPairs &nbpairs = mol.nbScale();
    
    double cnrg = 0;
    double ljnrg = 0;
      
    //loop over all pairs of CutGroups in the molecule
    for (quint32 igroup=0; igroup<ngroups; ++igroup)
    {
        const CGParams &params0 = molparams_array[igroup];

        if (params0.isNull())
            //all of the atoms in this CutGroup are dummies
            continue;

        const CoordGroup &group0 = groups_array[igroup];
        const AABox &aabox0 = group0.aaBox();
        const quint32 nats0 = group0.count();
        const Parameter *params0_array = params0.constData();
    
        for (quint32 jgroup=igroup; jgroup<ngroups; ++jgroup)
        {
            const CoordGroup &group1 = groups_array[jgroup];
            const CGParams &params1 = molparams_array[jgroup];

            //check first that these two CoordGroups could be within cutoff
            //(don't test igroup==jgroup as this is the same CutGroup
            // and definitely within cutoff!)
            const bool within_cutoff = (igroup == jgroup) or not
                                        space->beyond(switchfunc->cutoffDistance(), 
                                                      aabox0, group1.aaBox());
            
            if (not within_cutoff or params1.isNull())
                //this CutGroup is either beyond the cutoff distance
                //or all of the atoms are dummies
                continue;
            
            //calculate all of the interatomic distances
            const double mindist = space->calcDist(group0, group1, distmat);
            
            if (mindist > switchfunc->cutoffDistance())
                //all of the atoms are definitely beyond cutoff
                continue;
                
            //get the non-bonded scale factors for all pairs of atoms
            //between these groups (or within this group, if igroup == jgroup)
            const GroupCLJNBPairs &group_pairs = nbpairs.get(igroup,jgroup);

            double icnrg = 0;
            double iljnrg = 0;
            
            //loop over all intraatomic pairs and calculate the energies
            const quint32 nats1 = group1.count();
            const Parameter *params1_array = params1_constData();
            
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
                                     params1_array.reduced_charge / distmat[j];
                        }
                    }
                    else
                    {
                        for (quint32 j=0; j<nats2; ++j)
                        {
                            //do both coulomb and LJ
                            const Parameter &param1 = params1_array[j];
                        
                            const double invdist = double(1) / distmat[j];
                        
                            icnrg += param0.reduced_charge * param1.reduced_chg 
                                            * invdist;
                              
                            if (param1.ljid != 0)
                            {
                                const LJPair &ljpair = ljpairs.constData()[
                                                       ljpairs.map(param0.ljid,
                                                                   param1.ljid)];
                        
                                double sig_over_dist6 = pow_6(ljpair.sigma*invdist);
                                double sig_over_dist12 = pow_2(sig_over_dist6);

                                iljnrg += ljpair.epsilon * (sig_over_dist12 - 
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
                const SparseMatrix<CLJFactor> &cljmat = group_pairs.constData();
                
                for (quint32 i=0; i<nats0; ++i)
                {
                    distmat.setOuterIndex(i);
                    const Parameter &param0 = params0_array[i];
                
                    if (param0.ljid == 0)
                    {
                        //null LJ parameter - only add on the coulomb energy
                        for (quint32 j=0; j<nats1; ++j)
                        {
                            const CLJFactor &cljscl = cljmat.get(i,j);
                            
                            if (cljscl.coulomb != 0)
                                icnrg += cljscl.coulomb * 
                                         param0.reduced_charge * 
                                         params1_array.reduced_charge / distmat[j];
                        }
                    }
                    else
                    {
                        for (quint32 j=0; j<nats2; ++j)
                        {
                            //do both coulomb and LJ
                            const CLJFactor &cljscl = cljmat.get(i,j);

                            if (cljscl.coulomb != 0 or cljscl.lj != 0)
                            {
                                const Parameter &param1 = params1_array[j];
                        
                                const double invdist = double(1) / distmat[j];
                        
                                icnrg += cljscl.coulomb *  
                                         param0.reduced_charge * 
                                         param1.reduced_chg * invdist;
                              
                                if (cljscl.lj != 0 and param1.ljid != 0)
                                {
                                    const LJPair &ljpair = ljpairs.constData()[
                                                        ljpairs.map(param0.ljid,
                                                                    param1.ljid)];
                        
                                    double sig_over_dist6 = pow_6(ljpair.sigma*invdist);
                                    double sig_over_dist12 = pow_2(sig_over_dist6);

                                    iljnrg += cljscl.lj * ljpair.epsilon * 
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
                icnrg -= params0.totalCharge() * 
                         params1.totalCharge() * 
                         switchfunc->invCutoffDistance();

            //now add these energies onto the total for the molecule,
            //scaled by any non-bonded feather factor
            if (mindist > switchfunc->featherDistance())
            {
                cnrg += switchfunc->coulombScaleFactor(mindist) * icnrg;
                ljnrg += switchfunc->ljScaleFactor(mindist) * iljnrg;
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

//force workspace is a PairMatrix< DistVector >

// A DistVector is a struct{ double distance; Vector unit_vector; }

// Holds a unit vector that points from point0 to point1, and the 
// distance between those two points

// A DistVector could be a derived class of Vector, using the spacer 
// to hold the distance?

/** Calculate the coulomb and LJ forces on the atoms between the passed pair
    of molecules and add these energies onto 'forces'. This uses
    the passed workspace to perform the calculation */
void CLJPotential::_pvt_calculateForce(const Molecule &mol0, const Molecule &mol1,
                                       MolForceTable &forces0, 
                                       MolForceTable &forces1,
                                       Workspace &workspace,
                                       double scale_force) const
{
    const quint32 ngroups0 = mol0.nCutGroups();
    const quint32 ngroups1 = mol1.nCutGroups();
    
    const CoordGroup *groups0_array = mol0.coordGroups().constData();
    const CoordGroup *groups1_array = mol1.coordGroups().constData();
    
    const CGParams *molparams0_array = mol0.parameters().constData();
    const CGParams *molparams1_array = mol1.parameters().constData();
    
    BOOST_ASSERT(forces0.count() == ngroups0);
    BOOST_ASSERT(forces1.count() == ngroups1);
    
    GroupForceTable *forces0_array = forces0.data();
    GroupForceTable *forces1_array = forces1.data();
    
    //loop over all pairs of CutGroups in the two molecules
    for (quint32 igroup=0; igroup<ngroups0; ++igroup)
    {
        const CGParams &params0 = molparams0_array[igroup];

        if (params0.isNull())
            //all of the atoms in this CutGroup are dummies
            continue;

        const CoordGroup &group0 = groups0_array[igroup];
        const AABox &aabox0 = group0.aaBox();
        const quint32 nats0 = group0.count();
        const Parameter *params0_array = params0.constData();
    
        //get the table that holds the forces acting on all of the
        //atoms of this CutGroup
        GroupForceTable &group_forces0 = forces0_array[igroup];
        BOOST_ASSERT(group_forces0.count() == nats0);
    
        Vector *group_forces0_array = group_forces0.data();
    
        for (quint32 jgroup=0; jgroup<ngroups1; ++jgroup)
        {
            const CoordGroup &group1 = groups1_array[jgroup];
            const CGParams &params1 = molparams1_array[jgroup];

            //check first that these two CoordGroups could be within cutoff
            //(if there is only one CutGroup in both molecules then this
            //test has already been performed and passed)
            const bool within_cutoff = (ngroups0 == 1 and ngroups1 == 1) or not
                                        space->beyond(switchfunc->cutoffDistance(), 
                                                      aabox0, group1.aaBox());
            
            if (not within_cutoff or params1.isNull())
                //this CutGroup is either beyond the cutoff distance
                //or all of the atoms are dummies
                continue;
            
            //calculate all of the interatomic distances
            const double mindist = space->calcDist(group0, group1, distmat);
            
            if (mindist > switchfunc->cutoffDistance())
                //all of the atoms are definitely beyond cutoff
                continue;

            const quint32 nats1 = group1.count();

            //get the table that holds all of the forces acting on the
            //atoms of this CutGroup
            GroupForceTable &group_forces1 = forces1_array[jgroup];
            BOOST_ASSERT(group_forces1.count() == nats1);
            
            Vector *group_forces1_array = group_forces1.data();
            
            //loop over all interatomic pairs and calculate the energies
            const Parameter *params1_array = params1_constData();

            if (mindist > switchfunc->featherDistance())
            {
                //we need to calculate the forces taking into account
                //the derivative of the switching function!
                
                //calculate the switching scale factors and their 
                //derivatives
                const double scl_coul = switchfunc->coulombScaleFactor(mindist);
                const double scl_lj = switchfunc->ljScaleFactor(mindist);
                
                Vector group_sep = (aabox0.center() - group1.aaBox().center())
                                        .normalise();
                
                Vector dscl_coul = switchfunc->dCoulombScaleFactor(mindist) 
                                     * group_sep;
                                     
                Vector dscl_lj = switchfunc->dLJScaleFactor(mindist)
                                     * group_sep;
                
                double shift_coul = 0;

                if (use_electrostatic_shifting)
                    shift_coul = params0.totalCharge() * params1.totalCharge()
                                    * switchfunc->invCutoffDistance();
                
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
                                                 distmat[j].unitVector()) +
                                             
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
                        for (quint32 j=0; j<nats2; ++j)
                        {
                            //do both coulomb and LJ
                            const Parameter &param1 = params1_array[j];
                        
                            const double invdist = double(1) / distmat[j].length();
                            const double invdist2 = pow_2(invdist);
                        
                            Vector force;
                            
                            const double q2 = param0.reduced_charge *
                                              param1.reduced_charge;
                        
                            if (q2 != 0)
                            {
                                //calculate the energy
                                const double cnrg = q2 * invdist;
                        
                                //calculate the force
                                force = (scl_coul * -cnrg / distmat[j].length() *
                                         distmat[j].unitVector()) +
                                             
                                         ((cnrg-shift_coul) * dscl_coul);
                            }
                              
                            if (param1.ljid != 0)
                            {
                                const LJPair &ljpair = ljpairs.constData()[
                                                        ljpairs.map(param0.ljid,
                                                                    param1.ljid)];
                        
                                double sig_over_dist6 = pow_6(ljpair.sigma*invdist);
                                double sig_over_dist12 = pow_2(sig_over_dist6);

                                //calculate the energy
                                const double ljnrg = ljpair.epsilon() *
                                                      (sig_over_dist12 - sig_over_dist6);

                                // dU/dr requires an extra power of r
                                sig_over_dist6 *= invdist;
                                sig_over_dist12 *= invdist;

                                force += ((scl_lj * ljpair.epsilon * 
                                            (6.0*sig_over_dist6 - 12.0*sig_over_dist12))
                                            * distmat[j].unitVector())
                                            
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
                                             
                                              distmat[j].unitVector();

                            //add this force onto the tables for the two atoms
                            cforce *= scale_force;
                        
                            group_forces0_array[i] -= cforce;
                            group_forces1_array[j] += cforce;
                        }
                    }
                    else
                    {
                        for (quint32 j=0; j<nats2; ++j)
                        {
                            //do both coulomb and LJ
                            const Parameter &param1 = params1_array[j];
                        
                            const double invdist = double(1) / distmat[j].length();
                            const double invdist2 = pow_2(invdist);
                        
                            //calculate the force
                            Vector force = -(param0.reduced_charge * 
                                             param1.reduced_charge * invdist2) 
                                            
                                            * distmat[j].vector();
                              
                            if (param1.ljid != 0)
                            {
                                const LJPair &ljpair = ljpairs.constData()[
                                                        ljpairs.map(param0.ljid,
                                                                    param1.ljid)];
                        
                                double sig_over_dist6 = pow_6(ljpair.sigma*invdist);
                                double sig_over_dist12 = pow_2(sig_over_dist6);

                                // dU/dr requires an extra power of r
                                sig_over_dist6 *= invdist;
                                sig_over_dist12 *= invdist;

                                force += (ljpair.epsilon * (6.0*sig_over_dist6 - 
                                                            12.0*sig_over_dist12))
                                        * distmat[j].unitVector();
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

/** Calculate the coulomb and LJ forces between the atoms in the molecule 'mol'
    and add these forces onto 'forces'. This uses
    the passed workspace to perform the calculation */
void CLJPotential::calculateForce(const Molecule &mol,
                                  MolForceTable &forces, 
                                  Workspace &workspace,
                                  double scale_force) const
{
    if (scale_forces == 0)
        return;
    
    const quint32 ngroups = mol.nCutGroups();
    
    const CoordGroup *groups_array = mol.coordGroups().constData();
    
    const CGParams *molparams_array = mol.parameters().constData();
    
    BOOST_ASSERT(forces.count() == ngroups);
    
    GroupForceTable *forces_array = forces.data();

    const CLJNBPairs &nbpairs = mol.nbScale();
    
    //loop over all pairs of CutGroups in the molecule
    for (quint32 igroup=0; igroup<ngroups; ++igroup)
    {
        const CGParams &params0 = molparams_array[igroup];

        if (params0.isNull())
            //all of the atoms in this CutGroup are dummies
            continue;

        const CoordGroup &group0 = groups_array[igroup];
        const AABox &aabox0 = group0.aaBox();
        const quint32 nats0 = group0.count();
        const Parameter *params0_array = params0.constData();
    
        //get the table that holds the forces acting on all of the
        //atoms of this CutGroup
        GroupForceTable &group_forces0 = forces_array[igroup];
        BOOST_ASSERT(group_forces0.count() == nats0);
    
        Vector *group_forces0_array = group_forces0.data();
    
        for (quint32 jgroup=igroup; jgroup<ngroups; ++jgroup)
        {
            const CoordGroup &group1 = groups_array[jgroup];
            const CGParams &params1 = molparams_array[jgroup];

            //check first that these two CoordGroups could be within cutoff
            const bool outside_cutoff = space->beyond(switchfunc->cutoffDistance(), 
                                                      aabox0, group1.aaBox());
            
            if (outside_cutoff or params1.isNull())
                //this CutGroup is either beyond the cutoff distance
                //or all of the atoms are dummies
                continue;
            
            //calculate all of the interatomic distances
            const double mindist = space->calcDist(group0, group1, distmat);
            
            if (mindist > switchfunc->cutoffDistance())
                //all of the atoms are definitely beyond cutoff
                continue;

            const quint32 nats1 = group1.count();

            //get the table that holds all of the forces acting on the
            //atoms of this CutGroup
            GroupForceTable &group_forces1 = forces1_array[jgroup];
            BOOST_ASSERT(group_forces1.count() == nats1);
            
            Vector *group_forces1_array = group_forces1.data();
            
            //loop over all interatomic pairs and calculate the energies
            const Parameter *params1_array = params1_constData();

            //get the non-bonded scale factors for all pairs of atoms
            //between these two groups (or within this group, if igroup == jgroup)
            const GroupCLJNBPairs &group_pairs = nbpairs.get(igroup,jgroup);

            if (mindist > switchfunc->featherDistance())
            {
                //we need to calculate the forces taking into account
                //the derivative of the switching function!
                
                //calculate the switching scale factors and their 
                //derivatives
                const double scl_coul = switchfunc->coulombScaleFactor(mindist);
                const double scl_lj = switchfunc->ljScaleFactor(mindist);
                
                Vector group_sep = (group1.aaBox().center() -
                                    group0.aaBox().center()).normalise(); 
                
                Vector dscl_coul = switchfunc->dCoulombScaleFactor(mindist) 
                                     * group_sep;
                                     
                Vector dscl_lj = switchfunc->dLJScaleFactor(mindist)
                                     * group_sep;
                
                double shift_coul = 0;

                if (use_electrostatic_shifting)
                    shift_coul = params0.totalCharge() * params1.totalCharge()
                                    * switchfunc->invCutoffDistance();

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
                                                     distmat[j].unitVector()) +
                                             
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
                            for (quint32 j=0; j<nats2; ++j)
                            {
                                //do both coulomb and LJ
                                const Parameter &param1 = params1_array[j];
                        
                                const double invdist = double(1) / distmat[j].length();
                                const double invdist2 = pow_2(invdist);
                        
                                Vector force;

                                const double q2 = param0.reduced_charge *
                                                  param1.reduced_charge;
                                
                                if (q2 != 0)
                                {
                                    //calculate the energy
                                    const double cnrg = scl_coul * q2 * invdist;

                                    //calculate the force
                                    force = (-cnrg / distmat[j].length() *
                                              distmat[j].unitVector()) +
                                             
                                              ((cnrg-shift_coul) * dscl_coul);
                                }
                              
                                if (param1.ljid != 0)
                                {
                                    const LJPair &ljpair = ljpairs.constData()[
                                                           ljpairs.map(param0.ljid,
                                                                        param1.ljid)];
                        
                                    double sig_over_dist6 = pow_6(ljpair.sigma*invdist);
                                    double sig_over_dist12 = pow_2(sig_over_dist6);

                                    //calculate the energy
                                    const double ljnrg = ljpair.epsilon() *
                                                      (sig_over_dist12 - sig_over_dist6);

                                    // dU/dr requires an extra power of r
                                    sig_over_dist6 *= invdist;
                                    sig_over_dist12 *= invdist;

                                    force += ((scl_lj * ljpair.epsilon * 
                                            (6.0*sig_over_dist6 - 12.0*sig_over_dist12))
                                            * distmat[j].unitVector())
                                            
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
                    const SparseMatrix<CLJFactor> &cljmat = group_pairs.constData();
                    
                    for (quint32 i=0; i<nats0; ++i)
                    {
                        distmat.setOuterIndex(i);
                        const Parameter &param0 = params0_array[i];
                
                        if (param0.ljid == 0)
                        {
                            //null LJ parameter - only add on the coulomb energy
                            for (quint32 j=0; j<nats1; ++j)
                            {
                                const CLJFactor &cljscl = cljmat.get(i,j);
                            
                                if (cljscl.coulomb != 0)
                                {
                                    const double q2 = param0.reduced_charge * 
                                                      param1_array[i].reduced_charge;
                                                      
                                    if (q2 != 0)
                                    {
                                        //calculate the coulomb energy
                                        const double cnrg = cljscl.coulomb *
                                                            scl_coul * q2
                                                            / distmat[j].length();
                                               
                                        //calculate the coulomb force
                                        Vector cforce = (-cnrg 
                                                        / distmat[j].length() *
                                                        distmat[j].unitVector()) +
                                             
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
                            for (quint32 j=0; j<nats2; ++j)
                            {
                                //do both coulomb and LJ
                                const CLJFactor &cljscl = cljmat.get(i,j);
                                
                                if (cljscl.coulomb != 0 or cljscl.lj != 0)
                                {
                                    const Parameter &param1 = params1_array[j];
                        
                                    const double invdist = double(1) 
                                                               / distmat[j].length();
                                    const double invdist2 = pow_2(invdist);
                        
                                    Vector force;
                                    
                                    const double q2 = param0.reduced_charge *
                                                      param1.reduced_charge;

                                    if (q2 != 0)
                                    {
                                        //calculate the energy
                                        const double cnrg = cljscl.coulomb *
                                                            scl_coul * q2 * invdist;
                        
                                        //calculate the force
                                        force = (scl_coul * -cnrg 
                                                    / distmat[j].length() *
                                                 distmat[j].unitVector()) +
                                             
                                                 ((cnrg-shift_coul) * dscl_coul);
                                    }
                              
                                    if (cljscl.lj != 0 and param1.ljid != 0)
                                    {
                                        const LJPair &ljpair = ljpairs.constData()[
                                                            ljpairs.map(param0.ljid,
                                                                        param1.ljid)];
                        
                                        double sig_over_dist6 = pow_6(ljpair.sigma*invdist);
                                        double sig_over_dist12 = pow_2(sig_over_dist6);

                                        //calculate the energy
                                        const double ljnrg = cljscl.lj *
                                                             ljpair.epsilon() *
                                                      (sig_over_dist12 - sig_over_dist6);

                                        // dU/dr requires an extra power of r
                                        sig_over_dist6 *= invdist;
                                        sig_over_dist12 *= invdist;

                                        force += ((scl_lj * ljpair.epsilon * 
                                            (6.0*sig_over_dist6 - 12.0*sig_over_dist12))
                                            * distmat[j].unitVector())
                                            
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
                                             
                                                  distmat[j].unitVector();

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
                            for (quint32 j=0; j<nats2; ++j)
                            {
                                //do both coulomb and LJ
                                const Parameter &param1 = params1_array[j];
                            
                                const double invdist = double(1) / distmat[j].length();
                                const double invdist2 = pow_2(invdist);
                        
                                //calculate the force
                                Vector force = -(param0.reduced_charge * 
                                                 param1.reduced_charge * invdist2) 
                                            
                                                * distmat[j].vector();
                              
                                if (param1.ljid != 0)
                                {
                                    const LJPair &ljpair = ljpairs.constData()[
                                                           ljpairs.map(param0.ljid,
                                                                    param1.ljid)];
                        
                                    double sig_over_dist6 = pow_6(ljpair.sigma*invdist);
                                    double sig_over_dist12 = pow_2(sig_over_dist6);

                                    // dU/dr requires an extra power of r
                                    sig_over_dist6 *= invdist;
                                    sig_over_dist12 *= invdist;

                                    force += (ljpair.epsilon * (6.0*sig_over_dist6 - 
                                                                12.0*sig_over_dist12))
                                            * distmat[j].unitVector();
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
                    const SparseMatrix<CLJFactor> &cljmat = group_pairs.constData();
                    
                    for (quint32 i=0; i<nats0; ++i)
                    {
                        distmat.setOuterIndex(i);
                        const Parameter &param0 = params0_array[i];
                
                        if (param0.ljid == 0)
                        {
                            //null LJ parameter - only add on the coulomb energy
                            for (quint32 j=0; j<nats1; ++j)
                            {
                                const CLJFactor &cljscl = cljmat.get(i,j);
                            
                                if (cljscl.coulomb != 0)
                                {
                                    //calculate the coulomb force
                                    Vector cforce = -(cljscl.coulomb *
                                                      param0.reduced_charge *
                                                      params1_array[j].reduced_charge / 
                                                      distmat[j].length2()) *
                                             
                                                      distmat[j].unitVector();

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
                            for (quint32 j=0; j<nats2; ++j)
                            {
                                //do both coulomb and LJ
                                const CLJFactor &cljscl = cljmat.get(i,j);
                                
                                if (cljscl.coulomb != 0 or cljscl.lj != 0)
                                {
                                    const Parameter &param1 = params1_array[j];
                            
                                    const double invdist = double(1)    
                                                            / distmat[j].length();
                                    const double invdist2 = pow_2(invdist);
                        
                                    //calculate the force
                                    Vector force = -(cljscl.coulomb *
                                                     param0.reduced_charge * 
                                                     param1.reduced_charge * invdist2) 
                                            
                                                    * distmat[j].vector();
                              
                                    if (cljscl.lj != 0 and param1.ljid != 0)
                                    {
                                        const LJPair &ljpair = ljpairs.constData()[
                                                               ljpairs.map(param0.ljid,
                                                                    param1.ljid)];
                        
                                        double sig_over_dist6 = pow_6(ljpair.sigma*invdist);
                                        double sig_over_dist12 = pow_2(sig_over_dist6);

                                        // dU/dr requires an extra power of r
                                        sig_over_dist6 *= invdist;
                                        sig_over_dist12 *= invdist;

                                        force += (cljscl.lj *
                                                  ljpair.epsilon * (6.0*sig_over_dist6 - 
                                                                    12.0*sig_over_dist12))
                                                * distmat[j].unitVector();
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
