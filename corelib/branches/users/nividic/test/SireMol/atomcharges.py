
from Sire.Units import *
from Sire.MM import *
from Sire.Mol import *
from Sire.IO import *
from Sire.Base import *
from Sire.Units import *

import os

water = PDB().readMolecule("test/io/water.pdb")

protoms = ProtoMS("%s/Work/ProtoMS/protoms2" % os.getenv("HOME"))
protoms.addParameterFile("test/ff/solvents.ff")

water = protoms.parameterise(water, ProtoMS.SOLVENT)

print water.property("charge").array()
print water.property("LJ").array()
