
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

#t.start()
#cljff.packCoordinates()
#ms = t.elapsed()
#
#print "Packing the coordinates took %d ms" % ms

for i in range(0,5):
    t.start()
    cljff.mustNowRecalculateFromScratch()
    nrg = cljff.energy()
    ms = t.elapsed()
    print nrg
    print nrg.value()
    print "Took %d ms" % ms

print "Done!"
