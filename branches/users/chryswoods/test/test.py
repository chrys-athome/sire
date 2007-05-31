
from Sire.Mol import *
from Sire.IO import *
from Sire.Vol import *
from Sire.FF import *
from Sire.MM import *
from Sire.CAS import *
from Sire.Maths import *
from Sire.Qt import *
from Sire.Units import *
from Sire.System import *
from Sire.Move import *
from Sire.Squire import *
from Sire.Base import *

import sys

if len(sys.argv) < 5:
    print "USAGE: test.py input_file lambda start stop"
    sys.exit(-1)
    
inpfile = sys.argv[1]
lamval = float(sys.argv[2])
start = int(sys.argv[3])
stop = int(sys.argv[4])

if lamval < 0:
   lamval = 0
elif lamval > 1:
   lamval = 1

print "Running lambda == %f, input file == %s" % (lamval,inpfile)

timer = QTime()

#read in the molecules
print "Loading the input files..."
mols = PDB().read(inpfile)

solute = mols[0]
solvent = mols[1:]

#specify the space in which the molecules are placed
space = PeriodicBox( (-18.4040,-18.68748,-18.4632),
                     ( 18.4040, 18.68748, 18.4632) )

#specify the type of switching function to use
switchfunc = HarmonicSwitchingFunction(15.0, 14.5)

#create a forcefield to calculate the solvent-solvent energy
solventff = InterCLJFF(space, switchfunc)
solventff.setName("SolventFF")

#create a forcefield to calculate the MM solute-solvent energy
mmff = InterGroupCLJFF(space, switchfunc)
mmff.setName("MMFF")

#create a forcefield to calculate the QM solute-solvent energy
qmff = MolproFF(space, switchfunc)
qmff.setMolproExe( findExe("molpro") )
qmff.setName("QMFF")

qm_prog = "HF\nMP2"
qm_basis = "VDZ"

qmff.setProgram(qm_prog)
qmff.setBasisSet(qm_basis)

#parametise each molecule and add it to the forcefields
print "Parametising the molecules..."

solvent_chgs = AtomicCharges( [0.0, 0.52 * mod_electron,
                               0.52 * mod_electron,
                              -1.04 * mod_electron] )

solvent_ljs = AtomicLJs( [ LJParameter( 3.15365 * angstrom, \
                                        0.1550 * kcal_per_mol ), \
                           LJParameter.dummy(), \
                           LJParameter.dummy(), \
                           LJParameter.dummy() ] )

solute_chgs = AtomicCharges( [-0.24 * mod_electron, 
                               0.06 * mod_electron,
                               0.06 * mod_electron,
                               0.06 * mod_electron,
                               0.06 * mod_electron] )

solute_ljs = AtomicLJs( [ LJParameter( 3.5000 * angstrom, \
                                       0.0660 * kcal_per_mol ), \
                          LJParameter( 2.5000 * angstrom,
                                       0.0300 * angstrom), \
                          LJParameter( 2.5000 * angstrom,
                                       0.0300 * angstrom), \
                          LJParameter( 2.5000 * angstrom,
                                       0.0300 * angstrom), \
                          LJParameter( 2.5000 * angstrom,
                                       0.0300 * angstrom) ] )

#first parametise the solute and add it to the right forcefields
solute.setProperty( "charges", solute_chgs )
solute.setProperty( "ljs", solute_ljs )

mmff.addToA( solute, {mmff.parameters().coulomb() : "charges",
                      mmff.parameters().lj() : "ljs"} )

qmff.addToQM( solute )

# QM energies are absolute, while MM energies are relative. We
# will set the QM energy relative to the QM energy of the gas-phase
# methane molecule
qmff.setEnergyOrigin( qmff.energy() )

#now parametise the solvent molecules and add them to the right
#forcefields
for mol in solvent:
    mol.setProperty( "charges", solvent_chgs )
    mol.setProperty( "ljs", solvent_ljs )

solventff.add( solvent, {solventff.parameters().coulomb() : "charges",
                         solventff.parameters().lj() : "ljs"} )
                         
mmff.addToB( solvent, {mmff.parameters().coulomb() : "charges",
                       mmff.parameters().lj() : "ljs"} )
                       
qmff.addToMM( solvent, {qmff.parameters().coulomb() : "charges"} )

#add all of the forcefields to a forcefields object
ffields = ForceFields()
ffields.add(solventff)
ffields.add(mmff)
ffields.add(qmff)

# there are two ways of calculating the total energy - either
# the slow way, using the QM solute-solvent interaction or
# the fast way, using the MM approximation of the solute-solvent
# interaction

e_fast = FFExpression( "e_fast", solventff.components().total() + 
                                 mmff.components().total() )

e_slow = FFExpression( "e_slow", solventff.components().total() + 
                                 mmff.components().lj() + 
                                 qmff.components().total() )

# we will perform a free energy simulation that perturbs from 
# e_slow to e_fast. This will allow us to calculate the correction
# free energy from e_fast to e_slow
#
#  We do this by using lambda to scale from e_slow to e_fast, e.g.
#   e_total = [(1-lambda) * e_slow] + [lambda * e_fast]

lam = Symbol("lambda")

e_total = FFExpression("e_total", ( (1-lam) * e_slow.function() ) 
                                    + ( lam * e_fast.function() ))

# the free energy gradient moving from e_slow to e_fast is given
# by the difference between e_fast and e_slow (dE / dlambda)
#
# Create an equation for this gradient so that we can monitor
# it during the simulation
de_by_dlam = FFExpression("de_by_dlam", e_fast.function() 
                                          - e_slow.function())

# add all of these energy components to the forcefields
ffields.add(e_fast)
ffields.add(e_slow)
ffields.add(e_total)
ffields.add(de_by_dlam)

# start at lambda = lamval
ffields.setParameter(lam, lamval)

print "Calculating initial energies..."
timer.start()

# let's see what the initial values of these functions are...
print "e_fast == %f kcal mol-1" % ffields.energy(e_fast)
print "e_slow == %f kcal mol-1" % ffields.energy(e_slow)
print "e_total == %f kcal mol-1" % ffields.energy(e_total)
print "de_by_dlam == %f kcal mol-1" % ffields.energy(de_by_dlam)

print "Took %d ms" % timer.elapsed()

sys.exit(0)

#monitor the de/dlambda and the total energy
monitors = SystemMonitors()
monitors.set( e_total.function(), MonitorEnergy(e_total.function()) )
monitors.set( de_by_dlam.function(), MonitorEnergy(de_by_dlam.function()) )

#there are two groups of molecules, the solute and the solvent
# - lets create these groups (moves move random molecules from a group)
solvent_group = MoleculeGroup("solvent", solvent)
solute_group = MoleculeGroup("solute", solute)

all_mols = MoleculeGroup("all", solvent)
all_mols.add(solute)

groups = MoleculeGroups()
groups.add(solvent_group)
groups.add(solute_group)
groups.add(all_mols)

# create a system that can be used to simulate these groups
# using the forcefields that we have constructed
system = System( groups, ffields, monitors )

# set the space in which the simulation will occur
system.setSpace(space)

temperature = 25 * celsius
pressure = 1 * atm

# create a rigid body MC move that moves random solvent
# molecules, with the solvent molecules picked according
# to how close they are to the solute
solv_mc = RigidBodyMC( PrefSampler(solute, 200.0, solvent_group) )

solv_mc.setTemperature(temperature)

# create a rigid body MC move that moves the solute molecule
solu_mc = RigidBodyMC( solute_group )

solu_mc.setTemperature(temperature)

# create a volume move that moves everything using NPT
vol_mc = VolumeMove( all_mols )

vol_mc.setTemperature(temperature)
vol_mc.setPressure(pressure)

# add all of the moves to a selection of moves, giving each
# one a weight
moves = WeightedMoves()

moves.add(solv_mc, 1)
moves.add(solu_mc, 1)
moves.add(vol_mc, 1)

# Create a multiple-time-step MC move that performs 500
# rigid body moves of the solvent using e_fast, then accepts
# or rejects the whole block of moves using e_total
mtsmc = MTSMC(moves, e_fast.function(), 500)

mtsmc.setEnergyComponent(e_total.function())

PDB().write(system.info().groups().molecules(), "output_%003d_dup.pdb" % (start-1))

nmoves = 100

for i in range(start,stop+1):

    timer.start()

    moves = system.run(mtsmc,nmoves)

    ms = timer.elapsed()

    print "%d moves took %d ms" % (nmoves, ms)

    mtsmc = moves.moves()[0].clone()

    print "%d accepted, %d rejected, ratio == %f %%" % \
               (mtsmc.nAccepted(), mtsmc.nRejected(), mtsmc.acceptanceRatio())


    # check that the QM and MM energies have been conserved...
    new_molpro = MolproFF(space, switchfunc)
    new_molpro.setMolproExe( qmff.molproExe() )
    new_molpro.setEnergyOrigin( qmff.energyOrigin() )

    system_qmff = system.forceFields().forceField(qmff.ID())

    new_molpro.addToQM( system_qmff.molecules(qmff.groups().qm()) )
    new_molpro.addToMM( system_qmff.molecules(qmff.groups().mm()), 
                        {new_molpro.parameters().coulomb() : "charges"} )

    print "QM energy = %f, new molproff = %f" % \
                ( system.forceFields().forceField(qmff.ID()).energy(),
                  new_molpro.energy() )

    qmff.change(system.forceFields().molecules())

    print "qmff == %f" % qmff.energy()

    gradmon = system.monitors().monitor(de_by_dlam.function())
    totalmon = system.monitors().monitor(e_total.function())

    print "AVGS: %f (%d), %f (%d)" % (gradmon.average(), gradmon.nUpdates(),
                                      totalmon.average(), totalmon.nUpdates())

    PDB().write(system.info().groups().molecules(), "output_%003d.pdb" % i)
