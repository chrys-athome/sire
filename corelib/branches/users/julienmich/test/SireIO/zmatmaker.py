
from Sire.IO import *
from Sire.Mol import *
from Sire.CAS import *
from Sire.System import *
from Sire.Move import *
from Sire.MM import *
from Sire.FF import *
from Sire.Units import *
from Sire.Vol import *
from Sire.Maths import *
from Sire.Base import *
from Sire.Qt import *

import os,re,sys
import shutil

#zmat_maker = ProtoMSZMatrixMaker()
#zmat_maker.readtemplates("amber.pmszmat")
#residues = zmat_Maker.addZmatrix( residues )// adds property['z-matrix'] to each residue in residues
#  and
#  flexibility_maker = FlexibilityMaker()
#flexibility_maker.readFlexibilityTemplates("solute.mover")
#molecule = flexibility_maker.addFlexibility( molecule ) // adds property['flexibility'] to molecule
#4:14 PM Where property['flexibility'] contains the following information
#  ==>
#maximum_translation'
#maximum_rotation
#var_bonds
#var_angles
#var_dihedrals
#bond_deltas
#angle_deltas

zmat_maker = ZmatrixMaker()
zmat_maker.loadTemplates("test/io/amber.zmatrices")

top_file = "test/io/SYSTEM.top"
crd_file = "test/io/SYSTEM.crd"
amber = Amber()
molecules, space = amber.readcrdtop(crd_file, top_file)

moleculeNumbers = molecules.molNums()
moleculeList = []

for moleculeNumber in moleculeNumbers:
    molecule = molecules.molecule(moleculeNumber).molecule()
    moleculeList.append(molecule)

solute = moleculeList[0]
solute = MoleculeGroup("solute", solute)
protein = moleculeList[1]

protein = zmat_maker.applyTemplates( protein )

#residues = MoleculeGroup("residues")
#for i in range(0, protein.nResidues()):
#    res = protein.residue( ResIdx(i) )
#    residues.add(res)
protein = MoleculeGroup("protein", protein)
solvent = MoleculeGroup("solvent")
for molecule in moleculeList[2:]:
    solvent.add(molecule)

all = MoleculeGroup("all")
all.add(solute)
all.add(solvent)
all.add(protein)

# Add these groups to the System
system = System()

system.add(solute)
system.add(solvent)
system.add(protein)
system.add(residues)
system.add(all)



# residues = zmat_Maker.addZmatrix( residues )// adds property['z-matrix'] to each residue in residues

# Setup force fields

# Setup Moves

# Run a short simulation
