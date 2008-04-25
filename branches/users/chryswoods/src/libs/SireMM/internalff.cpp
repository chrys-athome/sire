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

#include "internalff.h"

#include "SireMaths/line.h"
#include "SireMaths/triangle.h"
#include "SireMaths/torsion.h"

#include "SireMol/mover.hpp"

#include "SireFF/errors.h"

#include "SireUnits/dimensions.h"

#include "SireStream/datastream.h"
#include "SireStream/shareddatastream.h"

#include "tostring.h"

using namespace SireMM;
using namespace SireFF;
using namespace SireMaths;
using namespace SireUnits::Dimension;
using namespace SireStream;

////////
//////// Implementation of InternalPotential
////////

/** Return the coordinates of the atom 'atom' using the coordinates in 'cgroup_array' */
static const Vector& getCoords(const CGAtomIdx &atom,
                               const CoordGroup *cgroup_array)
{
    return cgroup_array[ atom.cutGroup() ].constData()[ atom.atom() ];
}

/** Calculate the energy caused by the physical terms (bond, angle, dihedral) */
void InternalPotential::calculatePhysicalEnergy(
                                         const GroupInternalParameters &group_params,
                                         const CoordGroup *cgroup_array,
                                         InternalPotential::Energy &energy,
                                         double scale_energy) const
{
    if (not group_params.bondPotential().isEmpty())
    {
        //bonds only use the symbol 'r', representing the bond length
        Values vals;
        const Symbol &r = InternalPotential::symbols().bond().r();
    
        int nbonds = group_params.bondPotential().count();
        const TwoAtomFunction *bonds_array = group_params.bondPotential().constData();
        
        double bndnrg = 0;
        
        for (int i=0; i<nbonds; ++i)
        {
            const TwoAtomFunction &bond = bonds_array[i];
            
            //get the coordinates of the two atoms in the bond
            const Vector &atom0 = getCoords(bond.atom0(), cgroup_array);
            const Vector &atom1 = getCoords(bond.atom1(), cgroup_array);
            
            //calculate the interatomic distance, r
            double dist = Vector::distance(atom0, atom1);
            
            //substitute this distance into 'vals'
            vals.set( r, dist );
            
            //calculate the energy
            bndnrg += bond.function().evaluate(vals);
        }
        
        energy += BondEnergy( scale_energy * bndnrg );
    }
    
    if (not group_params.anglePotential().isEmpty())
    {
        //angles use the symbol 'theta', representing the angle size
        Values vals;
        const Symbol &theta = InternalPotential::symbols().angle().theta();
        
        int nangles = group_params.anglePotential().count();
        const ThreeAtomFunction *angles_array 
                                    = group_params.anglePotential().constData();
         
        double angnrg = 0;
                                                              
        for (int i=0; i<nangles; ++i)
        {
            const ThreeAtomFunction &angle = angles_array[i];
            
            Angle ang = Vector::angle( getCoords(angle.atom0(), cgroup_array),
                                       getCoords(angle.atom1(), cgroup_array),
                                       getCoords(angle.atom2(), cgroup_array) );
                           
            vals.set(theta, ang);
            
            angnrg += angle.function().evaluate(vals);
        }
        
        energy += AngleEnergy( scale_energy * angnrg );
    }
    
    if (not group_params.dihedralPotential().isEmpty())
    {
        //angles use the symbol 'phi', representing the torsion angle
        Values vals;
        const Symbol &phi = InternalPotential::symbols().dihedral().phi();
        
        int ndihedrals = group_params.dihedralPotential().count();
        const FourAtomFunction *dihedrals_array 
                                    = group_params.dihedralPotential().constData();
         
        double dihnrg = 0;
                                                              
        for (int i=0; i<ndihedrals; ++i)
        {
            const FourAtomFunction &dihedral = dihedrals_array[i];
            
            Angle ang = Vector::dihedral( getCoords(dihedral.atom0(), cgroup_array),
                                          getCoords(dihedral.atom1(), cgroup_array),
                                          getCoords(dihedral.atom2(), cgroup_array),
                                          getCoords(dihedral.atom3(), cgroup_array) );
                           
            vals.set(phi, ang);
            
            dihnrg += dihedral.function().evaluate(vals);
        }
        
        energy += DihedralEnergy( scale_energy * dihnrg );
    }
}

/** Calculate the energy caused by the non-physical terms (improper, Urey-Bradley) */
void InternalPotential::calculateNonPhysicalEnergy(
                                         const GroupInternalParameters &group_params,
                                         const CoordGroup *cgroup_array,
                                         InternalPotential::Energy &energy,
                                         double scale_energy) const
{
    if (not group_params.improperPotential().isEmpty())
    {
        Values vals;
        const Symbol &theta = InternalPotential::symbols().improper().theta();
        const Symbol &phi = InternalPotential::symbols().improper().phi();
        
        int nimpropers = group_params.improperPotential().count();
        const FourAtomFunction *impropers_array 
                                    = group_params.improperPotential().constData();
    
        double impnrg = 0;
        
        for (int i=0; i<nimpropers; ++i)
        {
            const FourAtomFunction &improper = impropers_array[i];
            
            Torsion torsion( getCoords(improper.atom0(), cgroup_array),
                             getCoords(improper.atom1(), cgroup_array),
                             getCoords(improper.atom2(), cgroup_array),
                             getCoords(improper.atom3(), cgroup_array) );
                             
            Angle theta_angle = torsion.improperAngle();
            Angle phi_angle = torsion.angle();
            
            vals.set(theta, theta_angle);
            vals.set(phi, phi_angle);
            
            impnrg += improper.function().evaluate(vals);
        }
        
        energy += ImproperEnergy( scale_energy * impnrg );
    }
    
    if (not group_params.ureyBradleyPotential().isEmpty())
    {
        Values vals;
        const Symbol &r = InternalPotential::symbols().ureyBradley().r();
        
        int nubs = group_params.ureyBradleyPotential().count();
        const TwoAtomFunction *ub_array 
                                = group_params.ureyBradleyPotential().constData();
                                
        double ubnrg = 0;
        
        for (int i=0; i<nubs; ++i)
        {
            const TwoAtomFunction &ub = ub_array[i];
            
            double dist = Vector::distance( getCoords(ub.atom0(), cgroup_array),
                                            getCoords(ub.atom1(), cgroup_array) );
                                            
            vals.set( r, dist );
            
            ubnrg += ub.function().evaluate(vals);
        }
        
        energy += UreyBradleyEnergy( scale_energy * ubnrg );
    }
}

/** Calculate the energy caused by the cross terms (improper, Urey-Bradley) */
void InternalPotential::calculateCrossEnergy(
                                         const GroupInternalParameters &group_params,
                                         const CoordGroup *cgroup_array,
                                         InternalPotential::Energy &energy,
                                         double scale_energy) const
{
    if (not group_params.stretchStretchPotential().isEmpty())
    {
        Values vals;
        const Symbol &r01 = InternalPotential::symbols().stretchStretch().r01();
        const Symbol &r21 = InternalPotential::symbols().stretchStretch().r21();
        
        int nss = group_params.stretchStretchPotential().count();
        const ThreeAtomFunction *ss_array 
                              = group_params.stretchStretchPotential().constData();
                              
        double ssnrg = 0;
        
        for (int i=0; i<nss; ++i)
        {
            const ThreeAtomFunction &ss = ss_array[i];
            
            const Vector &atom0 = getCoords(ss.atom0(), cgroup_array);
            const Vector &atom1 = getCoords(ss.atom1(), cgroup_array);
            const Vector &atom2 = getCoords(ss.atom2(), cgroup_array);
                           
            vals.set( r01, Vector::distance(atom0, atom1) );
            vals.set( r21, Vector::distance(atom2, atom1) );
            
            ssnrg += ss.function().evaluate(vals);
        }
        
        energy += StretchStretchEnergy( scale_energy * ssnrg );
    }

    if (not group_params.stretchBendPotential().isEmpty())
    {
        Values vals;
        const Symbol &r01 = InternalPotential::symbols().stretchBend().r01();
        const Symbol &r21 = InternalPotential::symbols().stretchBend().r21();
        const Symbol &theta = InternalPotential::symbols().stretchBend().theta();
        
        int nsb = group_params.stretchBendPotential().count();
        const ThreeAtomFunction *sb_array 
                              = group_params.stretchBendPotential().constData();
                              
        double sbnrg = 0;
        
        for (int i=0; i<nsb; ++i)
        {
            const ThreeAtomFunction &sb = sb_array[i];
            
            const Vector &atom0 = getCoords(sb.atom0(), cgroup_array);
            const Vector &atom1 = getCoords(sb.atom1(), cgroup_array);
            const Vector &atom2 = getCoords(sb.atom2(), cgroup_array);
            
            vals.set( r01, Vector::distance(atom0, atom1) );
            vals.set( r21, Vector::distance(atom2, atom1) );
                
            vals.set( theta, Vector::angle(atom0, atom1, atom2) );
            
            sbnrg += sb.function().evaluate(vals);
        }
        
        energy += StretchBendEnergy( scale_energy * sbnrg );
    }

    if (not group_params.bendBendPotential().isEmpty())
    {
        Values vals;
        const Symbol &theta012 = InternalPotential::symbols().bendBend().theta012();
        const Symbol &theta213 = InternalPotential::symbols().bendBend().theta213();
        const Symbol &theta310 = InternalPotential::symbols().bendBend().theta310();
        
        int nbb = group_params.bendBendPotential().count();
        const FourAtomFunction *bb_array 
                              = group_params.bendBendPotential().constData();
                              
        double bbnrg = 0;
        
        for (int i=0; i<nbb; ++i)
        {
            const FourAtomFunction &bb = bb_array[i];
            
            const Vector &atom0 = getCoords(bb.atom0(), cgroup_array);
            const Vector &atom1 = getCoords(bb.atom1(), cgroup_array);
            const Vector &atom2 = getCoords(bb.atom2(), cgroup_array);
            const Vector &atom3 = getCoords(bb.atom3(), cgroup_array);

            Vector v10 = atom0 - atom1;
            Vector v12 = atom2 - atom1;
            Vector v13 = atom3 - atom1;
        
            vals.set( theta012, Vector::angle(v12,v10) );
            vals.set( theta213, Vector::angle(v13,v12) );
            vals.set( theta310, Vector::angle(v10,v13) );
            
            bbnrg += bb.function().evaluate(vals);
        }
        
        energy += BendBendEnergy( scale_energy * bbnrg );
    }

    if (not group_params.stretchBendTorsionPotential().isEmpty())
    {
        Values vals;
    
        const Symbol &phi = InternalPotential::symbols().stretchBendTorsion().phi();
        const Symbol &r01 = InternalPotential::symbols().stretchBendTorsion().r01();
        const Symbol &r12 = InternalPotential::symbols().stretchBendTorsion().r12();
        const Symbol &r32 = InternalPotential::symbols().stretchBendTorsion().r32();
        const Symbol &r03 = InternalPotential::symbols().stretchBendTorsion().r03();
        const Symbol &theta012 
                = InternalPotential::symbols().stretchBendTorsion().theta012();
        const Symbol &theta321
                = InternalPotential::symbols().stretchBendTorsion().theta321();
        
        int nsbt = group_params.stretchBendTorsionPotential().count();
        const FourAtomFunction *sbt_array 
                          = group_params.stretchBendTorsionPotential().constData();
                              
        double sbtnrg = 0;
        
        for (int i=0; i<nsbt; ++i)
        {
            const FourAtomFunction &sbt = sbt_array[i];
            
            const Vector &atom0 = getCoords(sbt.atom0(), cgroup_array);
            const Vector &atom1 = getCoords(sbt.atom1(), cgroup_array);
            const Vector &atom2 = getCoords(sbt.atom2(), cgroup_array);
            const Vector &atom3 = getCoords(sbt.atom3(), cgroup_array);
            
            vals.set( phi, Vector::dihedral(atom0, atom1, atom2, atom3) );
            
            vals.set( r01, Vector::distance(atom0, atom1) );
            vals.set( r12, Vector::distance(atom1, atom2) );
            vals.set( r32, Vector::distance(atom2, atom3) );
            vals.set( r03, Vector::distance(atom0, atom3) );
            
            vals.set( theta012, Vector::angle(atom0, atom1, atom2) );
            vals.set( theta321, Vector::angle(atom3, atom2, atom1) );   
            
            sbtnrg += sbt.function().evaluate(vals);
        }
        
        energy += StretchBendTorsionEnergy( scale_energy * sbtnrg );
    }
}

/** Calculate the total intramolecular energy of 'molecule' and
    add it onto 'energy', optionally mutiplying it by a scale factor */
void InternalPotential::calculateEnergy(const InternalPotential::Molecule &molecule,
                                        InternalPotential::Energy &energy,
                                        double scale_energy) const
{
    if (scale_energy == 0)
        return;
        
    //get the array of all parameters for each group...
    const GroupInternalParameters *params_array 
                             = molecule.parameters().groupParameters().constData();
    
    int ngroups = molecule.parameters().groupParameters().count();

    //get the array of CoordGroups
    const CoordGroup *cgroup_array 
                             = molecule.parameters().atomicCoordinates().constData();

    for (int i=0; i<ngroups; ++i)
    {
        const GroupInternalParameters &group_params = params_array[i];
        
        if (group_params.hasPhysicalParameters())
            this->calculatePhysicalEnergy(group_params, cgroup_array, 
                                          energy, scale_energy);
            
        if (group_params.hasNonPhysicalParameters())
            this->calculateNonPhysicalEnergy(group_params, cgroup_array,
                                             energy, scale_energy);
        
        if (group_params.hasCrossTerms())
            this->calculateCrossEnergy(group_params, cgroup_array,
                                       energy, scale_energy);
    }
}

static void addForce(const Vector &force, const CGAtomIdx &atom,
                     MolForceTable &forces)
{
    int idx = forces.map(atom.cutGroup());
    
    if (idx >= 0)
    {
        forces.data(idx)[atom.atom()] += force;
    }
}

/** Calculate the total bond force acting on the molecule 'molecule', and add it
    to the forces in 'forces', optionally scaled by 'scale_force' */
void InternalPotential::calculateBondForce(const InternalPotential::Molecule &molecule,
                                           MolForceTable &forces,
                                           double scale_force) const
{
    if (not molecule.parameters().hasBondParameters() or
            scale_force == 0)
        return;

    //get the array of all parameters for each group...
    const GroupInternalParameters *params_array 
                             = molecule.parameters().groupParameters().constData();
    
    int ngroups = molecule.parameters().groupParameters().count();

    //get the array of CoordGroups
    const CoordGroup *cgroup_array 
                             = molecule.parameters().atomicCoordinates().constData();

    Values vals;
    const Symbol &r = InternalPotential::symbols().bond().r();

    for (int i=0; i<ngroups; ++i)
    {
        const GroupInternalParameters &group_params = params_array[i];

        //Is this group in the force table, and does this group have any bonds?
        if ( group_params.bondForces().isEmpty() or
             not (forces.selected(group_params.cgIdx0()) or
                  forces.selected(group_params.cgIdx1())) )
        {
            continue;
        }
                      
        int nbonds = group_params.bondForces().count();
        const TwoAtomFunction *bonds_array = group_params.bondForces().constData();
        
        for (int j=0; j<nbonds; ++j)
        {
            const TwoAtomFunction &bond = bonds_array[j];
            
            const Vector &atom0 = getCoords(bond.atom0(), cgroup_array);
            const Vector &atom1 = getCoords(bond.atom1(), cgroup_array);
            
            Vector v01 = atom1 - atom0;
            double dist = v01.length();
            
            if (dist == 0)
                //cannot calculate forces of overlapping atoms!
                continue;
            
            v01 /= dist;
            vals.set( r, dist );

            //evaluate the force vector
            Vector force = scale_force * bond.function().evaluate(vals) * v01;

            //add the force onto the forces array
            addForce(force, bond.atom0(), forces);
            addForce(-force, bond.atom1(), forces);
        }
    }
}

/** Calculate d theta / d r_i, d theta / d r_j and d theta / d r_k for 
    the passed r_i, r_j and r_k  (three points that make up an angle)

    Atoms at coords  r_i, r_j, r_k  => r  (9 element vector)
    
    Force = - d V / d r      but V is a function of theta, not r
    
          d V / d r  = d V / d theta  *  d theta / d r
          
    d V / d theta is straightforward (Sire can do this automatically)
          
    So need to calculate d theta / d r
    
    Can do this via cos(theta), as
    
    d theta / d r = d cos(theta) / d r  * d theta / d cos(theta)
    
    d theta / d cos(theta) = -1 / sin(theta)
   
    so  d theta / d r = -1 / sin(theta)  * d cos(theta) / d r
      
    We can construct two vectors, A = (r_i - r_j) and B = (r_k - r_j)
      
    cos(theta) = A dot B / ( |A| * |B| )
    
    then
    
    d cos(theta) / dr  = d cos(theta) / d A B  *  d A B / dr
    
    now following the derivation in Blondel and Karplus, J. Comp. Chem. 1996
    
    d cos(theta) / d A = ( B / (|A| |B|)) + ( (A.B/|B|).(d(1/|A|) d A) )
    
    introducing d |A| / d A  =  A / |A|
    
    d cos(theta) / d A = (1 / |A|^3 |B|) * (A^2 B - (A.B) A)
   
    d theta / d A  =  d theta / d cos(theta)  *  d cos(theta) / d A
    
    d theta / d A  = (-1 / sin(theta)) * (1 / |A|^3 |B|) * (A^2 B - (A.B) A)
    
    from symmetry
    
    d theta / d B = (-1 / sin(theta)) * (1 / |B|^3 |A|) * (B^2 A - (B.A) B)
    
    as
    
    d theta / r_?  = (d theta / d A * d A / d r_?) + (d theta / d B * d B / d r_?)
    
    and
    
    d A / d r_i  = 1    d A / d r_j  = -1    d A / d r_k  = 0
    d B / d r_i  = 0    d B / d r_j  = -1    d B / d r_k  = 1
    
    then
    
    d theta / d r_i = (-1 / sin(theta)) * (1 / |A|^3 |B|) * (A^2 B - (A.B) A)
    
    d theta / d r_j = (1 / sin(theta)) * (1 / |A|^3 |B|) * (A^2 B - (A.B) A) +
                      (1 / sin(theta)) * (1 / |B|^3 |A|) * (B^2 A - (B.A) B)
    
    d theta / d r_k = (-1 / sin(theta)) * (1 / |B|^3 |A|) * (B^2 A - (B.A) B)  
*/
static void d_theta_by_dr(const Vector &ri, const Vector &rj,
                          const Vector &rk,
                          
                          Vector &dtheta_by_dri, Vector &dtheta_by_drj,
                          Vector &dtheta_by_drk, 
                          
                          Angle &theta, double &dist_ij, double &dist_kj)
{
    //get the vector A, from j -> i
    const Vector A = ri - rj;
    
    //B goes from j -> k
    const Vector B = rk - rj;
    
    //get the lengths of these vectors
    const double a2 = A.length2();
    const double a = std::sqrt(a2);
    
    const double b2 = B.length2();
    const double b = std::sqrt(b2);

    const double ab = a*b;

    if (ab == 0)
    {
        //there is no force if the atoms are on top of one another!
        dtheta_by_dri.set(0,0,0);
        dtheta_by_drj.set(0,0,0);
        dtheta_by_drk.set(0,0,0);
        theta = Angle(0);
        dist_ij = a;
        dist_kj = b;
        
        return;
    }
    
    //get A.B and B.A
    const double A_dot_B = Vector::dot(A,B);
    const double B_dot_A = A_dot_B;
    
    //calculate the angle between A and B
    double AB_over_ab = A_dot_B / ab;
    
    if (AB_over_ab > 1)
        AB_over_ab = 1;
    else if (AB_over_ab < -1)
        AB_over_ab = -1;
    
    const double thta = std::acos(AB_over_ab);
    
    const double sin_theta = std::sin(thta);
    
    if (sin_theta == 0)
    {
        //the atoms lie on a line - there is no way to calculate the force now
        dtheta_by_dri.set(0,0,0);
        dtheta_by_drj.set(0,0,0);
        dtheta_by_drk.set(0,0,0);
        
        theta = Angle(thta);
        dist_ij = a;
        dist_kj = b;
        
        return;
    }
    
    const double one_over_sin_theta = 1.0 / sin_theta;
    
    dtheta_by_dri = -one_over_sin_theta * (1.0 / (a2*ab)) * 
                                     ( (a2 * B) - (A_dot_B * A) ); 

    dtheta_by_drk = -one_over_sin_theta * (1.0 / (b2*ab)) *
                                     ( (b2 * A) - (B_dot_A * B) );
                                     
    dtheta_by_drj = -(dtheta_by_dri + dtheta_by_drk);
    
    theta = Angle(thta);
    dist_ij = a;
    dist_kj = b;
}
                   
/** Calculate the total angle force acting on the molecule 'molecule', and add it
    to the forces in 'forces', optionally scaled by 'scale_force' 
    
*/
void InternalPotential::calculateAngleForce(const InternalPotential::Molecule &molecule,
                                            MolForceTable &forces,
                                            double scale_force) const
{
    if (not molecule.parameters().hasAngleParameters() or
            scale_force == 0)
        return;

    //get the array of all parameters for each group...
    const GroupInternalParameters *params_array 
                             = molecule.parameters().groupParameters().constData();
    
    int ngroups = molecule.parameters().groupParameters().count();

    //get the array of CoordGroups
    const CoordGroup *cgroup_array 
                             = molecule.parameters().atomicCoordinates().constData();

    Values vals;
    const Symbol &theta = InternalPotential::symbols().angle().theta();

    for (int i=0; i<ngroups; ++i)
    {
        const GroupInternalParameters &group_params = params_array[i];

        //Is this group in the force table, and does this group have any angles?
        if ( group_params.angleForces().isEmpty() or
             not (forces.selected(group_params.cgIdx0()) or
                  forces.selected(group_params.cgIdx1()) or
                  forces.selected(group_params.cgIdx2())) )
        {
            continue;
        }
                      
        int nangles = group_params.angleForces().count();
        const ThreeAtomFunction *angles_array = group_params.angleForces().constData();
        
        for (int j=0; j<nangles; ++j)
        {
            const ThreeAtomFunction &angle = angles_array[j];
            
            const Vector &atom0 = getCoords(angle.atom0(), cgroup_array);
            const Vector &atom1 = getCoords(angle.atom1(), cgroup_array);
            const Vector &atom2 = getCoords(angle.atom2(), cgroup_array);

            //d V(theta) / dr  = d V(theta) / dtheta  *  dtheta / dr
            
            //so first calculate d theta / dr
            Vector dtheta_by_d0, dtheta_by_d1, dtheta_by_d2;
            Angle t;
            double r01, r21;
            
            d_theta_by_dr(atom0, atom1, atom2,
                          dtheta_by_d0, dtheta_by_d1, dtheta_by_d2,
                          t, r01, r21);
                          
            //now calcualte d V(theta) / d theta
            vals.set(theta, t);
            double dv_by_dtheta = angle.function().evaluate(vals);

            //add the force onto the forces array
            addForce(dv_by_dtheta * dtheta_by_d0, angle.atom0(), forces);
            addForce(dv_by_dtheta * dtheta_by_d1, angle.atom1(), forces);
            addForce(dv_by_dtheta * dtheta_by_d2, angle.atom2(), forces);
        }
    }
}
                         
/** Calculate d phi / d r_i, d phi / d r_j, d phi / d r_k and d phi / d r_l.

    This uses the algorithm presented in Blondel and Karplus, J. Comp. Chem.
    Vol. 17, No. 9, 1132-1141, 1996, which shows how to calculate the derivative
    avoiding the singularity at phi = 0 (where sin(phi) = 0, so 1 / sin(phi) is
    undefined).
    
    Four points, ri, rj, rk and rl form the torsion. 
    
    Three inter-point vectors,
    
    F = ri - rj
    G = rj - rk
    H = rl - rk
    
    Also second set of intermediate vectors,
    
    A = F cross G
    B = H cross G
    
    A and B define the two planes of the torsion, and the angle
    between these vectors, (based on A dot B) is the torsion angle.
    
    cos(phi) = A.B / |A||B|
    
    sin(phi) = B cross A.G / |A||B||G|
    
    From equation 27 from this paper, we get;
    
    d phi / d F  =  - (|G| / A^2) A
    d phi / d H  =  (|G| / B^2) B
    d phi / d G  =  ( F.G / A^2|G| ) A - ( H.G / B^2|G| ) B
    
    d phi / d r_i  =  d phi / d F
    d phi / d r_j  =  -d phi / d F + d phi / d G
    d phi / d r_k  =  -d phi / d H - d phi / d G
    d phi / d r_l  =  d phi / dH
*/
static void d_phi_by_dr(const Vector &ri, const Vector &rj,
                        const Vector &rk, const Vector &rl,
                        
                        Vector &dphi_by_dri, Vector &dphi_by_drj,
                        Vector &dphi_by_drk, Vector &dphi_by_drl)
{
    const Vector F = ri - rj;
    const Vector G = rj - rk;
    const Vector H = rl - rk;
    
    const double g = G.length();
    
    const double F_dot_G = Vector::dot(F,G);
    const double H_dot_G = Vector::dot(H,G);
    
    const Vector A( F.y()*G.z() - F.z()*G.y(),
                    F.z()*G.x() - F.x()*G.z(),
                    F.x()*G.y() - F.y()*G.x() );
                    
    const Vector B( H.y()*G.z() - H.z()*G.y(),
                    H.z()*G.x() - H.x()*G.z(),
                    H.x()*G.y() - H.y()*G.x() );

    //get the lengths of A and B
    const double a2 = A.length2();
    const double a = std::sqrt(a2);
    
    const double b2 = B.length2();
    const double b = std::sqrt(b2);
    
    if (a*b == 0)
    {
        //ijk or jkl are on a straight line - we cannot calculate
        //a valid force!
        dphi_by_dri.set(0,0,0);
        dphi_by_drj.set(0,0,0);
        dphi_by_drk.set(0,0,0);
        dphi_by_drl.set(0,0,0);
        
        return;
    }
    
    const Vector dphi_by_dF = -( g / a2 ) * A;
    const Vector dphi_by_dH = ( g / b2 ) * B;
    
    const Vector dphi_by_dG = ( (F_dot_G / (a2 * g)) * A ) -
                              ( (H_dot_G / (b2 * g)) * B );

    dphi_by_dri = dphi_by_dF;
    dphi_by_drj = dphi_by_dG - dphi_by_dF;
    dphi_by_drk = -(dphi_by_dH+dphi_by_dG);
    dphi_by_drl = dphi_by_dH;
}

/** Calculate the total dihedral force acting on the molecule 'molecule', and add it
    to the forces in 'forces', optionally scaled by 'scale_force' 
    
*/
void InternalPotential::calculateDihedralForce(
                                    const InternalPotential::Molecule &molecule,
                                    MolForceTable &forces,
                                    double scale_force) const
{
    if (not molecule.parameters().hasDihedralParameters() or
            scale_force == 0)
        return;

    //get the array of all parameters for each group...
    const GroupInternalParameters *params_array 
                             = molecule.parameters().groupParameters().constData();
    
    int ngroups = molecule.parameters().groupParameters().count();

    //get the array of CoordGroups
    const CoordGroup *cgroup_array 
                             = molecule.parameters().atomicCoordinates().constData();

    Values vals;
    const Symbol &phi = InternalPotential::symbols().dihedral().phi();

    for (int i=0; i<ngroups; ++i)
    {
        const GroupInternalParameters &group_params = params_array[i];

        //Is this group in the force table, and does this group have any angles?
        if ( group_params.dihedralForces().isEmpty() or
             not (forces.selected(group_params.cgIdx0()) or
                  forces.selected(group_params.cgIdx1()) or
                  forces.selected(group_params.cgIdx2()) or
                  forces.selected(group_params.cgIdx3()) ) )
        {
            continue;
        }
                      
        int ndihedrals = group_params.dihedralForces().count();
        const FourAtomFunction *dihedrals_array 
                                    = group_params.dihedralForces().constData();
        
        for (int j=0; j<ndihedrals; ++j)
        {
            const FourAtomFunction &dihedral = dihedrals_array[j];
            
            const Vector &atom0 = getCoords(dihedral.atom0(), cgroup_array);
            const Vector &atom1 = getCoords(dihedral.atom1(), cgroup_array);
            const Vector &atom2 = getCoords(dihedral.atom2(), cgroup_array);
            const Vector &atom3 = getCoords(dihedral.atom3(), cgroup_array);

            //d V(phi) / dr  = d V(phi) / dphi  *  dphi / dr
            
            //so first calculate d phi / dr
            Vector dphi_by_d0, dphi_by_d1, dphi_by_d2, dphi_by_d3;
            
            d_phi_by_dr(atom0, atom1, atom2, atom3,
                        dphi_by_d0, dphi_by_d1, dphi_by_d2, dphi_by_d3);
                          
            //now calcualte d V(phi) / d phi
            vals.set(phi, Vector::dihedral(atom0, atom1, atom2, atom2));
            double dv_by_dphi = dihedral.function().evaluate(vals);

            //add the force onto the forces array
            addForce(dv_by_dphi * dphi_by_d0, dihedral.atom0(), forces);
            addForce(dv_by_dphi * dphi_by_d1, dihedral.atom1(), forces);
            addForce(dv_by_dphi * dphi_by_d2, dihedral.atom2(), forces);
            addForce(dv_by_dphi * dphi_by_d3, dihedral.atom3(), forces);
        }
    }
}

void InternalPotential::calculateImproperForce(
                                    const InternalPotential::Molecule &molecule,
                                    MolForceTable &forces,
                                    double scale_force) const;
                            
/** Calculate the total Urey-Bradley force acting on the molecule 'molecule', 
    and add it to the forces in 'forces', optionally scaled by 'scale_force' */
void InternalPotential::calculateUBForce(const InternalPotential::Molecule &molecule,
                                         MolForceTable &forces,
                                         double scale_force) const
{
    if (not molecule.parameters().hasUreyBradleyParameters() or
            scale_force == 0)
        return;

    //get the array of all parameters for each group...
    const GroupInternalParameters *params_array 
                             = molecule.parameters().groupParameters().constData();
    
    int ngroups = molecule.parameters().groupParameters().count();

    //get the array of CoordGroups
    const CoordGroup *cgroup_array 
                             = molecule.parameters().atomicCoordinates().constData();

    Values vals;
    const Symbol &r = InternalPotential::symbols().ureyBradley().r();

    for (int i=0; i<ngroups; ++i)
    {
        const GroupInternalParameters &group_params = params_array[i];

        //Is this group in the force table, and does this group have any Urey Bradleys?
        if ( group_params.ureyBradleyForces().isEmpty() or
             not (forces.selected(group_params.cgIdx0()) or
                  forces.selected(group_params.cgIdx1())) )
        {
            continue;
        }
                      
        int nubs = group_params.ureyBradleyForces().count();
        const TwoAtomFunction *ubs_array = group_params.ureyBradleyForces().constData();
        
        for (int j=0; j<nubs; ++j)
        {
            const TwoAtomFunction &ub = ubs_array[j];
            
            const Vector &atom0 = getCoords(ub.atom0(), cgroup_array);
            const Vector &atom1 = getCoords(ub.atom1(), cgroup_array);
            
            Vector v01 = atom1 - atom0;
            double dist = v01.length();
            
            if (dist == 0)
                //cannot calculate forces of overlapping atoms!
                continue;
            
            v01 /= dist;
            vals.set( r, dist );

            //evaluate the force vector
            Vector force = scale_force * ub.function().evaluate(vals) * v01;

            //add the force onto the forces array
            addForce(force, ub.atom0(), forces);
            addForce(-force, ub.atom1(), forces);
        }
    }
}

/** Calculate the total stretch-stretch force acting on the molecule 'molecule', 
    and add it to the forces in 'forces', optionally scaled by 'scale_force' */
void InternalPotential::calculateSSForce(const InternalPotential::Molecule &molecule,
                                         MolForceTable &forces,
                                         double scale_force) const
{
    if (not molecule.parameters().hasStretchStretchParameters() or
            scale_force == 0)
        return;

    //get the array of all parameters for each group...
    const GroupInternalParameters *params_array 
                             = molecule.parameters().groupParameters().constData();
    
    int ngroups = molecule.parameters().groupParameters().count();

    //get the array of CoordGroups
    const CoordGroup *cgroup_array 
                             = molecule.parameters().atomicCoordinates().constData();

    Values vals;
    const Symbol &r01 = InternalPotential::symbols().stretchStretch().r01();
    const Symbol &r21 = InternalPotential::symbols().stretchStretch().r21();

    for (int i=0; i<ngroups; ++i)
    {
        const GroupInternalParameters &group_params = params_array[i];

        //Is this group in the force table, and does this group 
        //have any stretch-stretch forces?
        if ( (group_params.stretchStretch_R01_Forces().isEmpty() and
              group_params.stretchStretch_R21_Forces().isEmpty()) or
             not (forces.selected(group_params.cgIdx0()) or
                  forces.selected(group_params.cgIdx1()) or
                  forces.selected(group_params.cgIdx2())) )
        {
            continue;
        }
                      
        //calculate the forces along r01
        int nss = group_params.stretchStretch_R01_Forces().count();
        const ThreeAtomFunction *ss_array 
                            = group_params.stretchStretch_R01_Forces().constData();
        
        for (int j=0; j<nss; ++j)
        {
            const ThreeAtomFunction &ss = ss_array[j];
            
            const Vector &atom0 = getCoords(ss.atom0(), cgroup_array);
            const Vector &atom1 = getCoords(ss.atom1(), cgroup_array);
            const Vector &atom2 = getCoords(ss.atom2(), cgroup_array);
            
            Vector v = atom1 - atom0;
            double dist = v.length();
            
            if (dist == 0)
                //cannot calculate forces of overlapping atoms!
                continue;

            v /= dist;
            
            vals.set( r01, dist );
            vals.set( r21, Vector::distance(atom1, atom2) );

            //evaluate the force vector
            Vector force = scale_force * ss.function().evaluate(vals) * v;

            //add the force onto the forces array
            addForce(force, ss.atom0(), forces);
            addForce(-force, ss.atom1(), forces);
        }
                      
        //now calculate the forces along r21
        nss = group_params.stretchStretch_R21_Forces().count();
        ss_array = group_params.stretchStretch_R21_Forces().constData();
        
        for (int j=0; j<nss; ++j)
        {
            const ThreeAtomFunction &ss = ss_array[j];
            
            const Vector &atom0 = getCoords(ss.atom0(), cgroup_array);
            const Vector &atom1 = getCoords(ss.atom1(), cgroup_array);
            const Vector &atom2 = getCoords(ss.atom2(), cgroup_array);
            
            Vector v = atom1 - atom2;
            double dist = v.length();
            
            if (dist == 0)
                //cannot calculate forces of overlapping atoms!
                continue;

            v /= dist;
            
            vals.set( r21, dist );
            vals.set( r01, Vector::distance(atom1, atom0) );

            //evaluate the force vector
            Vector force = scale_force * ss.function().evaluate(vals) * v;

            //add the force onto the forces array
            addForce(force, ss.atom2(), forces);
            addForce(-force, ss.atom1(), forces);
        }
    }
}

void InternalPotential::calculateSBForce(const InternalPotential::Molecule &molecule,
                      MolForceTable &forces,
                      double scale_force=1) const;

void InternalPotential::calculateBBForce(const InternalPotential::Molecule &molecule,
                      MolForceTable &forces,
                      double scale_force=1) const;

void InternalPotential::calculateSBTForce(const InternalPotential::Molecule &molecule,
                       MolForceTable &forces,
                       double scale_force=1) const;

/** Calculate the total force acting on the molecule 'molecule', and add it
    to the forces in 'forces', optionally scaled by 'scale_force' */
void InternalPotential::calculateForce(const InternalPotential::Molecule &molecule,
                                       MolForceTable &forces,
                                       double scale_force) const
{
    if (scale_force == 0)
        return;
    
    if (molecule.parameters().hasPhysicalParameters())
    {    
        calculateBondForce(molecule, forces, scale_force);
        calculateAngleForce(molecule, forces, scale_force);
        calculateDihedralForce(molecule, forces, scale_force);
    }
    
    if (molecule.parameters().hasNonPhysicalParameters())
    {
        calculateImproperForce(molecule, forces, scale_force);
        calculateUBForce(molecule, forces, scale_force);
    }
    
    if (molecule.parameters().hasCrossTerms())
    {
        calculateSSForce(molecule, forces, scale_force);
        calculateSBForce(molecule, forces, scale_force);
        calculateBBForce(molecule, forces, scale_force);
        calculateSBTForce(molecule, forces, scale_force);
    }
}

/** Calculate the forces in the molecule caused by the component represented
    by the symbol 'symbol' acting on the molecule 'molecule' and add this
    onto the forces in 'forces', optionally scaled by 'scale_force' 
    
    \throw SireFF::missing_component
*/
void InternalPotential::calculateForce(const InternalPotential::Molecule &molecule,
                                       MolForceTable &forces,
                                       const Symbol &symbol,
                                       const Components &components,
                                       double scale_force) const
{
    if (symbol == components.total())
        calculateForce(molecule, forces, scale_force);

    else if (symbol == components.bond())
        calculateBondForce(molecule, forces, scale_force);

    else if (symbol == components.angle())
        calculateAngleForce(molecule, forces, scale_force);

    else if (symbol == components.dihedral())
        calculateDihedralForce(molecule, forces, scale_force);

    else if (symbol == components.improper())
        calculateImproperForce(molecule, forces, scale_force);

    else if (symbol == components.ureyBradley())
        calculateUBForce(molecule, forces, scale_force);

    else if (symbol == components.stretchStretch())
        calculateSSForce(molecule, forces, scale_force);

    else if (symbol == components.stretchBend())
        calculateSBForce(molecule, forces, scale_force);

    else if (symbol == components.bendBend())
        calculateBBForce(molecule, forces, scale_force);

    else if (symbol == components.stretchBendTorsion())
        calculateSBTForce(molecule, forces, scale_force);

    else
        throw SireFF::missing_component( QObject::tr(
            "There is no component %1 in this potential. Available "
            "components are %2.")
                .arg(symbol.toString(), Sire::toString(components.contents())),
                    CODELOC );
}

////////
//////// Implementation of InternalFF
////////
