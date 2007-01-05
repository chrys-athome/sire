
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

import time

timer = QTime()

#read in all of the molecules
print "Loading the molecules..."
timer.start()
mols = PDB().read("test/io/water.pdb")

ms = timer.elapsed()
print "... took %d ms" % ms

#specify the space in which the molecules are placed
space = Cartesian()

#space = PeriodicBox(Vector(-18.3854,-18.66855,-18.4445), \
#                    Vector( 18.3854, 18.66855, 18.4445))

#specify the type of switching function to use
switchfunc = HarmonicSwitchingFunction(80.0)
switchfunc = HarmonicSwitchingFunction(15.0, 14.5)

#create a forcefield for the molecules
cljff = InterCLJFF( Space(space), \
                    SwitchingFunction(switchfunc) )

cljff2 = Tip4PFF( Space(space), SwitchingFunction(switchfunc) )

#parametise each molecule and add it to the forcefield
print "Parametising the molecules..."

chgs = AtomicCharges( [0.0, 0.52 * mod_electrons, \
                            0.52 * mod_electrons, \
                           -1.04 * mod_electrons] )

ljs = AtomicLJs( [ LJParameter( 3.15365 * angstrom, \
                                0.1550 * kcal_per_mol ), \
                   LJParameter.dummy(), \
                   LJParameter.dummy(), \
                   LJParameter.dummy() ] )

timer.start()
for mol in mols:

      mol.setProperty( "charges", chgs )
      mol.setProperty( "ljs", ljs )

      cljff.add(mol, [cljff.parameters().coulomb() == "charges", \
                      cljff.parameters().lj() == "ljs"])
                      
      cljff2.add(mol, [cljff2.parameters().coulomb() == "charges", \
                       cljff2.parameters().lj() == "ljs"])

ms = timer.elapsed()
print "... took %d ms" % ms

cljff = ForceField(cljff)
cljff2 = ForceField(cljff2)

#create a thread processor and calculate the energy in the background
threadproc = FFThreadProcessor(cljff)

active_threadproc = threadproc.activate()

print "Starting background calculation..."
active_threadproc.recalculateEnergy()

print "Off it goes...."
print "Da de da da da..."

#create an FFProcessor, and place the cljff onto it...
ffproc = FFProcessor(cljff)

print "Is active?", ffproc.isActive()

active_ffproc = ffproc.activate()

print "Is active?", ffproc.isActive()

print "MAIN THREAD PROCESS"
print "Total energy == ",active_ffproc.energy()
print "Coulomb == %f, LJ == %f" % ( active_ffproc.energy(cljff.components().coulomb()), \
                                    active_ffproc.energy(cljff.components().lj()) )

print "BACKGROUND THREAD PROCESS"
print "Total energy == ",active_threadproc.energy()
print "Coulomb == %f, LJ == %f" % ( active_threadproc.energy(cljff.components().coulomb()), \
                                    active_threadproc.energy(cljff.components().lj()) )

