
from Sire.IO import *
from Sire.Mol import *
from Sire.CAS import *
from Sire.System import *
from Sire.Move import *
from Sire.MM import *
from Sire.FF import *
from Sire.Units import *
from Sire.Vol import *
from Sire.Maths import *
from Sire.Base import *
from Sire.Qt import *

import os,re,sys
import shutil

from IPython.Debugger import Tracer; debug_here = Tracer()

top_file = "test/io/SYSTEM.top"
crd_file = "test/io/SYSTEM.crd"

combining_rules = "arithmetic"
temperature = 25 * celsius
pressure = 1 * atm
coulomb_cutoff = 10 * angstrom
coulomb_feather = 9.5 * angstrom
lj_cutoff = 10 * angstrom
lj_feather = 9.5 * angstrom
# Here are the soft-core parameters
coulomb_power = 0      # coulomb potential softcore power
shift_delta = 2.0      # delta value for softcore LJ shifting function

print "Loading a top file..."
timer = QTime()
timer.start()

amber = Amber()
molecules, space = amber.readcrdtop(crd_file, top_file)

ms = timer.elapsed()
print "...took %d s" % (ms/1000.)

moleculeNumbers = molecules.molNums()
moleculeList = []

for moleculeNumber in moleculeNumbers:
    molecule = molecules.molecule(moleculeNumber).molecule()
    moleculeList.append(molecule)

system = System()

solute = MoleculeGroup("solute", moleculeList[0])
protein = MoleculeGroup("protein", moleculeList[1])
solvent = MoleculeGroup("solvent")
for molecule in moleculeList[2:]:
    solvent.add(molecule)

all = MoleculeGroup("all")
all.add(solute)
all.add(solvent)
all.add(protein)
# Add these groups to the System
system.add(solute)
system.add(solvent)
system.add(protein)
#system.add(residues)
system.add(all)

# Now create all of the forcefields
# - first solvent-solvent coulomb/LJ (CLJ) energy
solventff = InterCLJFF("solvent:solvent")
solventff.add(solvent)

# Now solute bond, angle, dihedral energy
solute_intraff = InternalFF("solute_intraff")
solute_intraff.add(solute)

# Now solute intramolecular CLJ energy
solute_intraclj = IntraCLJFF("solute_intraclj")
solute_intraclj.add(solute)

# The protein bond, angle, dihedral energy
protein_intraff = InternalFF("protein_intraff")
protein_intraff.add(protein)

# The protein intramolecular CLJ energy
protein_intraclj = IntraCLJFF("protein_intraclj")
protein_intraclj.add(protein)

# Now the solute-solvent (soft-core) CLJ energy
solute_solventff = InterGroupSoftCLJFF("solute:solvent")
solute_solventff.add(solute, MGIdx(0))
solute_solventff.add(solvent, MGIdx(1))

# Now the solute-protein (soft-core) CLJ energy
solute_proteinff = InterGroupSoftCLJFF("solute:protein")
solute_proteinff.add(solute, MGIdx(0))
solute_proteinff.add(protein, MGIdx(1))
  
# The protein-solvent energy 
protein_solventff = InterGroupCLJFF("protein:solvent")
protein_solventff.add(protein, MGIdx(0))
protein_solventff.add(solvent, MGIdx(1))

# Here is the list of all forcefields
forcefields = [ solventff, 
                solute_intraff, solute_intraclj, 
                protein_intraff, protein_intraclj,
                solute_solventff, solute_proteinff, protein_solventff ] 
# Add these forcefields to the system
for forcefield in forcefields:
    system.add(forcefield)

#space = PeriodicBox( Vector( float(words[2]), float(words[3]), float(words[4]) ),
#                     Vector( float(words[5]), float(words[6]), float(words[7]) ) )
#system.setProperty( "space", space )
system.setProperty( "switchingFunction", 
                    HarmonicSwitchingFunction(coulomb_cutoff, coulomb_feather,
                                              lj_cutoff, lj_feather) )
system.setProperty( "combiningRules", VariantProperty(combining_rules) )
system.setProperty( "coulombPower", VariantProperty(coulomb_power) )
system.setProperty( "shiftDelta", VariantProperty(shift_delta) )

total_nrg = solventff.components().total() + \
    solute_intraclj.components().total() + solute_intraff.components().total() + \
    protein_intraclj.components().total() + protein_intraff.components().total() + \
    solute_solventff.components().total() + \
    solute_proteinff.components().total() + \
    protein_solventff.components().total() 

e_total = system.totalComponent()
system.setComponent( e_total, total_nrg )

# Add a space wrapper that wraps all molecules into the box centered at (0,0,0)
#system.add( SpaceWrapper(Vector(0,0,0), all) )
print "\nTotal energy "
print system.energy()
