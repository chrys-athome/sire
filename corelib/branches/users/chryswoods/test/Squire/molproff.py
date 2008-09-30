
from Sire.Mol import *
from Sire.IO import *
from Sire.Vol import *
from Sire.FF import *
from Sire.MM import *
from Sire.CAS import *
from Sire.Base import *
from Sire.Maths import *
from Sire.Qt import *
from Sire.Units import *
from Sire.Squire import *

import os

timer = QTime()

#read in all of the molecules
print "Loading the molecules..."
timer.start()
mols = PDB().read("test/io/water.pdb")

ms = timer.elapsed()
print "... took %d ms" % ms

#specify the space in which the molecules are placed
space = Cartesian()

space = PeriodicBox(Vector(-18.3854,-18.66855,-18.4445), \
                    Vector( 18.3854, 18.66855, 18.4445))

#specify the type of switching function to use
switchfunc = HarmonicSwitchingFunction(80.0)
switchfunc = HarmonicSwitchingFunction(15.0, 14.5)

#create a forcefield for the molecules
qmff = QMFF("QMFF")

qmff.setProperty("space", space)

molpro = Molpro()
molpro.setEnvironment("TMPDIR", "/Users/chris/tmp")

qmff.setProperty("quantum program", molpro)

qmff.add(mols.moleculeAt(0))

print qmff.energy()
