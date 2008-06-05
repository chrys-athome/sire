
#include "qhash_siremol.h"

#include "splitmolecule.h"
#include "atomidgroup.h"
#include "bond.h"
#include "angle.h"
#include "dihedral.h"
#include "improper.h"
#include "residuebonds.h"
#include "moleculebonds.h"

#include "SireMol/errors.h"

namespace SireMol
{

void traceAtoms(const AtomIndex &start, const AtomIndex &root,
                const ResidueBonds &connectivity, 
                const AtomIDGroup &exclude, AtomIDGroup &group);
                
void traceAtoms(const AtomIndex &start, const AtomIndex &root,
                const MoleculeBonds &connectivity,
                const AtomIDGroup &exclude, AtomIDGroup &group);


/** This is a recursive function that traces all atoms that can trace their bonding
    to 'strt', and are in the same residue as 'strt', but that don't go through 'root', 
    and to add those atoms to 'group'. If any atoms are found that are in 'exclude' then
    an exception is thrown, as this indicates that the atoms found form part
    of a ring. This is an internal function that is only intended to be used 
    by the splitMolecule and splitResidue functions. 
    
    \throw SireMol::ring_error
*/
void traceAtoms(const AtomIndex &start, const AtomIndex &root,
                const ResidueBonds &connectivity, 
                const AtomIDGroup &exclude, AtomIDGroup &group)
{
    //there are no atoms if this is atom is not in the residue
    if (start.resNum() != connectivity.resNum())
        return;

    //trace all of the intraresidue bonds...
    
    //get all of the intra-residue bonds that involve the start atom
    BondList intrabnds = connectivity.intraBonds(start.name());
    
    //loop over each bond...
    int nbnds = intrabnds.count();
    for (int i=0; i<nbnds; ++i)
    {
        //get the other atom in this bond...
        const AtomIndex &other = intrabnds.at(i).other(start);
        
        //we will only add this atom to the set if;
        // (1) it has not already been added
        // (2) it is not the root atom (since we don't want to move backwards)
        // (3) it is not in 'exclude' (which would indicate that we have found a ring)
        if (not group.contains(other) and other != root)
        {
            if (exclude.contains(other))
            {
                //we have probably found an intra-residue ring!
                throw SireMol::ring_error(QObject::tr(
                    "Atoms %1-%2-%3 form part of a ring, and cannot be unambiguously split.")
                        .arg(start.toString(), other.toString(), root.toString()), CODELOC);
            }
            else
            {
                //add the 'other' atom to the list of atoms in this group
                group.add(other);
                
                //now we can trace the atoms from the 'other' atom...
                traceAtoms(other, start, connectivity, exclude, group);
            }
        }
    }
        
    //ok - we have added all of the atoms that are connected to this atom. We
    //have finished with this atom, so we can return.
    return;
}

/** Trace all residues that are bonded to 'startres', which do not trace their 
    bonding through 'root', and for with the original starting atom for this
    trace being in 'start'. */
void traceResidues(ResNum startres, const AtomIndex &start, const AtomIndex &root,
                   const MoleculeBonds &connectivity,
                   const AtomIDGroup &exclude, AtomIDGroup &group)
{
    //get the list of inter-residue bonds for this residue...
    BondList interbnds = connectivity.residue(startres).interBonds();
    
    //loop over each bond...
    int nbnds = interbnds.count();
    for( int i=0; i<nbnds; ++i )
    {
        //get the other atom in the bond...
        const AtomIndex &other = interbnds.at(i).other(startres);
        
        //only continue if this atom is not already in the group, or is
        //the root atom
        if (not group.contains(other) and other != root)
        {
            //is this in the excluded group?
            if (exclude.contains(other))
                throw SireMol::ring_error(QObject::tr(
                    "Residues %1-%2-%3 for part of a ring? (Atoms %1-%2-%3)")
                        .arg(startres.toString(),other.resNum().toString(),
                                                 root.resNum().toString())
                        .arg(start.toString(),other.toString(),root.toString()), CODELOC);
            else
            {
                //if this atom is in the same residue as 'start' then we need
                //to trace all of the atoms bonded to this one manually...
                if (other.resNum() == start.resNum())
                {
                    group.add(other);
                    traceAtoms(other,root,connectivity,exclude,group);
                }
                else
                {
                    //assume all of the atoms in the residue are in the group,
                    //and just trace residue connections...
                    group.add(other.resNum());
                    traceResidues(other.resNum(),start,root,connectivity,exclude,group);
                }
            }
        }
    }
    
    return;
}

/** This is a recursive function that traces all atoms that can trace their bonding
    to 'strt', but that don't go through 'root', 
    and to add those atoms to 'group'. If any atoms are found that are in 'exclude' then
    a exception is thrown, as this indicates that the atoms found form part
    of a ring. This is an internal function that is only intended to be used 
    by the splitMolecule and splitResidue functions. 
    
    \throw SireMol::ring_error
*/
void traceAtoms(const AtomIndex &start, const AtomIndex &root,
                const MoleculeBonds &connectivity,
                const AtomIDGroup &exclude, AtomIDGroup &group)
{
    //get the residue connectivity for the start atom...
    ResidueBonds resbnds = connectivity.residue(start.resNum());
    
    //get the list of intramolecular bonds involving this atom...
    BondList intrabnds = resbnds.intraBonds(start.name());
    
    int nbnds = intrabnds.count();
    for (int i=0; i<nbnds; ++i)
    {
        //get the other atom in this bond...
        const AtomIndex &other = intrabnds.at(i).other(start);
        
        //we will only add this atom to the set if;
        // (1) it has not already been added
        // (2) it is not the root atom (since we don't want to move backwards)
        // (3) it is not in 'exclude' (which would indicate that we have found a ring)
        if (not group.contains(other) and other != root)
        {
            if (exclude.contains(other))
            {
                //we have probably found an intra-residue ring!
                throw SireMol::ring_error(QObject::tr(
                    "Atoms %1-%2-%3 form part of a ring, and cannot be unambiguously split.")
                        .arg(start.toString(), other.toString(), root.toString()), CODELOC);
            }
            else
            {
                //add the 'other' atom to the list of atoms in this group
                group.add(other);
                
                //now we can trace the atoms from the 'other' atom...
                traceAtoms(other, start, connectivity, exclude, group);
            }
        }
    }
    
    //now loop over all of the inter-residue bonds...
    BondList interbnds = resbnds.interBonds(start.name());
    
    nbnds = interbnds.count();
    for (int i=0; i<nbnds; ++i)
    {
        //get the other atom in this bond...
        const AtomIndex &other = intrabnds.at(i).other(start);
        
        if (not group.contains(other) and other != root)
        {
            if (exclude.contains(other))
            {
                //we have probably found an inter-residue ring!
                throw SireMol::ring_error(QObject::tr(
                    "Atoms %1-%2-%3 form part of a ring, and cannot be unambiguously split.")
                        .arg(start.toString(), other.toString(), root.toString()), CODELOC);
            }
            else
            {
                //if the residue number of 'other' is the same as 'root'
                //then add only the atom...
                if (other.resNum() == root.resNum())
                {
                    group.add(other);
                    
                    //now we can trace the atoms that are bonded to 'other'
                    traceAtoms(other, start, connectivity, exclude, group);
                }
                else
                {
                    //the atom is in a residue that is neither the same as start
                    //nor the same as root - we will assume that all of the atoms 
                    //in this other residue (and all those in residues bonded to
                    //that residue) are in group0. This will be correct, as long
                    //as we assume that all of the atoms in a residue are bonded
                    //together...
                    group.add(other.resNum());
                    
                    traceResidues(other.resNum(), start, root,
                                  connectivity, exclude, group);
                }
            }
        }
    }
        
    return;
}

/** This function splits the atoms in the residue whose connectivity is contained 
    in 'connectivity' into two parts, based on the bond 'bnd'. At least one atom
    must be in this residue, and be connected to part of this residue, or else
    an exception will be thrown.
    
    An exception will also be thrown if it is not possible to split the molecule
    unambiguously in two, as the bond is part of a ring.
    
    For example;
 
          C1--C2--C3--C4--C5--C6--C7--C8
          
    Splitting C3 and C4 would result in two groups, {C1,C2,C3} and {C4,C5,C6,C7,C8}
    
          C1\
          C2-C4--C5
          C3/
 
    Splitting C4 and C5 would result in two groups, {C1,C2,C3,C4} and {C5}
 
    However splitting C1 and C5 would add a bond between C1 and C5. This would mean 
    than C1-C4-C5 would form a ring, so an exception would be thrown.
    
    \throw SireMol::missing_atom
    \throw SireMol::ring_error
*/
AtomIDGroupPair SIREMOL_EXPORT splitResidue(const Bond &bnd, 
                                            const ResidueBonds &connectivity)
{
    AtomIDGroup group0,group1;
    
    if (bnd.atom0().resNum() == connectivity.resNum())
    {
        //add the atom being split to its group
        group0.add(bnd.atom0());
        //group0 is constructed starting at 'atom0' and rooted at 'atom1'
        traceAtoms(bnd.atom0(),bnd.atom1(),connectivity,group1,group0);
    }
    
    if (bnd.atom1().resNum() == connectivity.resNum())
    {
        //add the atom being split to its group
        group1.add(bnd.atom1());
        //group0 is constructed starting at 'atom0' and rooted at 'atom1'
        traceAtoms(bnd.atom1(),bnd.atom0(),connectivity,group0,group1);
    }
        
    //throw an exception if both groups are empty (as this implies that neither
    //of the atoms were in this residue
    if (group0.isEmpty() and group1.isEmpty())
        throw SireMol::missing_atom(QObject::tr(
            "Cannot split the bond \"%1\" as both of the atoms aren't in residue %1")
                .arg(bnd.toString(),connectivity.resNum().toString()), CODELOC);

    return AtomIDGroupPair(group0,group1);
}

/** This function splits the atoms in the residue whose connectivity is contained 
    in 'connectivity' into two parts, based on the angle 'ang'. At least one of 
    the end atoms of the angle (e.g. ang.atom0() or ang.atom2())
    must be in this residue, and be connected to part of this residue, or else
    an exception will be thrown.
    
    An exception will also be thrown if it is not possible to split the molecule
    unambiguously in two, as the angle is part of a ring.
    
    For example;
 
      C1   C3--C8
        \ /
        C2
       /  \
     C4   C5--C6-C7
     
    Splitting C5,C2,C3 would return {C5,C6,C7} in one group, and {C3,C8} in the other group.
    
    \throw SireMol::missing_atom
    \throw SireMol::ring_error
*/
AtomIDGroupPair SIREMOL_EXPORT splitResidue(const SireMol::Angle &ang, 
                                            const ResidueBonds &connectivity)
{
    AtomIDGroup group0,group1;
    
    if (ang.atom0().resNum() == connectivity.resNum())
    {
        //add the atom being split to its group
        group0.add(ang.atom0());
        //group0 is constructed starting at 'atom0' and rooted at 'atom1'
        traceAtoms(ang.atom0(),ang.atom1(),connectivity,group1,group0);
    }
    
    if (ang.atom2().resNum() == connectivity.resNum())
    {
        //add the atom being split to its group
        group1.add(ang.atom2());
        //group0 is constructed starting at 'atom2' and rooted at 'atom1'
        traceAtoms(ang.atom2(),ang.atom1(),connectivity,group0,group1);
    }
        
    //throw an exception if both groups are empty (as this implies that neither
    //of the atoms were in this residue
    if (group0.isEmpty() and group1.isEmpty())
        throw SireMol::missing_atom(QObject::tr(
            "Cannot split the angle \"%1\" as both of the end atoms aren't in residue %1")
                .arg(ang.toString(),connectivity.resNum().toString()), CODELOC);

    return AtomIDGroupPair(group0,group1);
}

/** This function splits the atoms in the residue whose connectivity is contained 
    in 'connectivity' into two parts, based on the dihedral 'dih'. At least one of 
    the end atoms of the dihedral (e.g. dih.atom0() or dih.atom3())
    must be in this residue, and be connected to part of this residue, or else
    an exception will be thrown.
    
    An exception will also be thrown if it is not possible to split the molecule
    unambiguously in two, as the dihedral is part of a ring.
    
    C1   C4--C5--C6
      \ /
      C2    C8--C9
     /  \  /
    C3   C7
           \
            C10--C11
            
    Splitting C4,C2,C7,C10 will return {C2,C4,C5,C6} and {C7,C10,C11}. If this molecule had 
    been split by just Bond(C2,C7) using the above function, then the first returned group would 
    be {C1,C2,C3,C4,C5,C6}, while the second group would be {C7,C8,C9,C10,C11}.
    
    \throw SireMol::missing_atom
    \throw SireMol::ring_error
*/    
AtomIDGroupPair SIREMOL_EXPORT splitResidue(const Dihedral &dih, 
                                            const ResidueBonds &connectivity)
{
    AtomIDGroup group0,group1;
    
    if (dih.atom0().resNum() == connectivity.resNum())
    {
        //add the atom being split to its group
        group0.add(dih.atom0());
        //group0 is constructed starting at 'atom0' and rooted at 'atom1'
        traceAtoms(dih.atom0(),dih.atom1(),connectivity,group1,group0);
    }
    
    if (dih.atom2().resNum() == connectivity.resNum())
    {
        //add the atom being split to its group
        group1.add(dih.atom2());
        //group0 is constructed starting at 'atom3' and rooted at 'atom2'
        traceAtoms(dih.atom3(),dih.atom2(),connectivity,group0,group1);
    }
        
    //throw an exception if both groups are empty (as this implies that neither
    //of the atoms were in this residue
    if (group0.isEmpty() and group1.isEmpty())
        throw SireMol::missing_atom(QObject::tr(
            "Cannot split the dihedral \"%1\" as both of the end atoms aren't in residue %1")
                .arg(dih.toString(),connectivity.resNum().toString()), CODELOC);

    return AtomIDGroupPair(group0,group1);
}

/** This function splits the atoms in the residue whose connectivity is contained 
    in 'connectivity' into two parts, based on the improper angle 'imp'. At least 
    one of the end atoms of the improper (e.g. imp.atom0(), imp.atom2() or imp.atom3())
    must be in this residue, and be connected to part of this residue, or else
    an exception will be thrown.
    
    An exception will also be thrown if it is not possible to split the molecule
    unambiguously in two, as the dihedral is part of a ring.
    
    The improper is split based on the bond between imp.atom0() and imp.atom1(), with 
    the additional requirement that imp.atom2() and imp.atom3(), and all those 
    connected to imp.atom2() and imp.atom3() are added to group1 as well.
    
    For example
    
    C1   C4--C5--C6
      \ /
      C2    C8--C9
     /  \  /
    C3   C7
           \
            C10--C11
    
    splitting the improper C2-C7-C8-C10 will result in two groups;
    
    [C1,C2,C3,C4,C5,C6] and [C7,C8,C9,C10,C11]
    
    \throw SireMol::missing_atom
    \throw SireMol::ring_error
*/    
AtomIDGroupPair SIREMOL_EXPORT splitResidue(const Improper &imp, 
                                            const ResidueBonds &connectivity)
{
    AtomIDGroup group0,group1;
    
    if (imp.atom0().resNum() == connectivity.resNum())
    {
        //add the atom being split to its group
        group0.add(imp.atom0());
        //group0 is constructed starting at 'atom0' and rooted at 'atom1'
        traceAtoms(imp.atom0(),imp.atom1(),connectivity,group1,group0);
    }
    
    if (imp.atom1().resNum() == connectivity.resNum())
    {
        //add the atom being split to its group
        group1.add(imp.atom1());
        //group1 is constructed starting at 'atom1' and rooted at 'atom0'
        traceAtoms(imp.atom1(),imp.atom0(),connectivity,group0,group1);
    }
    
    if (imp.atom2().resNum() == connectivity.resNum())
    {
        //add the atom being split to its group
        group1.add(imp.atom2());
        //group1 is added to by starting at 'atom2' and rooted at 'atom1'
        traceAtoms(imp.atom2(),imp.atom1(),connectivity,group0,group1);
    }
    
    if (imp.atom3().resNum() == connectivity.resNum())
    {
        //add the atom being split to its group
        group1.add(imp.atom3());
        //group1 is added to by starting at 'atom3' and rooted at 'atom1'
        traceAtoms(imp.atom3(),imp.atom1(),connectivity,group0,group1);
    }
    
    //throw an exception if both groups are empty (as this implies that neither
    //of the atoms were in this residue
    if (group0.isEmpty() and group1.isEmpty())
        throw SireMol::missing_atom(QObject::tr(
            "Cannot split the improper angle \"%1\" as the end atoms aren't in residue %1")
                .arg(imp.toString(),connectivity.resNum().toString()), CODELOC);

    return AtomIDGroupPair(group0,group1);
}

/** Split a whole molecule into two based on the bond 'bnd' and the molecular 
    connectivity in 'connectivity'. At least one atom
    must be in this residue, and be connected to part of this molecule, or else
    an exception will be thrown.
    
    An exception will also be thrown if it is not possible to split the molecule
    unambiguously in two, as the bond is part of a ring.
    
    For example;
 
          C1--C2--C3--C4--C5--C6--C7--C8
          
    Splitting C3 and C4 would result in two groups, {C1,C2,C3} and {C4,C5,C6,C7,C8}
    
          C1\
          C2-C4--C5
          C3/
 
    Splitting C4 and C5 would result in two groups, {C1,C2,C3,C4} and {C5}
 
    However splitting C1 and C5 would add a bond between C1 and C5. This would mean 
    than C1-C4-C5 would form a ring, so an exception would be thrown.
    
    \throw SireMol::missing_atom
    \throw SireMol::ring_error
*/
AtomIDGroupPair SIREMOL_EXPORT splitMolecule(const Bond &bnd, 
                                             const MoleculeBonds &connectivity)
{
    AtomIDGroup group0,group1;
    
    //add the atoms to their groups...
    group0.add(bnd.atom0());
    group1.add(bnd.atom1());
    
    //group0 starts with atom0 and is rooted at atom1
    traceAtoms(bnd.atom0(),bnd.atom1(),connectivity,group1,group0);
    //group1 starts with atom1 and is rooted at atom0
    traceAtoms(bnd.atom1(),bnd.atom0(),connectivity,group0,group1);

    return AtomIDGroupPair(group0,group1);
}

AtomIDGroupPair SIREMOL_EXPORT splitMolecule(const SireMol::Angle &ang, 
                                             const MoleculeBonds &connectivity)
{
    AtomIDGroup group0,group1;
    
    //add the atoms to their groups...
    group0.add(ang.atom0());
    group1.add(ang.atom2());
    
    //group0 starts with atom0 and is rooted at atom1
    traceAtoms(ang.atom0(),ang.atom1(),connectivity,group1,group0);
    //group1 starts with atom2 and is rooted at atom1
    traceAtoms(ang.atom2(),ang.atom1(),connectivity,group0,group1);

    return AtomIDGroupPair(group0,group1);
}

AtomIDGroupPair SIREMOL_EXPORT splitMolecule(const Dihedral &dih, 
                                             const MoleculeBonds &connectivity)
{
    AtomIDGroup group0,group1;
    
    //add the atoms to their groups
    group0.add(dih.atom0());
    group1.add(dih.atom3());
    
    //group0 starts with atom0 and is rooted at atom1
    traceAtoms(dih.atom0(),dih.atom1(),connectivity,group1,group0);
    //group1 starts with atom3 and is rooted at atom2
    traceAtoms(dih.atom3(),dih.atom2(),connectivity,group0,group1);

    return AtomIDGroupPair(group0,group1);
}

AtomIDGroupPair splitMolecule(const Improper &imp, const MoleculeBonds &connectivity)
{
    AtomIDGroup group0,group1;
    
    //add the atoms to their groups
    group0.add(imp.atom0());
    
    group1.add(imp.atom1());
    group1.add(imp.atom2());
    group1.add(imp.atom3());
    
    //group0 starts at atom0 and is rooted at atom1
    traceAtoms(imp.atom0(),imp.atom1(),connectivity,group1,group0);
    //group1 starts at atom2 and is rooted at atom1
    traceAtoms(imp.atom2(),imp.atom1(),connectivity,group0,group1);
    //group1 also has the atoms starting at atom3 and rooted at atom1
    traceAtoms(imp.atom3(),imp.atom1(),connectivity,group0,group1);
    
    return AtomIDGroupPair(group0,group1);
}

} // end of namespace SireMol
