
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
from Sire.MPI import *
from Sire.System import *
from Sire.Move import *

import os
import sys

timer = QTime()

#read in all of the molecules
print "Loading and parameterising the molecules..."
timer.start()
mols = PDB().read("test/io/water.pdb")

mol = mols.moleculeAt(0).molecule()

mol = mol.edit().atom( AtomName("H01") ) \
                    .setProperty("charge", 0.520 * mod_electron).molecule() \
                .atom( AtomName("H02") ) \
                    .setProperty("charge", 0.520 * mod_electron).molecule() \
                .atom( AtomName("M03") ) \
                    .setProperty("charge", -1.04 * mod_electron).molecule() \
         .commit()

charges = mol.property("charge")

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

print "QM energy in current thread"

qmnrg = qmff.energy()

print qmnrg

qmmmff = QMMMFF("qmmmff")

qmmmff.setSpace(space)
qmmmff.setSwitchingFunction(switchfunc)
qmmmff.setQuantumProgram(molpro)

qmmmff.add(mols.moleculeAt(0), MGIdx(0))

for i in range(1, mols.nMolecules()):
    mol = mols.moleculeAt(i).molecule()

    mol = mol.edit().setProperty("charge", charges) \
             .commit()

    qmmmff.add(mol, MGIdx(1))

qmmmnrg = qmmmff.energy()

print qmmmnrg
print qmmmnrg - qmnrg

system = System()

system.add(qmmmff)

print "Initial energy = %s" % system.energy()

system.mustNowRecalculateFromScratch()

mc = RigidBodyMC(qmmmff.group(MGIdx(1)))

moves = SameMoves(mc)

print "Running 5 moves on the MPI master node..."
nodes = MPINodes()
node = nodes.getFreeNode()
print "node rank = %d of %d" % (node.rank(), nodes.nNodes())

promise = node.start( MPISimWorker(system, moves, 5, True) )

print "Job submitted. Waiting..."

promise.wait()

print "Job complete!"

sys.exit(0)
