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
        PDB().write(system.molecules(), "test1.pdb")
        system = centerSystem(system, ligand_mol)
        PDB().write(system.molecules(), "test2.pdb")
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


def loadStage1():
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

    return (lsrc_system,lsrc_moves)


def loadStage2():
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

    return (lsrc_system,lsrc_moves)


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

    (lsrc1_system,lsrc1_moves) = loadStage1()
    (lsrc2_system,lsrc2_moves) = loadStage2()
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
