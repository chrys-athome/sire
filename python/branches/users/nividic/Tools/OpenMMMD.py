#
# Python script to perform a MD simulation in Sire with OpenMM
#

import os,re, sys, shutil
import math

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

from Sire.Tools.DCDFile import *

from Sire.Tools import Parameter, resolveParameters

import Sire.Stream

gpu = Parameter("gpu", 0, """The device ID of the GPU on which to run the simulation.""")

rf_dielectric = Parameter("reaction field dielectric", 78.3,
                          """Dielectric constant to use if the reaction field cutoff method is used.""")

temperature = Parameter("temperature", 25*celsius, """Simulation temperature""")

pressure = Parameter("pressure", 1*atm, """Simulation pressure""")

topfile = Parameter("topfile", "SYSTEM.top",
                    """Name of the topology file containing the system to be simulated.""")

crdfile = Parameter("crdfile", "SYSTEM.crd",
                    """Name of the coordinate file containing the coordinates of the 
                       system to be simulated.""")

s3file = Parameter("s3file", "SYSTEM.s3",
                    """Name to use for the intermediate s3 file that will contain the 
                       simulation syste after it has been loaded from the top/crd files.""")

restart_file = Parameter("restart file", "sim_restart.s3",
                         """Name of the restart file to use to save progress during the simulation.""")

dcd_root = Parameter("dcd root", "traj", """Root of the filename of the output DCD trajectory files.""")

nmoves = Parameter("nmoves", 1000, """Number of Molecular Dynamics moves to perform during the simulation.""")

random_seed = Parameter("random seed", None, """Random number seed. Set this if you
                         want to have reproducible simulations.""")

ncycles = Parameter("ncycles", 1, """The number of MD cycles. The total elapsed time will be nmoves*ncycles*time_step""")

ncycles_per_snap = Parameter("ncycles_per_snap", 1, """Number of cycles between saving snapshots""")

save_coords = Parameter("save coordinates", True, """Whether or not to save coordinates.""")

time_to_skip = Parameter("time to skip", 0*picosecond, """Time to skip?""")


minimize = Parameter("minimize", False, """Whether or not to perform minimization before the simulation.""")

minimize_tol = Parameter("minimize tolerance", 1e-8, """Tolerance used to know when minimization is complete.""")

minimize_max_iter = Parameter("minimize maximum iterations", 1000, """Maximum number of iterations for minimization.""")

equilibrate = Parameter("equilibrate", False, """Whether or not to perform equilibration before dynamics.""")

equil_iterations = Parameter("equilibration iterations", 2000, """Number of equilibration steps to perform.""")

equil_timestep = Parameter("equilibration timestep", 0.5*femtosecond, """Timestep to use during equilibration.""")

combining_rules = Parameter("combining rules", "arithmetic", """Combining rules to use for the non-bonded interactions.""")

timestep = Parameter("timestep", 2 * femtosecond, """Timestep for the dynamics simulation.""")

platform = Parameter("platform", "CPU", """Which OpenMM platform should be used to perform the dynamics.""")

precision = Parameter("precision", "double", """The floating point precision model to use during dynamics.""")

constraint = Parameter("constraint", "none", """The constraint model to use during dynamics.""")

cutoff_type = Parameter("cutoff type", "cutoffperiodic", """The cutoff method to use during the simulation.""")

cutoff_dist = Parameter("cutoff distance", 10*angstrom, """The cutoff distance to use for the non-bonded interactions.""")

integrator_type = Parameter("integrator", "leapfrogverlet", """The integrator to use for dynamics.""")

inverse_friction = Parameter("inverse friction", 0.1*picosecond, """Inverse friction time for the Langevin thermostat.""")

anderson = Parameter("thermostat", True, """Whether or not to use the Andersen thermostat (needed for NVT or NPT simulation).""")

barostat = Parameter("barostat", True, """Whether or not to use a barostat (needed for NPT simulation).""")

andersen_frequency = Parameter("andersen frequency", 10.0, """Collision frequency in units of (1/ps)""")

barostat_frequency = Parameter("barostat frequency", 25, """Number of steps before attempting box changes if using the barostat.""")

lj_dispersion = Parameter("lj dispersion", False, """Whether or not to calculate and include the LJ dispersion term.""")

cmm_removal = Parameter("center of mass frequency", 0, "Frequency of which the system center of mass motion is removed.""")

center_solute = Parameter("center solute", True, """Whether or not to centre the centre of geometry of the solute in the box.""")

use_restraints = Parameter("use restraints", False, """Whether or not to use harmonic restaints on the solute atoms.""")

k_restraint = Parameter("restraint force constant", 100.0, """Force constant to use for the harmonic restaints.""")

heavy_mass_restraint = Parameter("heavy mass restraint", 1.10, """Only restrain solute atoms whose mass is greater than this value.""")

unrestrained_residues = Parameter("unrestrained residues", ["WAT", "HOH"], """Names of residues that are never restrained.""")

freeze_residues = Parameter("freeze residues", True, """Whether or not to freeze certain residues.""")

frozen_residues = Parameter("frozen residues", ["LGR", "SIT", "NEG", "POS"], """List of residues to freeze if 'freeze residues' is True.""")

#####################################

def setupDCD(DCD_root, system):

    files = os.listdir(os.getcwd())
    
    dcds = []
    for f in files:
        if f.endswith(".dcd"):
            dcds.append(f)
    
    dcds.sort()
    
    index = len(dcds) + 1
    
    dcd_filename = DCD_root + "%0009d" % index + ".dcd"

    Trajectory = DCDFile(dcd_filename, system[MGName("all")], system.property("space"), time_step, interval=buffer_freq*ncycles_per_snap)

    return Trajectory


def writeSystemData( system, moves, Trajectory, block):

    localtimer = QTime()
    localtimer.start()

    if (block % ncycles_per_snap == 0):
        #PDB().write(system[MGName("all")], "output%0009d.pdb" % block)

        if buffer_freq > 0:
            dimensions = {}
            sysprops = system.propertyKeys()
            for prop in sysprops:
                if prop.startswith("buffered_space"):
                    dimensions[str(prop)] = system.property(prop) 
            Trajectory.writeBufferedModels( system[MGName("all")], dimensions )
        else:
            Trajectory.writeModel( system[MGName("all")], system.property("space") )

    FILE = open("moves.dat", "w")
    print("%s" % moves, file=FILE)

    print(" Time to write coordinates %s ms " % localtimer.elapsed())


def centerSolute(system, space):

    # ! Assuming first molecule in the system is the solute ! 
    
    box_center = space.dimensions()/2

    solute = system.molecules().at(MolNum(1)) #first().molecule()

    solute_cog = CenterOfGeometry(solute).point()

    delta = box_center - solute_cog

    molNums = system.molNums()

    for molnum in molNums:
        mol = system.molecule(molnum).molecule()
        molcoords = mol.property("coordinates")
        molcoords.translate(delta)
        mol = mol.edit().setProperty("coordinates",molcoords).commit()
        system.update(mol)

    return system


def createSystem(molecules):
    print("Applying flexibility and zmatrix templates...")

    moleculeNumbers = molecules.molNums()
    moleculeList = []

    for moleculeNumber in moleculeNumbers:
        molecule = molecules.molecule(moleculeNumber).molecule()
        moleculeList.append(molecule)

    molecules = MoleculeGroup("molecules")
    ions = MoleculeGroup("ions")
    
    for molecule in moleculeList:
        natoms = molecule.nAtoms()
        if natoms == 1:
            ions.add(molecule)
        else:
            molecules.add(molecule)

    all = MoleculeGroup("all")
    all.add(molecules)
    all.add(ions)

    # Add these groups to the System
    system = System()

    system.add(all)
    system.add(molecules)
    system.add(ions)
       
    return system

def setupForcefields(system, space):

    print("Creating force fields... ")

    all = system[ MGName("all") ]
    molecules = system[ MGName("molecules")]
    ions = system[ MGName("ions") ]
    
    # - first solvent-solvent coulomb/LJ (CLJ) energy
    internonbondedff = InterCLJFF("molecules:molecules")
    if (Cutofftype != "nocutoff") :
        internonbondedff.setUseReactionField(True)
        internonbondedff.setReactionFieldDielectric(Dielectric)
    internonbondedff.add(molecules)

    inter_ions_nonbondedff = InterCLJFF("ions:ions")
    if (Cutofftype != "nocutoff") :
        inter_ions_nonbondedff.setUseReactionField(True)
        inter_ions_nonbondedff.setReactionFieldDielectric(Dielectric)
    inter_ions_nonbondedff.add(ions)
    
    inter_ions_molecules_nonbondedff = InterGroupCLJFF("ions:molecules")
    if (Cutofftype != "nocutoff") :
        inter_ions_molecules_nonbondedff.setUseReactionField(True)
        inter_ions_molecules_nonbondedff.setReactionFieldDielectric(Dielectric)
    inter_ions_molecules_nonbondedff.add(ions, MGIdx(0) )
    inter_ions_molecules_nonbondedff.add(molecules, MGIdx(1) )
    
    # Now solute bond, angle, dihedral energy
    intrabondedff = InternalFF("molecules-intrabonded")
    intrabondedff.add(molecules)

    # Now solute intramolecular CLJ energy
    intranonbondedff = IntraCLJFF("molecules-intranonbonded")
    if (Cutofftype != "nocutoff") :
        intranonbondedff.setUseReactionField(True)
        intranonbondedff.setReactionFieldDielectric(Dielectric)
    intranonbondedff.add(molecules)

    # solute restraint energy
    # 
    # We restrain atoms based ont he contents of the property "restrainedatoms"
    #
    restraintff = RestraintFF("restraint")

    if UseRestraints:
        molnums = molecules.molecules().molNums()

        for molnum in molnums:
            mol = molecules.molecule(molnum).molecule()
            try:
                mol_restrained_atoms = propertyToAtomNumVectorList(mol.property("restrainedatoms"))
            except UserWarning as error:
                error_type = re.search(r"(Sire\w*::\w*)", str(error)).group(0)
                if error_type == "SireBase::missing_property":
                    continue
                else:
                    raise error

            for restrained_line in mol_restrained_atoms:
                atnum = restrained_line[0]
                restraint_atom = mol.select(atnum)
                restraint_coords = restrained_line[1]
                restraint_k = restrained_line[2] * kcal_per_mol / (angstrom*angstrom)
            
                restraint = DistanceRestraint.harmonic(restraint_atom, restraint_coords, restraint_k)

                restraintff.add(restraint)

    # Here is the list of all forcefields
    forcefields = [ internonbondedff, intrabondedff, intranonbondedff, inter_ions_nonbondedff, inter_ions_molecules_nonbondedff, restraintff ] 
    
    for forcefield in forcefields:
        system.add(forcefield)

    system.setProperty( "space", space )
    system.setProperty("switchingFunction", CHARMMSwitchingFunction(Cutoff_dist) )
    system.setProperty( "combiningRules", VariantProperty(combining_rules) )
    #system.setProperty( "useReactionField", VariantProperty(True) )
    #system.setProperty( "reactionFieldDielectric", VariantProperty(rfdielectric) )

    total_nrg = internonbondedff.components().total() +\
                intranonbondedff.components().total() + intrabondedff.components().total() +\
                inter_ions_nonbondedff.components().total() + inter_ions_molecules_nonbondedff.components().total() +\
                restraintff.components().total()

    e_total = system.totalComponent()

    system.setComponent( e_total, total_nrg )

    # Add a monitor that calculates the average total energy and average energy
    # deltas - we will collect both a mean average and an zwanzig average
    system.add( "total_energy", MonitorComponent(e_total, Average()) )

    return system

def setupMoves(system, random_seed, GPUS):

    print("Setting up moves...")


    molecules = system[ MGName("all") ]

    Integrator_OpenMM = OpenMMMDIntegrator( molecules )
  
    Integrator_OpenMM.setPlatform(platforn.val)
    Integrator_OpenMM.setConstraintType(constraint.val)
    Integrator_OpenMM.setCutoffType(cutoff_type.val)
    Integrator_OpenMM.setIntegrator(integrator_type.val)
    Integrator_OpenMM.setFriction( inverse_friction.val )# Only meaningful for Langevin/Brownian integrators
    Integrator_OpenMM.setPrecision(precision.val)
    Integrator_OpenMM.setTimetoSkip(time_to_skip)
    Integrator_OpenMM.setMinimization(Minimize)
    Integrator_OpenMM.setMinimizeTol(minimize_tol)
    Integrator_OpenMM.setMinimizeIterations(minimize_max_iter)
    Integrator_OpenMM.setEquilib_iterations(equilib_iterations)
    Integrator_OpenMM.setEquilib_time_step(equilib_time_step)
    Integrator_OpenMM.setDeviceIndex(GPUS)
    Integrator_OpenMM.setLJDispersion(LJDispersion)

    if Cutofftype != "nocutoff":
        Integrator_OpenMM.setCutoff_distance(Cutoff_dist)
    if Cutofftype == "cutoffperiodic":
        Integrator_OpenMM.setField_dielectric(Dielectric)

    Integrator_OpenMM.setCMMremoval_frequency(CMMremoval)

    Integrator_OpenMM.setBufferFrequency(buffer_freq)

    if UseRestraints:
        Integrator_OpenMM.setRestraint(True)

    if Andersen:
        Integrator_OpenMM.setTemperature(temperature)
        Integrator_OpenMM.setAndersen(Andersen)
        Integrator_OpenMM.setAndersen_frequency(Andersen_frequency)

    if Barostat:
        Integrator_OpenMM.setPressure(pressure)
        Integrator_OpenMM.setMCBarostat(Barostat)
        Integrator_OpenMM.setMCBarostat_frequency(MCBarostat_frequency)
    


    #print Integrator_OpenMM.getDeviceIndex()
    Integrator_OpenMM.initialise()

    mdmove = MolecularDynamics(molecules, Integrator_OpenMM, time_step, {"velocity generator":MaxwellBoltzmann(temperature)})

    print("Created a MD move that uses OpenMM for all molecules on GPU %s " % GPUS)

    moves = WeightedMoves()
    moves.add(mdmove, 1)
    
    if (not random_seed):
        random_seed = RanGenerator().randInt(100000,1000000)
    print("Generated random seed number %d " % random_seed)

    moves.setGenerator( RanGenerator(random_seed) )

    return moves

def atomNumListToProperty( list ):
    prop = Properties()

    i = 0

    for value in list:
        prop.setProperty(str(i), VariantProperty(value.value()))
        i += 1

    return prop

def atomNumVectorListToProperty( list ):
    prop = Properties()

    i = 0
                
    for value in list:
        prop.setProperty("AtomNum(%d)" % i, VariantProperty(value[0].value()))
        prop.setProperty("x(%d)" % i, VariantProperty(value[1].x()))
        prop.setProperty("y(%d)" % i, VariantProperty(value[1].y()))
        prop.setProperty("z(%d)" % i, VariantProperty(value[1].z()))
        prop.setProperty("k(%d)" % i, VariantProperty(value[2]))
        i += 1
    
    prop.setProperty("nrestrainedatoms", VariantProperty(i) );

    return prop

def propertyToAtomNumList( prop ):
    list = []

    i = 0

    try:
        while True:
            list.append( AtomNum(prop[str(i)].toInt()) )
            i += 1
    except:
        pass

    return list

def propertyToAtomNumVectorList( prop ):
    list = []
            
    i = 0
        
    try:
        while True:
            num = AtomNum(prop["AtomNum(%d)" % i].toInt())
            x = prop["x(%d)" % i].toDouble()
            y = prop["y(%d)" % i].toDouble()
            z = prop["z(%d)" % i].toDouble()
            k = prop["k(%d)" % i].toDouble()

            list.append( (num, Vector(x,y,z), k ) )
 
            i += 1
    except:
        pass

    return list


def setupRestraints(system):

    molecules = system[ MGName("all") ].molecules()

    molnums = molecules.molNums()

    for molnum in molnums:
        mol = molecules.molecule(molnum).molecule()
        nats = mol.nAtoms()
        atoms = mol.atoms()
        
        restrainedAtoms = []

        #
        # This will apply a restraint to every atom that is 
        # A) NOT a hydrogen
        # B) NOT in an unrestrained residue.
        #
        for x in range(0,nats):
            at = atoms[x]
            atnumber = at.number()
            #print at, atnumber
            if at.residue().name().value() in unrestrained_residues.val:
                continue
            #print at, at.property("mass"), heavyMass
            if ( at.property("mass").value() < heavy_mass_restraint.val ):
                #print "LIGHT, skip"
                continue
            atcoords = at.property("coordinates")
            #print at
            restrainedAtoms.append( ( atnumber , atcoords, Krestraint) )
            #restrainedAtoms.append( atnumber )
        
        if len(restrainedAtoms) > 0:
            mol = mol.edit().setProperty("restrainedatoms", atomNumVectorListToProperty(restrainedAtoms)).commit()
            #print restrainedAtoms
            #print propertyToAtomNumVectorList( mol.property("restrainedatoms") )
            system.update(mol)

    return system

def freezeResidues(system):
   
    molecules = system[ MGName("all") ].molecules()

    molnums = molecules.molNums()

    for molnum in molnums:
        mol = molecules.molecule(molnum).molecule()
        nats = mol.nAtoms()
        atoms = mol.atoms()

        for x in range(0,nats):
            at = atoms[x]
            atnumber = at.number()
            if at.residue().name().value() in frozen_residues.val:
                print("Freezing %s %s %s " % (at, atnumber, at.residue().name().value() ))
                mol = at.edit().setProperty("mass", 0 * g_per_mol).molecule()
                    
        system.update(mol)

    return system

######## MAIN SCRIPT  #############

@resolveParameters
def run():

    if (save_coords.val):
        buffer_freq = 500
    else:
        buffer_freq = 0

    try:
        host = os.environ['HOSTNAME']
    except KeyError:
        host = "unknown"

    print(" ### Running Molecular Dynamics on %s ### " % host)

    timer = QTime()
    timer.start()

    # Setup the system from scratch if no restart file is available

    if not os.path.exists(restart_file.val):

        print("New run. Loading input and creating restart")
        
        amber = Amber()
        
        if os.path.exists(s3file.val):
            (molecules, space) = Sire.Stream.load(s3file.val)
        else:
            (molecules, space) = amber.readCrdTop(crdfile.val, topfile.val)
            Sire.Stream.save( (molecules,space), s3file.val )

        system = createSystem(molecules)

        if (center_solute.val):
            system = centerSolute(system, space)

        if use_restraints.val:
            system = setupRestraints(system)
        
        # Note that this just set the mass to zero which freezes residues in OpenMM but Sire doesn't known that
        if freeze_residues.val:
            system = freezeResidues(system)

        system = setupForcefields(system, space)

        if random_seed.val:
            ranseed = random_seed.val
        else:
            ranseed = RanGenerator().randInt(100000,1000000)

        print("Setting up the simulation with random seed %s" % ranseed)

        moves = setupMoves(system, ranseed, gpu.val)

        print("Saving restart")
        Sire.Stream.save( [system, moves], restart_file.val )
    else:
        system, moves = Sire.Stream.load( restart_file.val )
        move0 =  moves.moves()[0]
        integrator = move0.integrator()
        integrator.setDeviceIndex(gpu.val)
        move0.setIntegrator(integrator.val)
        moves = WeightedMoves()
        moves.add(move0)
        print("Index GPU = %s " % moves.moves()[0].integrator().getDeviceIndex())
        print("Loaded a restart file on wich we have performed %d moves." % moves.nMoves())

    cycle_start = int(moves.nMoves() / nmoves.val)  + 1
    cycle_end = cycle_start + ncycles 

    if (save_coordinates.val):
        trajectory = setupDCD(dcd_root.val, system)

    s1 = timer.elapsed()/1000.

    print("Running MD simulation ")

    for i in range(cycle_start,cycle_end):
        print("\nCycle = ",i,"\n")

        print("Energy before = %s kJ mol-1" % (system.energy().to(kJ_per_mol)))
        system = moves.move(system, nmoves.val, True)
        print("Energy after = %s kJ mol-1" % (system.energy().to(kJ_per_mol)))

        if (save_coordinates.val):
            writeSystemData(system, moves, trajectory, i)

    s2 = timer.elapsed()/1000.
    print("Simulation took %d s " % ( s2 - s1))

    print("Saving restart")
    Sire.Stream.save( [system, moves], restart_file.val )

