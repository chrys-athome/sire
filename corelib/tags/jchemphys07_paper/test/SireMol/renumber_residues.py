
from Sire.IO import *
from Sire.Mol import *

mols = PDB().read("test/io/water.pdb")

def renumberResidues(mols, newnum):

    newmols = []

    for mol in mols:
        editor = mol.edit()
    
        editor.setName("WATER")
        editor.renumberResidue(mol.atoms()[0].resNum(), newnum)
    
        newmols.append(Molecule(editor))

    return newmols

mols = renumberResidues(mols, ResNum(1))

for mol in mols:
    print mol.name(), mol.atoms()[0].resNum()
