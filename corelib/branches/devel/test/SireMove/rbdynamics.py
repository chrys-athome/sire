
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

ion = Molecule("ion")
ion = ion.edit().add( CGName("1") ) \
                .add( AtomName("Na") ).molecule().commit()

salt = Molecule("salt")
salt = salt.edit().add( CGName("1") ) \
                  .add( AtomName("Na") ).cutGroup() \
                  .add( AtomName("Cl") ).molecule().commit()

ion = ion.edit().atom(AtomName("Na")) \
                .setProperty("coordinates", Vector(0,0,0)) \
                .setProperty("element", Element("Sodium")) \
                .setProperty("charge", 1*mod_electron) \
                .setProperty("LJ", LJParameter(3.0522*angstrom,0.4598*kcal_per_mol)) \
                .molecule().commit()

salt = salt.edit().atom(AtomName("Na")) \
                  .setProperty("coordinates", Vector(0,0,0)) \
                  .setProperty("element", Element("Sodium")) \
                  .setProperty("charge", 1*mod_electron) \
                  .setProperty("LJ", LJParameter(3.0522*angstrom,0.4598*kcal_per_mol)) \
                  .molecule() \
                  .atom(AtomName("Cl")) \
                  .setProperty("coordinates", Vector(0,4,0)) \
                  .setProperty("element", Element("Sodium")) \
                  .setProperty("charge", -1*mod_electron) \
                  .setProperty("LJ", LJParameter(3.0522*angstrom,0.4598*kcal_per_mol)) \
                  .molecule() \
                  .commit()

m0 = salt
m1 = salt.edit().renumber() \
         .atom(AtomName("Na")) \
         .setProperty("coordinates", Vector(5,2,0)) \
         .molecule().atom(AtomName("Cl")) \
         .setProperty("coordinates", Vector(9,2,0)) \
         .molecule().commit()

m2 = salt.edit().renumber() \
         .atom(AtomName("Na")) \
         .setProperty("coordinates", Vector(-6,4.1,0)) \
         .molecule().atom(AtomName("Cl")) \
         .setProperty("coordinates", Vector(-6,0.1,0)) \
         .molecule().commit()

salt = MoleculeGroup("salt")
salt.add(m0)
salt.add(m1)
salt.add(m2)

cljff = InterCLJFF("salt-salt")
cljff.add(salt)

system = System()
system.add(salt)
system.add(cljff)

t.start()                                       
print "Initial energy = %s" % system.energy()
print "(took %d ms)" % t.elapsed()

#mdmove = MolecularDynamics( salt, VelocityVerlet(), 
#                            {"velocity generator":MaxwellBoltzmann(25*celsius)} )

mdmove = MolecularDynamics(salt, DLMRigidBody())

mdmove.setTimeStep(0.1*femtosecond)

print system.property("space")

print "\nMove 0"
print system.energy()
print mdmove.kineticEnergy()
print system.energy() + mdmove.kineticEnergy()
PDB().write(system.molecules(), "test%0004d.pdb" % 0)

for i in range(1,250):
    print "\nmove %d" % (i)
    mdmove.move(system, 600)

    print system.energy()
    print mdmove.kineticEnergy()
    print system.energy() + mdmove.kineticEnergy()

    PDB().write(system.molecules(), "test%0004d.pdb" % i)
