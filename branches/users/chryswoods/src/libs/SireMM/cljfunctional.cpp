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
void CLJPotential::calculateEnergy(const CLJPotential::Molecule &mol0,
                                   const CLJPotential::Molecule &mol1,
                                   CLJPotential::Energy &energy,
                                   CLJPotential::Workspace &distmat) const
{
    const quint32 ngroups0 = mol0.nCutGroups();
    const quint32 ngroups1 = mol1.nCutGroups();
    
    const CoordGroup *groups0_array = mol0.coordGroups().constData();
    const CoordGroup *groups1_array = mol1.coordGroups().constData();
    
    const CGParams *params0_array = mol0.parameters().constData();
    const CGParams *params1_array = mol1.parameters().constData();
    
    double cnrg = 0;
    double ljnrg = 0;
    
    //loop over all pairs of CutGroups in the two molecules
    for (quint32 igroup=0; igroup<ngroups0; ++igroup)
    {
        const CGParams &params0 = params0_array[igroup];

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
            const CGParams params1 = params1_array[jgroup];

            //check first that these two CoordGroups could be within cutoff
            const bool outside_cutoff = space->beyond(nbcut, aabox0, group1.aaBox());
            
            if (outside_cutoff or params1.isNull())
                //this CutGroup is either beyond the cutoff distance
                //or all of the atoms are dummies
                continue;
            
            //calculate all of the interatomic distances
            const double mindist = space->calcDist(group0, group1, distmat);
            
            if (mindist > cutoffDistance())
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
            
            //now add these energies onto the total for the molecule,
            //scaled by any non-bonded feather factor
            cnrg += coulombScaleFactor(mindist) * icnrg;
            ljnrg += ljScaleFactor(mindist) * iljnrg;
        }
    }
    
    //add this molecule pair's energy onto the total
    energy += Energy(cnrg, ljnrg);
}
