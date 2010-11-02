
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
from Sire.Config import *
import os,re,sys
import shutil

####### INPUT FILES ############

# Amber topology and coordinate files describing the solvated protein-ligand complex. These 
# are usually prepared using AmberTools. The present script assumes that the ligand is 
# the first molecule, the protein the second molecule, and the rest are solvent molecules. 
# If this is not the case (e.g, multiple proteins, ligands ), you must modify the subroutine 
# createSystem
top_file = "test/io/SYSTEM.top"
crd_file = "test/io/SYSTEM.crd"
# Flexibility files describing how the ligand is to be moved. Usually generated using 
# the utility sireflex.py
ligand_flex_file = "test/io/ligand.flex"
# Optional, the name of the ligand used in the flexibility file. If the ligand has a single 
# residue, the program will use the residue name by default to look up the flexibility template
# lig_name = "LIG"

######  SIMULATION PARAMETERS ##
protein_zmatrices = os.path.join(parameter_directory,"amber.zmatrices")
combining_rules = "arithmetic"
temperature = 25 * celsius
pressure = 1 * atm
coulomb_cutoff = 10 * angstrom
coulomb_feather = 9.5 * angstrom
lj_cutoff = 10 * angstrom
lj_feather = 9.5 * angstrom
pref_constant = 200 * angstrom2
# Could be nice to make this template dependent 
max_solvent_translation = 0.15 * angstrom
max_solvent_rotation = 15 * degrees

solvent_mc_weight = 3899
protein_mc_weight = 1000
solute_mc_weight = 100
volume_mc_weight = 1
random_seed = 42

####### FUNCTIONS  ###############

def createSystem(molecules):
    print "Applying flexibility and zmatrix templates..."

    moleculeNumbers = molecules.molNums()
    moleculeList = []

    for moleculeNumber in moleculeNumbers:
        molecule = molecules.molecule(moleculeNumber).molecule()
        moleculeList.append(molecule)

    solute = moleculeList[0]
    # If lig_name has not been defined, and there is a single residue,
    # use the residue name
    try:
        lig_name
    except NameError:
        if ( solute.nResidues() == 1 ):
            lig_name = solute.residue( ResIdx(0) ).name().value()
        else:
            lig_name = "ligand" # Likely not good...
    solute = solute.edit().rename(lig_name).commit()
    # This will add the property "flexibility" to the solute
    flexibility_maker = FlexibilityMaker()
    flexibility_maker.loadTemplates( ligand_flex_file )
    solute = flexibility_maker.applyTemplates( solute )

    # editmol = solute.edit()
    # weightproperty = PropertyName("weight function")
    # editmol.setProperty(weightproperty, AbsFromNumber() )
    # solute = editmol.commit()

    solute = MoleculeGroup("solute", solute)

    protein = moleculeList[1]
    # This will add the property "z-matrix" to the protein
    zmat_maker = ZmatrixMaker()
    zmat_maker.loadTemplates( protein_zmatrices )
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

    return system

def setupForcefields(system, space):

    print "Creating force fields... "

    solute = system[ MGName("solute") ]
    protein = system[ MGName("protein") ]
    solvent = system[ MGName("solvent") ]
    residues = system[ MGName("residues") ]
    all = system[ MGName("all") ]

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
    system.add( "trajectory", TrajectoryMonitor(system[MGName('all')]), 1000 )

    # Add a space wrapper that wraps all molecules into the box centered at (0,0,0)
    system.add( SpaceWrapper(Vector(0,0,0), all) )

    return system

def setupMoves(system):

    solute = system[ MGName("solute") ]
    protein = system[ MGName("protein") ]
    solvent = system[ MGName("solvent") ]
    residues = system[ MGName("residues") ]

    print "Setting up moves..."
    # Setup Moves
    max_volume_change = 0.1 * solvent.nMolecules() * angstrom3

    # This should automatically set MaxTransl and MaxRot based on the contents of property("flexibility")
    solute_moves = RigidBodyMC( solute ) 
    solute_moves.setMaximumTranslation(solute[MolIdx(0)].molecule().property('flexibility').translation() )
    solute_moves.setMaximumRotation(solute[MolIdx(0)].molecule().property('flexibility').rotation() )

    solute_intra_moves = MoverMove( solute )
    
    solvent_moves = RigidBodyMC( PrefSampler(solute[MolIdx(0)], 
                                             solvent, pref_constant) )

    solvent_moves.setMaximumTranslation(max_solvent_translation)
    solvent_moves.setMaximumRotation(max_solvent_rotation)

    protein_intra_moves = ZMatMove( PrefSampler(solute[MolIdx(0)], residues, pref_constant) ) 
    
    volume_moves = VolumeMove()
    volume_moves.setMaximumVolumeChange(max_volume_change)

    moves = WeightedMoves()
    moves.add( solute_moves, solute_mc_weight / 2 )
    moves.add( solute_intra_moves, solute_mc_weight / 2)
    moves.add( protein_intra_moves, protein_mc_weight )
    moves.add( solvent_moves, solvent_mc_weight )
    moves.add( volume_moves, volume_mc_weight )

    moves.setTemperature(temperature)
    moves.setPressure(pressure)
    moves.setGenerator( RanGenerator(random_seed+3) )
    
    return moves
######## MAIN SCRIPT  #############

print "Loading input..."
amber = Amber()
molecules, space = amber.readcrdtop(crd_file, top_file)

system = createSystem(molecules)

system = setupForcefields(system, space)

moves = setupMoves(system)

# Run a short simulation

print "Performing a short simulation"
nmoves = 1000
for x in range(0,10):
    print "Doing %s moves..." % nmoves
    system = moves.move(system, nmoves, True)

print moves

system.monitor( MonitorName("trajectory") ).writeToDisk("outputXXXXXX.pdb")
