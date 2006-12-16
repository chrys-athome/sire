
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

timer.start()
for mol in mols:
      chgs = ChargeTable( mol.info() )
      ljs = LJTable( mol.info() )
      
      resnum = mol.residueNumbers()[0]
      
      chgs.setParameter( AtomIndex("O00",resnum), 0.0 * mod_electrons )
      chgs.setParameter( AtomIndex("H01",resnum), 0.52 * mod_electrons )
      chgs.setParameter( AtomIndex("H02",resnum), 0.52 * mod_electrons )
      chgs.setParameter( AtomIndex("M03",resnum), -1.04 * mod_electrons )
      
      ljs.setParameter( AtomIndex("O00",resnum), \
                        LJParameter( 3.15365 * angstrom, \
                                     0.1550 * kcal_per_mol ) )

      ljs.setParameter( AtomIndex("H01",resnum), LJParameter.dummy() )
      ljs.setParameter( AtomIndex("H02",resnum), LJParameter.dummy() )
      ljs.setParameter( AtomIndex("M03",resnum), LJParameter.dummy() )
      
      cljff.add(mol, chgs, ljs)
      cljff2.add(mol, chgs, ljs)

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
print "Coulomb == %f, LJ == %f" % ( active_ffproc.energy(cljff.component(CLJFF.COULOMB())), \
                                    active_ffproc.energy(cljff.component(CLJFF.LJ())) )

print "BACKGROUND THREAD PROCESS"
print "Total energy == ",active_threadproc.energy()
print "Coulomb == %f, LJ == %f" % ( active_threadproc.energy(cljff.component(CLJFF.COULOMB())), \
                                    active_threadproc.energy(cljff.component(CLJFF.LJ())) )

