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

use_fixed_ligand = Parameter("fixed ligand", False,
                             """Whether or not to completely fix the ligand during the simulation.""")

use_rot_trans_ligand = Parameter("ligand rot-trans", False,
                                 """Whether or not the ligand is free to translate and rotate.""")

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

coul_power = Parameter("coulomb power", 0,
                        """The soft-core coulomb power parameter""")

shift_delta = Parameter("shift delta", 1.2,
                        """The soft-core shift delta parameter""")

use_single_topology = Parameter("single topology", True,
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


def mergeLSRC(sys0, ligand0_mol, sys1, ligand1_mol, watersys):
    
    print("Merging the two ligand complexes with the water system to create the stage 1 and stage 2 systems...")

    stage1 = System("LSRC stage 1")
    stage2 = System("LSRC stage 2")

    if sys0.containsProperty("reflection center"):
        if not sys1.containsProperty("reflection center"):
            print("Lack of reflection sphere in sys1 when it exists in sys0!")
            sys.exit(-1)

        reflection_center = sys0.property("reflection center").toVector()[0]
        reflection_radius = float(str(sys0.property("reflection sphere radius")))

        if reflection_center != sys1.property("reflection center") or \
           reflection_center != watersys.property("reflection center") or \
           reflection_radius != float(str(sys1.property("reflection sphere radius"))) or \
           reflection_radius != float(str(watersys.property("reflection sphere radius"))):

            print("Disagreement of the reflection sphere in the boxes!")
            print("sys0: %s and %s    sys1: %s and %s   water: %s and %s" % \
                    (reflection_center,reflection_radius,
                     sys1.property("reflection center"), float(str(sys1.property("reflection sphere radius"))),
                     watersys.property("reflection center"), float(str(watersys.property("reflection sphere radius")))))

            sys.exit(-1)

        stage1.setProperty("reflection center", AtomCoords(CoordGroup(1,reflection_center)))
        stage1.setProperty("reflection sphere radius", VariantProperty(reflection_radius))

        stage2.setProperty("reflection center", AtomCoords(CoordGroup(1,reflection_center)))
        stage2.setProperty("reflection sphere radius", VariantProperty(reflection_radius))

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

    # create a molecule group for the ligand
    ligand_group = MoleculeGroup("ligand")
    ligand_group.add(ligand_mol)

    bound_leg = MoleculeGroup("bound_leg")
    free_leg = MoleculeGroup("free_leg")

    bound_leg.add(ligand_mol)
    free_leg.add(ligand_mol)

    # pull out the groups that we want from the two systems

    # create a group to hold all of the mobile water molecules in the free leg
    mobile_free_water_group = MoleculeGroup("mobile_free")
    water_mol = None
    if MGName("mobile_solvents") in water_system.mgNames():
        mols = water_system[MGName("mobile_solvents")].molecules()
        for molnum in mols.molNums():
            water_mol = mols[molnum].molecule().edit().renumber().commit()
            for j in range(0,water_mol.nResidues()):
                water_mol = water_mol.residue( ResIdx(j) ).edit() \
                                           .setProperty( PDB.parameters().pdbResidueName(), "FWT" ) \
                                           .commit().molecule()

            mobile_free_water_group.add(water_mol)

    # create a group to hold all of the fixed water molecules in the free leg
    fixed_free_water_group = MoleculeGroup("fixed_free")
    if MGName("fixed_molecules") in water_system.mgNames():
        mols = water_system[MGName("fixed_molecules")].molecules()
        for molnum in mols.molNums():
            fixed_free_water_group.add( mols[molnum].molecule().edit().renumber().commit() )

    # create a group to hold all of the fixed molecules in the bound leg
    fixed_bound_group = MoleculeGroup("fixed_bound")
    if MGName("fixed_molecules") in protein_system.mgNames():
        fixed_bound_group.add( protein_system[ MGName("fixed_molecules") ] )

    if save_pdb.val:
        # write a PDB of the fixed atoms in the bound and free legs
        if not os.path.exists(outdir.val):
            os.makedirs(outdir.val)

        PDB().write(fixed_bound_group, "%s/bound_fixed.pdb" % outdir.val)
        PDB().write(fixed_free_water_group, "%s/free_fixed.pdb" % outdir.val)

    # create a group to hold all of the mobile solute molecules in the bound leg
    mobile_bound_solutes_group = MoleculeGroup("mobile_bound_solutes")
    if MGName("mobile_solutes") in protein_system.mgNames():
        mobile_bound_solutes_group.add( protein_system[MGName("mobile_solutes")] )
        mobile_bound_solutes_group.remove(ligand_mol)
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

    # get the identity points for the ligand
    print("\nObtaining the identity points...")

    if identity_atoms.val is None:
        print("Auto-identifying the identity atoms...")
        identity_points = getIdentityPoints(ligand_mol)
    else:
        identity_points = []
        for identity_atom in identity_atoms.val:
            identity_points.append( ligand_mol.atom( AtomName(identity_atom) ) )

    print("Using identity points:")
    print(identity_points)

    if use_fixed_points.val:
        print("\nUsing fixed identity points...")
        fixed_points = []
        for point in identity_points:
            fixed_points.append( point.property("coordinates") )
        
        identity_points = fixed_points
        print(identity_points) 

    print("\nIdentifying the swap-water cluster...")
    swap_water_group = MoleculeGroup("swap water")
    mobile_free_water_group = IdentityConstraint.constrain( mobile_free_water_group, identity_points )

    # Rename the residues of the swap solvent so that they are easy
    # to find in the output PDBs
    for i in range(0,len(identity_points)):
        swap_water_mol = mobile_free_water_group.moleculeAt(i).molecule()
        mobile_free_water_group.remove(swap_water_mol)

        for j in range(0,swap_water_mol.nResidues()):
            swap_water_mol = swap_water_mol.residue( ResIdx(j) ).edit() \
                                           .setProperty( PDB.parameters().pdbResidueName(), "SWP" ) \
                                           .commit().molecule()

        swap_water_group.add(swap_water_mol)

    bound_leg.add(swap_water_group)
    bound_leg.add(mobile_bound_solvents_group)

    free_leg.add(swap_water_group)
    free_leg.add(mobile_free_water_group)

    system.add(bound_leg)
    system.add(free_leg)

    # now add in the forcefields for the system...
    print("Creating the forcefields for the WSRC system...")

    # first, group together the molecules grouped above into convenient
    # groups for the forcefields

    # group holding just the ligand
    ligand_mols = ligand_group.molecules()

    # group holding just the swap water cluster
    swap_water_mols = swap_water_group.molecules()

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
    
    # intramolecular energy of the ligand
    ligand_intraclj = IntraCLJFF("ligand:intraclj")
    ligand_intraclj = setCLJProperties(ligand_intraclj)
    ligand_intraclj.add(ligand_mols)

    ligand_intraff = InternalFF("ligand:intra")
    ligand_intraff.add(ligand_mols)

    # intramolecular energy of the swap water cluster
    swap_interclj = InterCLJFF("swap:interclj")
    swap_interclj = setCLJProperties(swap_interclj)
    swap_interclj.add(swap_water_mols)

    ###
    ### FORCEFIELDS INVOLVING THE LIGAND/CLUSTER BOUND LEG
    ###

    # forcefield holding the energy between the ligand and the mobile atoms in the
    # bound leg
    bound_ligand_mobile = InterGroupSoftCLJFF("bound:ligand-mobile")
    bound_ligand_mobile = setCLJProperties(bound_ligand_mobile)
    bound_ligand_mobile = setSoftCoreProperties(bound_ligand_mobile)

    bound_ligand_mobile.add(ligand_mols, MGIdx(0))
    bound_ligand_mobile.add(mobile_bound_mols, MGIdx(1))

    # forcefield holding the energy between the swap water cluster and the mobile
    # atoms in the bound leg
    bound_swap_mobile = InterGroupSoftCLJFF("bound:swap-mobile")
    bound_swap_mobile = setCLJProperties(bound_swap_mobile)
    bound_swap_mobile = setSoftCoreProperties(bound_swap_mobile)

    bound_swap_mobile.add(swap_water_mols, MGIdx(0))
    bound_swap_mobile.add(mobile_bound_mols, MGIdx(1))

    # Whether or not to disable the grid and calculate all energies atomisticly
    if disable_grid:
        # we need to renumber all of the fixed molecules so that they don't clash
        # with the mobile molecules
        print("Renumbering fixed molecules...")
        fixed_bound_group = renumberMolecules(fixed_bound_group)
        fixed_free_group = renumberMolecules(fixed_free_group)

    # forcefield holding the energy between the ligand and the fixed atoms in the bound leg
    if disable_grid:
        bound_ligand_fixed = InterGroupCLJFF("bound:ligand-fixed")
        bound_ligand_fixed = setCLJProperties(bound_ligand_fixed)
        bound_ligand_fixed = setFakeGridProperties(bound_ligand_fixed)

        bound_ligand_fixed.add(ligand_mols, MGIdx(0))
        bound_ligand_fixed.add(fixed_bound_group, MGIdx(1))
    else:
        bound_ligand_fixed = GridFF("bound:ligand-fixed")
        bound_ligand_fixed = setCLJProperties(bound_ligand_fixed)
        bound_ligand_fixed = setGridProperties(bound_ligand_fixed)

        bound_ligand_fixed.add(ligand_mols, MGIdx(0))
        bound_ligand_fixed.addFixedAtoms( fixed_bound_group )

    # forcefield holding the energy between the swap water cluster and the
    # fixed atoms in the bound leg
    if disable_grid:
        bound_swap_fixed = InterGroupCLJFF("bound:swap-fixed")
        bound_swap_fixed = setCLJProperties(bound_swap_fixed)
        bound_swap_fixed = setFakeGridProperties(bound_swap_fixed)

        bound_swap_fixed.add(swap_water_mols, MGIdx(0))
        bound_swap_fixed.add( fixed_bound_group, MGIdx(1) )
    else:
        bound_swap_fixed = GridFF("bound:swap-fixed")
        bound_swap_fixed = setCLJProperties(bound_swap_fixed)
        # The swap water cluster is more mobile, so needs a bigger grid buffer
        bound_swap_fixed = setGridProperties(bound_swap_fixed, 6*angstrom)

        bound_swap_fixed.add(swap_water_mols, MGIdx(0))
        bound_swap_fixed.addFixedAtoms(fixed_bound_group)

    ###
    ### FORCEFIELDS INVOLVING THE LIGAND/CLUSTER FREE LEG
    ###

    # forcefield holding the energy between the ligand and the mobile atoms
    # in the free leg
    free_ligand_mobile = InterGroupSoftCLJFF("free:ligand-mobile")
    free_ligand_mobile = setCLJProperties(free_ligand_mobile)
    free_ligand_mobile = setSoftCoreProperties(free_ligand_mobile)

    free_ligand_mobile.add(ligand_mols, MGIdx(0))
    free_ligand_mobile.add(mobile_free_mols, MGIdx(1))

    # forcefield holding the energy between the swap water cluster and the
    # mobile atoms of the free leg
    free_swap_mobile = InterGroupSoftCLJFF("free:swap-mobile")
    free_swap_mobile = setCLJProperties(free_swap_mobile)
    free_swap_mobile = setSoftCoreProperties(free_swap_mobile)

    free_swap_mobile.add(swap_water_mols, MGIdx(0))
    free_swap_mobile.add(mobile_free_mols, MGIdx(1))

    # forcefield holding the energy between the ligand and the fixed atoms
    # in the free leg
    if disable_grid.val:
        free_ligand_fixed = InterGroupCLJFF("free:ligand_fixed")
        free_ligand_fixed = setCLJProperties(free_ligand_fixed)
        free_ligand_fixed = setFakeGridProperties(free_ligand_fixed)

        free_ligand_fixed.add(ligand_mols, MGIdx(0))
        free_ligand_fixed.add(fixed_free_group, MGIdx(1))
    else:
        free_ligand_fixed = GridFF("free:ligand-fixed")
        free_ligand_fixed = setCLJProperties(free_ligand_fixed)
        free_ligand_fixed = setGridProperties(free_ligand_fixed)

        free_ligand_fixed.add(ligand_mols, MGIdx(0))
        free_ligand_fixed.addFixedAtoms(fixed_free_group)

    # forcefield holding the energy between the swap water cluster and the 
    # fixed atoms in the free leg
    if disable_grid.val:
        free_swap_fixed = InterGroupCLJFF("free:swap-fixed")
        free_swap_fixed = setCLJProperties(free_swap_fixed)
        free_swap_fixed = setFakeGridProperties(free_swap_fixed)

        free_swap_fixed.add(swap_water_mols, MGIdx(0))
        free_swap_fixed.add(fixed_free_group, MGIdx(1))
    else:
        free_swap_fixed = GridFF("free:swap-fixed")
        free_swap_fixed = setCLJProperties(free_swap_fixed)
        # The swap water cluster is more mobile so needs a bigger grid buffer
        free_swap_fixed = setGridProperties(free_swap_fixed, 6*angstrom)

        free_swap_fixed.add(swap_water_mols, MGIdx(0))
        free_swap_fixed.addFixedAtoms(fixed_free_group)

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

    bound_ligand_fixed_nrg = bound_ligand_fixed.components().total()
    free_ligand_fixed_nrg = free_ligand_fixed.components().total()
    bound_swap_fixed_nrg = bound_swap_fixed.components().total()
    free_swap_fixed_nrg = free_swap_fixed.components().total()

    ligand_int_nrg_sym = Symbol("E_{ligand:internal}")
    ligand_int_nrg = ligand_intraclj.components().total() + \
                     ligand_intraff.components().total()

    ligand_int_nrg_f_sym = Symbol("E_{ligand:internal_{f}}")
    ligand_int_nrg_f = ligand_intraclj.components().total() + \
                       ligand_intraff.components().total()

    ligand_int_nrg_b_sym = Symbol("E_{ligand:internal_{b}}")
    ligand_int_nrg_b = ligand_intraclj.components().total() + \
                       ligand_intraff.components().total()

    ligand_int_nrg_next_sym = Symbol("E_{ligand:internal_{next}}")
    ligand_int_nrg_next = ligand_intraclj.components().total() + \
                          ligand_intraff.components().total()

    ligand_int_nrg_prev_sym = Symbol("E_{ligand:internal_{prev}}")
    ligand_int_nrg_prev = ligand_intraclj.components().total() + \
                          ligand_intraff.components().total()

    ligand_bound_nrg_sym = Symbol("E_{ligand:bound}")

    ligand_bound_nrg = bound_ligand_mobile.components().total(0) + \
                       bound_ligand_fixed_nrg

    ligand_bound_nrg_f_sym = Symbol("E_{ligand:bound_{f}}")
    ligand_bound_nrg_f = bound_ligand_mobile.components().total(1) + \
                         bound_ligand_fixed_nrg

    ligand_bound_nrg_b_sym = Symbol("E_{ligand:bound_{b}}")
    ligand_bound_nrg_b = bound_ligand_mobile.components().total(2) + \
                         bound_ligand_fixed_nrg

    ligand_bound_nrg_next_sym = Symbol("E_{ligand:bound_{next}}")
    ligand_bound_nrg_next = bound_ligand_mobile.components().total(3) + \
                            bound_ligand_fixed_nrg

    ligand_bound_nrg_prev_sym = Symbol("E_{ligand:bound_{prev}}")
    ligand_bound_nrg_prev = bound_ligand_mobile.components().total(4) + \
                            bound_ligand_fixed_nrg

    ligand_free_nrg_sym = Symbol("E_{ligand:free}")
    ligand_free_nrg = free_ligand_mobile.components().total(0) + \
                      free_ligand_fixed_nrg

    ligand_free_nrg_f_sym = Symbol("E_{ligand:free_{f}}")
    ligand_free_nrg_f = free_ligand_mobile.components().total(1) + \
                        free_ligand_fixed_nrg

    ligand_free_nrg_b_sym = Symbol("E_{ligand:free_{b}}")
    ligand_free_nrg_b = free_ligand_mobile.components().total(2) + \
                        free_ligand_fixed_nrg

    ligand_free_nrg_next_sym = Symbol("E_{ligand:free_{next}}")
    ligand_free_nrg_next = free_ligand_mobile.components().total(3) + \
                           free_ligand_fixed_nrg

    ligand_free_nrg_prev_sym = Symbol("E_{ligand:free_{prev}}")
    ligand_free_nrg_prev = free_ligand_mobile.components().total(4) + \
                           free_ligand_fixed_nrg

    lam = Symbol("lambda")
    lam_f = Symbol("lambda_{f}")
    lam_b = Symbol("lambda_{b}")
    lam_next = Symbol("lambda_{next}")
    lam_prev = Symbol("lambda_{prev}")

    S_sym = Symbol("S")
    S_scl = S_sym - 4*(S_sym-1)*(lam-0.5)**2
    S_scl_f = S_sym - 4*(S_sym-1)*(lam_f-0.5)**2
    S_scl_b = S_sym - 4*(S_sym-1)*(lam_b-0.5)**2
    S_scl_next = S_sym - 4*(S_sym-1)*(lam_next-0.5)**2
    S_scl_prev = S_sym - 4*(S_sym-1)*(lam_prev-0.5)**2

    swap_int_nrg_sym = Symbol("E_{swap:internal}")
    swap_int_nrg = ((S_scl) * swap_interclj.components().coulomb()) + \
                              swap_interclj.components().lj()

    swap_int_nrg_f_sym = Symbol("E_{swap:internal_{f}}")
    swap_int_nrg_f = ((S_scl_f) * swap_interclj.components().coulomb()) + \
                              swap_interclj.components().lj()

    swap_int_nrg_b_sym = Symbol("E_{swap:internal_{b}}")
    swap_int_nrg_b = ((S_scl_b) * swap_interclj.components().coulomb()) + \
                              swap_interclj.components().lj()

    swap_int_nrg_next_sym = Symbol("E_{swap:internal_{next}}")
    swap_int_nrg_next = ((S_scl_next) * swap_interclj.components().coulomb()) + \
                              swap_interclj.components().lj()

    swap_int_nrg_prev_sym = Symbol("E_{swap:internal_{prev}}")
    swap_int_nrg_prev = ((S_scl_prev) * swap_interclj.components().coulomb()) + \
                              swap_interclj.components().lj()

    swap_bound_nrg_sym = Symbol("E_{swap:bound}")
    swap_bound_nrg = bound_swap_mobile.components().total(0) + \
                     bound_swap_fixed_nrg

    swap_bound_nrg_f_sym = Symbol("E_{swap:bound_{f}}")
    swap_bound_nrg_f = bound_swap_mobile.components().total(1) + \
                       bound_swap_fixed_nrg

    swap_bound_nrg_b_sym = Symbol("E_{swap:bound_{b}}")
    swap_bound_nrg_b = bound_swap_mobile.components().total(2) + \
                       bound_swap_fixed_nrg

    swap_bound_nrg_next_sym = Symbol("E_{swap:bound_{next}}")
    swap_bound_nrg_next = bound_swap_mobile.components().total(3) + \
                          bound_swap_fixed_nrg

    swap_bound_nrg_prev_sym = Symbol("E_{swap:bound_{prev}}")
    swap_bound_nrg_prev = bound_swap_mobile.components().total(4) + \
                          bound_swap_fixed_nrg

    swap_free_nrg_sym = Symbol("E_{swap:free}")
    swap_free_nrg = free_swap_mobile.components().total(0) + \
                    free_swap_fixed_nrg

    swap_free_nrg_f_sym = Symbol("E_{swap:free_{f}}")
    swap_free_nrg_f = free_swap_mobile.components().total(1) + \
                      free_swap_fixed_nrg

    swap_free_nrg_b_sym = Symbol("E_{swap:free_{b}}")
    swap_free_nrg_b = free_swap_mobile.components().total(2) + \
                      free_swap_fixed_nrg

    swap_free_nrg_next_sym = Symbol("E_{swap:free_{next}}")
    swap_free_nrg_next = free_swap_mobile.components().total(3) + \
                         free_swap_fixed_nrg

    swap_free_nrg_prev_sym = Symbol("E_{swap:free_{prev}}")
    swap_free_nrg_prev = free_swap_mobile.components().total(4) + \
                         free_swap_fixed_nrg

    system.add(ligand_intraclj)
    system.add(ligand_intraff)
    system.add(swap_interclj)
    system.add(bound_ligand_mobile)
    system.add(bound_swap_mobile)
    system.add(free_ligand_mobile)
    system.add(free_swap_mobile)
    system.add(bound_ligand_fixed)
    system.add(bound_swap_fixed)
    system.add(free_ligand_fixed)
    system.add(free_swap_fixed)

    system.setConstant(lam, 0.0)
    system.setConstant(lam_f, 0.0)
    system.setConstant(lam_b, 0.0)
    system.setConstant(lam_next, 0.0)
    system.setConstant(lam_prev, 0.0)

    system.setComponent(S_sym, soften_water.val)

    system.setComponent(ligand_int_nrg_sym, ligand_int_nrg)
    system.setComponent(ligand_int_nrg_f_sym, ligand_int_nrg_f)
    system.setComponent(ligand_int_nrg_b_sym, ligand_int_nrg_b)
    system.setComponent(ligand_int_nrg_next_sym, ligand_int_nrg_next)
    system.setComponent(ligand_int_nrg_prev_sym, ligand_int_nrg_prev)

    system.setComponent(ligand_bound_nrg_sym, ligand_bound_nrg)
    system.setComponent(ligand_bound_nrg_f_sym, ligand_bound_nrg_f)
    system.setComponent(ligand_bound_nrg_b_sym, ligand_bound_nrg_b)
    system.setComponent(ligand_bound_nrg_next_sym, ligand_bound_nrg_next)
    system.setComponent(ligand_bound_nrg_prev_sym, ligand_bound_nrg_prev)

    system.setComponent(ligand_free_nrg_sym, ligand_free_nrg)
    system.setComponent(ligand_free_nrg_f_sym, ligand_free_nrg_f)
    system.setComponent(ligand_free_nrg_b_sym, ligand_free_nrg_b)
    system.setComponent(ligand_free_nrg_next_sym, ligand_free_nrg_next)
    system.setComponent(ligand_free_nrg_prev_sym, ligand_free_nrg_prev)

    system.setComponent(swap_int_nrg_sym, swap_int_nrg)
    system.setComponent(swap_int_nrg_f_sym, swap_int_nrg_f)
    system.setComponent(swap_int_nrg_b_sym, swap_int_nrg_b)
    system.setComponent(swap_int_nrg_next_sym, swap_int_nrg_next)
    system.setComponent(swap_int_nrg_prev_sym, swap_int_nrg_prev)

    system.setComponent(swap_bound_nrg_sym, swap_bound_nrg)
    system.setComponent(swap_bound_nrg_f_sym, swap_bound_nrg_f)
    system.setComponent(swap_bound_nrg_b_sym, swap_bound_nrg_b)
    system.setComponent(swap_bound_nrg_next_sym, swap_bound_nrg_next)
    system.setComponent(swap_bound_nrg_prev_sym, swap_bound_nrg_prev)

    system.setComponent(swap_free_nrg_sym, swap_free_nrg)
    system.setComponent(swap_free_nrg_f_sym, swap_free_nrg_f)
    system.setComponent(swap_free_nrg_b_sym, swap_free_nrg_b)
    system.setComponent(swap_free_nrg_next_sym, swap_free_nrg_next)
    system.setComponent(swap_free_nrg_prev_sym, swap_free_nrg_prev)

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
    bound_nrg = ((1-lam) * ligand_bound_nrg_sym) + (lam * swap_bound_nrg_sym)

    bound_nrg_f_sym = Symbol("E_{bound_{f}}")
    bound_nrg_f = ((1-lam_f) * ligand_bound_nrg_f_sym) + (lam_f * swap_bound_nrg_f_sym)

    bound_nrg_b_sym = Symbol("E_{bound_{b}}")
    bound_nrg_b = ((1-lam_b) * ligand_bound_nrg_b_sym) + (lam_b * swap_bound_nrg_b_sym)

    bound_nrg_next_sym = Symbol("E_{bound_{next}}")
    bound_nrg_next = ((1-lam_next) * ligand_bound_nrg_next_sym) + (lam_next * swap_bound_nrg_next_sym)

    bound_nrg_prev_sym = Symbol("E_{bound_{prev}}")
    bound_nrg_prev = ((1-lam_prev) * ligand_bound_nrg_prev_sym) + (lam_prev * swap_bound_nrg_prev_sym)
    
    free_nrg_sym = Symbol("E_{free}")
    free_nrg = (lam * ligand_free_nrg_sym) + ((1-lam) * swap_free_nrg_sym)

    free_nrg_f_sym = Symbol("E_{free_{f}}")
    free_nrg_f = (lam_f * ligand_free_nrg_f_sym) + ((1-lam_f) * swap_free_nrg_f_sym)

    free_nrg_b_sym = Symbol("E_{free_{b}}")
    free_nrg_b = (lam_b * ligand_free_nrg_b_sym) + ((1-lam_b) * swap_free_nrg_b_sym)

    free_nrg_next_sym = Symbol("E_{free_{next}}")
    free_nrg_next = (lam_next * ligand_free_nrg_next_sym) + ((1-lam_next) * swap_free_nrg_next_sym)

    free_nrg_prev_sym = Symbol("E_{free_{prev}}")
    free_nrg_prev = (lam_prev * ligand_free_nrg_prev_sym) + ((1-lam_prev) * swap_free_nrg_prev_sym)

    box_nrg_sym = Symbol("E_{box}")
    box_nrg = bound_bound_nrg_sym + free_free_nrg_sym + ligand_int_nrg_sym + swap_int_nrg_sym

    box_nrg_f_sym = Symbol("E_{box_{f}}")
    box_nrg_f = bound_bound_nrg_sym + free_free_nrg_sym + ligand_int_nrg_f_sym + swap_int_nrg_f_sym

    box_nrg_b_sym = Symbol("E_{box_{b}}")
    box_nrg_b = bound_bound_nrg_sym + free_free_nrg_sym + ligand_int_nrg_b_sym + swap_int_nrg_b_sym

    box_nrg_next_sym = Symbol("E_{box_{next}}")
    box_nrg_next = bound_bound_nrg_sym + free_free_nrg_sym + ligand_int_nrg_next_sym + swap_int_nrg_next_sym

    box_nrg_prev_sym = Symbol("E_{box_{prev}}")
    box_nrg_prev = bound_bound_nrg_sym + free_free_nrg_sym + ligand_int_nrg_prev_sym + swap_int_nrg_prev_sym

    total_nrg_sym = system.totalComponent()
    total_nrg = bound_nrg_sym + free_nrg_sym + box_nrg_sym

    total_nrg_f_sym = Symbol("E_{total_{f}}")
    total_nrg_f = bound_nrg_f_sym + free_nrg_f_sym + box_nrg_f_sym

    total_nrg_b_sym = Symbol("E_{total_{b}}")
    total_nrg_b = bound_nrg_b_sym + free_nrg_b_sym + box_nrg_b_sym

    total_nrg_next_sym = Symbol("E_{total_{next}}")
    total_nrg_next = bound_nrg_next_sym + free_nrg_next_sym + box_nrg_next_sym

    total_nrg_prev_sym = Symbol("E_{total_{prev}}")
    total_nrg_prev = bound_nrg_prev_sym + free_nrg_prev_sym + box_nrg_prev_sym

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
    system.setComponent(box_nrg_f_sym, box_nrg_f)
    system.setComponent(box_nrg_b_sym, box_nrg_b)
    system.setComponent(box_nrg_next_sym, box_nrg_next)
    system.setComponent(box_nrg_prev_sym, box_nrg_prev)

    system.setComponent(total_nrg_sym, total_nrg)
    system.setComponent(total_nrg_f_sym, total_nrg_f)
    system.setComponent(total_nrg_b_sym, total_nrg_b)
    system.setComponent(total_nrg_next_sym, total_nrg_next)
    system.setComponent(total_nrg_prev_sym, total_nrg_prev)

    system.setComponent( Symbol("delta_nrg^{F}"), (total_nrg_f_sym - total_nrg_sym) )
    system.setComponent( Symbol("delta_nrg^{B}"), (total_nrg_sym - total_nrg_b_sym) )
    system.setComponent( Symbol("delta_nrg^{next}"), (total_nrg_next_sym - total_nrg_sym) )
    system.setComponent( Symbol("delta_nrg^{prev}"), (total_nrg_sym - total_nrg_prev_sym) )

    system.setComponent( Symbol("delta_bound_nrg^{F}"), (bound_nrg_f_sym - bound_nrg_sym) )
    system.setComponent( Symbol("delta_bound_nrg^{B}"), (bound_nrg_sym - bound_nrg_b_sym) )
    system.setComponent( Symbol("delta_free_nrg^{F}"), (free_nrg_f_sym - free_nrg_sym) )
    system.setComponent( Symbol("delta_free_nrg^{B}"), (free_nrg_sym - free_nrg_b_sym) )

    # Now add constraints. These are used to keep the identity of the 
    # swap water, to keep all lambda values between 0 and 1, and to
    # map the alpha values of the softcore forcefields to lambda
    print("\nCreating WSRC system constraints...\n")

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
    system.add( PropertyConstraint( "alpha0", FFName("free:swap-mobile"), alpha_scale.val * lam ) )
    system.add( PropertyConstraint( "alpha0", FFName("bound:swap-mobile"), alpha_scale.val * (1 - lam) ) )

    system.add( PropertyConstraint( "alpha0", FFName("bound:ligand-mobile"), alpha_scale.val * lam ) )
    system.add( PropertyConstraint( "alpha0", FFName("free:ligand-mobile"), alpha_scale.val * (1 - lam) ) )

    # Now the forwards perturbed state (alpha1)
    system.add( PropertyConstraint( "alpha1", FFName("free:swap-mobile"),  alpha_scale.val * lam_f ) )
    system.add( PropertyConstraint( "alpha1", FFName("bound:swap-mobile"),  alpha_scale.val * (1 - lam_f) ) )

    system.add( PropertyConstraint( "alpha1", FFName("bound:ligand-mobile"),  alpha_scale.val * lam_f ) )
    system.add( PropertyConstraint( "alpha1", FFName("free:ligand-mobile"),  alpha_scale.val * (1 - lam_f) ) )

    # Now the backwards perturbed state (alpha2)
    system.add( PropertyConstraint( "alpha2", FFName("free:swap-mobile"),  alpha_scale.val * lam_b ) )
    system.add( PropertyConstraint( "alpha2", FFName("bound:swap-mobile"),  alpha_scale.val * (1 - lam_b) ) )

    system.add( PropertyConstraint( "alpha2", FFName("bound:ligand-mobile"),  alpha_scale.val * lam_b ) )
    system.add( PropertyConstraint( "alpha2", FFName("free:ligand-mobile"),  alpha_scale.val * (1 - lam_b) ) )

    # Now the next window perturbed state (alpha3)
    system.add( PropertyConstraint( "alpha3", FFName("free:swap-mobile"),  alpha_scale.val * lam_next ) )
    system.add( PropertyConstraint( "alpha3", FFName("bound:swap-mobile"),  alpha_scale.val * (1 - lam_next) ) )

    system.add( PropertyConstraint( "alpha3", FFName("bound:ligand-mobile"),  alpha_scale.val * lam_next ) )
    system.add( PropertyConstraint( "alpha3", FFName("free:ligand-mobile"),  alpha_scale.val * (1 - lam_next) ) )

    # Now the previous window perturbed state (alpha4)
    system.add( PropertyConstraint( "alpha4", FFName("free:swap-mobile"),  alpha_scale.val * lam_prev ) )
    system.add( PropertyConstraint( "alpha4", FFName("bound:swap-mobile"),  alpha_scale.val * (1 - lam_prev) ) )

    system.add( PropertyConstraint( "alpha4", FFName("bound:ligand-mobile"),  alpha_scale.val * lam_prev ) )
    system.add( PropertyConstraint( "alpha4", FFName("free:ligand-mobile"),  alpha_scale.val * (1 - lam_prev) ) )

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
    mobile_ligand.add(ligand_mol)

    system.add( mobile_ligand )

    # The swap water cluster is moved in its own group
    mobile_swap = MoleculeGroup("mobile_swap_water")
    mobile_swap.add(swap_water_group.molecules())

    system.add( mobile_swap )

    print("Adding the identity constraint...")

    # Now add the constraint that keeps the identities of the
    # swap molecules. The swap molecules are chosen from all available mobile
    # water molecules. We need to build a group of all mobile water molecules that
    # are waters (as opposed to ions, as other molecules may be in mobile_solvent)
    mobile_water = MoleculeGroup("mobile_water")

    # The mobile water *must* contain the swap waters, so that they can be swapped
    mobile_water.add(swap_water_group)
    mobile_water.add(mobile_free_water_group)

    if waterbox_only.val:
        print("Choosing water molecules only from the free water box.")
    else:
        print("Choosing swap waters from both the protein box and water box.")
        mobile_water.add(mobile_bound_water_group)

    print("The number of candidates for the swap water equals: %d" % mobile_water.nMolecules())

    system.add(mobile_water)
    system.add( IdentityConstraint(identity_points, mobile_water, { "space" : Cartesian() } ) )

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
                                                                   dlam * binwidth.val) ) )

    system.add( "delta_g^{next}", MonitorComponent( Symbol("delta_nrg^{next}"),
                                                    BennettsFreeEnergyAverage(temperature.val,
                                                                              0.1 * binwidth.val) ) )

    system.add( "delta_g^{prev}", MonitorComponent( Symbol("delta_nrg^{prev}"),
                                                    BennettsFreeEnergyAverage(temperature.val,
                                                                              0.1 * binwidth.val) ) )
    
    system.add( "delta_bound_g^{F}", MonitorComponent( Symbol("delta_bound_nrg^{F}"),
                                                       FreeEnergyAverage(temperature.val,
                                                                         dlam * binwidth.val) ) )
    system.add( "delta_bound_g^{B}", MonitorComponent( Symbol("delta_bound_nrg^{B}"),
                                                       FreeEnergyAverage(temperature.val,
                                                                         dlam * binwidth.val) ) )
    system.add( "delta_free_g^{F}", MonitorComponent( Symbol("delta_free_nrg^{F}"),
                                                      FreeEnergyAverage(temperature.val,
                                                                        dlam * binwidth.val) ) )
    system.add( "delta_free_g^{B}", MonitorComponent( Symbol("delta_free_nrg^{B}"),
                                                      FreeEnergyAverage(temperature.val,
                                                                        dlam * binwidth.val) ) )

    # we will monitor the average energy between the swap cluster/ligand and each
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

    residue_nrgmon = FreeEnergyMonitor(monitor_prosol, ligand_group, mobile_swap)

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
            if getMinimumDistance(ligand_mol,water_mol) < dist:
                # we should monitor this water
                boundwater_points.append( VectorPoint(water_mol.evaluate().center()) )
    
        for molnum in mobile_free_water_group.molNums():
            #this is a mobile water, so a candidate for monitoring
            water_mol = mobile_free_water_group[molnum].molecule()
            if getMinimumDistance(ligand_mol,water_mol) < dist:
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

        boundwater_nrgmon = FreeEnergyMonitor(boundwater_assigner, ligand_group, mobile_swap)
        freewater_nrgmon = FreeEnergyMonitor(freewater_assigner, ligand_group, mobile_swap)

        nrgmons["boundwater_nrgmon"] = boundwater_nrgmon
        nrgmons["freewater_nrgmon"] = freewater_nrgmon

    for key in list(nrgmons.keys()):
        nrgmons[key].setCoulombPower(coul_power.val)
        nrgmons[key].setShiftDelta(shift_delta.val)
        nrgmons[key].setTemperature(temperature.val)

        system.add(key, nrgmons[key], nrgmon_frequency.val)

    return system


def loadWater():
    """Load the the water box used for the free leg"""

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
    (lsrc3_system,lsrc3_moves) = loadStage3()

    n1 = lsrc1_moves.nMoves()
    n2 = lsrc2_moves.nMoves()
    n3 = lsrc3_moves.nMoves()

    nmax = max(n1,n2,n3)
    nmin = min(n1,n2,n3)

    print("Number of iterations to perform: %d. Number of iterations completed: %d, %d, %d." % (nmoves.val, n1, n2, n3))

    if nmax >= nmoves.val and nmin == nmax:
        print("All iterations complete. Simulation complete.")
        sys.exit(0)

    # make sure all of the output directories exist
    mustMakeDir(outdir.val)
    mustMakeDir("%s/stage1" % outdir.val)
    mustMakeDir("%s/stage2" % outdir.val)
    mustMakeDir("%s/stage3" % outdir.val)

    # See if we have any existing free energy statistics files...
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

    freenrgs_file3 = "%s/stage3/freenrgs.s3" % outdir.val

    if not os.path.exists(freenrgs_file3):
        bennetts_freenrgs3 = Bennetts()
        fep_freenrgs3 = FEP()
        ti_freenrgs3 = TI()
    else:
        [bennetts_freenrgs3, fep_freenrgs3, ti_freenrgs3] = Sire.Stream.load(freenrgs_file3)

    freenrg_parts_file3 = "%s/stage3/freenrg_parts.s3" % outdir.val

    if not os.path.exists(freenrg_parts_file3):
        bound0_freenrgs3 = TI()
        bound1_freenrgs3 = TI()
    else:
        [bound0_freenrgs3, bound1_freenrgs3] = Sire.Stream.load(freenrg_parts_file3)

    freenrg_components_file3 = "%s/stage3/freenrg_components.s3" % outdir.val

    if not os.path.exists(freenrg_components_file3):
        res0_freenrgs3 = TIComponents()
        res1_freenrgs3 = TIComponents()
        bound0_water_freenrgs3 = TIComponents()
        bound1_water_freenrgs3 = TIComponents()
    else:
        [res0_freenrgs3, res1_freenrgs3, bound0_water_freenrgs1, bound1_water_freenrgs1] = Sire.Stream.load(freenrg_components_file3)

    print("Initialising / loading the free energy files took %d ms" % t.elapsed())

    while (nmin != nmax) or (nmax < nmoves.val):
        t.start()
        sim1 = SupraSim()
        sim2 = SupraSim()
        sim3 = SupraSim()

        if nmin != nmax:
            print("Catching up some of the stages: %d vs. %d vs. %d" % (n1,n2,n3))

            if n1 < nmax:
                sim1 = SupraSim.run( lsrc_system1, lsrc_moves1, 1, True )

            if n2 < nmax:
                sim2 = SupraSim.run( lsrc_system2, lsrc_moves2, 1, True )

            if n3 < nmax:
                sim3 = SupraSim.run( lsrc_system3, lsrc_moves3, 1, True )

        else:
            print("Performing iteration %d..." % (nmax+1))
            sim1 = SupraSim.run( lsrc_system1, lsrc_moves1, 1, True )
            sim2 = SupraSim.run( lsrc_system2, lsrc_moves2, 1, True )
            sim3 = SupraSim.run( lsrc_system3, lsrc_moves3, 1, True )

        sim1.wait()
        sim2.wait()
        sim3.wait()

        ms = t.elapsed()
        print("...iteration complete. Took %d ms" % ms)

        if nmin != nmax:
            if n1 < nmax:
                lsrc1_system = sim1.system()
                lsrc1_moves = sim1.moves()

            if n2 < nmax:
                lsrc2_system = sim2.system()
                lsrc2_moves = sim2.moves()

            if n3 < nmax:
                lsrc3_system = sim3.system()
                lsrc3_moves = sim3.moves()

        else:
            lsrc1_system = sim1.system()
            lsrc1_moves = sim1.moves()
            lsrc2_system = sim2.system()
            lsrc2_moves = sim2.moves()
            lsrc3_system = sim3.system()
            lsrc3_moves = sim3.moves()
   
        n1 = lsrc1_moves.nMoves()
        n2 = lsrc2_moves.nMoves()
        n3 = lsrc3_moves.nMoves()

        nmax = max(n1,n2,n3)
        nmin = min(n1,n2,n3)
        
        if nmin == nmax:
            # we have successfully completed one iteration of each system
            iteration = nmax

            # perform analysis
            t.start()
            print("Analysing iteration %d..." % i)
            analyseLSRC("%s/stage1" % outdir.val,
                        lsrc_system1, iteration, bennetts_freenrgs1, fep_freenrgs1, ti_freenrgs1, bound_freenrgs1, free_freenrgs1,
                        res_freenrgs1, bound_water_freenrgs1, free_water_freenrgs1)

            lsrc_system1.clearAllStatistics()

            analyseLSRC("%s/stage2" % outdir.val,
                        lsrc_system2, iteration, bennetts_freenrgs2, fep_freenrgs2, ti_freenrgs2, bound_freenrgs2, free_freenrgs2,
                        res_freenrgs2, bound_water_freenrgs2, free_water_freenrgs2)

            lsrc_system2.clearAllStatistics()

            analyseLSRC3("%s/stage3" % outdir.val,
                         lsrc_system3, iteration, bennetts_freenrgs3, fep_freenrgs3, ti_freenrgs3, bound0_freenrgs3, bound1_freenrgs3,
                         res0_freenrgs3, res1_freenrgs3, bound0_water_freenrgs3, bound1_water_freenrgs3)

            lsrc_system3.clearAllStatistics()

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
                tryBackup(freenrgs_file3)
                tryBackup(freenrg_components_file3)
                tryBackup(freenrg_parts_file3)

                Sire.Stream.save( [bennetts_freenrgs1, fep_freenrgs1, ti_freenrgs1], freenrgs_file1 )
                Sire.Stream.save( [bound_freenrgs1, free_freenrgs1], freenrg_parts_file1 )
                Sire.Stream.save( [res_freenrgs1, bound_water_freenrgs1, free_water_freenrgs1], freenrg_components_file1 )

                Sire.Stream.save( [bennetts_freenrgs2, fep_freenrgs2, ti_freenrgs2], freenrgs_file2 )
                Sire.Stream.save( [bound_freenrgs2, free_freenrgs2], freenrg_parts_file2 )
                Sire.Stream.save( [res_freenrgs2, bound_water_freenrgs2, free_water_freenrgs2], freenrg_components_file2 )

                Sire.Stream.save( [bennetts_freenrgs3, fep_freenrgs3, ti_freenrgs3], freenrgs_file3 )
                Sire.Stream.save( [bound0_freenrgs3, bound1_freenrgs3], freenrg_parts_file3 )
                Sire.Stream.save( [res0_freenrgs3, res1_freenrgs3, bound0_water_freenrgs3, bound1_water_freenrgs3], freenrg_components_file3 )
          
                print("...save complete (took %d ms)" % t.elapsed())

            # write a restart file every N moves in case of crash or run out of time
            if iteration % restart_frequency.val == 0 or iteration == nmoves.val:
                t.start()
                print("Saving the restart file from iteration %d..." % iteration)

                # save the old file to a backup
                tryBackup(restart_file1.val)
                tryBackup(restart_file2.val)
                tryBackup(restart_file3.val)

                Sire.Stream.save( (lsrc_system1, lsrc_moves1), restart_file1.val )
                Sire.Stream.save( (lsrc_system2, lsrc_moves2), restart_file2.val )
                Sire.Stream.save( (lsrc_system3, lsrc_moves3), restart_file3.val )

                print("...save complete (took %d ms)" % t.elapsed())


    print("All iterations complete. Total runtime was %d ms" % total_t.elapsed())

    return
