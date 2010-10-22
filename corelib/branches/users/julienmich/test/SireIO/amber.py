
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
#############################################################
#top_file = "test/io/SYSTEM.top"
#crd_file = "test/io/SYSTEM.crd"

#top_file = "/home/julien/projects/cyps/sire/setup/leap/LI8/LIG_SOLV.top"
#crd_file = "/home/julien/projects/cyps/sire/setup/leap/LI8/LIG_SOLV.crd"

top_file = "/home/julien/projects/cyps/sire/setup/leap/LI8/LIG_GAS.top"
crd_file = "/home/julien/projects/cyps/sire/setup/leap/LI8/LIG_GAS.crd"

#top_file = "/home/julien/projects/cyps/sire/setup/leap/methanol/LIG_GAS.top"
#crd_file = "/home/julien/projects/cyps/sire/setup/leap/methanol/LIG_GAS.crd"

#top_file = "/home/julien/projects/cyps/sire/setup/leap/ethane/LIG_GAS.top"
#crd_file = "/home/julien/projects/cyps/sire/setup/leap/ethane/LIG_GAS.crd"


combining_rules = "arithmetic"
temperature = 25 * celsius
pressure = 1 * atm
coulomb_cutoff = 1000 * angstrom
coulomb_feather = 999.5 * angstrom
lj_cutoff = 1000 * angstrom
lj_feather = 999.5 * angstrom
#############################################################
print "Loading a top file..."
timer = QTime()
timer.start()

amber = Amber()
molecules, space = amber.readcrdtop(crd_file, top_file)

# Overload, we want to calc the energy in a non periodic box for comparison with Sander
space = Cartesian()

ms = timer.elapsed()
print "...took %d s" % (ms/1000.)

moleculeNumbers = molecules.molNums()
moleculeList = []

for moleculeNumber in moleculeNumbers:
    molecule = molecules.molecule(moleculeNumber).molecule()
    moleculeList.append(molecule)

system = System()

solute = MoleculeGroup("solute", moleculeList[0])
#protein = MoleculeGroup("protein", moleculeList[1])
#solvent = MoleculeGroup("solvent")
#for molecule in moleculeList[1:]:
#    solvent.add(molecule)

all = MoleculeGroup("all")
all.add(solute)
#all.add(solvent)
#all.add(protein)
# Add these groups to the System
system.add(solute)
#system.add(solvent)
#system.add(protein)
#system.add(residues)
system.add(all)

# Now create all of the forcefields
# - first solvent-solvent coulomb/LJ (CLJ) energy
#solventff = InterCLJFF("solvent:solvent")
#solventff.add(solvent)

# Now solute bond, angle, dihedral energy
solute_intraff = InternalFF("solute_intraff")
solute_intraff.add(solute)

# Now solute intramolecular CLJ energy
solute_intraclj = IntraCLJFF("solute_intraclj")
solute_intraclj.add(solute)

# The protein bond, angle, dihedral energy
#protein_intraff = InternalFF("protein_intraff")
#protein_intraff.add(protein)

# The protein intramolecular CLJ energy
#protein_intraclj = IntraCLJFF("protein_intraclj")
#protein_intraclj.add(protein)

# Now the solute-solvent CLJ energy
#solute_solventff = InterGroupCLJFF("solute:solvent")
#solute_solventff.add(solute, MGIdx(0))
#solute_solventff.add(solvent, MGIdx(1))

# Now the solute-protein CLJ energy
#solute_proteinff = InterGroupCLJFF("solute:protein")
#solute_proteinff.add(solute, MGIdx(0))
#solute_proteinff.add(protein, MGIdx(1))
  
# The protein-solvent energy 
#protein_solventff = InterGroupCLJFF("protein:solvent")
#protein_solventff.add(protein, MGIdx(0))
#protein_solventff.add(solvent, MGIdx(1))

# Here is the list of all forcefields
forcefields = [ solute_intraff, solute_intraclj]
# solventff, solute_solventff]
#                protein_intraff, protein_intraclj,
#                solute_proteinff, protein_solventff ] 
# Add these forcefields to the system
for forcefield in forcefields:
    system.add(forcefield)

system.setProperty( "space", space )
system.setProperty( "switchingFunction", 
                    HarmonicSwitchingFunction(coulomb_cutoff, coulomb_feather,
                                              lj_cutoff, lj_feather) )
system.setProperty( "combiningRules", VariantProperty(combining_rules) )

total_nrg = solute_intraclj.components().total() + solute_intraff.components().total() #+\
#    solventff.components().total() + solute_solventff.components().total()
#    protein_intraclj.components().total() + protein_intraff.components().total() + \
    #   solute_proteinff.components().total() + \
    #    protein_solventff.components().total() 

e_total = system.totalComponent()
system.setComponent( e_total, total_nrg )

# Add a space wrapper that wraps all molecules into the box centered at (0,0,0)
#system.add( SpaceWrapper(Vector(0,0,0), all) )

print "\nTotal energy "
print system.energy()

for component in system.energyComponents().keys():
    print component, system.energyComponents().value(component) * kcal_per_mol
# Write a PBD

# Note that tip3p water are likely to have bonds between hydrogen atoms.
PDB().write(all, "out.pdb")

# SYSTEM - solute + protein solvated, non periodic
# NSTEP =        0   TIME(PS) =       0.000  TEMP(K) =     0.00  PRESS =     0.0
# Etot   =    -47010.2216  EKtot   =         0.0000  EPtot      =    -47010.2216
# BOND   =       898.1982  ANGLE   =      5310.2620  DIHED      =      2922.5644
# 1-4 NB =       790.8755  1-4 EEL =      7702.0145  VDWAALS    =      7345.0484
# EELEC  =    -71979.1846  EHBOND  =         0.0000  RESTRAINT  =         0.0000
# EKCMT  =         0.0000  VIRIAL  =         0.0000  VOLUME     =   1856243.3813

# Found
# total = -47292.8 ( difference -282.57840000000579 )
# bond = 246.348 + 651.531  = 897.87899999999991 (difference 0.31920000000013715 )
# angle = 39.9317 + 5270.33 = 5310.2617 ( difference 0.00029999999969732016 )
# dihedral = 7.20538 + 1193.68 = 1200.8853800000002 ( difference ALOT )
# improper = 0.600387 + 275.317 = 275.91738700000002 
# Coulomb = -59944.6 + -29.9042 + -3028.07 + -1.5792 + -13.4313 + -9.63812 = -63027.222819999995
# Vdwaals = 6852 + 809.743 + 422.156 + -1.69799 + -14.1416 + -18.5434 = 8049.5160100000012

# SOLUTE solvated, non periodic
#
#Etot   =     -4270.5482  EKtot   =         0.0000  EPtot      =     -4270.5482
# BOND   =       246.3819  ANGLE   =        39.9317  DIHED      =        18.2875
#1-4 NB =        17.8213  1-4 EEL =      -100.7708  VDWAALS    =      1484.6639
# EELEC  =     -5976.8636  EHBOND  =        0.0000  RESTRAINT  =         0.0000
#KCMT  =         0.0000  VIRIAL  =         0.0000  VOLUME     =    704972.9702

# GOT 
# total -4232.13 (difference 38.4182 )
# bond 246.348  (difference 0.033899999999988495)
# angle 39.9317 (difference 0.0000)
# dihedral 7.20538
# improper 0.600387
# coulomb -5992.75 2.71662 
# lj 809.743 + -10.2106 + 694.194

# SOLUTE in gas
# NSTEP =        0   TIME(PS) =       0.000  TEMP(K) =     0.00  PRESS =     0.0
# Etot   =      1035.2593  EKtot   =         0.0000  EPtot      =      1035.2593
# BOND   =       246.3483  ANGLE   =        39.9317  DIHED      =        18.2875
# 1-4 NB =        17.8213  1-4 EEL =      -100.7804  VDWAALS    =       800.6806
# EELEC  =        12.9704  EHBOND  =         0.0000  RESTRAINT  =         0.0000
# EKCMT  =         0.0000  VIRIAL  =         0.0000  VOLUME     =    291200.3229
#                                                    Density    =         0.0012
# ------------------------------------------------------------------------------

# E_{total} 1073.92 kcal mol-1 (difference 38.66070)
# E_{solute_intraclj}^{coulomb} -29.9042 kcal mol-1
# E_{solute_intraclj}^{LJ} 809.743 kcal mol-1
# E_{solute_intraff}^{bond} 246.348 kcal mol-1          --> bond energies ok
# E_{solute_intraff}^{angle} 39.9317 kcal mol-1         --> angle energies ok
# E_{solute_intraff}^{dihedral} 7.20538 kcal mol-1
# E_{solute_intraff}^{improper} 0.600387 kcal mol-1



# Methanol gas phase
# NSTEP =        0   TIME(PS) =       0.000  TEMP(K) =     0.00  PRESS =     0.0
# Etot   =         4.5332  EKtot   =         0.0000  EPtot      =         4.5332
# BOND   =         0.1647  ANGLE   =         0.3909  DIHED      =         0.1363
# 1-4 NB =         0.0000  1-4 EEL =         3.8414  VDWAALS    =         0.0000
# EELEC  =         0.0000  EHBOND  =         0.0000  RESTRAINT  =         0.0000
# EKCMT  =         0.0000  VIRIAL  =         0.0000  VOLUME     =    239214.0118
#                                                    Density    =         0.0002
#Total energy 
# 4.53337 kcal mol-1
# E_{total} 4.53337 kcal mol-1
# E_{solute_intraclj}^{CLJ} 3.84152 kcal mol-1
# E_{solute_intraclj}^{coulomb} 3.84152 kcal mol-1 --> SLIGHT DEVIATION
# E_{solute_intraclj}^{LJ} 0 kcal mol-1
# E_{solute_intraff}^{internal} 0.691847 kcal mol-1
# E_{solute_intraff}^{bond} 0.164661 kcal mol-1 --> OK
# E_{solute_intraff}^{angle} 0.390913 kcal mol-1 --> OK
# E_{solute_intraff}^{dihedral} 0.136273 kcal mol-1 --> OK
# E_{solute_intraff}^{improper} 0 kcal mol-1


# Ethane
# NSTEP =        0   TIME(PS) =       0.000  TEMP(K) =     0.00  PRESS =     0.0
# Etot   =         1.8211  EKtot   =         0.0000  EPtot      =         1.8211
# BOND   =         0.0352  ANGLE   =         0.3727  DIHED      =         0.3701
# 1-4 NB =         0.1184  1-4 EEL =         0.9247  VDWAALS    =         0.0000
# EELEC  =         0.0000  EHBOND  =         0.0000  RESTRAINT  =         0.0000
# EKCMT  =         0.0000  VIRIAL  =         0.0000  VOLUME     =    248073.6160
#                                                    Density    =         0.0002
# ------------------------------------------------------------------------------

#Total energy 
# 1.16144 kcal mol-1
# E_{total} 1.16144 kcal mol-1
# E_{solute_intraclj}^{coulomb} 0.317755 kcal mol-1 --> WRONG
# E_{solute_intraclj}^{LJ} 0.0657261 kcal mol-1 --> WRONG
# E_{solute_intraff}^{bond} 0.0352199 kcal mol-1 ---> OK
# E_{solute_intraff}^{angle} 0.372679 kcal mol-1 --> OK
# E_{solute_intraff}^{dihedral} 0.370065 kcal mol-1 --> OK
# E_{solute_intraff}^{improper} 0 kcal mol-1
