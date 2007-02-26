
from Sire.Mol import *
from Sire.IO import *
from Sire.Vol import *
from Sire.MM import *
from Sire.CAS import *
from Sire.Maths import *
from Sire.Qt import *
from Sire.Units import *
from Sire.Squire import *

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
molpro = MolproFF()

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

nrg = molpro.energy()

