
from Sire.IO import *
from Sire.Mol import *
from Sire.Maths import *
from Sire.Move import *
from Sire.MM import *
from Sire.Vol import *
from Sire.System import *
from Sire.Qt import *
from Sire.Units import *

import sys
import os

solvent = PDB().read("test/io/water.pdb")

tip4p = solvent[MolIdx(0)].molecule()

protoms_dir = "%s/Work/ProtoMS" % os.getenv("HOME")

protoms = ProtoMS("%s/protoms2" % protoms_dir)
protoms.addParameterFile("%s/parameter/solvents.ff" % protoms_dir)

tip4p = tip4p.edit().rename("T4P").commit()
tip4p = protoms.parameterise(tip4p, ProtoMS.SOLVENT)

tip4p_chgs = tip4p.property("charge")
tip4p_ljs = tip4p.property("LJ")

for i in range(0, solvent.nMolecules()):
    mol = solvent.moleculeAt(i).molecule()
    mol = mol.edit().rename("T4P") \
                    .setProperty("charge", tip4p_chgs) \
                    .setProperty("LJ", tip4p_ljs) \
             .commit()

    solvent.update(mol)

words = open("test/io/water.xsc", "r").readlines()[0].split()

space = PeriodicBox( Vector(float(words[0]), float(words[1]), float(words[2])), \
                     Vector(float(words[3]), float(words[4]), float(words[5])) )

cljff = InterCLJFF("cljff")
cljff.add(solvent)

all = MoleculeGroup("all")
all.add(solvent)

switchfunc = HarmonicSwitchingFunction(15*angstrom, 14.5*angstrom,
                                       15*angstrom, 14.5*angstrom)

system = System()
system.add(cljff)
system.add(all)
system.setProperty("space", space)
system.setProperty("switchingFunction", switchfunc)

mc = RigidBodyMC(solvent)
mc.setMaximumTranslation( 0.25 * angstrom )
mc.setMaximumRotation( 5 * degrees )

def run(system, moves, nmoves):
    t = QTime()

    print "Running %d moves..." % nmoves
    t.start()
    moves.move(system, nmoves, True)
    ms = t.elapsed()

    print "%d moves took %d ms (%f moves per second)" % (nmoves, ms, (1000.0*nmoves)/ms)

print "Calculating total energy..."
t = QTime()
t.start()
print system.energy(),t.elapsed()

run(system, mc, 500)

print system.energy()
system.mustNowRecalculateFromScratch()
print system.energy()

# make the box bigger (3x3)
delta = space.dimensions()
print "Space == %s" % delta

for i in range(-1,1):
    for j in range(-1,1):
        for k in range(-1,1):
            if i == 0 and j == 0 and k == 0:
                continue

            print "Adding molecules to box %d,%d,%d..." % (i,j,k)

            add_mols = Molecules()

            for imol in range(0,solvent.nMolecules()):
                mol = solvent.moleculeAt(imol).molecule()
                mol = mol.edit().renumber().commit()
                mol = mol.move().translate( Vector( i*delta.x(), j*delta.y(), k*delta.z() ) ).commit()
                add_mols.add(mol)

            cljff.add(add_mols)
            all.add(add_mols)

print "Updating the system..."
system = System()
system.add(cljff)
system.add(all)

new_space = PeriodicBox( 3 * delta )
system.setProperty("space", new_space)
system.setProperty("switchingFunction", switchfunc)

#print "Test pdb"
#PDB().write(system.molecules(), "test.pdb")

print "Calculating total energy..."
t.start()
print system.energy(),t.elapsed()

run(system, mc, 100)
run(system, mc, 100)
