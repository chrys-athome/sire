
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
cljff = InterCLJFF( space, switchfunc )
ljff = InterLJFF( space, switchfunc )
coulff = InterCoulombFF( space, switchfunc )

#parametise each molecule and add it to the forcefield
print "Parametising the molecules..."

chgs = AtomicCharges( [0.0, 0.52 * mod_electron, \
                            0.52 * mod_electron, \
                           -1.04 * mod_electron] )

ljs = AtomicLJs( [ LJParameter( 3.15365 * angstrom, \
                                0.1550 * kcal_per_mol ), \
                   LJParameter.dummy(), \
                   LJParameter.dummy(), \
                   LJParameter.dummy() ] )

timer.start()
for mol in mols:

      mol.setProperty( "charges", chgs )
      mol.setProperty( "ljs", ljs )

      cljff.add(mol, {cljff.parameters().coulomb() : "charges",
                      cljff.parameters().lj() : "ljs"})
                      
      coulff.add(mol, {coulff.parameters().coulomb() : "charges"})
      ljff.add(mol, {ljff.parameters().lj() : "ljs"})

ms = timer.elapsed()
print "... took %d ms" % ms

#create a thread processor and calculate the energy in the background
threadproc1 = FFThreadProcessor(cljff)
threadproc2 = FFThreadProcessor(ljff)
threadproc3 = FFThreadProcessor(coulff)

active_threadproc1 = threadproc1.activate()
active_threadproc2 = threadproc2.activate()
active_threadproc3 = threadproc3.activate()

print "Starting background calculation..."
active_threadproc1.recalculateEnergy()
active_threadproc2.recalculateEnergy()
active_threadproc3.recalculateEnergy()

print "Off it goes...."
print "Da de da da da..."

#create an FFProcessor, and place the cljff onto it...
ffproc1 = FFProcessor(cljff)
ffproc2 = FFProcessor(ljff)
ffproc3 = FFProcessor(coulff)

print "Is active?", ffproc1.isActive()
print "Is active?", ffproc2.isActive()
print "Is active?", ffproc3.isActive()

active_ffproc1 = ffproc1.activate()
active_ffproc2 = ffproc2.activate()
active_ffproc3 = ffproc3.activate()

print "Is active?", ffproc1.isActive()
print "Is active?", ffproc2.isActive()
print "Is active?", ffproc3.isActive()

print "MAIN THREAD PROCESS"
print "Total energy == ",active_ffproc1.energy()
print "Coulomb == %f, LJ == %f" % ( active_ffproc1.energy(cljff.components().coulomb()), \
                                    active_ffproc1.energy(cljff.components().lj()) )

print "Total energy == ",active_ffproc2.energy()
print "Total energy == ",active_ffproc3.energy()

print "BACKGROUND THREAD PROCESS"
print "Total energy == ",active_threadproc1.energy()
print "Coulomb == %f, LJ == %f" % ( active_threadproc1.energy(cljff.components().coulomb()), \
                                    active_threadproc1.energy(cljff.components().lj()) )

print "Total energy == ",active_threadproc2.energy()
print "Total energy == ",active_threadproc3.energy()
