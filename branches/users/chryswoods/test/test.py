
from Sire.Mol import *
from Sire.IO import *
from Sire.Vol import *
from Sire.FF import *
from Sire.MM import *
from Sire.CAS import *
from Sire.Cluster import *
from Sire.Maths import *
from Sire.Qt import *
from Sire.Units import *

t = QTime()

t.start()
mols = PDB().read("test/io/water.pdb")

ms = t.elapsed()

print "Reading waters took %d ms..." % ms

tip4p = mols[0]

tip4p_1 = mols[1]

cgroups = tip4p.coordGroups()

box = PeriodicBox( Vector(5,5,5), Vector(-5,-5,-5) )

t.start()
PDB().write(tip4p, "water0.pdb")
ms = t.elapsed()

print "Writing a water took %d ms..." % ms

t.start()
cgroups = box.moveToCenterBox(cgroups)
ms = t.elapsed()

print "Moving to central box took %d ms..." % ms

t.start()
tip4p.setCoordinates(cgroups)
ms = t.elapsed()

print box.minimumDistance(tip4p.coordGroups()[0], \
                          tip4p_1.coordGroups()[0])

print "Setting the coordinates took %d ms..." % ms

PDB().write(tip4p, "water1.pdb")

cgroups = box.getMinimumImage(cgroups, Vector(1025,1025,1025))
tip4p.setCoordinates(cgroups)

PDB().write(tip4p, "water2.pdb")

print box.minimumDistance(tip4p.coordGroups()[0], \
                          tip4p_1.coordGroups()[0])

