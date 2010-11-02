
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

protein = PDB().readMolecule("test/io/peptide.pdb")

protoms = ProtoMS("%s/protoms2" % protoms_dir)
protoms.addParameterFile("%s/parameter/amber99.ff" % protoms_dir)
protoms.addParameterFile("%s/parameter/amber99-residues.ff" % protoms_dir)

print "Parameterising the protein (this can take a while)..."
protein = protoms.parameterise(protein, ProtoMS.PROTEIN)
print "...done!"

residues = MoleculeGroup("residues")

hn_atoms = AtomName("N", CaseInsensitive) * \
           AtomName("HN", CaseInsensitive) * AtomName("HN1", CaseInsensitive) * \
           AtomName("HN2", CaseInsensitive) * AtomName("HN3", CaseInsensitive)

print hn_atoms

for i in range(0, protein.nResidues()):
    atoms = protein.select(ResIdx(i)).selection()

    if i < (protein.nResidues()-1):
       try:
           atoms.deselect( hn_atoms + ResIdx(i) )    
       except:
           pass

    if i > 0:
       try:
           atoms.select( hn_atoms + ResIdx(i-1) )
       except:
           pass

    ats = Selector_Atom_(protein,atoms)

    print "RESIDUE %d" % i

    for at in ats:
        print at

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

bbmoves.setMaximumTranslation(0.025*angstrom)
bbmoves.setMaximumRotation(1*degrees)
bbmoves.setCenterOfRotation( GetCOGPoint( AtomName("CA", Sire.ID.CaseInsensitive),
                                          AtomName("N", Sire.ID.CaseInsensitive) ) )

scmoves = ZMatMove(residues)

moves = WeightedMoves()
moves.add(bbmoves,1)
moves.add(scmoves,1)

print system.energy()

PDB().write(system.molecules(), "test0000.pdb")

print "Performing moves..."

for i in range(0,10):
    print "Moves %d..." % (i+1)
    system = moves.move(system, 10000, False)

    print system.energy()

    PDB().write(system.molecules(), "test%0004d.pdb" % (i+1))

print moves

