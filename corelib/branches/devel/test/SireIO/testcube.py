
from Sire.IO import *
from Sire.FF import *
from Sire.MM import *
from Sire.Mol import *
from Sire.Units import *
from Sire.Vol import *

import os

protoms_dir = "%s/Work/ProtoMS" % os.getenv("HOME")

tip4p = PDB().readMolecule("test/io/water.pdb")

tip4p = tip4p.edit().rename("T4P").commit()

protoms = ProtoMS("%s/protoms2" % protoms_dir)
protoms.addParameterFile("%s/parameter/solvents.ff" % protoms_dir)

tip4p = protoms.parameterise(tip4p, ProtoMS.SOLVENT)

cljff = InterCLJFF()

cljff.add(tip4p)

print cljff.energy()

grid = RegularGrid(tip4p.evaluate().center(), 100, 0.1*angstrom)

print grid
print grid.minCoords(), grid.maxCoords(), grid.gridSpacing()

table = PotentialTable( cljff[MGIdx(0)], grid )

cljff.potential(table, CLJProbe(1*mod_electron))

Cube().write(table, cljff, "test.cube")
