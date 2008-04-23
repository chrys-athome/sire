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

#include "SireUnits/dimensions.h"

#include "SireStream/datastream.h"
#include "SireStream/shareddatastream.h"

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

/** Calculate the total bond force acting on the molecule 'molecule', and add it
    to the forces in 'forces', optionally scaled by 'scale_force' */
void InternalPotential::calculateBondForce(const InternalPotential::Molecule &molecule,
                                           MolForceTable &forces,
                                           double scale_force) const
{
    if (scale_force == 0)
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
        
        for (int j=0; j<ngroups; ++j)
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
                        
/*
void InternalPotential::calculateAngleForce(const InternalPotential::Molecule &molecule,
                         MolForceTable &forces,
                         double scale_force=1) const;
                         
void InternalPotential::calculateDihedralForce(const InternalPotential::Molecule &molecule,
                            MolForceTable &forces,
                            double scale_force=1) const;

void InternalPotential::calculateImproperForce(const InternalPotential::Molecule &molecule,
                            MolForceTable &forces,
                            double scale_force=1) const;
                            
void InternalPotential::calculateUBForce(const InternalPotential::Molecule &molecule,
                      MolForceTable &forces,
                      double scale_force=1) const;

void InternalPotential::calculateSSForce(const InternalPotential::Molecule &molecule,
                      MolForceTable &forces,
                      double scale_force=1) const;

void InternalPotential::calculateSBForce(const InternalPotential::Molecule &molecule,
                      MolForceTable &forces,
                      double scale_force=1) const;

void InternalPotential::calculateBBForce(const InternalPotential::Molecule &molecule,
                      MolForceTable &forces,
                      double scale_force=1) const;

void InternalPotential::calculateSBTForce(const InternalPotential::Molecule &molecule,
                       MolForceTable &forces,
                       double scale_force=1) const;

void InternalPotential::calculateForce(const InternalPotential::Molecule &molecule,
                    MolForceTable &forces,
                    double scale_force=1) const;
                      
void InternalPotential::calculateForce(const InternalPotential::Molecule &molecule,
                    MolForceTable &forces,
                    const Symbol &symbol,
                    const Components &components,
                    double scale_force=1) const;
*/

////////
//////// Implementation of InternalFF
////////
