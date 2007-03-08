
from Sire.Mol import *
from Sire.IO import *
from Sire.Vol import *
from Sire.MM import *
from Sire.CAS import *
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
switchfunc = HarmonicSwitchingFunction(10.0, 9.5)

#create a forcefield for the molecules
molpro = MolproFF(space, switchfunc)

molpro.setMolproExe("../../../../../software/molpro/devel/molpro")

#parametise each molecule and add it to the forcefield
print "Parametising the molecules..."

chgs = AtomicCharges( [0.0, 0.52 * mod_electrons,
                            0.52 * mod_electrons,
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

qm_mol = mols[0]
mm_mols = mols[1:]

molpro.addToMM(mm_mols)
molpro.addToQM(qm_mol)

ms = timer.elapsed()
print "... took %d ms" % ms
      
#now calculate the energy of the forcefield
print "Calculating the energy..."

timer.start()
nrg = molpro.energy()
ms = timer.elapsed()

print "Energy = %f kcal mol-1, took %d ms" % (nrg, ms)

f = open( os.path.join(os.getenv("HOME"),"test.cmd"), "w" )
f.write( str(molpro.molproCommandInput()) )
f.close()

# set the origin of the QM energy
molpro.setEnergyOrigin(nrg)

nrg = molpro.energy()
print "Scaled energy = %f kcal mol-1" % nrg

#lets translate the QM molecule by 0.1 A
qm_mol.translate( (-5.0,0.0,0.0) )

molpro.change(qm_mol)

timer.start()
nrg = molpro.energy()
ms = timer.elapsed()

print "Energy = %f kcal mol-1, took %d ms" % (nrg, ms)

#now lets translate the QM molecule back again
qm_mol.translate( (5.0,0.0,0.0) )

molpro.change(qm_mol)

timer.start()
nrg = molpro.energy()
ms = timer.elapsed()

print "Energy = %f kcal mol-1, took %d ms" % (nrg, ms)
