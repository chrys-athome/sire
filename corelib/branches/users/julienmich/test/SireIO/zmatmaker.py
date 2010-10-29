
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

#zmat_maker = ProtoMSZMatrixMaker()
#zmat_maker.readtemplates("amber.pmszmat")
#residues = zmat_Maker.addZmatrix( residues )// adds property['z-matrix'] to each residue in residues
#  and
#  flexibility_maker = FlexibilityMaker()
#flexibility_maker.readFlexibilityTemplates("solute.mover")
#molecule = flexibility_maker.addFlexibility( molecule ) // adds property['flexibility'] to molecule
#4:14 PM Where property['flexibility'] contains the following information
#  ==>
#maximum_translation'
#maximum_rotation
#var_bonds
#var_angles
#var_dihedrals
#bond_deltas
#angle_deltas

################################
combining_rules = "arithmetic"
temperature = 25 * celsius
pressure = 1 * atm
coulomb_cutoff = 10 * angstrom
coulomb_feather = 9.5 * angstrom
lj_cutoff = 10 * angstrom
lj_feather = 9.5 * angstrom
pref_constant = 200 * angstrom2
# Could be nice to make this template dependent (but can live without)
max_solvent_translation = 0.15 * angstrom
max_solvent_rotation = 15 * degrees

solvent_mc_weight = 3899
protein_mc_weight = 1000
solute_mc_weight = 100
volume_mc_weight = 1
random_seed = 42
###################################

#top_file = "test/io/SYSTEM.top"
#crd_file = "test/io/SYSTEM.crd"

top_file = "/home/julien/projects/cyps/sire/setup/leap/LI8/COMPLEX_GAS.top"
crd_file = "/home/julien/projects/cyps/sire/setup/leap/LI8/COMPLEX_GAS.crd"

amber = Amber()
molecules, space = amber.readcrdtop(crd_file, top_file)

moleculeNumbers = molecules.molNums()
moleculeList = []

for moleculeNumber in moleculeNumbers:
    molecule = molecules.molecule(moleculeNumber).molecule()
    moleculeList.append(molecule)

solute = moleculeList[0]
solute = solute.edit().rename("LIG").commit()
# This will add the property "flexibility" to the solute
flexibility_maker = FlexibilityMaker()
flexibility_maker.loadTemplates("test/io/ligand.flex")
# solute = flexibility_maker.applyTemplates( solute )

sys.exit(-1)

solute = MoleculeGroup("solute", solute)

protein = moleculeList[1]
# This will add the property "z-matrix" to the protein
zmat_maker = ZmatrixMaker()
zmat_maker.loadTemplates("test/io/amber.zmatrices")
protein = zmat_maker.applyTemplates( protein )

residues = MoleculeGroup("residues")
for i in range(0, protein.nResidues()):
    res = protein.residue( ResIdx(i) )
    residues.add(res)

protein = MoleculeGroup("protein", protein)

solvent = MoleculeGroup("solvent")
for molecule in moleculeList[2:]:
    solvent.add(molecule)

all = MoleculeGroup("all")
all.add(solute)
all.add(protein)
all.add(solvent)

# Add these groups to the System
system = System()

system.add(solute)
system.add(protein)
system.add(solvent)
system.add(residues)
system.add(all)

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

# Now the solute-solvent CLJ energy
solute_solventff = InterGroupCLJFF("solute:solvent")
solute_solventff.add(solute, MGIdx(0))
solute_solventff.add(solvent, MGIdx(1))

# Now the solute-protein CLJ energy
solute_proteinff = InterGroupCLJFF("solute:protein")
solute_proteinff.add(solute, MGIdx(0))
solute_proteinff.add(protein, MGIdx(1))
  
# The protein-solvent energy 
protein_solventff = InterGroupCLJFF("protein:solvent")
protein_solventff.add(protein, MGIdx(0))
protein_solventff.add(solvent, MGIdx(1))

# Here is the list of all forcefields
forcefields = [ solute_intraff, solute_intraclj,
                solventff, solute_solventff,
                protein_intraff, protein_intraclj,
                solute_proteinff, protein_solventff ] 
# Add these forcefields to the system
for forcefield in forcefields:
    system.add(forcefield)

system.setProperty( "space", space )
system.setProperty( "switchingFunction", 
                    HarmonicSwitchingFunction(coulomb_cutoff, coulomb_feather,
                                              lj_cutoff, lj_feather) )
system.setProperty( "combiningRules", VariantProperty(combining_rules) )

total_nrg = solute_intraclj.components().total() + solute_intraff.components().total() +\
    solventff.components().total() + solute_solventff.components().total() +\
    protein_intraclj.components().total() + protein_intraff.components().total() + \
    solute_proteinff.components().total() + protein_solventff.components().total() 

e_total = system.totalComponent()
system.setComponent( e_total, total_nrg )
system.add( "trajectory", TrajectoryMonitor(system[MGName('all')]), 100 )

# Add a space wrapper that wraps all molecules into the box centered at (0,0,0)
system.add( SpaceWrapper(Vector(0,0,0), all) )

# Setup Moves
max_volume_change = 0.1 * solvent.nMolecules() * angstrom3

# This should automatically set MaxTransl and MaxRot based on the contents of property("flexibility")
# solute_moves = RigidBodyMC( solute ) 
# This should automatically set setBonds, setBondDeltas, setAngles, setDihedrals, setAngleDeltas based on the 
# contents of the property("flexibility")
# solute_intra_moves = MoverMove( solute )

solvent_moves = RigidBodyMC( PrefSampler(solute[MolIdx(0)], 
                                         solvent, pref_constant) )

solvent_moves.setMaximumTranslation(max_solvent_translation)
solvent_moves.setMaximumRotation(max_solvent_rotation)

protein_intra_moves = ZMatMove( PrefSampler(solute[MolIdx(0)], residues, pref_constant) ) 

volume_moves = VolumeMove()
volume_moves.setMaximumVolumeChange(max_volume_change)

moves = WeightedMoves()
#moves.add( solute_moves, solute_mc_weight / 2 )
#moves.add( solute_intra_moves, solute_mc_weight / 2)
moves.add( protein_intra_moves, protein_mc_weight )
moves.add( solvent_moves, solvent_mc_weight )
moves.add( volume_moves, volume_mc_weight )

moves.setTemperature(temperature)
moves.setPressure(pressure)
moves.setGenerator( RanGenerator(random_seed+3) )
# Run a short simulation
system = moves.move(system, 1000, True)

system.monitor( MonitorName("trajectory") ).writeToDisk("outputXXXXXX.pdb")
