import os,re, sys, shutil

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
from Sire.ID import *
from Sire.Config import *

from Sire.Tools import Parameter, resolveParameters

import Sire.Stream

##### This is how we can have the script specify all of the 
##### user-controllable parameters

sphere_radius = Parameter("spherical boundary radius", 10*angstrom,
                          """The radius for the spherical boundary conditions.
                             Set to "None" if you don't want to use spherical
                             boundary conditions.""")

grid_spacing = Parameter("grid spacing", 0.5*angstrom,
                         """The spacing between grid points if a grid is used""")

use_grid = Parameter("use grid", True,
                     """Whether or not to use a grid for the interactions with atoms 
                        that are beyond the spherical boundary""")

top_file = Parameter("topology file", "../../SYSTEM.top",
                     """The name of the topology file that contains the solvated solute.""")

crd_file = Parameter("coordinate file", "../../SYSTEM.crd",
                     """The name of the coordinate file that contains the solvated solute.""")

ligand_flex_file = Parameter("ligand flex file", "../../MORPH.flex",
                             """Flexibility file describing how the morph is perturbed.""")

ligand_pert_file = Parameter("ligand perturbation file", "../../MORPH.pert",
                             """Perturbation file describing how the morph is perturbed.""")

lig_name = Parameter("ligand name", "LIG",
                     """Optional, the name of the ligand used in the flexibility file.
                        If the ligand has a single residue, the program will use the residue name
                        by default to look up the flexibility template""")

restart_file = Parameter("restart file", "sim_restart.s3",
                         """The name of the restart file.""")

random_seed = Parameter("random seed", 0, """The random number seed""")

nmoves = Parameter("number of moves", 50, """The number of moves per block""")

nmoves_per_energy = Parameter("number of energy snapshots", 1,
                              """The number of times during the simulation that you want the 
                                 energy to be recorded.""")

nmoves_per_pdb = Parameter("number of structure snapshots", 1,
                           """The number of times during the simulation that you want the 
                                    structure to be recorded (as a PDB).""")

nmoves_per_pdb_intermediates = Parameter("number of intermediate structure snapshots", 0,
                                         """The number of times during an intermediate simulation to save 
                                            the structure (as a PDB).""")

temperature = Parameter("temperature", 25 * celsius, """The temperature of the simulation""")

pressure = Parameter("pressure", 1 * atm,
                     """The pressure of the simulation. Note that this is ignored if you
                        are using spherical boundary conditions.""")

coulomb_cutoff = Parameter("coulomb cutoff", 10*angstrom,
                           """The cutoff radius for non-bonded coulomb interactions""")

coulomb_feather = Parameter("coulomb feather", 9.5*angstrom,
                            """The feather radius for the non-bonded coulomb interactions""")

lj_cutoff = Parameter("lj cutoff", 10*angstrom,
                      """The cutoff radius for non-bonded LJ interactions""")

lj_feather = Parameter("lj feather", 9.5*angstrom,
                       """The feather radius for the non-bonded LJ interactions""")

coulomb_power = Parameter("coulomb power", 0,
                          """The soft-core coulomb power parameter""")

shift_delta = Parameter("shift delta", 2.0,
                        """The soft-core shift delta parameter""")

combining_rules = Parameter("combining rules", "arithmetic",
                            """The combinining rules for LJ interactions""")

pref_constant = Parameter("preferential constant", 200 * angstrom2,
                          """The preferential sampling constant""")

max_solvent_translation = Parameter("maximum solvent translation", 0.15*angstrom,
                                    """Maximum amount to translate the solvent""")

max_solvent_rotation = Parameter("maximum solvent rotation", 15*degrees,
                                 """Maximum amount to rotate the solvent""")
                             
solvent_mc_weight_factor = Parameter("solvent move weight", 1,
                                     """Factor used to multiply the weight of the solvent moves.""")

solute_mc_weight = Parameter("solute move weight", 100,
                             """Factor used to multiply the weight of the solute moves.""")

volume_mc_weight = Parameter("volume move weight", 1,
                             """Factor used to multiply the weight of the volume moves.""")

delta_lambda = Parameter("delta lambda", 0.001,
                         """Delta lambda for finite difference gradients.""")

compress = Parameter("compression method", "bzip2 -f",
                     """Command used to compress output files.""")

lam_val = Parameter("lambda", 0.0, """Value of lambda for the simulation""")

def adjustPerturbedDOFs( molecule ):
    
    perturbations = molecule.property("perturbations").perturbations()
    
    r0 = Symbol("r0")
    theta0 = Symbol("theta0")

    for pert in perturbations:
        if ( pert.what() == 'SireMM::TwoAtomPerturbation'):
            ri = pert.initialForms()[r0].toString().toDouble()
            rf = pert.finalForms()[r0].toString().toDouble()
            if (abs(ri-rf) > 0.001):
                #rint ri,rf
                r = (1-lam_val.val) * ri + lam_val.val * rf
                r = r * angstrom
                bond = BondID(pert.atom0(), pert.atom1() )
                mover = molecule.move()
                try:
                    mover.set(bond, r)
                except UserWarning as error:
                    # extract the type of the errror
                    error_type = re.search(r"(Sire\w*::\w*)", str(error)).group(0)
                    if error_type == "SireMol::ring_error":
                        continue
                molecule = mover.commit()
        elif ( pert.what() == 'SireMM::ThreeAtomPerturbation'):
            thetai = pert.initialForms()[theta0].toString().toDouble()
            thetaf = pert.finalForms()[theta0].toString().toDouble()
            if (abs(thetai-thetaf) > 0.001):
                #rint thetai,thetaf
                theta = (1-lam_val.val) * thetai + lam_val.val * thetaf
                theta = theta * radians
                angle = AngleID(pert.atom0(), pert.atom1(), pert.atom2() )
                mover = molecule.move()
                try:                
                    mover.set(angle, theta)
                except UserWarning as error:
                    # extract the type of the errror
                    error_type = re.search(r"(Sire\w*::\w*)", str(error)).group(0)
                    if error_type == "SireMol::ring_error":
                        continue
                molecule = mover.commit()
    return molecule


def getDummies(molecule):
    print "Selecting dummy groups"
    natoms = molecule.nAtoms()
    atoms = molecule.atoms()

    from_dummies = None
    to_dummies = None

    for x in range(0,natoms):
        atom = atoms[x]
        if atom.property("initial_ambertype") == "du":
            if from_dummies is None:
                from_dummies = molecule.selectAll( atom.index() )
            else:
                from_dummies += molecule.selectAll( atom.index() )
        elif atom.property("final_ambertype") == "du":
            if to_dummies is None:
                to_dummies = molecule.selectAll( atom.index() )
            else:
                to_dummies += molecule.selectAll( atom.index() )
    
    return to_dummies, from_dummies


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

    ligand_name = lig_name.val

    if ligand_name is None:
        if ( solute.nResidues() == 1 ):
            ligand_name = solute.residue( ResIdx(0) ).name().value()
        else:
            ligand_name = "ligand" # Likely not good...

    #print lig_name
    solute = solute.edit().rename(ligand_name).commit()
    #print solute
    # This will add the property "flexibility" to the solute
    flexibility_lib = FlexibilityLibrary(ligand_flex_file.val)
    flexibility = flexibility_lib.getFlexibility(solute)
    solute = solute.edit().setProperty("flexibility", flexibility).commit()

    perturbations_lib = PerturbationsLibrary(ligand_pert_file.val)
    solute = perturbations_lib.applyTemplate(solute)

    perturbations = solute.property("perturbations")

    #print lam_val
    lam = Symbol("lambda")
    lam_fwd = Symbol("lambda_{fwd}")
    lam_bwd = Symbol("lambda_{bwd}")
    
    initial = Perturbation.symbols().initial()
    final = Perturbation.symbols().final()

    solute = solute.edit().setProperty("perturbations",
                perturbations.recreate( (1-lam)*initial + lam*final ) ).commit()

    # Set the geometry of perturbed bonds/angles to match the corresponding equilibrium value 
    solute = adjustPerturbedDOFs( solute )

    solute_fwd = solute.edit().renumber().setProperty("perturbations",
                perturbations.substitute( lam, lam_fwd ) ).commit()
    solute_bwd = solute.edit().renumber().setProperty("perturbations",
                perturbations.substitute( lam, lam_bwd ) ).commit()

    #print solute
    #print solute_fwd
    #print solute_bwd

    # We put atoms in three groups depending on what happens in the perturbation
    # non dummy to non dummy --> the hard group, uses a normal intermolecular FF
    # non dummy to dummy --> the todummy group, uses SoftFF with alpha = Lambda
    # dummy to non dummy --> the fromdummy group, uses SoftFF with alpha = 1 - Lambda
    # We start assuming all atoms are hard atoms. Then we call getDummies to find which atoms 
    # start/end as dummies and update the hard, todummy and fromdummy groups accordingly

    solute_grp_ref = MoleculeGroup("solute_ref", solute)
    solute_grp_ref_hard = MoleculeGroup("solute_ref_hard")
    solute_grp_ref_todummy = MoleculeGroup("solute_ref_todummy")
    solute_grp_ref_fromdummy = MoleculeGroup("solute_ref_fromdummy")

    solute_grp_fwd = MoleculeGroup("solute_fwd", solute_fwd)
    solute_grp_fwd_hard = MoleculeGroup("solute_fwd_hard")
    solute_grp_fwd_todummy = MoleculeGroup("solute_fwd_todummy")
    solute_grp_fwd_fromdummy = MoleculeGroup("solute_fwd_fromdummy")

    solute_grp_bwd = MoleculeGroup("solute_bwd", solute_bwd)
    solute_grp_bwd_hard = MoleculeGroup("solute_bwd_hard")
    solute_grp_bwd_todummy = MoleculeGroup("solute_bwd_todummy")
    solute_grp_bwd_fromdummy = MoleculeGroup("solute_bwd_fromdummy")
    
    solute_ref_hard = solute.selectAllAtoms()
    solute_ref_todummy = solute_ref_hard.invert()
    solute_ref_fromdummy = solute_ref_hard.invert()
    solute_fwd_hard = solute_fwd.selectAllAtoms()
    solute_fwd_todummy = solute_fwd_hard.invert()
    solute_fwd_fromdummy = solute_fwd_hard.invert()
    solute_bwd_hard = solute_bwd.selectAllAtoms()
    solute_bwd_todummy = solute_bwd_hard.invert()
    solute_bwd_fromdummy = solute_bwd_hard.invert()

    to_dummies, from_dummies = getDummies(solute)

    #print to_dummies
    #print from_dummies

    if to_dummies is not None:
        ndummies = to_dummies.count()
        dummies = to_dummies.atoms()

        for x in range(0,ndummies):
            dummy_index = dummies[x].index()
            solute_ref_hard = solute_ref_hard.subtract( solute.select( dummy_index ) )
            solute_fwd_hard = solute_fwd_hard.subtract( solute_fwd.select( dummy_index ) )
            solute_bwd_hard = solute_bwd_hard.subtract( solute_bwd.select( dummy_index ) )
            solute_ref_todummy = solute_ref_todummy.add( solute.select( dummy_index ) )
            solute_fwd_todummy = solute_fwd_todummy.add( solute_fwd.select( dummy_index ) )
            solute_bwd_todummy = solute_bwd_todummy.add( solute_bwd.select( dummy_index ) )

    if from_dummies is not None:
        ndummies = from_dummies.count()
        dummies = from_dummies.atoms()

        for x in range(0,ndummies):
            dummy_index = dummies[x].index()
            solute_ref_hard = solute_ref_hard.subtract( solute.select( dummy_index ) )
            solute_fwd_hard = solute_fwd_hard.subtract( solute_fwd.select( dummy_index ) )
            solute_bwd_hard = solute_bwd_hard.subtract( solute_bwd.select( dummy_index ) )
            solute_ref_fromdummy = solute_ref_fromdummy.add( solute.select( dummy_index ) )
            solute_fwd_fromdummy = solute_fwd_fromdummy.add( solute_fwd.select( dummy_index ) )
            solute_bwd_fromdummy = solute_bwd_fromdummy.add( solute_bwd.select( dummy_index ) )

    solute_grp_ref_hard.add(solute_ref_hard)
    solute_grp_fwd_hard.add(solute_fwd_hard)
    solute_grp_bwd_hard.add(solute_bwd_hard)

    solute_grp_ref_todummy.add(solute_ref_todummy)
    solute_grp_fwd_todummy.add(solute_fwd_todummy)
    solute_grp_bwd_todummy.add(solute_bwd_todummy)

    solute_grp_ref_fromdummy.add(solute_ref_fromdummy)
    solute_grp_fwd_fromdummy.add(solute_fwd_fromdummy)
    solute_grp_bwd_fromdummy.add(solute_bwd_fromdummy)

    solutes = MoleculeGroup("solutes")
    solutes.add(solute)
    solutes.add(solute_fwd)
    solutes.add(solute_bwd)

    perturbed_solutes = MoleculeGroup("perturbed_solutes")
    perturbed_solutes.add(solute_fwd)
    perturbed_solutes.add(solute_bwd)

    solvent = MoleculeGroup("solvent")
    for molecule in moleculeList[1:]:
        solvent.add(molecule)

    all = MoleculeGroup("all")
    all.add(solutes)

    all.add(perturbed_solutes)

    all.add(solute_grp_ref)
    all.add(solute_grp_fwd)
    all.add(solute_grp_bwd)

    all.add(solute_grp_ref_hard)
    all.add(solute_grp_ref_todummy)
    all.add(solute_grp_ref_fromdummy)

    all.add(solute_grp_fwd_hard)
    all.add(solute_grp_fwd_todummy)
    all.add(solute_grp_fwd_fromdummy)

    all.add(solute_grp_bwd_hard)
    all.add(solute_grp_bwd_todummy)
    all.add(solute_grp_bwd_fromdummy)

    all.add(solvent)

    traj = MoleculeGroup("traj")
    traj.add(solute)
    traj.add(solvent)

    # Add these groups to the System
    system = System()

    system.add(solutes)

    system.add(perturbed_solutes)

    system.add(solute_grp_ref)
    system.add(solute_grp_fwd)
    system.add(solute_grp_bwd)

    system.add(solute_grp_ref_hard)
    system.add(solute_grp_ref_todummy)
    system.add(solute_grp_ref_fromdummy)

    system.add(solute_grp_fwd_hard)
    system.add(solute_grp_fwd_todummy)
    system.add(solute_grp_fwd_fromdummy)

    system.add(solute_grp_bwd_hard)
    system.add(solute_grp_bwd_todummy)
    system.add(solute_grp_bwd_fromdummy)

    system.add(solvent)

    system.add(all)

    system.add(traj)

    return system

def setupForcefields(system, space):

    print "Creating force fields... "

    solutes = system[ MGName("solutes") ]

    solute = system[ MGName("solute_ref") ]
    solute_hard = system[ MGName("solute_ref_hard") ]
    solute_todummy = system[ MGName("solute_ref_todummy") ]
    solute_fromdummy = system[ MGName("solute_ref_fromdummy") ]

    solute_fwd = system[ MGName("solute_fwd") ]
    solute_fwd_hard = system[ MGName("solute_fwd_hard") ]
    solute_fwd_todummy = system[ MGName("solute_fwd_todummy") ]
    solute_fwd_fromdummy = system[ MGName("solute_fwd_fromdummy") ]

    solute_bwd = system[ MGName("solute_bwd") ]
    solute_bwd_hard = system[ MGName("solute_bwd_hard") ]
    solute_bwd_todummy = system[ MGName("solute_bwd_todummy") ]
    solute_bwd_fromdummy = system[ MGName("solute_bwd_fromdummy") ]

    solvent = system[ MGName("solvent") ]

    all = system[ MGName("all") ]

    # - first solvent-solvent coulomb/LJ (CLJ) energy
    solventff = InterCLJFF("solvent:solvent")
    solventff.add(solvent)

    # Now solute bond, angle, dihedral energy
    solute_intraff = InternalFF("solute_intraff")
    solute_intraff.add(solute)
    
    solute_fwd_intraff = InternalFF("solute_fwd_intraff")
    solute_fwd_intraff.add(solute_fwd)

    solute_bwd_intraff = InternalFF("solute_bwd_intraff")
    solute_bwd_intraff.add(solute_bwd)

    # Now solute intramolecular CLJ energy
    solute_hard_intraclj = IntraCLJFF("solute_hard_intraclj")
    solute_hard_intraclj.add(solute_hard)

    solute_todummy_intraclj = IntraSoftCLJFF("solute_todummy_intraclj")
    solute_todummy_intraclj.add(solute_todummy)

    solute_fromdummy_intraclj = IntraSoftCLJFF("solute_fromdummy_intraclj")
    solute_fromdummy_intraclj.add(solute_fromdummy)

    solute_hard_todummy_intraclj = IntraGroupSoftCLJFF("solute_hard:todummy_intraclj")
    solute_hard_todummy_intraclj.add(solute_hard, MGIdx(0))
    solute_hard_todummy_intraclj.add(solute_todummy, MGIdx(1))

    solute_hard_fromdummy_intraclj = IntraGroupSoftCLJFF("solute_hard:fromdummy_intraclj")
    solute_hard_fromdummy_intraclj.add(solute_hard, MGIdx(0))
    solute_hard_fromdummy_intraclj.add(solute_fromdummy, MGIdx(1))

    solute_todummy_fromdummy_intraclj = IntraGroupSoftCLJFF("solute_todummy:fromdummy_intraclj")
    solute_todummy_fromdummy_intraclj.add(solute_todummy, MGIdx(0))
    solute_todummy_fromdummy_intraclj.add(solute_fromdummy, MGIdx(1))

    # The forwards intramoleculer CLJ energy

    solute_fwd_hard_intraclj = IntraCLJFF("solute_fwd_hard_intraclj")
    solute_fwd_hard_intraclj.add(solute_fwd_hard)

    solute_fwd_todummy_intraclj = IntraSoftCLJFF("solute_fwd_todummy_intraclj")
    solute_fwd_todummy_intraclj.add(solute_fwd_todummy)

    solute_fwd_fromdummy_intraclj = IntraSoftCLJFF("solute_fwd_fromdummy_intraclj")
    solute_fwd_fromdummy_intraclj.add(solute_fwd_fromdummy)

    solute_fwd_hard_todummy_intraclj = IntraGroupSoftCLJFF("solute_fwd_hard:todummy_intraclj")
    solute_fwd_hard_todummy_intraclj.add(solute_fwd_hard, MGIdx(0))
    solute_fwd_hard_todummy_intraclj.add(solute_fwd_todummy, MGIdx(1))

    solute_fwd_hard_fromdummy_intraclj = IntraGroupSoftCLJFF("solute_fwd_hard:fromdummy_intraclj")
    solute_fwd_hard_fromdummy_intraclj.add(solute_fwd_hard, MGIdx(0))
    solute_fwd_hard_fromdummy_intraclj.add(solute_fwd_fromdummy, MGIdx(1))

    solute_fwd_todummy_fromdummy_intraclj = IntraGroupSoftCLJFF("solute_fwd_todummy:fromdummy_intraclj")
    solute_fwd_todummy_fromdummy_intraclj.add(solute_fwd_todummy, MGIdx(0))
    solute_fwd_todummy_fromdummy_intraclj.add(solute_fwd_fromdummy, MGIdx(1))

    # The backwards intramolecular CLJ energy

    solute_bwd_hard_intraclj = IntraCLJFF("solute_bwd_hard_intraclj")
    solute_bwd_hard_intraclj.add(solute_bwd_hard)

    solute_bwd_todummy_intraclj = IntraSoftCLJFF("solute_bwd_todummy_intraclj")
    solute_bwd_todummy_intraclj.add(solute_bwd_todummy)

    solute_bwd_fromdummy_intraclj = IntraSoftCLJFF("solute_bwd_fromdummy_intraclj")
    solute_bwd_fromdummy_intraclj.add(solute_bwd_fromdummy)

    solute_bwd_hard_todummy_intraclj = IntraGroupSoftCLJFF("solute_bwd_hard:todummy_intraclj")
    solute_bwd_hard_todummy_intraclj.add(solute_bwd_hard, MGIdx(0))
    solute_bwd_hard_todummy_intraclj.add(solute_bwd_todummy, MGIdx(1))

    solute_bwd_hard_fromdummy_intraclj = IntraGroupSoftCLJFF("solute_bwd_hard:fromdummy_intraclj")
    solute_bwd_hard_fromdummy_intraclj.add(solute_bwd_hard, MGIdx(0))
    solute_bwd_hard_fromdummy_intraclj.add(solute_bwd_fromdummy, MGIdx(1))

    solute_bwd_todummy_fromdummy_intraclj = IntraGroupSoftCLJFF("solute_bwd_todummy:fromdummy_intraclj")
    solute_bwd_todummy_fromdummy_intraclj.add(solute_bwd_todummy, MGIdx(0))
    solute_bwd_todummy_fromdummy_intraclj.add(solute_bwd_fromdummy, MGIdx(1))

    # Now the solute-solvent CLJ energy
    solute_hard_solventff = InterGroupCLJFF("solute_hard:solvent")
    solute_hard_solventff.add(solute_hard, MGIdx(0))
    solute_hard_solventff.add(solvent, MGIdx(1))

    solute_todummy_solventff = InterGroupSoftCLJFF("solute_todummy:solvent")
    solute_todummy_solventff.add(solute_todummy, MGIdx(0))
    solute_todummy_solventff.add(solvent, MGIdx(1))

    solute_fromdummy_solventff = InterGroupSoftCLJFF("solute_fromdummy:solvent")
    solute_fromdummy_solventff.add(solute_fromdummy, MGIdx(0))
    solute_fromdummy_solventff.add(solvent, MGIdx(1))

    #Now the forwards solute-solvent CLJ energy
    solute_fwd_hard_solventff = InterGroupCLJFF("solute_fwd_hard:solvent")
    solute_fwd_hard_solventff.add(solute_fwd_hard, MGIdx(0))
    solute_fwd_hard_solventff.add(solvent, MGIdx(1))

    solute_fwd_todummy_solventff = InterGroupSoftCLJFF("solute_fwd_todummy:solvent")
    solute_fwd_todummy_solventff.add(solute_fwd_todummy, MGIdx(0))
    solute_fwd_todummy_solventff.add(solvent, MGIdx(1))

    solute_fwd_fromdummy_solventff = InterGroupSoftCLJFF("solute_fwd_fromdummy:solvent")
    solute_fwd_fromdummy_solventff.add(solute_fwd_fromdummy, MGIdx(0))
    solute_fwd_fromdummy_solventff.add(solvent, MGIdx(1))

    # Now the backwards solute-solvent CLJ energy
    solute_bwd_hard_solventff = InterGroupCLJFF("solute_bwd_hard:solvent")
    solute_bwd_hard_solventff.add(solute_bwd_hard, MGIdx(0))
    solute_bwd_hard_solventff.add(solvent, MGIdx(1))

    solute_bwd_todummy_solventff = InterGroupSoftCLJFF("solute_bwd_todummy:solvent")
    solute_bwd_todummy_solventff.add(solute_bwd_todummy, MGIdx(0))
    solute_bwd_todummy_solventff.add(solvent, MGIdx(1))

    solute_bwd_fromdummy_solventff = InterGroupSoftCLJFF("solute_bwd_fromdummy:solvent")
    solute_bwd_fromdummy_solventff.add(solute_bwd_fromdummy, MGIdx(0))
    solute_bwd_fromdummy_solventff.add(solvent, MGIdx(1))

    # Here is the list of all forcefields
    forcefields = [ solute_intraff, solute_fwd_intraff, solute_bwd_intraff,
                    solute_hard_intraclj, solute_todummy_intraclj, solute_fromdummy_intraclj,
                    solute_hard_todummy_intraclj, solute_hard_fromdummy_intraclj, 
                    solute_todummy_fromdummy_intraclj,
                    solute_fwd_hard_intraclj, solute_fwd_todummy_intraclj, solute_fwd_fromdummy_intraclj,
                    solute_fwd_hard_todummy_intraclj, solute_fwd_hard_fromdummy_intraclj, 
                    solute_fwd_todummy_fromdummy_intraclj,
                    solute_bwd_hard_intraclj, solute_bwd_todummy_intraclj, solute_bwd_fromdummy_intraclj,
                    solute_bwd_hard_todummy_intraclj, solute_bwd_hard_fromdummy_intraclj, 
                    solute_bwd_todummy_fromdummy_intraclj,
                    solventff, 
                    solute_hard_solventff, solute_todummy_solventff, solute_fromdummy_solventff,
                    solute_fwd_hard_solventff, solute_fwd_todummy_solventff, solute_fwd_fromdummy_solventff,
                    solute_bwd_hard_solventff, solute_bwd_todummy_solventff, solute_bwd_fromdummy_solventff ]
    
    for forcefield in forcefields:
        system.add(forcefield)

    system.setProperty( "space", space )
    system.setProperty( "switchingFunction", 
                        HarmonicSwitchingFunction(coulomb_cutoff.val, coulomb_feather.val,
                                                  lj_cutoff.val, lj_feather.val) )
    system.setProperty( "combiningRules", VariantProperty(combining_rules.val) )
    system.setProperty( "coulombPower", VariantProperty(coulomb_power.val) )
    system.setProperty( "shiftDelta", VariantProperty(shift_delta.val) )
    
    total_nrg = solute_intraff.components().total() + solute_hard_intraclj.components().total() +\
        solute_todummy_intraclj.components().total(0) + solute_fromdummy_intraclj.components().total(0) +\
        solute_hard_todummy_intraclj.components().total(0) + solute_hard_fromdummy_intraclj.components().total(0) +\
        solute_todummy_fromdummy_intraclj.components().total(0) +\
        solventff.components().total() +\
        solute_hard_solventff.components().total() +\
        solute_todummy_solventff.components().total(0) +\
        solute_fromdummy_solventff.components().total(0)

    fwd_nrg = solute_fwd_intraff.components().total() + solute_fwd_hard_intraclj.components().total() +\
        solute_fwd_todummy_intraclj.components().total(0) + solute_fwd_fromdummy_intraclj.components().total(0) +\
        solute_fwd_hard_todummy_intraclj.components().total(0) + solute_fwd_hard_fromdummy_intraclj.components().total(0) +\
        solute_fwd_todummy_fromdummy_intraclj.components().total(0) +\
        solventff.components().total() +\
        solute_fwd_hard_solventff.components().total() +\
        solute_fwd_todummy_solventff.components().total(0) +\
        solute_fwd_fromdummy_solventff.components().total(0)
        
    bwd_nrg = solute_bwd_intraff.components().total() + solute_bwd_hard_intraclj.components().total() +\
        solute_bwd_todummy_intraclj.components().total(0) + solute_bwd_fromdummy_intraclj.components().total(0) +\
        solute_bwd_hard_todummy_intraclj.components().total(0) + solute_bwd_hard_fromdummy_intraclj.components().total(0) +\
        solute_bwd_todummy_fromdummy_intraclj.components().total(0) +\
        solventff.components().total() +\
        solute_bwd_hard_solventff.components().total() +\
        solute_bwd_todummy_solventff.components().total(0) +\
        solute_bwd_fromdummy_solventff.components().total(0)

    e_total = system.totalComponent()
    e_fwd = Symbol("E_{fwd}")
    e_bwd = Symbol("E_{bwd}")

    lam = Symbol("lambda")
    lam_fwd = Symbol("lambda_{fwd}")
    lam_bwd = Symbol("lambda_{bwd}")

    system.setComponent( e_total, total_nrg )
    system.setComponent( e_fwd, fwd_nrg )
    system.setComponent( e_bwd, bwd_nrg )

    system.setConstant(lam, 0.0)
    system.setConstant(lam_fwd, 0.0)
    system.setConstant(lam_bwd, 0.0)

    de_fwd = Symbol("dE_{fwd}")
    de_bwd = Symbol("dE_{bwd}")

    system.setComponent( de_fwd, fwd_nrg - total_nrg )
    system.setComponent( de_bwd, total_nrg - bwd_nrg )

    # Add a space wrapper that wraps all molecules into the box centered at (0,0,0)
    # system.add( SpaceWrapper(Vector(0,0,0), all) )

    # Add a monitor that calculates the average total energy and average energy
    # deltas - we will collect both a mean average and an zwanzig average
    system.add( "total_energy", MonitorComponent(e_total, Average()) )
    system.add( "dg_fwd", MonitorComponent(de_fwd, FreeEnergyAverage(temperature.val)) )
    system.add( "dg_bwd", MonitorComponent(de_bwd, FreeEnergyAverage(temperature.val)) )

    system.add( PerturbationConstraint(solutes) )

    system.add( ComponentConstraint( lam_fwd, Min( lam + delta_lambda.val, 1 ) ) )
    system.add( ComponentConstraint( lam_bwd, Max( lam - delta_lambda.val, 0 ) ) )

    # Add a monitor that records the value of all energy components
    system.add( "energies", MonitorComponents(RecordValues()), nmoves_per_energy.val )
    
    # Add a monitor that records the coordinates of the system
    if (lam_val.val < 0.001 or lam_val.val > 0.999):
        system.add( "trajectory", TrajectoryMonitor(MGName("traj")), nmoves_per_pdb.val )
    else:
        system.add( "trajectory", TrajectoryMonitor(MGName("traj")), nmoves_per_pdb_intermediates.val )

    # Alpha constraints for the soft force fields

    system.add( PropertyConstraint( "alpha0", FFName("solute_todummy_intraclj"), lam ) )
    system.add( PropertyConstraint( "alpha0", FFName("solute_fromdummy_intraclj"), 1 - lam ) )
    system.add( PropertyConstraint( "alpha0", FFName("solute_hard:todummy_intraclj"), lam ) )
    system.add( PropertyConstraint( "alpha0", FFName("solute_hard:fromdummy_intraclj"), 1 - lam ) )
    system.add( PropertyConstraint( "alpha0", FFName("solute_todummy:fromdummy_intraclj"), Min( lam, 1 - lam )  ) ) 
    system.add( PropertyConstraint( "alpha0", FFName("solute_todummy:solvent"), lam ) )
    system.add( PropertyConstraint( "alpha0", FFName("solute_fromdummy:solvent"), 1 - lam ) )

    system.add( PropertyConstraint( "alpha0", FFName("solute_fwd_todummy_intraclj"), lam_fwd ) )
    system.add( PropertyConstraint( "alpha0", FFName("solute_fwd_fromdummy_intraclj"), 1 - lam_fwd ) )
    system.add( PropertyConstraint( "alpha0", FFName("solute_fwd_hard:todummy_intraclj"), lam_fwd ) )
    system.add( PropertyConstraint( "alpha0", FFName("solute_fwd_hard:fromdummy_intraclj"), 1 - lam_fwd ) )
    system.add( PropertyConstraint( "alpha0", FFName("solute_fwd_todummy:fromdummy_intraclj"), Min( lam_fwd, 1 - lam_fwd ) ) ) 
    system.add( PropertyConstraint( "alpha0", FFName("solute_fwd_todummy:solvent"), lam_fwd ) )
    system.add( PropertyConstraint( "alpha0", FFName("solute_fwd_fromdummy:solvent"), 1 - lam_fwd ) )

    system.add( PropertyConstraint( "alpha0", FFName("solute_bwd_todummy_intraclj"), lam_bwd ) )
    system.add( PropertyConstraint( "alpha0", FFName("solute_bwd_fromdummy_intraclj"), 1 - lam_bwd ) )
    system.add( PropertyConstraint( "alpha0", FFName("solute_bwd_hard:todummy_intraclj"), lam_bwd ) )
    system.add( PropertyConstraint( "alpha0", FFName("solute_bwd_hard:fromdummy_intraclj"), 1 - lam_bwd ) )
    system.add( PropertyConstraint( "alpha0", FFName("solute_bwd_todummy:fromdummy_intraclj"), Min( lam_bwd, 1 - lam_bwd ) ) ) 
    system.add( PropertyConstraint( "alpha0", FFName("solute_bwd_todummy:solvent"), lam_bwd ) )
    system.add( PropertyConstraint( "alpha0", FFName("solute_bwd_fromdummy:solvent"), 1 - lam_bwd ) )

    system.setComponent( lam, lam_val.val )

    return system

def getAtomNearCOG( molecule ):

    mol_centre = molecule.evaluate().center()
    mindist = 99999.0

    for x in range(0, molecule.nAtoms()):
        atom = molecule.atoms()[x]
        at_coords = atom.property('coordinates')
        dist = Vector().distance2(at_coords, mol_centre)
        if dist < mindist:
            mindist = dist
            nearest_atom = atom

    return nearest_atom

def setupMoves(system, random_seed):

    solutes = system[ MGName("solutes") ]
    solute_ref = system[ MGName("solute_ref") ]

    solvent = system[ MGName("solvent") ]

    all = system[ MGName("all") ]
    
    print "Setting up moves..."
    # Setup Moves
    max_volume_change = 0.50 * solvent.nMolecules() * angstrom3

    solute_moves = RigidBodyMC( solutes ) 


    # No translation for a solvation calculation
    solute_moves.setMaximumTranslation( 0.0 * angstrom )
    solute_moves.setMaximumTranslation(solutes[MolIdx(0)].molecule().property('flexibility').translation() )
    # Find solute atom nearest to the center of geometry
    nearestcog_atom = getAtomNearCOG( solutes[MolIdx(0)].molecule() )
    #print nearestcog_atom
    solute_moves.setCenterOfRotation( GetCOGPoint( nearestcog_atom.name() ) )
    solute_moves.setSynchronisedTranslation(True)
    solute_moves.setSynchronisedRotation(True)
    #solute_moves.setSharedRotationCenter(True)

    solute_intra_moves = InternalMoveSingle( solute_ref )

    # Each molecule in perturbed_solutes will have its coordinates set to those 
    # of solute_ref after the move
    perturbed_solutes = system[ MGName("perturbed_solutes") ]
    solute_intra_moves.setSynchronisedCoordinates(perturbed_solutes)
    
    moves = WeightedMoves()

    if False:  #  not (reflection_sphere_radius.val is None):
        # Do not use preferential sampling with the reflection sphere. Preferential
        # sampling causes volume expansion around the solute, which would push the
        # solvent towards the boundary of the sphere. Given we have already really
        # reduced the number of moving solvent molecules, preferential sampling 
        # is not needed
        solvent_moves = RigidBodyMC(solvent)
        solvent_moves.setReflectionSphereRadius(reflection_sphere_radius)

    else:
        solvent_moves = RigidBodyMC( PrefSampler(solute_ref[MolIdx(0)],
                                             solvent, pref_constant.val) )

        volume_moves = VolumeMove(all)
        volume_moves.setMaximumVolumeChange(max_volume_change)
        moves.add( volume_moves, volume_mc_weight.val )

    solvent_moves.setMaximumTranslation(max_solvent_translation.val)
    solvent_moves.setMaximumRotation(max_solvent_rotation.val)
    solvent_moves.setCenterOfRotation(GetCOGPoint( AtomName("O") ) )

    moves.add( solute_moves, solute_mc_weight.val / 2 )
    moves.add( solute_intra_moves, solute_mc_weight.val / 2)
    moves.add( solvent_moves, solvent.nMolecules() * solvent_mc_weight_factor.val)

    moves.setTemperature(temperature.val)
    moves.setPressure(pressure.val)

    if (not random_seed):
	random_seed = RanGenerator().randInt(100000,1000000)
	print "Generated random seed number %d " % random_seed

    moves.setGenerator( RanGenerator(random_seed) )
    
    return moves

def writeComponents(components, filename):
    """This function writes the energy components to the file 'filename'"""

    symbols = components.monitoredComponents()

    if len(symbols) == 0:
        return

    newrun = False
    if not os.path.exists(filename):
        newrun = True

    FILE = open(filename, "a")

    nrgs = {}

    for symbol in symbols:
        nrgs[str(symbol)] = components.accumulator(symbol).values()

    symbols = nrgs.keys()
    symbols.sort()

    if newrun:
        print >>FILE,"#step   ",

        for symbol in symbols:
            print >>FILE,"%s   " % symbol,

        print >>FILE,"\n",

    for i in range(0, len(nrgs[symbols[0]])):
        print >>FILE,"%d   " % i,

        for symbol in symbols:
            print >>FILE,"%f   " % nrgs[symbol][i],

        print >>FILE,"\n",


def writeSystemData(system, moves, block):
    nmoves = moves.nMoves()
    monitors = system.monitors()

    pdb = monitors[MonitorName("trajectory")]
    energies = monitors[MonitorName("energies")]
    total_energy = monitors[MonitorName("total_energy")]
    
    dg_fwd = monitors[MonitorName("dg_fwd")]
    dg_bwd = monitors[MonitorName("dg_bwd")]
    
    dg_fwd = dg_fwd.accumulator().average() / delta_lambda.val
    dg_bwd = dg_bwd.accumulator().average() / delta_lambda.val

    system.clearStatistics()
    
    pdb.writeToDisk("output%0009d.pdb" % block )

    if os.path.exists("energies.dat.bz2"):
        os.system("bunzip2 -f energies.dat.bz2")

    writeComponents( energies, "energies.dat" )

    # Ugly
    lam = system.constantExpression(Symbol("lambda")).toString().toDouble()    
    #print dg_bwd, dg_fwd, lam

    if lam < 0.0001:
        dg_bwd = dg_fwd
    elif lam > 0.9999:
        dg_fwd = dg_bwd

    dg_avg = 0.5 * ( dg_fwd + dg_bwd )

    #print dg_avg

    FILE = open("gradients.dat" , 'a')
    print >>FILE, "%9d %12.8f " % ( block, dg_avg)
   
    FILE = open("moves.dat", "w")
    print >>FILE, "%s" % moves

def printComponents(nrgs):
    keys = nrgs.keys()
    keys.sort()

    for key in keys:
        print "%s     %s" % (key, nrgs[key])

    print "\n",

@resolveParameters
def run():
    print " ### Running a \"free leg\" single topology free energy calculation ### "

    timer = QTime()
    timer.start()

    # Setup the system from scratch if no restart file is available

    if not os.path.exists(restart_file.val):
        print "New run. Loading input and creating restart"
        print "Lambda is %5.3f" % lam_val.val       

        amber = Amber()

        molecules, space = amber.readCrdTop(crd_file.val,top_file.val)

        system = createSystem(molecules)

        system = setupForcefields(system, space)

        moves = setupMoves(system, random_seed.val)
        print "Saving restart"
        Sire.Stream.save( [system, moves], restart_file.val )


    system, moves = Sire.Stream.load(restart_file.val)
    print "Loaded a restart file on wich we have performed %d moves." % moves.nMoves()
    block_number = moves.nMoves() / nmoves.val  + 1
    s1 = timer.elapsed()/1000.
    print "Setup took %d s " % ( s1 )

    # Run a short simulation

    print "Performing simulation for block number %d " % block_number

    printComponents(system.energies())
    
    system = moves.move(system, nmoves.val, True)

    s2 = timer.elapsed()/1000.
    print "Simulation took %d s " % ( s2 - s1)

    # Update statistics and save restart
    writeSystemData(system, moves, block_number) 

    Sire.Stream.save( [system, moves], restart_file.val )

    # Compress some output files
    outpdb = "./output%0009d.pdb" % block_number
    if os.path.exists(outpdb):
        os.system( "%s output%0009d*" % (compress.val, block_number) )
    if os.path.exists("energies.dat"):
        os.system(" %s energies.dat" % compress.val )
