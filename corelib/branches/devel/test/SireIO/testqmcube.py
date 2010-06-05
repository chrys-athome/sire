
from Sire.IO import *
from Sire.FF import *
from Sire.MM import *
from Sire.Mol import *
from Sire.Units import *
from Sire.Vol import *
from Sire.Squire import *

import os

protoms_dir = "%s/Work/ProtoMS" % os.getenv("HOME")

tip4p = PDB().readMolecule("test/io/water.pdb")

tip4p = tip4p.edit().rename("T4P").commit()

protoms = ProtoMS("%s/protoms2" % protoms_dir)
protoms.addParameterFile("%s/parameter/solvents.ff" % protoms_dir)

tip4p = protoms.parameterise(tip4p, ProtoMS.SOLVENT)

molpro = Molpro()
molpro.setMethod("HF")
molpro.setBasisSet("VDZ")

qmff = QMFF()
qmff.setQuantumProgram(molpro)

qmff.add(tip4p)

print qmff.energy()

grid = RegularGrid(tip4p.evaluate().center(), 4, 2*angstrom)

print grid
print grid.minCoords(), grid.maxCoords(), grid.gridSpacing()

table = PotentialTable( qmff[MGIdx(0)], grid )

qmff.potential(table, CLJProbe(1*mod_electron))

Cube( 10*kcal_per_mol ).write(table, qmff, "testqm.cube")
