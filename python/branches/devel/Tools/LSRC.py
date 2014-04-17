#!/bin/env python
# -*- coding: utf-8 -*-

from Sire.IO import *
from Sire.System import *
from Sire.Mol import *
from Sire.MM import *
from Sire.FF import *
from Sire.CAS import *
from Sire.Maths import *
from Sire.Analysis import *
from Sire.System import *
from Sire.Base import *
from Sire.Units import *

import Sire.Config
import Sire.Stream

from Sire.Tools.AmberLoader import *
from Sire.Tools import Parameter, resolveParameters

import os
import shutil
import copy

# We will use the waterbox held in the WSRC tools directory
wsrc_tools_dir = "%s/Tools/WSRC" % Sire.Config.share_directory

####################################################
# ALL OF THE GLOBAL USER-AVAILABLE LSRC PARAMETERS #
####################################################

cutoff_method = Parameter("cutoff method", "shift electrostatics",
                          """Method used to apply the non-bonded electrostatic cutoff.""")

rf_dielectric = Parameter("reaction field dielectric", 78.3,
                          """Dielectric constant to use if the reaction field cutoff method is used.""")

coul_cutoff = Parameter("coulomb cutoff", 15*angstrom,
                        """Coulomb cutoff length""")

lj_cutoff = Parameter("LJ cutoff", 15*angstrom,
                      """Lennard Jones cutoff length""")

grid_spacing = Parameter("grid spacing", 1.0*angstrom,
                         """Grid spacing used for the grid-based forcefields""")
grid_buffer = Parameter("grid buffer", 2*angstrom,
                        """Buffer around the grid used to prevent recalculation
                           in the grid-based forcefields.""")

disable_grid = Parameter("disable grid", False, """Whether or not to disable use of the grid""")

temperature = Parameter("temperature", 25*celsius, """Simulation temperature""")
random_seed = Parameter("random seed", None, """Random number seed. Set this if you
                         want to have reproducible simulations.""")

vacuum_calc = Parameter("vacuum calculation", False, 
                        """Whether or not to swap the ligand into vacuum. This is useful if you
                           want to calculate relative hydration free energies.""")

use_fixed_ligand = Parameter("fixed ligand", False,
                             """Whether or not to completely fix the ligand during the simulation.""")

use_rot_trans_ligand = Parameter("ligand rot-trans", True,
                                 """Whether or not the ligand is free to translate and rotate.""")

topology = Parameter("topology", "dual", 
                     """Whether to use 'single' or 'dual' topology to morph between the two ligands.""")

alpha_scale = Parameter("alpha_scale", 1.0,
                        """Amount by which to scale the alpha parameter. The lower the value,
                           the less softening with lambda, while the higher the value, the
                           more softening""")

delta_lambda = Parameter("delta_lambda", 0.001,
                         """Value of delta lambda used in the finite difference thermodynamic
                            integration algorithm used to calculate the free energy""")

water_monitor_distance = Parameter("water monitor distance", 5.0*angstrom,
                                   """The distance up to which the free energy of water molecules
                                      interacting with the ligand should be recorded.""")

nrgmon_frequency = Parameter("energy monitor frequency", 1000, 
                             """The number of steps between each evaluation of the energy monitors.""")

lambda_values = Parameter("lambda values", [ 0.005, 0.071, 0.137, 0.203, 0.269, 0.335, 0.401, 0.467, 0.533, 0.599, 0.665, 0.731, 0.797, 0.863, 0.929, 0.995 ],
                          """The values of lambda to use in the RETI free energy simulation.""")
nsubmoves = Parameter("nsubmoves", 50000,
                      """The number of moves to perform between each RETI move.""")

ligand_name0 = Parameter("ligand0", None,
                         """The name of ligand 0. This should be the name of one of the residues
                            in the ligand, so that this program can find the correct molecule. If it is not set, then 
                            the first non-protein, non solvent molecule is used.""")

ligand_name1 = Parameter("ligand1", None,
                         """The name of ligand 1. This should be the name of one of the residues
                            in the ligand, so that this program can find the correct molecule. If it is not set, then 
                            the first non-protein, non solvent molecule is used.""")

reflection_radius = Parameter("reflection radius", 15*angstrom,
                              """The radius of the reflection sphere""")

ligand_reflection_radius = Parameter("ligand reflection radius", 2*angstrom,
                                     """The reflection radius of the ligand. This is used to constrain the ligand
                                        to remain in the active site. This is needed to define the accessible volume
                                        of the bound state.""")

topfile0 = Parameter("topfile0", "complex0.top",
                     """Name of the topology file containing the solvated protein-ligand0 complex.""")

crdfile0 = Parameter("crdfile0", "complex0.crd",
                     """Name of the coordinate file containing the coordinates of the 
                        solvated protein-ligand0 complex.""")

topfile1 = Parameter("topfile1", "complex1.top",
                     """Name of the topology file containing the solvated protein-ligand1 complex.""")

crdfile1 = Parameter("crdfile1", "complex1.crd",
                     """Name of the coordinate file containing the coordinates of the 
                        solvated protein-ligand1 complex.""")

s3file0 = Parameter("s3file0", "complex0.s3",
                    """Name to use for the intermediate s3 file that will contain the 
                       solvated protein-ligand0 complex after it has been loaded from the top/crd files.""")

s3file1 = Parameter("s3file1", "complex1.s3",
                    """Name to use for the intermediate s3 file that will contain the 
                       solvated protein-ligand1 complex after it has been loaded from the top/crd files.""")

water_topfile = Parameter("water topfile", "%s/waterbox.top" % wsrc_tools_dir,
                          """Name of the topology file containing the water box.""")

water_crdfile = Parameter("water crdfile", "%s/waterbox.crd" % wsrc_tools_dir,
                          """Name of the coordinate file containing the coordinates of the water box.""")

water_s3file = Parameter("water s3file", "waterbox.s3",
                         """Name to use for the intermediate s3 file that will contain the 
                            water box after it has been loaded from the top/crd files.""")

outdir = Parameter("output directory", "output",
                   """Name of the directory in which to place all of the output files.""")

restart_file1 = Parameter("restart file 1", "lsrc_restart1.s3",
                          """Name of the restart file to use to save progress during the stage 1 calculation of 
                             the relative binding free energy of ligand 0 to ligand 1.""")

restart_file2 = Parameter("restart file 2", "lsrc_restart2.s3",
                           """Name of the restart file to use to save progress during the stage 2 calculation of 
                              the relative binding free energy of ligand 1 to ligand 0.""")

restart_file3 = Parameter("restart file 3", "lsrc_restart3.s3",
                          """Name of the restart file to use to save progress during the stage 3 calculation of 
                             the swap of ligand 0 and ligand 1 in the bound complex.""")

nmoves = Parameter("nmoves", 500, """Number of RETI moves to perform during the simulation.""")

nequilmoves = Parameter("nequilmoves", 50000,
                        """Number of equilibration moves to perform before setting up the free energy simulation.""")

coul_power = Parameter("coulomb power", 0,
                        """The soft-core coulomb power parameter""")

shift_delta = Parameter("shift delta", 1.2,
                        """The soft-core shift delta parameter""")

use_single_topology = Parameter("single topology", False,
                                """Whether or not to use single topology to morph from ligand 0 to ligand 1.""")

use_dual_topology = Parameter("dual topology", True,
                              """Whether or not to use dual topology to morph from ligand 0 to ligand 1.""")

save_pdb = Parameter("save pdb", True,
                     """Whether or not to write a PDB of the system after each iteration.""")

save_all_pdbs = Parameter("save all pdbs", False,
                          """Whether or not to write all of the PDBs. If not, only PDBs at the two 
                             end points of the simulation will be written.""")

pdb_frequency = Parameter("pdb frequency", 100,
                          """The frequency (number of iterations between) saving PDBs""")

binwidth = Parameter("free energy bin width", 1 * kcal_per_mol,
                     """The size of the bins used in the histogram of energies collected
                        as part of creating the free energy average, in multiples of delta lambda""")

restart_frequency = Parameter("restart frequency", 10,
                              """The frequency (number of iterations between) saving the restart file for the simulation.""")


####################################################


def renumberMolecules(molgroup):
    newgroup = MoleculeGroup(molgroup.name().value())
    for molnum in molgroup.molNums():
        mol = molgroup[molnum]
        newmol = mol.molecule().edit().renumber().commit()
        newgroup.add( ViewsOfMol(newmol,mol.selections()) )

    return newgroup


def getMinimumDistance(mol0, mol1):
    space = Cartesian()
    return space.minimumDistance(CoordGroup(mol0.molecule().property("coordinates").array()), \
                                 CoordGroup(mol1.molecule().property("coordinates").array()))


def setCLJProperties(forcefield):
    if cutoff_method.val.find("shift electrostatics") != -1:
        forcefield.setShiftElectrostatics(True)

    elif cutoff_method.val.find("reaction field") != -1:
        forcefield.setUseReactionField(True)
        forcefield.setReactionFieldDielectric(rf_dielectric.val)

    else:
        print("Cannot interpret the cutoff method from \"%s\"" % cutoff_method.val, file=sys.stderr)

    forcefield.setSpace(Cartesian())
    forcefield.setSwitchingFunction( HarmonicSwitchingFunction(coul_cutoff.val,coul_cutoff.val,
                                                               lj_cutoff.val,lj_cutoff.val) )

    return forcefield


def setFakeGridProperties(forcefield):
    forcefield.setSwitchingFunction( HarmonicSwitchingFunction(coul_cutoff.val,coul_cutoff.val,
                                                               lj_cutoff.val,lj_cutoff.val) )
    forcefield.setSpace(Cartesian())

    return forcefield


def setGridProperties(forcefield, extra_buffer=0*angstrom):
    forcefield.setGridSpacing(grid_spacing.val)
    forcefield.setBuffer(grid_buffer.val + extra_buffer)
    forcefield.setLJCutoff(lj_cutoff.val)
    forcefield.setCoulombCutoff(coul_cutoff.val)

    return forcefield


def setSoftCoreProperties(forcefield):
    forcefield.setCoulombPower(coul_power.val)
    forcefield.setShiftDelta(shift_delta.val)

    return forcefield


def createLSRCMoves(system):
    # pull out all of the molecule groups for the mobile parts of the system
    mobile_solvent = system[MGName("mobile_solvent")]
    mobile_sidechains = system[MGName("mobile_sidechains")]
    mobile_backbones = system[MGName("mobile_backbones")]
    mobile_solutes = system[MGName("mobile_solutes")]
    mobile_ligand = system[MGName("mobile_ligand")]

    print("Creating the Monte Carlo moves to sample the LSRC system...")

    # create the global set of moves that will be applied to
    # the system
    moves = WeightedMoves()

    # create zmatrix moves to move the protein sidechains
    if mobile_sidechains.nViews() > 0:
        sc_moves = ZMatMove(mobile_sidechains)
        moves.add( sc_moves, mobile_sidechains.nViews() )

    if mobile_backbones.nViews() > 0:
        bb_moves = RigidBodyMC(mobile_backbones)
        bb_moves.setCenterOfRotation( GetCOGPoint( AtomName("CA", CaseInsensitive),
                                                   AtomName("N", CaseInsensitive) ) )

        bb_moves.setMaximumTranslation(0.030*angstrom)
        bb_moves.setMaximumRotation(1.0*degrees)
        moves.add( bb_moves, mobile_backbones.nViews() )

    if not use_fixed_ligand.val:
        if mobile_ligand.nViews() > 0:
            scale_moves = 10

            # get the amount to translate and rotate from the ligand's flexibility object
            flex = mobile_ligand.moleculeAt(0).molecule().property("flexibility")

            if use_rot_trans_ligand.val:
                if (flex.translation().value() != 0 or flex.rotation().value() != 0):
                    rb_moves = RigidBodyMC(mobile_ligand)
                    rb_moves.setMaximumTranslation(flex.translation())
                    rb_moves.setMaximumRotation(flex.rotation())
                    rb_moves.setSynchronisedTranslation(True)
                    rb_moves.setSynchronisedRotation(True)
                    rb_moves.setSharedRotationCenter(True)

                    scale_moves = scale_moves / 2
                    moves.add( rb_moves, scale_moves * mobile_ligand.nViews() )

            intra_moves = InternalMove(mobile_ligand)
            moves.add( intra_moves, scale_moves * mobile_ligand.nViews() )

    if mobile_solutes.nViews() > 0:
        rb_moves = RigidBodyMC(mobile_solutes)

        if system.containsProperty("average solute translation delta"):
            translation_delta = float(str(system.property("average solute translation delta")))
        else:
            translation_delta = 0

        if system.containsProperty("average solute rotation delta"):
            rotation_delta = float(str(system.property("average solute rotation delta")))
        else:
            rotation_delta = 0

        if translation_delta > 0 and rotation_delta > 0:
            rb_moves.setMaximumTranslation(translation_delta * angstroms)
            rb_moves.setMaximumRotation(rotation_delta * degrees)

            if system.containsProperty("reflection sphere radius"):
                reflection_radius = float(str(system.property("reflection sphere radius"))) * angstroms
                reflection_center = system.property("reflection center").toVector()[0]
                rb_moves.setReflectionSphere(reflection_center, reflection_radius)

            moves.add(rb_moves, 4 * mobile_solutes.nViews())

        intra_moves = InternalMove(solute_group)
        moves.add(intra_moves, 4 * mobile_solutes.nViews())

    max_water_translation = 0.15 * angstroms
    max_water_rotation = 15 * degrees

    if mobile_solvent.nViews() > 0:
        rb_moves = RigidBodyMC(mobile_solvent)
        rb_moves.setMaximumTranslation(max_water_translation)
        rb_moves.setMaximumRotation(max_water_rotation)

        if system.containsProperty("reflection sphere radius"):
            reflection_radius = float(str(system.property("reflection sphere radius"))) * angstroms
            reflection_center = system.property("reflection center").toVector()[0]
            rb_moves.setReflectionSphere(reflection_center, reflection_radius)

        moves.add(rb_moves, 4 * mobile_solvent.nViews())

    moves.setTemperature(temperature.val)

    seed = random_seed.val

    if seed is None:
        seed = RanGenerator().randInt(100000,1000000)
        print("Using generated random number seed %d" % seed)
    else:
        print("Using supplied random number seed %d" % seed)
    
    moves.setGenerator( RanGenerator(seed) )

    return moves    


def createStage(system, protein_system, ligand_mol0, ligand_mol1, water_system, mapper):

    # align ligand1 against ligand0
    ligand_mol1 = ligand_mol1.move().align(ligand_mol0, AtomMatchInverter(mapper))

    # create a molecule group for the ligand
    ligand_group0 = MoleculeGroup("ligand0")
    ligand_group0.add(ligand_mol0)

    ligand_group1 = MoleculeGroup("ligand1")
    ligand_group1.add(ligand_mol1)

    system.add(ligand_group0)
    system.add(ligand_group1)

    bound_leg = MoleculeGroup("bound_leg")
    free_leg = MoleculeGroup("free_leg")

    bound_leg.add(ligand_mol0)
    bound_leg.add(ligand_mol1)
    free_leg.add(ligand_mol0)
    free_leg.add(ligand_mol1)

    # pull out the groups that we want from the two systems

    # create a group to hold all of the mobile water molecules in the free leg
    mobile_free_water_group = MoleculeGroup("mobile_free")

    if water_system:
        water_mol = None
        if MGName("mobile_solvents") in water_system.mgNames():
            mols = water_system[MGName("mobile_solvents")].molecules()
            for molnum in mols.molNums():
                # only add this water if it doesn't overlap with ligand1
                water_mol = mols[molnum].molecule().edit().renumber().commit()

                if getMinimumDistance(ligand_mol1,water_mol) > 1.5:
                    for j in range(0,water_mol.nResidues()):
                        water_mol = water_mol.residue( ResIdx(j) ).edit() \
                                                   .setProperty( PDB.parameters().pdbResidueName(), "FWT" ) \
                                                   .commit().molecule()

                    mobile_free_water_group.add(water_mol)

    # create a group to hold all of the fixed water molecules in the free leg
    fixed_free_water_group = MoleculeGroup("fixed_free")

    if water_system:
        if MGName("fixed_molecules") in water_system.mgNames():
            mols = water_system[MGName("fixed_molecules")].molecules()
            for molnum in mols.molNums():
                fixed_free_water_group.add( mols[molnum].molecule().edit().renumber().commit() )

    # create a group to hold all of the fixed molecules in the bound leg
    fixed_bound_group = MoleculeGroup("fixed_bound")
    if MGName("fixed_molecules") in protein_system.mgNames():
        fixed_bound_group.add( protein_system[ MGName("fixed_molecules") ] )

    # create a group to hold all of the mobile solute molecules in the bound leg
    mobile_bound_solutes_group = MoleculeGroup("mobile_bound_solutes")
    if MGName("mobile_solutes") in protein_system.mgNames():
        mobile_bound_solutes_group.add( protein_system[MGName("mobile_solutes")] )
        mobile_bound_solutes_group.remove(ligand_mol0)
        if mobile_bound_solutes_group.nMolecules() > 0:
            bound_leg.add(mobile_bound_solutes_group)
    
    # create a group to hold all of the mobile solvent molecules in the bound leg
    mobile_bound_solvents_group = MoleculeGroup("mobile_bound_solvents")
    mobile_bound_water_group = MoleculeGroup("mobile_bound_water")
    if MGName("mobile_solvents") in protein_system.mgNames():
        mols = protein_system[MGName("mobile_solvents")]
        for molnum in mols.molNums():
            solvent_mol = mols[molnum].molecule()

            try:
                # this is a water molecule if we can swap the coordinates with the 
                # water molecule from teh water box
                water_mol.edit().setProperty("coordinates", \
                                     solvent_mol.property("coordinates"))
                
                for j in range(0,solvent_mol.nResidues()):
                    solvent_mol = solvent_mol.residue( ResIdx(j) ).edit() \
                                             .setProperty( PDB.parameters().pdbResidueName(), "BWT" ) \
                                             .commit().molecule()
                
                mobile_bound_solvents_group.add(solvent_mol)
                mobile_bound_water_group.add(solvent_mol)
            except:
                # the test molecule is not compatible, so it is not 
                # compatible with the water in the water box
                mobile_bound_solvents_group.add(solvent_mol)

        print("The number of bound leg mobile solvent molecules is %d." % mobile_bound_solvents_group.nMolecules())
        print("The number of these which are compatible water molecules is %d." % mobile_bound_water_group.nMolecules())

    # create the groups to hold all of the protein molecules. We will use "extract" to 
    # pull out only those protein atoms that are in the mobile region
    bound_protein_intra_group = MoleculeGroup("bound_protein_intra_group")
    mobile_bound_proteins_group = MoleculeGroup("mobile_bound_proteins")
    mobile_bound_protein_sidechains_group = MoleculeGroup("mobile_bound_protein_sidechains")
    mobile_bound_protein_backbones_group = MoleculeGroup("mobile_bound_protein_backbones")

    if MGName("protein_sidechains") in protein_system.mgNames() or \
       MGName("protein_backbones") in protein_system.mgNames():

        all_proteins = Molecules()

        try:
            protein_sidechains = protein_system[MGName("protein_sidechains")]
            all_proteins.add(protein_sidechains.molecules())
        except:
            protein_sidechains = MoleculeGroup()

        try:
            protein_backbones = protein_system[MGName("protein_backbones")]
            all_proteins.add(protein_backbones.molecules())
        except:
            protein_backbones = MoleculeGroup()

        try:
            boundary_molecules = protein_system[MGName("boundary_molecules")]
            all_proteins.add(boundary_molecules.molecules())
        except:
            boundary_molecules = MoleculeGroup()

        for molnum in all_proteins.molNums():
            protein_mol = all_proteins[molnum].join()
            
            if protein_mol.selectedAll():
                bound_protein_intra_group.add(protein_mol)
                bound_leg.add(protein_mol)

                mobile_protein = None                

                try:
                    mobile_protein = protein_sidechains[molnum]
                    mobile_bound_protein_sidechains_group.add( mobile_protein )
                except:
                    pass

                try:
                    if mobile_protein is None:
                        mobile_protein = protein_backbones[molnum]
                        mobile_bound_protein_backbones_group.add( mobile_protein )
                    else:
                        mobile_protein.add( protein_backbones[molnum].selection() )
                        mobile_bound_protein_backbones_group.add( protein_backbones[molnum] )
                except:
                    pass

                if not (mobile_protein is None):
                    mobile_bound_proteins_group.add( mobile_protein.join() )

            else:
                # only some of the atoms have been selected. We will extract
                # the mobile atoms and will then update all of the other selections
                print("Extracting the mobile atoms of protein %s" % protein_mol)
                new_protein_mol = protein_mol.extract()
                print("Extracted %d mobile atoms from %d total atoms..." % \
                                        (new_protein_mol.nAtoms(), protein_mol.molecule().nAtoms()))

                bound_protein_intra_group.add(new_protein_mol)
                bound_leg.add( new_protein_mol )

                mobile_protein_view = new_protein_mol.selection()
                mobile_protein_view = mobile_protein_view.selectNone()

                try:
                    sidechains = protein_sidechains[molnum]

                    for i in range(0,sidechains.nViews()):
                        view = new_protein_mol.selection()
                        view = view.selectNone()

                        for atomid in sidechains.viewAt(i).selectedAtoms():
                            atom = protein_mol.atom(atomid)
                            resatomid = ResAtomID( atom.residue().number(), atom.name() )
                            view = view.select( resatomid )
                            mobile_protein_view = mobile_protein_view.select( resatomid )

                        if view.nSelected() > 0:
                            mobile_bound_protein_sidechains_group.add( PartialMolecule(new_protein_mol, view) )
                except:
                    pass

                try:
                    backbones = protein_backbones[molnum]

                    for i in range(0,backbones.nViews()):
                        view = new_protein_mol.selection()
                        view = view.selectNone()

                        for atomid in backbones.viewAt(i).selectedAtoms():
                            atom = protein_mol.atom(atomid)
                            resatomid = ResAtomID( atom.residue().number(), atom.name() )
                            view = view.select( resatomid )
                            mobile_protein_view = mobile_protein_view.select( resatomid )

                        if view.nSelected() > 0:
                            mobile_bound_protein_backbones_group.add( PartialMolecule(new_protein_mol, view) )
                except:
                    pass

                if mobile_protein_view.nSelected() > 0:
                    mobile_bound_proteins_group.add( PartialMolecule(new_protein_mol, mobile_protein_view) )

    # finished adding in all of the protein groups
    bound_leg.add(mobile_bound_solvents_group)
    free_leg.add(mobile_free_water_group)

    system.add(bound_leg)
    system.add(free_leg)

    # now add in the forcefields for the system...
    print("Creating the forcefields for the WSRC system...")

    # first, group together the molecules grouped above into convenient
    # groups for the forcefields

    # group holding all of the mobile atoms in the bound leg
    mobile_bound_mols = mobile_bound_solvents_group.molecules()
    mobile_bound_mols.add( mobile_bound_solutes_group.molecules() )
    mobile_bound_mols.add( bound_protein_intra_group.molecules() )

    # group holding all of the mobile atoms in the bound leg, excluding the 
    # buffer atoms that are fixed, but bonded to mobile atoms
    mobile_buffered_bound_mols = mobile_bound_solvents_group.molecules()
    mobile_buffered_bound_mols.add( mobile_bound_solutes_group.molecules() )
    mobile_buffered_bound_mols.add( mobile_bound_proteins_group.molecules() )

    # group holding all of the mobile water molecules in the free leg
    mobile_free_mols = mobile_free_water_group.molecules()

    # group holding all of the fixed water molecules in the free leg
    fixed_free_group = fixed_free_water_group

    # group holding all of the protein molecules that need intramolecular terms calculated
    bound_protein_intra_mols = bound_protein_intra_group.molecules()

    # group holding all of the solute molecules that nede intramolecular terms calculated
    bound_solute_intra_mols = mobile_bound_solutes_group.molecules()

    ###
    ### INTRA-ENERGY OF THE LIGAND AND CLUSTER
    ###
    
    # intramolecular energy of the ligands
    ligand_intraclj = IntraCLJFF("ligand:intraclj")
    ligand_intraclj = setCLJProperties(ligand_intraclj)
    ligand_intraclj.add(ligand_mol0)
    ligand_intraclj.add(ligand_mol1)

    ligand_intraff = InternalFF("ligand:intra")
    ligand_intraff.add(ligand_mol0)
    ligand_intraff.add(ligand_mol1)

    ###
    ### FORCEFIELDS INVOLVING THE LIGAND/CLUSTER BOUND LEG
    ###

    # forcefield holding the energy between the ligand and the mobile atoms in the
    # bound leg
    bound_ligand0_mobile = InterGroupSoftCLJFF("bound:ligand0-mobile")
    bound_ligand0_mobile = setCLJProperties(bound_ligand0_mobile)
    bound_ligand0_mobile = setSoftCoreProperties(bound_ligand0_mobile)

    bound_ligand0_mobile.add(ligand_mol0, MGIdx(0))
    bound_ligand0_mobile.add(mobile_bound_mols, MGIdx(1))

    bound_ligand1_mobile = InterGroupSoftCLJFF("bound:ligand1-mobile")
    bound_ligand1_mobile = setCLJProperties(bound_ligand1_mobile)
    bound_ligand1_mobile = setSoftCoreProperties(bound_ligand1_mobile)

    bound_ligand1_mobile.add(ligand_mol1, MGIdx(0))
    bound_ligand1_mobile.add(mobile_bound_mols, MGIdx(1))

    # Whether or not to disable the grid and calculate all energies atomisticly
    if disable_grid:
        # we need to renumber all of the fixed molecules so that they don't clash
        # with the mobile molecules
        print("Renumbering fixed molecules...")
        fixed_bound_group = renumberMolecules(fixed_bound_group)
        fixed_free_group = renumberMolecules(fixed_free_group)

    # forcefield holding the energy between the ligand and the fixed atoms in the bound leg
    if disable_grid:
        bound_ligand0_fixed = InterGroupCLJFF("bound:ligand0-fixed")
        bound_ligand0_fixed = setCLJProperties(bound_ligand0_fixed)
        bound_ligand0_fixed = setFakeGridProperties(bound_ligand0_fixed)

        bound_ligand0_fixed.add(ligand_mol0, MGIdx(0))
        bound_ligand0_fixed.add(fixed_bound_group, MGIdx(1))

        bound_ligand1_fixed = InterGroupCLJFF("bound:ligand1-fixed")
        bound_ligand1_fixed = setCLJProperties(bound_ligand1_fixed)
        bound_ligand1_fixed = setFakeGridProperties(bound_ligand1_fixed)

        bound_ligand1_fixed.add(ligand_mol1, MGIdx(0))
        bound_ligand1_fixed.add(fixed_bound_group, MGIdx(1))
    else:
        bound_ligand0_fixed = GridFF("bound:ligand0-fixed")
        bound_ligand0_fixed = setCLJProperties(bound_ligand0_fixed)
        bound_ligand0_fixed = setGridProperties(bound_ligand0_fixed)

        bound_ligand0_fixed.add(ligand_mol0, MGIdx(0))
        bound_ligand0_fixed.addFixedAtoms( fixed_bound_group )

        bound_ligand1_fixed = GridFF("bound:ligand1-fixed")
        bound_ligand1_fixed = setCLJProperties(bound_ligand1_fixed)
        bound_ligand1_fixed = setGridProperties(bound_ligand1_fixed)

        bound_ligand1_fixed.add(ligand_mol1, MGIdx(0))
        bound_ligand1_fixed.addFixedAtoms( fixed_bound_group )

    ###
    ### FORCEFIELDS INVOLVING THE LIGAND/CLUSTER FREE LEG
    ###

    # forcefield holding the energy between the ligand and the mobile atoms
    # in the free leg
    free_ligand0_mobile = InterGroupSoftCLJFF("free:ligand0-mobile")
    free_ligand0_mobile = setCLJProperties(free_ligand0_mobile)
    free_ligand0_mobile = setSoftCoreProperties(free_ligand0_mobile)

    free_ligand0_mobile.add(ligand_mol0, MGIdx(0))
    free_ligand0_mobile.add(mobile_free_mols, MGIdx(1))

    free_ligand1_mobile = InterGroupSoftCLJFF("free:ligand1-mobile")
    free_ligand1_mobile = setCLJProperties(free_ligand1_mobile)
    free_ligand1_mobile = setSoftCoreProperties(free_ligand1_mobile)

    free_ligand1_mobile.add(ligand_mol1, MGIdx(0))
    free_ligand1_mobile.add(mobile_free_mols, MGIdx(1))

    # forcefield holding the energy between the ligand and the fixed atoms
    # in the free leg
    if disable_grid.val:
        free_ligand0_fixed = InterGroupCLJFF("free:ligand0_fixed")
        free_ligand0_fixed = setCLJProperties(free_ligand0_fixed)
        free_ligand0_fixed = setFakeGridProperties(free_ligand0_fixed)

        free_ligand0_fixed.add(ligand_mol0, MGIdx(0))
        free_ligand0_fixed.add(fixed_free_group, MGIdx(1))

        free_ligand1_fixed = InterGroupCLJFF("free:ligand1_fixed")
        free_ligand1_fixed = setCLJProperties(free_ligand1_fixed)
        free_ligand1_fixed = setFakeGridProperties(free_ligand1_fixed)

        free_ligand1_fixed.add(ligand_mol1, MGIdx(0))
        free_ligand1_fixed.add(fixed_free_group, MGIdx(1))
    else:
        free_ligand0_fixed = GridFF("free:ligand0-fixed")
        free_ligand0_fixed = setCLJProperties(free_ligand0_fixed)
        free_ligand0_fixed = setGridProperties(free_ligand0_fixed)

        free_ligand0_fixed.add(ligand_mol0, MGIdx(0))
        free_ligand0_fixed.addFixedAtoms(fixed_free_group)

        free_ligand1_fixed = GridFF("free:ligand1-fixed")
        free_ligand1_fixed = setCLJProperties(free_ligand1_fixed)
        free_ligand1_fixed = setGridProperties(free_ligand1_fixed)

        free_ligand1_fixed.add(ligand_mol1, MGIdx(0))
        free_ligand1_fixed.addFixedAtoms(fixed_free_group)

    ###
    ### FORCEFIELDS LOCAL ONLY TO THE BOUND LEG
    ###
    bound_forcefields = []

    # forcefield holding the energy between the bound leg mobile atoms and  
    # the bound leg fixed atoms
    if disable_grid.val:
        bound_mobile_fixed = InterGroupCLJFF("bound:mobile-fixed")
        bound_mobile_fixed = setCLJProperties(bound_mobile_fixed)
        bound_mobile_fixed = setFakeGridProperties(bound_mobile_fixed)
        bound_mobile_fixed.add(mobile_buffered_bound_mols, MGIdx(0))
        bound_mobile_fixed.add(fixed_bound_group, MGIdx(1))
        bound_forcefields.append(bound_mobile_fixed)
    else:
        bound_mobile_fixed = GridFF("bound:mobile-fixed")
        bound_mobile_fixed = setCLJProperties(bound_mobile_fixed)
        bound_mobile_fixed = setGridProperties(bound_mobile_fixed)

        # we use mobile_buffered_bound_group as this group misses out atoms that are bonded
        # to fixed atoms (thus preventing large energies caused by incorrect non-bonded calculations)
        bound_mobile_fixed.add(mobile_buffered_bound_mols, MGIdx(0))
        bound_mobile_fixed.addFixedAtoms(fixed_bound_group)

        bound_forcefields.append(bound_mobile_fixed)

    # forcefield holding the intermolecular energy between all bound molecules
    bound_mobile_mobile = InterCLJFF("bound:mobile-mobile")
    bound_mobile_mobile = setCLJProperties(bound_mobile_mobile)

    bound_mobile_mobile.add(mobile_bound_mols)

    bound_forcefields.append(bound_mobile_mobile)

    # intramolecular energy of the protein
    if bound_protein_intra_mols.nMolecules() > 0:
        protein_intraclj = IntraCLJFF("bound:protein_intraclj")
        protein_intraclj = setCLJProperties(protein_intraclj)

        protein_intraff = InternalFF("bound:protein_intra")

        for molnum in bound_protein_intra_mols.molNums():
            protein_mol = bound_protein_intra_mols[molnum].join()
            protein_intraclj.add(protein_mol)
            protein_intraff.add(protein_mol)

        bound_forcefields.append(protein_intraclj)
        bound_forcefields.append(protein_intraff)

    # intramolecular energy of any other solutes
    if bound_solute_intra_mols.nMolecules() > 0:
        solute_intraclj = IntraCLJFF("bound:solute_intraclj")
        solute_intraclj = setCLJProperties(solute_intraclj)

        solute_intraff = InternalFF("bound:solute_intra")

        for molnum in bound_solute_intra_mols.molNums():
            solute_mol = bound_solute_intra_mols[molnum].join()
            solute_intraclj.add(solute_mol)
            solute_intraff.add(solute_mol)

        bound_forcefields.append(solute_intraclj)
        bound_forcefields.append(solute_intraff)

    ###
    ### FORCEFIELDS LOCAL ONLY TO THE FREE LEG
    ###
    free_forcefields = []

    # forcefield holding the energy between the mobile free molecules and the
    # fixed free molecules
    if disable_grid:
        free_mobile_fixed = InterGroupCLJFF("free:mobile-fixed")
        free_mobile_fixed = setCLJProperties(free_mobile_fixed)
        free_mobile_fixed = setFakeGridProperties(free_mobile_fixed)

        free_mobile_fixed.add(mobile_free_mols, MGIdx(0))
        free_mobile_fixed.add(fixed_free_group, MGIdx(1))
        free_forcefields.append(free_mobile_fixed)
    else:
        free_mobile_fixed = GridFF("free:mobile-fixed")
        free_mobile_fixed = setCLJProperties(free_mobile_fixed)
        free_mobile_fixed = setGridProperties(free_mobile_fixed)

        free_mobile_fixed.add(mobile_free_mols, MGIdx(0))
        free_mobile_fixed.addFixedAtoms(fixed_free_group)

        free_forcefields.append(free_mobile_fixed)
    
    # forcefield holding the intermolecular energy between the mobile free molecules
    free_mobile_mobile = InterCLJFF("free:mobile-mobile")
    free_mobile_mobile = setCLJProperties(free_mobile_mobile)

    free_mobile_mobile.add(mobile_free_mols)

    free_forcefields.append(free_mobile_mobile)

    ###
    ### NOW ADD THE FORCEFIELDS TO THE SYSTEM
    ###
    ###
    ### SETTING THE FORCEFIELD EXPRESSIONS
    ###

    bound_ligand0_fixed_nrg = bound_ligand0_fixed.components().total()
    free_ligand0_fixed_nrg = free_ligand0_fixed.components().total()
    bound_ligand1_fixed_nrg = bound_ligand1_fixed.components().total()
    free_ligand1_fixed_nrg = free_ligand1_fixed.components().total()

    ligand_int_nrg_sym = Symbol("E_{ligand:internal}")
    ligand_int_nrg = ligand_intraclj.components().total() + \
                     ligand_intraff.components().total()

    ligand0_bound_nrg_sym = Symbol("E_{ligand0:bound}")

    ligand0_bound_nrg = bound_ligand0_mobile.components().total(0) + \
                        bound_ligand0_fixed_nrg

    ligand0_bound_nrg_f_sym = Symbol("E_{ligand0:bound_{f}}")
    ligand0_bound_nrg_f = bound_ligand0_mobile.components().total(1) + \
                          bound_ligand0_fixed_nrg

    ligand0_bound_nrg_b_sym = Symbol("E_{ligand0:bound_{b}}")
    ligand0_bound_nrg_b = bound_ligand0_mobile.components().total(2) + \
                          bound_ligand0_fixed_nrg

    ligand0_bound_nrg_next_sym = Symbol("E_{ligand0:bound_{next}}")
    ligand0_bound_nrg_next = bound_ligand0_mobile.components().total(3) + \
                             bound_ligand0_fixed_nrg

    ligand0_bound_nrg_prev_sym = Symbol("E_{ligand0:bound_{prev}}")
    ligand0_bound_nrg_prev = bound_ligand0_mobile.components().total(4) + \
                             bound_ligand0_fixed_nrg

    ligand1_bound_nrg_sym = Symbol("E_{ligand1:bound}")

    ligand1_bound_nrg = bound_ligand1_mobile.components().total(0) + \
                        bound_ligand1_fixed_nrg

    ligand1_bound_nrg_f_sym = Symbol("E_{ligand1:bound_{f}}")
    ligand1_bound_nrg_f = bound_ligand1_mobile.components().total(1) + \
                          bound_ligand1_fixed_nrg

    ligand1_bound_nrg_b_sym = Symbol("E_{ligand1:bound_{b}}")
    ligand1_bound_nrg_b = bound_ligand1_mobile.components().total(2) + \
                          bound_ligand1_fixed_nrg

    ligand1_bound_nrg_next_sym = Symbol("E_{ligand1:bound_{next}}")
    ligand1_bound_nrg_next = bound_ligand1_mobile.components().total(3) + \
                             bound_ligand1_fixed_nrg

    ligand1_bound_nrg_prev_sym = Symbol("E_{ligand1:bound_{prev}}")
    ligand1_bound_nrg_prev = bound_ligand1_mobile.components().total(4) + \
                             bound_ligand1_fixed_nrg

    ligand0_free_nrg_sym = Symbol("E_{ligand0:free}")
    ligand0_free_nrg = free_ligand0_mobile.components().total(0) + \
                       free_ligand0_fixed_nrg

    ligand0_free_nrg_f_sym = Symbol("E_{ligand0:free_{f}}")
    ligand0_free_nrg_f = free_ligand0_mobile.components().total(1) + \
                         free_ligand0_fixed_nrg

    ligand0_free_nrg_b_sym = Symbol("E_{ligand0:free_{b}}")
    ligand0_free_nrg_b = free_ligand0_mobile.components().total(2) + \
                         free_ligand0_fixed_nrg

    ligand0_free_nrg_next_sym = Symbol("E_{ligand0:free_{next}}")
    ligand0_free_nrg_next = free_ligand0_mobile.components().total(3) + \
                            free_ligand0_fixed_nrg

    ligand0_free_nrg_prev_sym = Symbol("E_{ligand0:free_{prev}}")
    ligand0_free_nrg_prev = free_ligand0_mobile.components().total(4) + \
                            free_ligand0_fixed_nrg

    ligand1_free_nrg_sym = Symbol("E_{ligand1:free}")
    ligand1_free_nrg = free_ligand1_mobile.components().total(0) + \
                       free_ligand1_fixed_nrg

    ligand1_free_nrg_f_sym = Symbol("E_{ligand1:free_{f}}")
    ligand1_free_nrg_f = free_ligand1_mobile.components().total(1) + \
                         free_ligand1_fixed_nrg

    ligand1_free_nrg_b_sym = Symbol("E_{ligand1:free_{b}}")
    ligand1_free_nrg_b = free_ligand1_mobile.components().total(2) + \
                         free_ligand1_fixed_nrg

    ligand1_free_nrg_next_sym = Symbol("E_{ligand1:free_{next}}")
    ligand1_free_nrg_next = free_ligand1_mobile.components().total(3) + \
                            free_ligand1_fixed_nrg

    ligand1_free_nrg_prev_sym = Symbol("E_{ligand1:free_{prev}}")
    ligand1_free_nrg_prev = free_ligand1_mobile.components().total(4) + \
                            free_ligand1_fixed_nrg

    lam = Symbol("lambda")
    lam_f = Symbol("lambda_{f}")
    lam_b = Symbol("lambda_{b}")
    lam_next = Symbol("lambda_{next}")
    lam_prev = Symbol("lambda_{prev}")

    system.add(ligand_intraclj)
    system.add(ligand_intraff)
    system.add(bound_ligand0_mobile)
    system.add(free_ligand0_mobile)
    system.add(bound_ligand0_fixed)
    system.add(free_ligand0_fixed)
    system.add(bound_ligand1_mobile)
    system.add(free_ligand1_mobile)
    system.add(bound_ligand1_fixed)
    system.add(free_ligand1_fixed)

    system.setConstant(lam, 0.0)
    system.setConstant(lam_f, 0.0)
    system.setConstant(lam_b, 0.0)
    system.setConstant(lam_next, 0.0)
    system.setConstant(lam_prev, 0.0)

    system.setComponent(ligand_int_nrg_sym, ligand_int_nrg)

    system.setComponent(ligand0_bound_nrg_sym, ligand0_bound_nrg)
    system.setComponent(ligand0_bound_nrg_f_sym, ligand0_bound_nrg_f)
    system.setComponent(ligand0_bound_nrg_b_sym, ligand0_bound_nrg_b)
    system.setComponent(ligand0_bound_nrg_next_sym, ligand0_bound_nrg_next)
    system.setComponent(ligand0_bound_nrg_prev_sym, ligand0_bound_nrg_prev)

    system.setComponent(ligand1_bound_nrg_sym, ligand1_bound_nrg)
    system.setComponent(ligand1_bound_nrg_f_sym, ligand1_bound_nrg_f)
    system.setComponent(ligand1_bound_nrg_b_sym, ligand1_bound_nrg_b)
    system.setComponent(ligand1_bound_nrg_next_sym, ligand1_bound_nrg_next)
    system.setComponent(ligand1_bound_nrg_prev_sym, ligand1_bound_nrg_prev)

    system.setComponent(ligand0_free_nrg_sym, ligand0_free_nrg)
    system.setComponent(ligand0_free_nrg_f_sym, ligand0_free_nrg_f)
    system.setComponent(ligand0_free_nrg_b_sym, ligand0_free_nrg_b)
    system.setComponent(ligand0_free_nrg_next_sym, ligand0_free_nrg_next)
    system.setComponent(ligand0_free_nrg_prev_sym, ligand0_free_nrg_prev)

    system.setComponent(ligand1_free_nrg_sym, ligand1_free_nrg)
    system.setComponent(ligand1_free_nrg_f_sym, ligand1_free_nrg_f)
    system.setComponent(ligand1_free_nrg_b_sym, ligand1_free_nrg_b)
    system.setComponent(ligand1_free_nrg_next_sym, ligand1_free_nrg_next)
    system.setComponent(ligand1_free_nrg_prev_sym, ligand1_free_nrg_prev)

    bound_bound_nrg_sym = Symbol("E_{bound-bound}")
    bound_bound_nrg = None
    
    for bound_forcefield in bound_forcefields:
        if bound_bound_nrg is None:
            bound_bound_nrg = bound_forcefield.components().total()
        else:
            bound_bound_nrg = bound_bound_nrg + bound_forcefield.components().total()

        system.add(bound_forcefield)

    system.setComponent(bound_bound_nrg_sym, bound_bound_nrg)

    free_free_nrg_sym = Symbol("E_{free-free}")
    free_free_nrg = None

    for free_forcefield in free_forcefields:
        if free_free_nrg is None:
            free_free_nrg = free_forcefield.components().total()
        else:
            free_free_nrg = free_free_nrg + free_forcefield.components().total()

        system.add(free_forcefield)

    system.setComponent(free_free_nrg_sym, free_free_nrg)

    bound_nrg_sym = Symbol("E_{bound}")
    bound_nrg = ((1-lam) * ligand0_bound_nrg_sym) + (lam * ligand1_bound_nrg_sym)

    bound_nrg_f_sym = Symbol("E_{bound_{f}}")
    bound_nrg_f = ((1-lam_f) * ligand0_bound_nrg_f_sym) + (lam_f * ligand1_bound_nrg_f_sym)

    bound_nrg_b_sym = Symbol("E_{bound_{b}}")
    bound_nrg_b = ((1-lam_b) * ligand0_bound_nrg_b_sym) + (lam_b * ligand1_bound_nrg_b_sym)

    bound_nrg_next_sym = Symbol("E_{bound_{next}}")
    bound_nrg_next = ((1-lam_next) * ligand0_bound_nrg_next_sym) + (lam_next * ligand1_bound_nrg_next_sym)

    bound_nrg_prev_sym = Symbol("E_{bound_{prev}}")
    bound_nrg_prev = ((1-lam_prev) * ligand0_bound_nrg_prev_sym) + (lam_prev * ligand1_bound_nrg_prev_sym)
    
    free_nrg_sym = Symbol("E_{free}")
    free_nrg = (lam * ligand0_free_nrg_sym) + ((1-lam) * ligand1_free_nrg_sym)

    free_nrg_f_sym = Symbol("E_{free_{f}}")
    free_nrg_f = (lam_f * ligand0_free_nrg_f_sym) + ((1-lam_f) * ligand1_free_nrg_f_sym)

    free_nrg_b_sym = Symbol("E_{free_{b}}")
    free_nrg_b = (lam_b * ligand0_free_nrg_b_sym) + ((1-lam_b) * ligand1_free_nrg_b_sym)

    free_nrg_next_sym = Symbol("E_{free_{next}}")
    free_nrg_next = (lam_next * ligand0_free_nrg_next_sym) + ((1-lam_next) * ligand1_free_nrg_next_sym)

    free_nrg_prev_sym = Symbol("E_{free_{prev}}")
    free_nrg_prev = (lam_prev * ligand0_free_nrg_prev_sym) + ((1-lam_prev) * ligand1_free_nrg_prev_sym)

    box_nrg_sym = Symbol("E_{box}")
    box_nrg = bound_bound_nrg_sym + free_free_nrg_sym + ligand_int_nrg_sym

    total_nrg_sym = system.totalComponent()
    total_nrg = bound_nrg_sym + free_nrg_sym + box_nrg_sym

    total_nrg_f_sym = Symbol("E_{total_{f}}")
    total_nrg_f = bound_nrg_f_sym + free_nrg_f_sym + box_nrg_sym

    total_nrg_b_sym = Symbol("E_{total_{b}}")
    total_nrg_b = bound_nrg_b_sym + free_nrg_b_sym + box_nrg_sym

    total_nrg_next_sym = Symbol("E_{total_{next}}")
    total_nrg_next = bound_nrg_next_sym + free_nrg_next_sym + box_nrg_sym

    total_nrg_prev_sym = Symbol("E_{total_{prev}}")
    total_nrg_prev = bound_nrg_prev_sym + free_nrg_prev_sym + box_nrg_sym

    system.setComponent(bound_nrg_sym, bound_nrg)
    system.setComponent(bound_nrg_f_sym, bound_nrg_f)
    system.setComponent(bound_nrg_b_sym, bound_nrg_b)
    system.setComponent(bound_nrg_next_sym, bound_nrg_next)
    system.setComponent(bound_nrg_prev_sym, bound_nrg_prev)

    system.setComponent(free_nrg_sym, free_nrg)
    system.setComponent(free_nrg_f_sym, free_nrg_f)
    system.setComponent(free_nrg_b_sym, free_nrg_b)
    system.setComponent(free_nrg_next_sym, free_nrg_next)
    system.setComponent(free_nrg_prev_sym, free_nrg_prev)

    system.setComponent(box_nrg_sym, box_nrg)

    system.setComponent(total_nrg_sym, total_nrg)
    system.setComponent(total_nrg_f_sym, total_nrg_f)
    system.setComponent(total_nrg_b_sym, total_nrg_b)
    system.setComponent(total_nrg_next_sym, total_nrg_next)
    system.setComponent(total_nrg_prev_sym, total_nrg_prev)

    system.setComponent( Symbol("delta_nrg^{F}"), (total_nrg_f_sym - total_nrg_sym) )
    system.setComponent( Symbol("delta_nrg^{B}"), (total_nrg_b_sym - total_nrg_sym) )
    system.setComponent( Symbol("delta_nrg^{next}"), (total_nrg_next_sym - total_nrg_sym) )
    system.setComponent( Symbol("delta_nrg^{prev}"), (total_nrg_prev_sym - total_nrg_sym) )

    system.setComponent( Symbol("delta_bound_nrg^{F}"), (bound_nrg_f_sym - bound_nrg_sym) )
    system.setComponent( Symbol("delta_bound_nrg^{B}"), (bound_nrg_b_sym - bound_nrg_sym) )
    system.setComponent( Symbol("delta_free_nrg^{F}"), (free_nrg_f_sym - free_nrg_sym) )
    system.setComponent( Symbol("delta_free_nrg^{B}"), (free_nrg_b_sym - free_nrg_sym) )

    # Now add constraints. These are used to keep 
    # all lambda values between 0 and 1, and to
    # map the alpha values of the softcore forcefields to lambda
    print("\nCreating LSRC system constraints...\n")

    # Add the constraint that lambda_f is lambda + delta_lambda and
    # lambda_b is lambda - delta_lambda (kept to between 0 and 1)
    dlam = delta_lambda.val

    if dlam > 1 or dlam < 0.0000001:
        print("WARNING: Weird value of delta_lambda (%s). Setting it to 0.001" % dlam)
        dlam = 0.001

    # Constrain lam_f and lam_b to lie with delta_lambda of lambda
    dlam_sym = Symbol("delta_lambda")
    system.setConstant( dlam_sym, dlam )
    system.add( ComponentConstraint( lam_f, Min( lam + dlam_sym, 1 ) ) )
    system.add( ComponentConstraint( lam_b, Max( lam - dlam_sym, 0 ) ) )

    # Constrain lam_next and lam_prev to be equal to the next and previous
    # windows lambda values
    lamvals = copy.deepcopy( lambda_values.val )

    if lamvals[-1] != 1:
        lamvals.append(1)

    if lamvals[0] != 0:
        lamvals.insert(0,0)

    system.add( WindowedComponent( lam_next, lam, lamvals, 1 ) )
    system.add( WindowedComponent( lam_prev, lam, lamvals, -1 ) )
    system.setConstant( lam, lambda_values.val[0] )

    # now add alpha variables that can be used by the EnergyMonitors
    alpha_on = Symbol("alpha_on")
    alpha_off = Symbol("alpha_off")

    system.setConstant(alpha_on, 0)
    system.setConstant(alpha_off, 1)

    system.setConstant( Symbol("alpha_scale"), alpha_scale.val )
    system.add( ComponentConstraint( alpha_on, alpha_scale.val * lam ) )
    system.add( ComponentConstraint( alpha_off, alpha_scale.val * (1-lam) ) )

    # Now constrain alpha to follow lambda
    # First, the reference state (alpha0)
    system.add( PropertyConstraint( "alpha0", FFName("free:ligand1-mobile"), alpha_scale.val * lam ) )
    system.add( PropertyConstraint( "alpha0", FFName("bound:ligand1-mobile"), alpha_scale.val * (1 - lam) ) )

    system.add( PropertyConstraint( "alpha0", FFName("bound:ligand0-mobile"), alpha_scale.val * lam ) )
    system.add( PropertyConstraint( "alpha0", FFName("free:ligand0-mobile"), alpha_scale.val * (1 - lam) ) )

    # Now the forwards perturbed state (alpha1)
    system.add( PropertyConstraint( "alpha1", FFName("free:ligand1-mobile"),  alpha_scale.val * lam_f ) )
    system.add( PropertyConstraint( "alpha1", FFName("bound:ligand1-mobile"),  alpha_scale.val * (1 - lam_f) ) )

    system.add( PropertyConstraint( "alpha1", FFName("bound:ligand0-mobile"),  alpha_scale.val * lam_f ) )
    system.add( PropertyConstraint( "alpha1", FFName("free:ligand0-mobile"),  alpha_scale.val * (1 - lam_f) ) )

    # Now the backwards perturbed state (alpha2)
    system.add( PropertyConstraint( "alpha2", FFName("free:ligand1-mobile"),  alpha_scale.val * lam_b ) )
    system.add( PropertyConstraint( "alpha2", FFName("bound:ligand1-mobile"),  alpha_scale.val * (1 - lam_b) ) )

    system.add( PropertyConstraint( "alpha2", FFName("bound:ligand0-mobile"),  alpha_scale.val * lam_b ) )
    system.add( PropertyConstraint( "alpha2", FFName("free:ligand0-mobile"),  alpha_scale.val * (1 - lam_b) ) )

    # Now the next window perturbed state (alpha3)
    system.add( PropertyConstraint( "alpha3", FFName("free:ligand1-mobile"),  alpha_scale.val * lam_next ) )
    system.add( PropertyConstraint( "alpha3", FFName("bound:ligand1-mobile"),  alpha_scale.val * (1 - lam_next) ) )

    system.add( PropertyConstraint( "alpha3", FFName("bound:ligand0-mobile"),  alpha_scale.val * lam_next ) )
    system.add( PropertyConstraint( "alpha3", FFName("free:ligand0-mobile"),  alpha_scale.val * (1 - lam_next) ) )

    # Now the previous window perturbed state (alpha4)
    system.add( PropertyConstraint( "alpha4", FFName("free:ligand1-mobile"),  alpha_scale.val * lam_prev ) )
    system.add( PropertyConstraint( "alpha4", FFName("bound:ligand1-mobile"),  alpha_scale.val * (1 - lam_prev) ) )

    system.add( PropertyConstraint( "alpha4", FFName("bound:ligand0-mobile"),  alpha_scale.val * lam_prev ) )
    system.add( PropertyConstraint( "alpha4", FFName("free:ligand0-mobile"),  alpha_scale.val * (1 - lam_prev) ) )

    # Now lets create all of the groups for moves based on the above

    # All solvent molecules in the bound and free legs are moved together
    mobile_solvent = MoleculeGroup("mobile_solvent")
    mobile_solvent.add( mobile_bound_solvents_group.molecules() )
    mobile_solvent.add( mobile_free_water_group.molecules() )

    system.add( mobile_solvent )

    # All protein sidechains are moved together
    mobile_sidechains = MoleculeGroup("mobile_sidechains")
    mobile_sidechains.add(mobile_bound_protein_sidechains_group.molecules())
    
    system.add( mobile_sidechains )

    # All protein backbones are moved together
    mobile_backbones = MoleculeGroup("mobile_backbones")
    mobile_backbones.add(mobile_bound_protein_backbones_group.molecules())

    system.add( mobile_backbones )

    # All solute molecules are moved together
    mobile_solutes = MoleculeGroup("mobile_solutes")
    mobile_solutes.add(mobile_bound_solutes_group.molecules())

    system.add( mobile_solutes )

    # The ligand is moved in its own group
    mobile_ligand = MoleculeGroup("mobile_ligand")
    mobile_ligand.add(ligand_mol0)
    mobile_ligand.add(ligand_mol1)

    system.add( mobile_ligand )

    # Apply all of the constraints
    system.applyConstraints()

    ###
    ### ADD THE SYSTEM MONITORS
    ###

    # Now we need to add the monitors...
    print("\nAdding system monitors...")

    system.add( "delta_g^{F}", MonitorComponent( Symbol("delta_nrg^{F}"),
                                                 FreeEnergyAverage(temperature.val,
                                                                   dlam * binwidth.val) ) )

    system.add( "delta_g^{B}", MonitorComponent( Symbol("delta_nrg^{B}"),
                                                 FreeEnergyAverage(temperature.val,
                                                                   dlam * binwidth.val, False) ) )

    system.add( "delta_g^{next}", MonitorComponent( Symbol("delta_nrg^{next}"),
                                                    BennettsFreeEnergyAverage(0 * kcal_per_mol,
                                                                              temperature.val,
                                                                              0.1 * binwidth.val) ) )

    system.add( "delta_g^{prev}", MonitorComponent( Symbol("delta_nrg^{prev}"),
                                                    BennettsFreeEnergyAverage(0 * kcal_per_mol,
                                                                              temperature.val,
                                                                              0.1 * binwidth.val, False) ) )
    
    system.add( "delta_bound_g^{F}", MonitorComponent( Symbol("delta_bound_nrg^{F}"),
                                                       FreeEnergyAverage(temperature.val,
                                                                         dlam * binwidth.val) ) )
    system.add( "delta_bound_g^{B}", MonitorComponent( Symbol("delta_bound_nrg^{B}"),
                                                       FreeEnergyAverage(temperature.val,
                                                                         dlam * binwidth.val, False) ) )
    system.add( "delta_free_g^{F}", MonitorComponent( Symbol("delta_free_nrg^{F}"),
                                                      FreeEnergyAverage(temperature.val,
                                                                        dlam * binwidth.val) ) )
    system.add( "delta_free_g^{B}", MonitorComponent( Symbol("delta_free_nrg^{B}"),
                                                      FreeEnergyAverage(temperature.val,
                                                                        dlam * binwidth.val, False) ) )

    # we will monitor the average energy between the two ligands and each
    # residue with mobile sidechain, and each mobile solute
    monitor_prosol = None

    if mobile_solutes.isEmpty():
        monitor_prosol = mobile_sidechains
    elif mobile_sidechains.isEmpty():
        monitor_prosol = mobile_solutes
    else:
        monitor_prosol = MoleculeGroup("monitored_protein_solute")
        monitor_prosol.add(mobile_sidechains)
        monitor_prosol.add(mobile_solutes)
        system.add(monitor_prosol)

    residue_nrgmon = FreeEnergyMonitor(monitor_prosol, ligand_group0, ligand_group1)

    nrgmons = {}
    nrgmons["residue_nrgmon"] = residue_nrgmon

    # because the water molecules can diffuse, we find all waters within
    # a certain distance of the ligand, and then identify them using identity
    # points (placed at the center of the initial positions of the waters),
    # and then monitor those...
    boundwater_points = []
    freewater_points = []

    if water_monitor_distance.val:
        dist = water_monitor_distance.val.to(angstrom)

        for molnum in mobile_bound_water_group.molNums():
            water_mol = mobile_bound_water_group[molnum].molecule()
            if getMinimumDistance(ligand_mol0,water_mol) < dist:
                # we should monitor this water
                boundwater_points.append( VectorPoint(water_mol.evaluate().center()) )
    
        for molnum in mobile_free_water_group.molNums():
            #this is a mobile water, so a candidate for monitoring
            water_mol = mobile_free_water_group[molnum].molecule()
            if getMinimumDistance(ligand_mol0,water_mol) < dist:
                # we should monitor this water
                freewater_points.append( VectorPoint(water_mol.evaluate().center()) )

        system.add(mobile_bound_water_group)
        system.add(mobile_free_water_group)

        boundwater_assigner = IDAssigner(boundwater_points, mobile_bound_water_group,
                                         {"space" : Cartesian()})

        boundwater_assigner.update(system)

        freewater_assigner = IDAssigner(freewater_points, mobile_free_water_group,
                                        {"space" : Cartesian()})

        freewater_assigner.update(system)

        boundwater_nrgmon = FreeEnergyMonitor(boundwater_assigner, ligand_group0, ligand_group1)
        freewater_nrgmon = FreeEnergyMonitor(freewater_assigner, ligand_group0, ligand_group1)

        nrgmons["boundwater_nrgmon"] = boundwater_nrgmon
        nrgmons["freewater_nrgmon"] = freewater_nrgmon

    for key in list(nrgmons.keys()):
        nrgmons[key].setCoulombPower(coul_power.val)
        nrgmons[key].setShiftDelta(shift_delta.val)
        nrgmons[key].setTemperature(temperature.val)

        system.add(key, nrgmons[key], nrgmon_frequency.val)

    moves = createLSRCMoves(system)    

    # now calculate the total energy of the system - this initialises grids etc.
    # ensuring that, when we make the replicas, the maximum amount of sharing between
    # replicas occurs
    print("\nEnergies of this system at lambda == 0...")
    system.setConstant(lam, 0.0)
    printEnergies(system.energies(), sys.stdout)

    print("\nEnergies of this system at lambda == 1...")
    system.setConstant(lam, 1.0)
    printEnergies(system.energies(), sys.stdout)

    system.setConstant(lam, 0.0)

    # Now equlibrate the system
    if nequilmoves.val:
        print("\nPerforming %s moves of equilibration..." % nequilmoves.val)
        system = moves.move(system, nequilmoves.val, False)
        print("\nEnergies after equilibration...")
        printEnergies(system.energies(), sys.stdout)
        moves.clearStatistics()

    return (system, moves)


def mergeLSRC(sys0, ligand0_mol, sys1, ligand1_mol, watersys):
    
    if watersys:
        print("Merging the two ligand complexes with the water system to create the stage 1 and stage 2 systems...")
    else:
        print("Merging the two ligand complexes with a vacuum box to create the stage 1 and stage 2 systems...")

    print("\nFirst, mapping the atoms from the first ligand to the atoms of the second...")
    mapping = AtomMCSMatcher(1*second).match(ligand0_mol, PropertyMap(), ligand1_mol, PropertyMap())

    lines = []
    for key in mapping.keys():
        lines.append( "%s <=> %s" % (ligand0_mol.atom(key).name().value(), \
                                     ligand1_mol.atom(mapping[key]).name().value()) )

    lines.sort()
    print("Mapping:\n%s\n" % "\n".join(lines))

    stage1 = System("LSRC stage 1 ( A => B )")
    stage2 = System("LSRC stage 2 ( A <= B )")

    if sys0.containsProperty("reflection center"):
        if not sys1.containsProperty("reflection center"):
            print("Lack of reflection sphere in sys1 when it exists in sys0!")
            sys.exit(-1)

        if watersys:
            if not watersys.containsProperty("reflection center"):
                print("Lack of reflection sphere in the water system when it exists in sys0 and sys1!")
                sys.exit(-1)

        reflection_center0 = sys0.property("reflection center").toVector()[0]
        reflection_radius0 = float(str(sys0.property("reflection sphere radius")))

        reflection_center1 = sys1.property("reflection center").toVector()[0]
        reflection_radius1 = float(str(sys1.property("reflection sphere radius")))

        if watersys:
            reflection_center_wat = watersys.property("reflection center").toVector()[0]
            reflection_radius_wat = float(str(watersys.property("reflection sphere radius")))
        else:
            reflection_center_wat = reflection_center0
            reflection_radius_wat = reflection_radius0

        if reflection_center0 != reflection_center1 or \
           reflection_center0 != reflection_center_wat or \
           reflection_radius0 != reflection_radius1 or \
           reflection_radius0 != reflection_radius_wat:

            print("Disagreement of the reflection sphere in the boxes!")
            print("sys0: %s and %s    sys1: %s and %s   water: %s and %s" % \
                    (reflection_center0,reflection_radius0,
                     reflection_center1,reflection_radius1,
                     reflection_center_wat,reflection_radius_wat))

            sys.exit(-1)

        stage1.setProperty("reflection center", AtomCoords(CoordGroup(1,reflection_center0)))
        stage1.setProperty("reflection sphere radius", VariantProperty(reflection_radius0))

        stage2.setProperty("reflection center", AtomCoords(CoordGroup(1,reflection_center0)))
        stage2.setProperty("reflection sphere radius", VariantProperty(reflection_radius0))

    elif sys1.containsProperty("reflection center"):
        print("Lack of reflection sphere in sys0 when it exists in sys1!")
        sys.exit(-1)

    if sys0.containsProperty("average solute translation delta"):
        stage1.setProperty("average solute translation delta", \
                           sys0.property("average solute translation delta"))
        stage2.setProperty("average solute translation delta", \
                           sys0.property("average solute translation delta"))

    if sys0.containsProperty("average solute rotation delta"):
        stage1.setProperty("average solute rotation delta", \
                           sys0.property("average solute rotation delta"))
        stage2.setProperty("average solute rotation delta", \
                           sys0.property("average solute rotation delta"))

    # first create stage 1
    (stage1,moves1) = createStage(stage1, sys0, ligand0_mol, ligand1_mol, watersys, AtomResultMatcher(mapping))

    # now create stage 2
    (stage2,moves2) = createStage(stage2, sys1, ligand1_mol, ligand0_mol, watersys, AtomResultMatcher(mapping,True))

    return (stage1, moves1, stage2, moves2)


def loadWater():
    """Load the the water box used for the free leg"""

    if vacuum_calc.val:
        print("Using a vacuum box, so calculating a relative hydration free energy.")
        return None

    if os.path.exists(water_s3file.val):
        print("Restoring from Sire Streamed Save file %s..." % water_s3file.val)
        watersys = Sire.Stream.load(water_s3file.val)
    else:
        print("Loading from Amber files %s / %s..." % (water_topfile.val, water_crdfile.val))
        watersys = createSystem(water_topfile.val, water_crdfile.val)
        watersys = addFlexibility(watersys, Vector(0,0,0), reflection_radius.val)
        Sire.Stream.save(watersys, water_s3file.val)

    return watersys


def loadSystem(topfile, crdfile, s3file, ligand_name):
    """Load the solvated protein-ligand system from topfile/crdfile, saving into the s3 file 's3file'
       and locating the ligand molecule called 'ligand_name'"""

    if os.path.exists(s3file):
        print("Restoring from Sire Streamed Save file %s..." % s3file)
        system = Sire.Stream.load(s3file)
    else:
        print("Loading from Amber files %s / %s..." % (topfile,crdfile))
        # Add the name of the ligand to the list of solute molecules
        scheme = NamingScheme()

        if ligand_name:
            scheme.addSoluteResidueName(ligand_name)

        # Load up the system. This will automatically find the protein, solute, water, solvent
        # and ion molecules and assign them to different groups
        system = createSystem(topfile, crdfile, scheme)
        ligand_mol = findMolecule(system, ligand_name)

        if ligand_mol is None:
            print("Cannot find the ligand (%s) in the set of loaded molecules!" % ligand_name)
            sys.exit(-1)

        # Center the system with the ligand at (0,0,0)
        system = centerSystem(system, ligand_mol)
        ligand_mol = system[ligand_mol.number()].molecule()

        system = addFlexibility(system, Vector(0,0,0), reflection_radius.val, scheme )
        Sire.Stream.save(system, s3file)

    ligand_mol = findMolecule(system, ligand_name)

    if ligand_mol is None:
        print("Cannot find the ligand (%s) in the set of loaded molecules!" % ligand_name)
        sys.exit(-1)

    return (system, ligand_mol)


def makeRETI(system, moves):
    """This function replicates 'system' over each of the supplied lambda values
       and uses 'moves' to sample each of the replicated systems. This uses RETI
       to perform replica exchange moves across lambda"""

    lam = Symbol("lambda")

    replicas = Replicas( len(lambda_values.val) )

    replicas.setSubSystem(system)
    replicas.setSubMoves(moves)
    replicas.setNSubMoves(nsubmoves.val)
    replicas.setLambdaComponent(lam)
    replicas.setRecordAllStatistics(True)        

    seed = random_seed.val
    
    if seed is None:
        seed = RanGenerator().randInt(100000,1000000)
        print("RETI system using generated random number seed %d" % seed)
    else:
        print("RETI system using supplied random number seed %d" % seed)
    
    replicas.setGenerator( RanGenerator(seed+5) )

    for i in range(0, len(lambda_values.val)):
        # set the initial lambda value for this replica
        print("Setting replica %s to lambda %s" % (i, lambda_values.val[i]))
        replicas.setLambdaValue(i, lambda_values.val[i])

    for i in range(0, len(lambda_values.val)):
        print(lambda_values.val[i])
        print(replicas[i].subSystem().constants())

    # Now add monitors for each replica that will copy back
    nrgmons = [ "delta_g^{F}", "delta_g^{B}", "delta_g^{next}", "delta_g^{prev}",
                "delta_bound_g^{F}", "delta_bound_g^{B}",
                "delta_free_g^{F}", "delta_free_g^{B}",
                "residue_nrgmon", "boundwater_nrgmon", "freewater_nrgmon" ]

    for nrgmon in nrgmons:
        replicas.add( nrgmon, MonitorMonitor(MonitorName(nrgmon), True) )

    # now create the replica exchange moves for the replicas
    replica_moves = RepExMove()
    #replica_moves.setDisableSwaps(True)
    replica_moves.setGenerator( RanGenerator(seed+7) )

    print("\nReturning the WSRC RETI replicas and moves...")
    return (replicas, replica_moves)


def loadSystem0():
    """This function loads the ligand 0 system"""
    return loadSystem(topfile0.val, crdfile0.val, s3file0.val, ligand_name0.val)


def loadSystem1():
    """This function loads the ligand 1 system"""
    return loadSystem(topfile1.val, crdfile1.val, s3file1.val, ligand_name1.val)


def loadStage1And2():
    """This is a high level function that loads the stage 1 and 2 systems that calculates the 
       relative binding free energy of swapping bound ligand 0 with free ligand 1"""

    have_stage1 = False
    have_stage2 = False

    if os.path.exists(restart_file1.val):
        (lsrc_system1,lsrc_moves1) = Sire.Stream.load(restart_file1.val)
        have_stage1 = True

    if os.path.exists(restart_file2.val):
        (lsrc_system2,lsrc_moves2) = Sire.Stream.load(restart_file2.val)
        have_stage2 = True

    if not (have_stage1 or have_stage2):
        # need to load both systems
        (sys0, ligand0_mol) = loadSystem0()
        (sys1, ligand1_mol) = loadSystem1()
        watersys = loadWater()

        (lsrc_system1,lsrc_moves1,lsrc_system2,lsrc_moves2) = mergeLSRC(sys0,ligand0_mol, sys1,ligand1_mol, watersys)

        (lsrc_system1,lsrc_moves1) = makeRETI(lsrc_system1, lsrc_moves1)
        (lsrc_system2,lsrc_moves2) = makeRETI(lsrc_system2, lsrc_moves2)

        Sire.Stream.save( (lsrc_system1,lsrc_moves1), restart_file1.val )
        Sire.Stream.save( (lsrc_system2,lsrc_moves2), restart_file2.val )

    return (lsrc_system1,lsrc_moves1,lsrc_system2,lsrc_moves2)


def loadStage3():
    """This is a high level function that runs the stage 3 calculation that calculates the
       relative free energy of swapping bound ligand 0 with bound ligand 1"""

    print("\nRUNNING STAGE 3\nSwapping bound ligand 0 with bound ligand 1\n")

    if os.path.exists(restart_file1.val):
        (lsrc_system,lsrc_moves) = Sire.Stream.load(restart_file3.val)
    else:
        (sys0, ligand0_mol) = loadSystem0()
        (sys1, ligand1_mol) = loadSystem1()

        (lsrc_system,lsrc_moves) = mergeLSRCBound(sys0,ligand0_mol, sys1,ligand1_mol)
        Sire.Stream.save( (lsrc_system,lsrc_moves), restart_file3.val)

    return (lsrc_system,lsrc_moves)

    return


def printEnergies(nrgs, FILE):
    """This function prints all of the energies in 'nrgs' to the file 'FILE'"""
    keys = list(nrgs.keys())
    keys.sort()

    for key in keys:
        FILE.write("%s  ==  %s kcal mol-1\n" % (key, nrgs[key]))


def printComponents(comps, FILE):
    """This function prints out all of the free energy components in the passed object"""
    print("RESIDUE    TOTAL    COULOMB    LJ", file=FILE)
    for i in range(0, comps.nComponents()):
        print("%s  %s  %s  %s" % (comps.viewAt(i).residue(), \
                                  comps.integrate(i).values()[-1].y(), \
                                  comps.integrateCoulomb(i).values()[-1].y(), \
                                  comps.integrateLJ(i).values()[-1].y()), file=FILE)


def printFreeEnergy(key1, key2, key3, total, bound, free, FILE):
    """This function prints out the total, bound and free free energies"""
    print("%s   %s   %s" % (key1,key2,key3), file=FILE)
    print("%s   %s   %s" % (total.integrate().values()[-1].y(), \
                            bound.integrate().values()[-1].y(), \
                            free.integrate().values()[-1].y()), file=FILE)


def analyseLSRC3(dirname, replicas, iteration, bennetts_freenrgs, fep_freenrgs, ti_freenrgs, bound0_freenrgs, bound1_freenrgs,
                 res0_freenrgs, res1_freenrgs, bound0_water_freenrgs, bound1_water_freenrgs):
    """This function is used to perform all analysis of iteration 'it' of the passed stage 3 LSRC system"""

    # read the value of delta_lambda from the first system
    system = replicas[0].subSystem()
    delta_lambda = system.constant(Symbol("delta_lambda"))

    logfile = "%s/results_%0004d.log" % (dirname, iteration)

    FILE = open(logfile, "w")

    print("===========================", file=FILE)
    print(" Results for iteration %d" % iteration, file=FILE)
    print("===========================", file=FILE)

    print("\ndelta_lambda == %f" % delta_lambda, file=FILE)
    print("temperature == %f K\n" % replicas[0].subMoves().temperature().to(kelvin), file=FILE) 

    nreplicas = replicas.nReplicas()

    # extract all of the monitors from the replicas
    lambda_values = []

    dg_f = {}
    dg_b = {}

    dg_next = {}
    dg_prev = {}

    dg_bound0_f = {}
    dg_bound0_b = {}
    
    dg_bound1_f = {}
    dg_bound1_b = {}

    dg_residue0 = {}
    dg_residue1 = {}
    dg_boundwater0 = {}
    dg_boundwater1 = {}

    write_pdbs = (save_pdb.val) and (iteration % pdb_frequency.val == 0)

    if write_pdbs:
        print("Saving PDBs of the system at iteration %d..." % iteration)

    for i in range(0, nreplicas):
        replica = replicas[i]
        monitors = replica.monitors()
        lamval = replica.lambdaValue()
        lambda_values.append(lamval)

        if write_pdbs:
            if save_all_pdbs.val or (i == 0) or (i == nreplicas-1):
                # Save a PDB of the final configuration for the bound and free legs for each lambda value
                system = replica.subSystem()
                bound0_leg = system[MGName("bound0_leg")]
                bound1_leg = system[MGName("bound1_leg")]

                PDB().write(bound0_leg, "%s/bound0_mobile_%000006d_%.5f.pdb" % (dirname, iteration, lamval))
                PDB().write(bound1_leg, "%s/bound1_mobile_%000006d_%.5f.pdb" % (dirname, iteration, lamval))

        dg_f[lamval] = monitors[MonitorName("delta_g^{F}")][-1].accumulator()
        dg_b[lamval] = monitors[MonitorName("delta_g^{B}")][-1].accumulator()
        dg_next[lamval] = monitors[MonitorName("delta_g^{next}")][-1].accumulator()
        dg_prev[lamval] = monitors[MonitorName("delta_g^{prev}")][-1].accumulator()
        dg_bound0_f[lamval] = monitors[MonitorName("delta_bound0_g^{F}")][-1].accumulator()
        dg_bound0_b[lamval] = monitors[MonitorName("delta_bound0_g^{B}")][-1].accumulator()
        dg_bound1_f[lamval] = monitors[MonitorName("delta_bound1_g^{F}")][-1].accumulator()
        dg_bound1_b[lamval] = monitors[MonitorName("delta_bound1_g^{B}")][-1].accumulator()

        dg_residue0[lamval] = monitors[MonitorName("residue0_nrgmon")][-1]
        dg_residue1[lamval] = monitors[MonitorName("residue1_nrgmon")][-1]

        dg_boundwater0[lamval] = monitors[MonitorName("boundwater0_nrgmon")][-1]
        dg_boundwater1[lamval] = monitors[MonitorName("boundwater1_nrgmon")][-1]

    windows = copy.deepcopy(lambda_values)
    windows.sort()

    if windows[-1] != 1:
        windows.append(1)

    if windows[0] != 0:
        windows.insert(0,0)

    bennetts_freenrgs.set( iteration, windows, dg_next, dg_prev )
    fep_freenrgs.set( iteration, windows, dg_next, dg_prev )
    ti_freenrgs.set( iteration, dg_f, dg_b, delta_lambda )

    bound0_freenrgs.set( iteration, dg_bound0_f, dg_bound0_b, delta_lambda )
    bound1_freenrgs.set( iteration, dg_bound1_f, dg_bound1_b, delta_lambda )

    print("\nRELATIVE FREE ENERGY\n", file=FILE)
    printFreeEnergy("TOTAL", "BOUND_0", "BOUND_1",
                    ti_freenrgs[iteration], bound0_freenrgs[iteration], bound1_freenrgs[iteration], FILE)

    res0_freenrgs.set( iteration, dg_residue0 )
    res1_freenrgs.set( iteration, dg_residue1 )
    bound0_water_freenrgs.set( iteration, dg_boundwater0 )
    bound1_water_freenrgs.set( iteration, dg_boundwater1 )

    print("\nRESIDUE 0 FREE ENERGY COMPONENTS\n", file=FILE)
    printComponents(res0_freenrgs[iteration], FILE)

    print("\nRESIDUE 1 FREE ENERGY COMPONENTS\n", file=FILE)
    printComponents(res1_freenrgs[iteration], FILE)

    print("\nPROTEIN BOX 0 WATER FREE ENERGY COMPONENTS\n", file=FILE)
    printComponents(bound0_water_freenrgs[iteration], FILE)

    print("\nPROTEIN BOX 1 WATER FREE ENERGY COMPONENTS\n", file=FILE)
    printComponents(bound1_water_freenrgs[iteration], FILE)

    print("\n=============", file=FILE)
    print("Relative free energy for iteration %d equals %s" % (iteration, \
                        ti_freenrgs[iteration].integrate().values()[-1].y()), file=FILE)
    print("==============", file=FILE)

    FILE.close()


def analyseLSRC(dirname, replicas, iteration, bennetts_freenrgs, fep_freenrgs, ti_freenrgs, bound_freenrgs, free_freenrgs,
                res_freenrgs, bound_water_freenrgs, free_water_freenrgs):
    """This function is used to perform all analysis of iteration 'it' of the passed stage 1 or stage 2 LSRC system"""

    # read the value of delta_lambda from the first system
    system = replicas[0].subSystem()
    delta_lambda = system.constant(Symbol("delta_lambda"))

    logfile = "%s/results_%0004d.log" % (dirname, iteration)

    FILE = open(logfile, "w")

    print("===========================", file=FILE)
    print(" Results for iteration %d" % iteration, file=FILE)
    print("===========================", file=FILE)

    print("\ndelta_lambda == %f" % delta_lambda, file=FILE)
    print("temperature == %f K\n" % replicas[0].subMoves().temperature().to(kelvin), file=FILE) 

    nreplicas = replicas.nReplicas()

    # extract all of the monitors from the replicas
    lambda_values = []

    dg_f = {}
    dg_b = {}

    dg_next = {}
    dg_prev = {}

    dg_bound_f = {}
    dg_bound_b = {}
    
    dg_free_f = {}
    dg_free_b = {}

    dg_residue = {}
    dg_boundwater = {}
    dg_freewater = {}

    write_pdbs = (save_pdb.val) and (iteration % pdb_frequency.val == 0)

    if write_pdbs:
        print("Saving PDBs of the system at iteration %d..." % iteration)

    for i in range(0, nreplicas):
        replica = replicas[i]
        monitors = replica.monitors()
        lamval = replica.lambdaValue()
        lambda_values.append(lamval)

        if write_pdbs:
            if save_all_pdbs.val or (i == 0) or (i == nreplicas-1):
                # Save a PDB of the final configuration for the bound and free legs for each lambda value
                system = replica.subSystem()
                bound_leg = system[MGName("bound_leg")]
                free_leg = system[MGName("free_leg")]

                PDB().write(bound_leg, "%s/bound_mobile_%000006d_%.5f.pdb" % (dirname, iteration, lamval))
                PDB().write(free_leg, "%s/free_mobile_%000006d_%.5f.pdb" % (dirname, iteration, lamval))

        dg_f[lamval] = monitors[MonitorName("delta_g^{F}")][-1].accumulator()
        dg_b[lamval] = monitors[MonitorName("delta_g^{B}")][-1].accumulator()
        dg_next[lamval] = monitors[MonitorName("delta_g^{next}")][-1].accumulator()
        dg_prev[lamval] = monitors[MonitorName("delta_g^{prev}")][-1].accumulator()
        dg_bound_f[lamval] = monitors[MonitorName("delta_bound_g^{F}")][-1].accumulator()
        dg_bound_b[lamval] = monitors[MonitorName("delta_bound_g^{B}")][-1].accumulator()
        dg_free_f[lamval] = monitors[MonitorName("delta_free_g^{F}")][-1].accumulator()
        dg_free_b[lamval] = monitors[MonitorName("delta_free_g^{B}")][-1].accumulator()

        dg_residue[lamval] = monitors[MonitorName("residue_nrgmon")][-1]
        dg_boundwater[lamval] = monitors[MonitorName("boundwater_nrgmon")][-1]
        dg_freewater[lamval] = monitors[MonitorName("freewater_nrgmon")][-1]

    windows = copy.deepcopy(lambda_values)
    windows.sort()

    if windows[-1] != 1:
        windows.append(1)

    if windows[0] != 0:
        windows.insert(0,0)

    bennetts_freenrgs.set( iteration, windows, dg_next, dg_prev )
    fep_freenrgs.set( iteration, windows, dg_next, dg_prev )
    ti_freenrgs.set( iteration, dg_f, dg_b, delta_lambda )

    bound_freenrgs.set( iteration, dg_bound_f, dg_bound_b, delta_lambda )
    free_freenrgs.set( iteration, dg_free_f, dg_free_b, delta_lambda )

    print("\nRELATIVE FREE ENERGY\n", file=FILE)
    printFreeEnergy("TOTAL", "BOUND", "FREE",
                    ti_freenrgs[iteration], bound_freenrgs[iteration], free_freenrgs[iteration], FILE)

    res_freenrgs.set( iteration, dg_residue )
    bound_water_freenrgs.set( iteration, dg_boundwater )
    free_water_freenrgs.set( iteration, dg_freewater )

    print("\nRESIDUE FREE ENERGY COMPONENTS\n", file=FILE)
    printComponents(res_freenrgs[iteration], FILE)

    print("\nPROTEIN BOX WATER FREE ENERGY COMPONENTS\n", file=FILE)
    printComponents(bound_water_freenrgs[iteration], FILE)

    print("\nWATER BOX WATER FREE ENERGY COMPONENTS\n", file=FILE)
    printComponents(free_water_freenrgs[iteration], FILE)

    print("\n=============", file=FILE)
    print("Relative free energy for iteration %d equals %s" % (iteration, \
                        ti_freenrgs[iteration].integrate().values()[-1].y()), file=FILE)
    print("==============", file=FILE)

    FILE.close()


def tryBackup(filename):
    # save the old file to a backup
    try:
        shutil.copy(filename, "%s.bak" % filename)
    except:
        pass


def mustMakeDir(dirname):
    if not os.path.exists(dirname):
        os.makedirs(dirname)


@resolveParameters
def run():
    """This is a very high level function that does everything to run a LSRC simulation"""

    (lsrc1_system,lsrc1_moves,lsrc2_system,lsrc2_moves) = loadStage1And2()
    #(lsrc3_system,lsrc3_moves) = loadStage3()

    n1 = lsrc1_moves.nMoves()
    n2 = lsrc2_moves.nMoves()
    #n3 = lsrc3_moves.nMoves()

    nmax = max(n1,n2)
    nmin = min(n1,n2)

    print("Number of iterations to perform: %d. Number of iterations completed: %d, %d." % (nmoves.val, n1, n2))

    if nmax >= nmoves.val and nmin == nmax:
        print("All iterations complete. Simulation complete.")
        sys.exit(0)

    # make sure all of the output directories exist
    mustMakeDir(outdir.val)
    mustMakeDir("%s/stage1" % outdir.val)
    mustMakeDir("%s/stage2" % outdir.val)

    # See if we have any existing free energy statistics files...
    t = QTime()
    t.start()
    freenrgs_file1 = "%s/stage1/freenrgs.s3" % outdir.val

    if not os.path.exists(freenrgs_file1):
        bennetts_freenrgs1 = Bennetts()
        fep_freenrgs1 = FEP()
        ti_freenrgs1 = TI()
    else:
        [bennetts_freenrgs1, fep_freenrgs1, ti_freenrgs1] = Sire.Stream.load(freenrgs_file1)

    freenrg_parts_file1 = "%s/stage1/freenrg_parts.s3" % outdir.val

    if not os.path.exists(freenrg_parts_file1):
        bound_freenrgs1 = TI()
        free_freenrgs1 = TI()
    else:
        [bound_freenrgs1, free_freenrgs1] = Sire.Stream.load(freenrg_parts_file1)

    freenrg_components_file1 = "%s/stage1/freenrg_components.s3" % outdir.val

    if not os.path.exists(freenrg_components_file1):
        res_freenrgs1 = TIComponents()
        bound_water_freenrgs1 = TIComponents()
        free_water_freenrgs1 = TIComponents()
    else:
        [res_freenrgs1, bound_water_freenrgs1, free_water_freenrgs1] = Sire.Stream.load(freenrg_components_file1)

    freenrgs_file2 = "%s/stage2/freenrgs.s3" % outdir.val

    if not os.path.exists(freenrgs_file2):
        bennetts_freenrgs2 = Bennetts()
        fep_freenrgs2 = FEP()
        ti_freenrgs2 = TI()
    else:
        [bennetts_freenrgs2, fep_freenrgs2, ti_freenrgs2] = Sire.Stream.load(freenrgs_file2)

    freenrg_parts_file2 = "%s/stage2/freenrg_parts.s3" % outdir.val

    if not os.path.exists(freenrg_parts_file2):
        bound_freenrgs2 = TI()
        free_freenrgs2 = TI()
    else:
        [bound_freenrgs2, free_freenrgs2] = Sire.Stream.load(freenrg_parts_file2)

    freenrg_components_file2 = "%s/stage2/freenrg_components.s3" % outdir.val

    if not os.path.exists(freenrg_components_file2):
        res_freenrgs2 = TIComponents()
        bound_water_freenrgs2 = TIComponents()
        free_water_freenrgs2 = TIComponents()
    else:
        [res_freenrgs2, bound_water_freenrgs2, free_water_freenrgs2] = Sire.Stream.load(freenrg_components_file2)

    print("Initialising / loading the free energy files took %d ms" % t.elapsed())

    while (nmin != nmax) or (nmax < nmoves.val):
        t.start()
        sim1 = SupraSim()
        sim2 = SupraSim()

        if nmin != nmax:
            print("Catching up some of the stages: %d vs. %d" % (n1,n2))

            if n1 < nmax:
                sim1 = SupraSim.run( lsrc1_system, lsrc1_moves, 1, True )

            if n2 < nmax:
                sim2 = SupraSim.run( lsrc2_system, lsrc2_moves, 1, True )

        else:
            print("Performing iteration %d..." % (nmax+1))
            sim1 = SupraSim.run( lsrc1_system, lsrc1_moves, 1, True )
            sim2 = SupraSim.run( lsrc2_system, lsrc2_moves, 1, True )

        sim1.wait()
        sim2.wait()

        ms = t.elapsed()
        print("...iteration complete. Took %d ms" % ms)

        if nmin != nmax:
            if n1 < nmax:
                lsrc1_system = sim1.system()
                lsrc1_moves = sim1.moves()

            if n2 < nmax:
                lsrc2_system = sim2.system()
                lsrc2_moves = sim2.moves()

        else:
            lsrc1_system = sim1.system()
            lsrc1_moves = sim1.moves()
            lsrc2_system = sim2.system()
            lsrc2_moves = sim2.moves()
   
        n1 = lsrc1_moves.nMoves()
        n2 = lsrc2_moves.nMoves()

        nmax = max(n1,n2)
        nmin = min(n1,n2)
        
        if nmin == nmax:
            # we have successfully completed one iteration of each system
            iteration = nmax

            # perform analysis
            t.start()
            print("Analysing iteration %d..." % iteration)
            analyseLSRC("%s/stage1" % outdir.val,
                        lsrc1_system, iteration, bennetts_freenrgs1, fep_freenrgs1, ti_freenrgs1, bound_freenrgs1, free_freenrgs1,
                        res_freenrgs1, bound_water_freenrgs1, free_water_freenrgs1)

            lsrc1_system.clearAllStatistics()

            analyseLSRC("%s/stage2" % outdir.val,
                        lsrc2_system, iteration, bennetts_freenrgs2, fep_freenrgs2, ti_freenrgs2, bound_freenrgs2, free_freenrgs2,
                        res_freenrgs2, bound_water_freenrgs2, free_water_freenrgs2)

            lsrc2_system.clearAllStatistics()

            print("...analysis complete (took %d ms)" % t.elapsed())

            # write a restart file for all of the free energies and component - this simplifies post-run analysis
            if iteration % restart_frequency.val == 0 or iteration == nmoves.val:
                t.start()
                print("Saving the free energy analysis files from iteration %d..." % iteration)
                tryBackup(freenrgs_file1)
                tryBackup(freenrg_components_file1)
                tryBackup(freenrg_parts_file1)
                tryBackup(freenrgs_file2)
                tryBackup(freenrg_components_file2)
                tryBackup(freenrg_parts_file2)

                Sire.Stream.save( [bennetts_freenrgs1, fep_freenrgs1, ti_freenrgs1], freenrgs_file1 )
                Sire.Stream.save( [bound_freenrgs1, free_freenrgs1], freenrg_parts_file1 )
                Sire.Stream.save( [res_freenrgs1, bound_water_freenrgs1, free_water_freenrgs1], freenrg_components_file1 )

                Sire.Stream.save( [bennetts_freenrgs2, fep_freenrgs2, ti_freenrgs2], freenrgs_file2 )
                Sire.Stream.save( [bound_freenrgs2, free_freenrgs2], freenrg_parts_file2 )
                Sire.Stream.save( [res_freenrgs2, bound_water_freenrgs2, free_water_freenrgs2], freenrg_components_file2 )
          
                print("...save complete (took %d ms)" % t.elapsed())

            # write a restart file every N moves in case of crash or run out of time
            if iteration % restart_frequency.val == 0 or iteration == nmoves.val:
                t.start()
                print("Saving the restart file from iteration %d..." % iteration)

                # save the old file to a backup
                tryBackup(restart_file1.val)
                tryBackup(restart_file2.val)

                Sire.Stream.save( (lsrc1_system, lsrc1_moves), restart_file1.val )
                Sire.Stream.save( (lsrc1_system, lsrc2_moves), restart_file2.val )

                print("...save complete (took %d ms)" % t.elapsed())


    print("All iterations complete.")

    return
