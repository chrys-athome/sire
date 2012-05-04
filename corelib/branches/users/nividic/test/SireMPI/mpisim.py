
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
from Sire.MPI import *

import sys

t = QTime()

cljff = InterCLJFF()

mincoords = Vector(-18.3854, -18.66855, -18.4445)
maxcoords = Vector( 18.3854,  18.66855,  18.4445)

vol = PeriodicBox(mincoords, maxcoords)
switchfunc = HarmonicSwitchingFunction(15*angstrom, 14.5*angstrom)

cljff.setSpace(vol)
cljff.setSwitchingFunction(switchfunc)

mols = PDB().read("test/io/water.pdb")
                                                
print "Read in %d molecules!" % mols.nMolecules()

i = 0

t.start()
mol = mols.moleculeAt(0).molecule()

mol = mol.edit().atom( AtomName("O00") ) \
                    .setProperty("LJ", LJParameter(3.15363*angstrom,  \
                                                   0.1550*kcal_per_mol)).molecule() \
                .atom( AtomName("H01") ) \
                    .setProperty("charge", 0.520 * mod_electron).molecule() \
                .atom( AtomName("H02") ) \
                    .setProperty("charge", 0.520 * mod_electron).molecule() \
                .atom( AtomName("M03") ) \
                    .setProperty("charge", -1.04 * mod_electron).molecule() \
         .commit()

charges = mol.property("charge")
ljs = mol.property("LJ")

cljff.add(mol)

for i in range(1, mols.nMolecules()):
    mol = mols.moleculeAt(i).molecule()

    mol = mol.edit().rename("T4P") \
                    .setProperty("charge", charges) \
                    .setProperty("LJ", ljs) \
             .commit()

    cljff.add(mol)

ms = t.elapsed()
print "Parameterised all of the water molecules (in %d ms)!" % ms

system = System()

system.add(cljff)

print "Initial energy = %s" % system.energy()

mc = RigidBodyMC(cljff.group(MGIdx(0)))

moves = SameMoves(mc)

print "Running 1000 moves on the MPI master node..."
nodes = MPINodes()
node = nodes.getFreeNode()
print "node rank = %d of %d" % (node.rank(), nodes.nNodes())

t.start()
sim = Simulation.run(node, system, moves, 1000)
ms = t.elapsed()

print "Submitted in %d ms" % ms

print sim.hasFinished()
print sim.hasStarted()
print sim.isRunning()

print "Waiting..."
sim.wait()
ms = t.elapsed()

system = sim.system()

print "Final energy = %s" % system.energy()

system.mustNowRecalculateFromScratch();

print "Are we sure? = %s" % system.energy()

mc = sim.moves().moves()[0]

print "nAccepted() == %d, nRejected() == %d  (%f %%)" % (mc.nAccepted(), \
                            mc.nRejected(), 100 * mc.acceptanceRatio())

print "Took %d ms" % ms
