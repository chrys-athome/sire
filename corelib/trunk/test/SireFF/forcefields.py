
from Sire.FF import *
from Sire.MM import *
from Sire.CAS import *
from Sire.Qt import *
from Sire.IO import *
from Sire.Vol import *
from Sire.Maths import *
from Sire.Mol import *
from Sire.Units import *

t = QTime()

forcefields = ForceFields()

cljff = InterCLJFF()

mincoords = Vector(-18.3854, -18.66855, -18.4445)
maxcoords = Vector( 18.3854,  18.66855,  18.4445)

vol = PeriodicBox(mincoords, maxcoords)
switchfunc = HarmonicSwitchingFunction(15*angstrom, 14.5*angstrom)

cljff.setSpace(vol)
cljff.setSwitchingFunction(switchfunc)

mols = PDB().read("test/io/water.pdb")
                                                
print "Read in %d molecules!" % mols.nMolecules()

i = 0

t.start()
mol = mols.moleculeAt(0).molecule()

mol = mol.edit().atom( AtomName("O00") ) \
                    .setProperty("LJ", LJParameter(3.15363*angstrom,  \
                                                   0.1550*kcal_per_mol)).molecule() \
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

    mol = mol.edit().rename("T4P") \
                    .setProperty("charge", charges) \
                    .setProperty("LJ", ljs) \
             .commit()

    cljff.add(mol)

ms = t.elapsed()
print "Parameterised all of the water molecules (in %d ms)!" % ms

forcefields.add(cljff)

print forcefields.properties()

x = Symbol("x")

forcefields.setComponent(x, 32)
print forcefields.constant(x)

forcefields.setComponent(x, cljff.components().coulomb() + cljff.components().lj())

lam = Symbol("lambda")

forcefields.setComponent(lam, 0.0)

forcefields.setComponent(x, (1-lam)*cljff.components().coulomb() + lam*cljff.components().lj())

print forcefields.energyExpression(x)

print forcefields.energy()
print forcefields.energy( cljff.components().coulomb() )
print forcefields.energy( cljff.components().lj() )
print forcefields.energy( cljff.components().total() )
print forcefields.energy( x )

forcefields.setComponent(lam, 1.0)
print forcefields.energy(x)

forcefields.setComponent(lam, 0.5)
print forcefields.energy(x)

