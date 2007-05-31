
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
switchfunc = HarmonicSwitchingFunction(15.0, 14.5)

#create a forcefield for the molecules
molpro = MolproFF(space, switchfunc)

#create a coulomb forcefield to estimate the change
#in coulomb electrostatic energy
coulff = InterGroupCoulombFF(space, switchfunc)

molpro.setMolproExe( findExe("molpro") )

#parametise each molecule and add it to the forcefield
print "Parametising the molecules..."

chgs = AtomicCharges( [0.0, 0.52 * mod_electron,
                            0.52 * mod_electron,
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

qm_mol = mols[0]
mm_mols = mols[1:]

print "Adding the QM molecule..."
molpro.addToQM(qm_mol)
print "...done"    
    
coulff.addTo( coulff.groups().A(), qm_mol )

print "Adding the MM molecules..."
coulff.addTo( coulff.groups().B(), mm_mols )

ms = timer.elapsed()
print "... took %d ms" % ms
      
#now calculate the energy of the forcefield
print "Calculating the energy..."

molpro.setProgram("HF\nMP2")
molpro.setBasisSet("AVDZ")

timer.start()
nrg = molpro.energy()
ms = timer.elapsed()

print "Energy = %f kcal mol-1, took %d ms" % (nrg, ms)

# set the origin of the QM energy
molpro.setEnergyOrigin(nrg)

# add the MM molecules
print "Adding the MM atoms to the QM forcefield..."
molpro.addToMM(mm_mols)

#write out the molpro command file to a file
open("test.cmd", "w").write( str(molpro.molproCommandInput()) )

print "Energy = %f kcal mol-1" % molpro.energy()

print "Coul Energy = %f kcal mol-1" % coulff.energy()

f = open( os.path.join(os.getenv("HOME"),"test.cmd"), "w" )
f.write( str(molpro.molproCommandInput()) )
f.close()

nrg = molpro.energy()
print "Scaled energy = %f kcal mol-1" % nrg

#lets translate the QM molecule by 0.1 A
qm_mol = Molecule(qm_mol.move().translate( (-1.0,0.0,0.0) ))

molpro.change(qm_mol)
coulff.change(qm_mol)

timer.start()
nrg = molpro.energy()
ms = timer.elapsed()

print "Energy = %f kcal mol-1, took %d ms" % (nrg, ms)

print "Coulomb energy = %f kcal mol-1" % coulff.energy()

#now lets translate the QM molecule back again
qm_mol = Molecule(qm_mol.move().translate( (1.0,0.0,0.0) ))

molpro.change(qm_mol)
coulff.change(qm_mol)

timer.start()
nrg = molpro.energy()
ms = timer.elapsed()

print "Energy = %f kcal mol-1, took %d ms" % (nrg, ms)
print "Coulomb energy = %f kcal mol-1" % coulff.energy()

# test the change in energy for MC size moves (0.2 A)
old_qm = molpro.energy()
old_mm = coulff.energy()

qm_mol = Molecule(qm_mol.move().translate( (0.2, 0.0, 0.0) ))

molpro.change(qm_mol)
coulff.change(qm_mol)

new_qm = molpro.energy()
new_mm = coulff.energy()

print old_qm, new_qm, new_qm - old_qm
print old_mm, new_mm, new_mm - old_mm
print (new_qm - new_mm) - (old_qm - old_mm)
