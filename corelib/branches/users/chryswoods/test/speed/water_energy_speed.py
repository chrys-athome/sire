
from Sire.Base import *
from Sire.MM import *
from Sire.IO import *
from Sire.FF import *
from Sire.Units import *
from Sire.Maths import *
from Sire.Mol import *
from Sire.Vol import *
from Sire.Qt import *

t = QTime()

cljff = InterCLJFF()

mincoords = Vector(-18.3854, -18.66855, -18.4445)
maxcoords = Vector( 18.3854,  18.66855,  18.4445)

vol = PeriodicBox(mincoords, maxcoords)
switchfunc = HarmonicSwitchingFunction(15, 14.5)

cljff.setSpace(vol)
cljff.setSwitchingFunction(switchfunc)

mols = PDB().read("test/io/water.pdb")

print "Read in %d molecules!" % mols.nMolecules()

i = 0

t.start()
mol = mols.moleculeAt(0).molecule()

mol = mol.edit().atom( AtomName("O00") ) \
                    .setProperty("LJ", LJParameter(3.15363,  \
                                                   0.1550)).molecule() \
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

    mol = mol.edit().rename( MolName("T4P") ) \
                    .setProperty("charge", charges) \
                    .setProperty("LJ", ljs) \
             .commit()
   
    cljff.add(mol)    

ms = t.elapsed()
print "Parameterised all of the water molecules (in %d ms)!" % ms

mols = cljff.molecules()

#for molnum in mols.molNums():
#    mol = mols[molnum].molecule()
#    charges = mol.property("charge").array()
#    ljs = mol.property("LJ").array()
#
#    print charges, ljs

t.start()
cljff.packCoordinates()
ms = t.elapsed()
print "Packing the coordinates took %d ms" % ms

#get the benchmark times
benchmark = 0.000001 * FlopsMark.benchmark()
benchmark_sum = 0.000001 * FlopsMark.benchmarkSum()
benchmark_quot = 0.000001 * FlopsMark.benchmarkQuotient()
benchmark_prod = 0.000001 * FlopsMark.benchmarkProduct()

print "\nThis machine can run at; %.1f MFLOPS for sum, %.1f MFLOPS for sum+product," % \
                (benchmark_sum, benchmark_prod)
print "%.1f MFLOPS for sum+quotient and %.1f MFLOPS for sum+product+sqrt.\n" % \
                (benchmark_quot, benchmark)

for i in range(0,5):
    t.start()
    cljff.mustNowRecalculateFromScratch()
    before_energy = FlopsMark()
    nrg = cljff.energy()
    after_energy = FlopsMark()
    ms = t.elapsed()
    print nrg
    print nrg.value()

    mflops = 0.000001 * (after_energy - before_energy)

    print "Took %d ms. " % ms,
    print "Speed is at least %.1f MFLOPS" % mflops

    print "(This is %.2f %% of the benchmark  (%.2f %%, %.2f %%, %.2f %%))" % \
             ( 100 * (mflops / benchmark), 100 * (mflops / benchmark_quot), \
               100 * (mflops / benchmark_sum), 100 * (mflops / benchmark_prod) )

print "Done!"

#now how long does it take to calculate a change in energy?
mols = cljff.molecules()
molnums = mols.molNums()

mol0 = mols[molnums[0]]

print mol0.evaluate().center()

newmol = mol0.move().translate( Vector(1,0,0) ).commit()

print mol0.evaluate().center()

cljff.update( newmol )

print "%s  (%f)" % (cljff.energy(), cljff.energy().value())

cljff.update( mol0 )

print "%s  (%f)" % (cljff.energy(), cljff.energy().value())

