
#include "qhash_siremol.h"

#include "convertmolecule.h"

#include "editmol.h"
#include "molecule.h"

namespace SireMol
{

/** Return the atoms in 'editmol' that are arranged into CutGroups
    such that each EditRes has its own CutGroup (residue-based cutting) */
CutGroupVector SIREMOL_EXPORT residueCutGroups(const EditMol &editmol)
{
    CutGroupVector cgroups;
    cgroups.reserve(editmol.nResidues());
    
    //put the atoms for each residue into its own CutGroup...
    ResNumList resnums = editmol.residueNumbers();
    foreach( ResNum resnum, resnums )
    {
        cgroups.append( CutGroup(editmol.atoms(resnum)) );
    }
    
    cgroups.squeeze();
    
    return cgroups;
}

/** Return the atoms in 'editmol' in a single CutGroup */
CutGroupVector SIREMOL_EXPORT moleculeCutGroups(const EditMol &editmol)
{
    CutGroupVector cgroups;
    cgroups.reserve(1);

    cgroups.append( CutGroup(editmol.atoms()) );
    cgroups.squeeze();

    return cgroups;
}

/** Convert each molecule in 'editmols' into a Molecule that is rigid
    (has no internal bonding) and that will use a residue-based
    non-bonded cutoff. This returns the set of Molecules */
MoleculeSet SIREMOL_EXPORT rigidAndResidueCutting(const EditMolList &editmols)
{
    //create a set of molecules to hold the output - reserve
    //sufficient space...
    MoleculeSet mols;
    
    int nmols = editmols.count();
    mols.reserve(nmols);

    //loop through each EditMol and convert it...
    for (int i=0; i<nmols; ++i)
    {
        const EditMol &editmol = editmols[i];
        
        //create the CutGroups that will contain the atoms in the molecule
        CutGroupVector cgroups = residueCutGroups(editmol);
        
        //now create the molecule
        mols.insert( Molecule( editmol.name(), editmol.residueIDs(), cgroups ) );
    }
    
    return mols;
}

/** Convert each molecule in 'editmols' into a Molecule that is flexible
    (has internal bonding) and that will use a residue-based non-bonded
    cutoff. This returns the set of Molecules */
MoleculeSet SIREMOL_EXPORT flexibleAndResidueCutting(const EditMolList &editmols)
{
    //create a set of molecules to hold the output - reserve
    //sufficient space...
    MoleculeSet mols;
    
    int nmols = editmols.count();
    mols.reserve(nmols);

    //loop through each EditMol and convert it...
    for (int i=0; i<nmols; ++i)
    {
        const EditMol &editmol = editmols[i];
        
        //create the CutGroups that will contain the atoms in the molecule
        CutGroupVector cgroups = residueCutGroups(editmol);
        
        //now create the molecule
        mols.insert( Molecule( editmol.name(), editmol.residueIDs(), 
                               cgroups, editmol.connectivity() ) );
    }
    
    return mols;
}

/** Convert each molecule in 'editmols' into a Molecule that is rigid
    (has no internal bonding) and that will use a molecule-based non-bonded
    cutoff. This returns the set of Molecules */
MoleculeSet SIREMOL_EXPORT rigidAndMoleculeCutting(const EditMolList &editmols)
{
    //create a set of molecules to hold the output - reserve
    //sufficient space...
    MoleculeSet mols;
    
    int nmols = editmols.count();
    mols.reserve(nmols);

    //loop through each EditMol and convert it...
    for (int i=0; i<nmols; ++i)
    {
        const EditMol &editmol = editmols[i];
        
        //create the CutGroups that will contain the atoms in the molecule
        CutGroupVector cgroups = moleculeCutGroups(editmol);
        
        //now create the molecule
        mols.insert( Molecule( editmol.name(), editmol.residueIDs(), cgroups ) );
    }
    
    return mols;
}

/** Convert each molecule in 'editmols' into a Molecule that is flexible
    (has internal bonding) and that will use a molecule-based non-bonded
    cutoff. This returns the set of Molecules */
MoleculeSet SIREMOL_EXPORT flexibleAndMoleculeCutting(const EditMolList &editmols)
{
    //create a set of molecules to hold the output - reserve
    //sufficient space...
    MoleculeSet mols;
    
    int nmols = editmols.count();
    mols.reserve(nmols);

    //loop through each EditMol and convert it...
    for (int i=0; i<nmols; ++i)
    {
        const EditMol &editmol = editmols[i];
        
        //create the CutGroups that will contain the atoms in the molecule
        CutGroupVector cgroups = moleculeCutGroups(editmol);
        
        //now create the molecule
        mols.insert( Molecule( editmol.name(), editmol.residueIDs(), 
                               cgroups, editmol.connectivity() ) );
    }
    
    return mols;
}

/** Convert each Molecule in 'mols' into an EditMol, and return the list
    of created EditMols */
EditMolList SIREMOL_EXPORT convert(const MoleculeSet &mols)
{
    EditMolList editmols;
    
    int nmols = mols.count();
    for (int i=0; i<nmols; ++i)
    {
        editmols.append( EditMol(mols[i]) );
    }
    
    return editmols;
}

}
