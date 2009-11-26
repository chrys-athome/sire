
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
from Sire.Stream import *

import sys

t = QTime()

cljff = InterCLJFF("cljff")

cljff_a = InterCLJFF("cljff_a")
cljff_b = InterCLJFF("cljff_b")
cljff_a_b = InterGroupCLJFF("cljff_a_b")

ffs = [ cljff, cljff_a, cljff_b, cljff_a_b ]

mincoords = Vector(-18.3854, -18.66855, -18.4445)
maxcoords = Vector( 18.3854,  18.66855,  18.4445)

vol = PeriodicBox(mincoords, maxcoords)
switchfunc = CHARMMSwitchingFunction(15*angstrom, 14.5*angstrom)

for ff in ffs:
    ff.setSpace(vol)
    ff.setSwitchingFunction(switchfunc)

solvent = MoleculeGroup("solvent")

mols = PDB().read("test/io/water.pdb")
                                                
print "Read in %d molecules!" % mols.nMolecules()

i = 0

t.start()
mol = mols.moleculeAt(0).molecule()

mol = mol.edit().atom( AtomName("O00") ) \
                    .setProperty("LJ", LJParameter(3.15363*angstrom,  \
                                                   0.15500*kcal_per_mol))\
                    .setProperty("mass", Element("O").mass() ).molecule() \
                .atom( AtomName("H01") ) \
                    .setProperty("charge", 0.520 * mod_electron) \
                    .setProperty("mass", Element("H").mass() ).molecule() \
                .atom( AtomName("H02") ) \
                    .setProperty("charge", 0.520 * mod_electron) \
                    .setProperty("mass", Element("H").mass() ).molecule() \
                .atom( AtomName("M03") ) \
                    .setProperty("charge", -1.04 * mod_electron).molecule() \
         .commit()

charges = mol.property("charge")
ljs = mol.property("LJ")
masses = mol.property("mass")

cljff.add(mol)
cljff_a.add(mol)
cljff_a_b.add(mol, MGIdx(0))

solvent.add(mol)

for i in range(1, mols.nMolecules()):
    mol = mols.moleculeAt(i).molecule()

    mol = mol.edit().rename("T4P") \
                    .setProperty("charge", charges) \
                    .setProperty("LJ", ljs) \
                    .setProperty("mass", masses) \
             .commit()

    solvent.add(mol)

    cljff.add(mol)

    if (i % 2 == 0):
        cljff_a.add(mol)
        cljff_a_b.add( mol, MGIdx(0) )
    else:
        cljff_b.add(mol)
        cljff_a_b.add( mol, MGIdx(1) )

ms = t.elapsed()
print "Parameterised all of the water molecules (in %d ms)!" % ms

print "Getting the force tables..."

t.start()
forcetable = ForceTable( solvent )

print "took %d ms" % t.elapsed()


print "Calculating the forces..."
t.start()

cljff.force(forcetable)

print "took %d ms" % t.elapsed()

system = System()
system.add(solvent)
system.add(cljff)

t.start()                                       
print "Initial energy = %s" % system.energy()
print "(took %d ms)" % t.elapsed()
print system.property("space")
print system.property("switchingFunction")

mdmove = MolecularDynamics( solvent, VelocityVerlet(1*femtosecond) )

for i in range(0,10):
    print "\nmove %d" % (i+1)
    mdmove.move(system, 1)

    print system.energy()
    print mdmove.kineticEnergy()
    print system.energy() + mdmove.kineticEnergy()

