
from Sire.Mol import *
from Sire.IO import *
from Sire.Vol import *
from Sire.FF import *
from Sire.MM import *
from Sire.CAS import *
from Sire.Maths import *
from Sire.Qt import *
from Sire.Units import *
from Sire.System import *
from Sire.Move import *
from Sire.Base import *

import sys

timer = QTime()

#read in the solute molecule
print "Loading the tip4p solute..."
tip4p = PDB().read("test/io/tip4p.pdb")[0]

#read in all of the solvent molecules
print "Loading the solvent..."
solvent = PDB().read("test/io/water.pdb")

#specify the space in which the molecules are placed
space = PeriodicBox( (-18.3854,-18.66855,-18.4445),
                     ( 18.3854, 18.66855, 18.4445) )

hff = HarmonicFF(space)

tip4p.setProperty( "k", QVariant(10.0) )
tip4p.setProperty( "r0", QVariant.fromValue(tip4p.extract().geometricCenter()) )

hff.add( tip4p, { hff.parameters().k() : "k",
                  hff.parameters().r0() : "r0" } )
                  
print hff.energy()

tip4p = tip4p.move().translate( (100,0,0) )

hff.change(tip4p)
print hff.isDirty()

print hff.energy()
print hff.isDirty()

