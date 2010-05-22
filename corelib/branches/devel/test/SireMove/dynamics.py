
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
from Sire.Stream import *

import sys

t = QTime()

cljff = InterCLJFF("cljff")

ne = Molecule("Neon")
ne = ne.edit().add( CGName("1") ).add( AtomName("Ne") ).molecule().commit()

ne = ne.edit().atom(AtomName("Ne")) \
              .setProperty("coordinates", Vector(0,0,0)) \
              .setProperty("element", Element("Neon")) \
              .setProperty("charge", 0*mod_electron) \
              .setProperty("LJ", LJParameter.fromRMinAndEpsilon(3.06*angstrom,-0.086*kcal_per_mol)) \
              .molecule() \
              .commit()

neon = MoleculeGroup("neon")

# create a box of neon
for i in range(0,6):
    for j in range(0,6):
        for k in range(0,6):
            ne = ne.edit().renumber() \
                   .setProperty("coordinates", AtomCoords([Vector(i*3.06,j*3.06,k*3.06)]) ) \
                   .commit()

            neon.add(ne)

cljff = InterCLJFF("neon-neon")
cljff.add(neon)

system = System()
system.add(neon)
system.add(cljff)

t.start()                                       
print "Initial energy = %s" % system.energy()
print "(took %d ms)" % t.elapsed()

mdmove = MolecularDynamics( neon, VelocityVerlet(), 
                            {"velocity generator":RandomVelocities()} )
mdmove.setTimeStep(2*femtosecond)

print system.property("space")

for i in range(0,10):
    print "\nmove %d" % (i+1)
    mdmove.move(system, 100)

    print system.energy()
    print mdmove.kineticEnergy()
    print system.energy() + mdmove.kineticEnergy()

    PDB().write(system.molecules(), "test%0004d.pdb" % i)
