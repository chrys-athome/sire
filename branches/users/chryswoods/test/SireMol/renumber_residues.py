
from Sire.IO import *
from Sire.Mol import *

mols = PDB().read("test/io/water.pdb")

newmols = []

for mol in mols[1:]:
    oldnum = mol.atoms()[0].resNum()
    
    if (oldnum != ResNum(1)):
        editmol = mol.edit()
    
        editmol.setName("WATER")
        editmol.renumberResidue(mol.atoms()[0].resNum(), ResNum(1))
    
        newmols.append(Molecule(editmol))

for mol in newmols:
    print mol.name(), mol.atoms()[0].resNum()
