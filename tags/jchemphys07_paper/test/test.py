
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

if len(sys.argv) < 3:
    print "USAGE: test.py input_file lambda"
    sys.exit(-1)
    
inpfile = sys.argv[1]
lamval = float(sys.argv[2])

if lamval < 0:
   lamval = 0
elif lamval > 1:
   lamval = 1

print "Running lambda == %f, input file == %s" % (lamval,inpfile)

timer = QTime()

#read in the molecules
print "Loading the input files..."
mols = PDB().read(inpfile)

def renumberResidues(mols, newnum):
    newmols = []
    
    for mol in mols:
       editor = mol.edit()
       editor.renumberResidue(mol.atoms()[0].resNum(), newnum)
       newmols.append( Molecule(editor) )
       
    return newmols

mols = renumberResidues(mols, ResNum(1))

solute = mols[0]
solvent = mols[1:]

#specify the space in which the molecules are placed
import os
line = os.popen("grep \"HEADER box\" %s" % inpfile, "r").readline()
words = line.split()

mincoords = Vector( float(words[2]), float(words[3]), float(words[4]) )
maxcoords = Vector( float(words[5]), float(words[6]), float(words[7]) )

print "Box dimension: %s to %s" % (mincoords,maxcoords)

space = PeriodicBox( mincoords, maxcoords )

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

solvent_tip4p_chgs = AtomicCharges( [0.0, 
                                     0.52 * mod_electron,
                                     0.52 * mod_electron,
                                    -1.04 * mod_electron] )

solvent_tip4p_ljs = AtomicLJs( [ LJParameter( 3.15365 * angstrom, \
                                              0.1550 * kcal_per_mol ), \
                                 LJParameter.dummy(), \
                                 LJParameter.dummy(), \
                                 LJParameter.dummy() ] )

solute_tip4p_chgs = AtomicCharges( [ 0.0,
                                     0.52 * mod_electron,
                                     0.52 * mod_electron,
                                    -1.04 * mod_electron,
                                     0.0,
                                     0.0 ] )

solute_tip4p_ljs = AtomicLJs( [ LJParameter( 3.15365 * angstrom, \
                                             0.1550 * kcal_per_mol ), \
                                LJParameter.dummy(),
                                LJParameter.dummy(),
                                LJParameter.dummy(),
                                LJParameter.dummy(),
                                LJParameter.dummy() ] )

solute_chgs = solute_tip4p_chgs
solute_ljs = solute_tip4p_ljs

solvent_chgs = solvent_tip4p_chgs
solvent_ljs = solvent_tip4p_ljs

#first parametise the solute and add it to the right forcefields
solute.setProperty( "charges", solute_chgs )
solute.setProperty( "ljs", solute_ljs )

mmff.addToA( solute, {mmff.parameters().coulomb() : "charges",
                      mmff.parameters().lj() : "ljs"} )

qmff.addToQM( solute )

# QM energies are absolute, while MM energies are relative. We
# will set the QM energy relative to the QM energy of the gas-phase
# solute molecule
timer.start()
qmff.setEnergyOrigin( qmff.energy() )

print "Gas-phase calculation took %d ms" % timer.elapsed()

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
ffields.setTotal(e_total)

print "Calculating initial energies..."
timer.start()

# let's see what the initial values of these functions are...
print "e_fast == %f kcal mol-1 (%d ms)" % (ffields.energy(e_fast), timer.elapsed())
print "e_slow == %f kcal mol-1 (%d ms)" % (ffields.energy(e_slow), timer.elapsed())
print "e_total == %f kcal mol-1 (%d ms)" % (ffields.energy(e_total), timer.elapsed())
print "de_by_dlam == %f kcal mol-1 (%d ms)" % (ffields.energy(de_by_dlam), timer.elapsed())

print "Took %d ms" % timer.elapsed()

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

#monitor the de/dlambda and the total energy
monitors = SystemMonitors()
monitors.set( e_total.function(), MonitorEnergy(e_total.function()) )
monitors.set( de_by_dlam.function(), MonitorEnergy(de_by_dlam.function()) )

#also monitor the RDF from the solute to the solvent
rdfmonitor = RDFMonitor(solute, solvent_group)

water_oxygen = [ ("o00",ResNum(1)) ]
water_hydrogen = [ ("h01",ResNum(1)), ("h02",ResNum(1)) ]

ch4_carbon = [ ("c",ResNum(1)) ]
ch4_hydrogen = [ ("h1",ResNum(1)), ("h2",ResNum(1)),
                 ("h3",ResNum(1)), ("h4",ResNum(1)) ]

#need to choose whether methane or water sim
rdfmonitor.addRDF( water_oxygen, water_oxygen, RDF(0,15,100) )
rdfmonitor.addRDF( water_oxygen, water_hydrogen, RDF(0,15,100) )
rdfmonitor.addRDF( water_hydrogen, water_oxygen, RDF(0,15,100) )
rdfmonitor.addRDF( water_hydrogen, water_hydrogen, RDF(0,15,100) )

monitors.set(Symbol("RDF"), rdfmonitor)

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
solv_mc.setMaximumTranslation( 0.2 * angstrom )
solv_mc.setMaximumRotation( 5 * degrees )

# create a rigid body MC move that moves the solute molecule
solu_mc = RigidBodyMC( UniformSampler(solute_group) )
solu_mc.setMaximumTranslation( 0.1 * angstrom )
solu_mc.setMaximumRotation( 2.5 * degrees )

solu_mc.setTemperature(temperature)

# create a volume move that moves everything using NPT
vol_mc = VolumeMove( MapAsCutGroups(all_mols) )

vol_mc.setTemperature(temperature)
vol_mc.setPressure(pressure)
vol_mc.setVolumeChangingFunction( UniformVolumeChange(167.9 * angstrom3) )

# add all of the moves to a selection of moves, giving each
# one a weight
moves = WeightedMoves()

moves.add(solv_mc, 1600)
moves.add(solu_mc, 100)
moves.add(vol_mc, 1)

# Create a multiple-time-step MC move that performs 500
# rigid body moves of the solvent using e_fast, then accepts
# or rejects the whole block of moves using e_total
mtsmc = MTSMC(moves, e_fast.function(), 500)

mtsmc.setEnergyComponent(e_total.function())

nmoves = 100

timer.start()

print "Initial Energy = %f kcal mol-1" % system.forceFields().energy()
print "Initial Volume = %f A^3" % system.info().space().volume()

moves = system.run(mtsmc,nmoves)

ms = timer.elapsed()
print "%d moves took %d ms" % (nmoves, ms)

print "Final Energy = %f kcal mol-1" % system.forceFields().energy()
print "Final Volume = %f A^3" % system.info().space().volume()

mtsmc = moves.moves()[0]

print "%d accepted, %d rejected, ratio == %f %%" % \
           (mtsmc.nAccepted(), mtsmc.nRejected(), mtsmc.acceptanceRatio())

solv_mc = mtsmc.moves()[0]
solu_mc = mtsmc.moves()[1]
vol_mc = mtsmc.moves()[2]
    
print "Solute: %d accepted, %d rejected." % \
                 (solu_mc.nAccepted(), solu_mc.nRejected())
                     
print "Solvent: %d accepted, %d rejected." % \
                 (solv_mc.nAccepted(), solv_mc.nRejected())
                     
print "Volume: %d accepted, %d rejected." % \
                 (vol_mc.nAccepted(), vol_mc.nRejected())                     

gradmon = system.monitors().monitor(de_by_dlam.function())
totalmon = system.monitors().monitor(e_total.function())

print "AVGS: %f (%d), %f (%d)" % (gradmon.average(), gradmon.nUpdates(),
                                  totalmon.average(), totalmon.nUpdates())

rdfmon = system.monitors().monitor(Symbol("RDF"))
    
print "OXYGEN-OXYGEN"
    
for point in rdfmon.getRDF(water_oxygen, water_oxygen).normalise():
   print "%f  %f" % point
    
#now print out the RETI energies
ffields = system.forceFields()

lamf = lamval + 0.05
lamb = lamval - 0.05

if (lamf > 1):
   lamf = 1
   
if (lamb < 0):
   lamb = 0
   
ffields.setParameter(lam, lamf)
print "Lambda = %f, energy = %f" % (lamf, ffields.energy())
ffields.setParameter(lam, lamb)
print "Lambda = %f, energy = %f" % (lamb, ffields.energy())

