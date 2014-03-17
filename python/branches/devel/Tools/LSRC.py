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

def loadWater():
    """Load the the water box used for the free leg"""

    if os.path.exists(water_s3file.val):
        print("Loading from existing s3 file %s..." % water_s3file.val)
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
        sys = Sire.Stream.load(s3file)
    else:
        print("Loading from Amber files %s / %s..." % (topfile,crdfile))
        # Add the name of the ligand to the list of solute molecules
        scheme = NamingScheme()

        if ligand_name:
            scheme.addSoluteResidueName(ligand_name)

        # Load up the system. This will automatically find the protein, solute, water, solvent
        # and ion molecules and assign them to different groups
        sys = createSystem(topfile, crdfile, scheme)
        ligand_mol = findMolecule(sys, ligand_name)

        if ligand_mol is None:
            print("Cannot find the ligand (%s) in the set of loaded molecules!" % ligand_name)
            sys.exit(-1)

        # Center the system with the ligand at (0,0,0)
        sys = centerSystem(sys, ligand_mol)
        ligand_mol = sys[ligand_mol.number()].molecule()

        sys = addFlexibility(sys, Vector(0,0,0), reflection_radius.val, scheme )
        Sire.Stream.save(sys, s3file)

    ligand_mol = findMolecule(sys, ligand_name)

    if ligand_mol is None:
        print("Cannot find the ligand (%s) in the set of loaded molecules!" % ligand_name)
        sys.exit(-1)

    return (sys, ligand_mol)


def loadSystem0():
    """This function loads the ligand 0 system"""
    return loadSystem(topfile0.val, crdfile0.val, s3file0.val, ligand_name0.val)


def loadSystem1():
    """This function loads the ligand 1 system"""
    return loadSystem(topfile1.val, crdfile1.val, s3file1.val, ligand_name1.val)


def runStage1():
    """This is a high level function that runs the stage 1 calculation that calculates the 
       relative binding free energy of swapping bound ligand 0 with free ligand 1"""

    print("\nRUNNING STAGE 1\nSwapping bound ligand 0 with free ligand 1\n")

    if os.path.exists(restart_file1.val):
        (lsrc_system,lsrc_moves) = Sire.Stream.load(restart_file1.val)
    else:
        (sys0, ligand0_mol) = loadSystem0()
        (sys1, ligand1_mol) = loadSystem1()
        watersys = loadWater()

        (lsrc_system,lsrc_moves) = mergeLSRC(sys0,ligand0_mol, sys1,ligand1_mol, watersys)
        Sire.Stream.save( (lsrc_system,lsrc_moves), restart_file1.val)

    return


def runStage2():
    """This is a high level function that runs the stage 2 calculation that calculates the
       relative binding free energy of swapping bound ligand 1 with free ligand 0"""

    print("\nRUNNING STAGE 2\nSwapping bound ligand 1 with free ligand 0\n")

    if os.path.exists(restart_file1.val):
        (lsrc_system,lsrc_moves) = Sire.Stream.load(restart_file2.val)
    else:
        (sys0, ligand0_mol) = loadSystem0()
        (sys1, ligand1_mol) = loadSystem1()
        watersys = loadWater()

        (lsrc_system,lsrc_moves) = mergeLSRC(sys1,ligand1_mol, sys0,ligand0_mol, watersys)
        Sire.Stream.save( (lsrc_system,lsrc_moves), restart_file2.val)

    return


def runStage3():
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

    return


@resolveParameters
def run():
    """This is a very high level function that does everything to run a LSRC simulation"""

    runStage1()
    runStage2()
    runStage3()

    return
