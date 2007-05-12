
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

import copy

timer = QTime()

#read in all of the molecules
print "Loading the molecules..."
timer.start()
mols = PDB().read("test/io/water.pdb")

ms = timer.elapsed()
print "... took %d ms" % ms

#specify the space in which the molecules are placed
space = Cartesian()

space = PeriodicBox( (-18.3854,-18.66855,-18.4445),
                     ( 18.3854, 18.66855, 18.4445) )

#specify the type of switching function to use
switchfunc = HarmonicSwitchingFunction(80.0)
switchfunc = HarmonicSwitchingFunction(15.0, 14.5)

cljff_a = InterCLJFF(space, switchfunc)
cljff_b = InterCLJFF(space, switchfunc)

cljff_a_b = InterGroupCLJFF(space, switchfunc)

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

rand = RanGenerator()

n_in_a = 0
n_in_b = 0

for mol in mols:
      
      mol.setProperty( "charges", chgs )
      mol.setProperty( "ljs", ljs )

      #randomly divide the molecules into the two groups
      if (rand.rand() < 0.5):
          cljff_a.add(mol, {cljff_a.parameters().coulomb() : "charges",
                            cljff_a.parameters().lj() : "ljs"})
                            
          cljff_a_b.addTo(cljff_a_b.groups().A(),
                          mol, {cljff_a_b.parameters().coulomb() : "charges",
                                cljff_a_b.parameters().lj() : "ljs"})
                         
          n_in_a = n_in_a + 1
      else:
          cljff_b.add(mol, {cljff_b.parameters().coulomb() : "charges",
                            cljff_b.parameters().lj() : "ljs"})
                            
          cljff_a_b.addTo(cljff_a_b.groups().B(),
                          mol, {cljff_a_b.parameters().coulomb() : "charges",
                                cljff_a_b.parameters().lj() : "ljs"})
      
          n_in_b = n_in_b + 1

ms = timer.elapsed()
print "... took %d ms" % ms
      
print "(%d molecules in group A, %d in group B)" % (n_in_a, n_in_b)

#add the forcefields to a forcefields object
ffields = ForceFields()
ffields.add(cljff_a)
ffields.add(cljff_b)
ffields.add(cljff_a_b)
      
print ffields.nForceFields()
      
#now create the molecule groups
group = MoleculeGroup("solvent", mols)

system = System(group, ffields)

mc = RigidBodyMC( UniformSampler(group) )

PDB().write(system.info().groups().molecules(), "test00.pdb")

timer.start()
moves = system.run(mc, 1000)
ms = timer.elapsed()

PDB().write(system.info().groups().molecules(), "test01.pdb")

print "1000 moves took %d ms" % ms

print moves.count(), moves.percentProgress()

