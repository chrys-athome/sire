
from Sire.IO import *
from Sire.MM import *
from Sire.Move import *
from Sire.Maths import *
from Sire.Mol import *
from Sire.System import *
from Sire.Units import *
from Sire.ID import *

import os

protoms_dir = "%s/Work/ProtoMS" % (os.getenv("HOME"))

protein = PDB().readMolecule("test/io/p38.pdb")

protoms = ProtoMS("%s/protoms2" % protoms_dir)
protoms.addParameterFile("%s/parameter/amber99.ff" % protoms_dir)
protoms.addParameterFile("%s/parameter/amber99-residues.ff" % protoms_dir)

print "Parameterising the protein (this can take a while)..."
protein = protoms.parameterise(protein, ProtoMS.PROTEIN)
print "...done!"

residues = MoleculeGroup("residues")

for i in range(0, protein.nResidues()):
    atoms = protein.select(ResIdx(i)).selection()

    if i < (protein.nResidues()-1):
       atoms.deselect( AtomName("N", CaseInsensitive) + ResIdx(i) )
    
    if i > 0:
       atoms.select( AtomName("N", CaseInsensitive) + ResIdx(i) )

    residues.add( PartialMolecule(protein, atoms) )

intraff = InternalFF("intraff")
intraclj = IntraCLJFF("intraclj")

intraff.add(protein)
intraclj.add(protein)

system = System()
system.add(residues)
system.add(intraff)
system.add(intraclj)

bbmoves = RigidBodyMC(residues)

# updated version would be something like;
# bbmoves = RigidBodyMC(residues, {"center" : GetBBMoveCenter()})

bbmoves.setMaximumTranslation(0.05*angstrom)
bbmoves.setMaximumRotation(2*degrees)

scmoves = ZMatMove(residues)

moves = WeightedMoves()
moves.add(bbmoves,1)
moves.add(scmoves,1)

print system.energy()

PDB().write(system.molecules(), "test0000.pdb")

print "Performing moves..."

for i in range(0,10):
    print "Moves %d..." % (i+1)
    system = moves.move(system, 1000, False)

    print system.energy()

    PDB().write(system.molecules(), "test%0004d.pdb" % (i+1))

print moves

