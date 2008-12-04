
from Sire.IO import *
from Sire.MM import *
from Sire.Mol import *
from Sire.Move import *
from Sire.MM import *
from Sire.System import *
from Sire.Vol import *
from Sire.Units import *

protodir = "/Users/chris/Work/ProtoMS"

print "Parameterising the oscillator..."

oscillator = PDB().readMolecule("test/io/oscillator.pdb")
oscillator = oscillator.edit().rename("harm1t2").commit()

protoms = ProtoMS("%s/protoms2" % protodir)

protoms.addParameterFile( "test/io/oscillators.ff" )

oscillator = protoms.parameterise(oscillator, ProtoMS.SOLUTE)

print "...parameterisation complete!"

internalff = InternalFF("InternalFF")

internalff.add( oscillator )

system = System()
system.add( internalff )

system.add( "average energy", MonitorComponent(system.totalComponent()) )

zmatmove = ZMatMove( internalff.groups()[0] )

print "Running a simulation - initial energy = %f kcal mol-1" % system.energy().to(kcal_per_mol)

for i in range(0,100):
    sim = Simulation.run( system, zmatmove, 10000 )
    
    system = sim.system()
    zmatmove = sim.moves()[0]

    PDB().write( system.molecules(), "test%003d.pdb" % (i+1) )

    print "%d : Energy = %f kcal mol-1" % ( (i+1)*10000, \
                                            system.energy().to(kcal_per_mol) )

    print "      Average energy = %f kcal mol-1" % system[MonitorName("average energy")] \
                                                       .accumulator().average()

print "Finished - final energy = %f kcal mol-1" % system.energy().to(kcal_per_mol)
print "         - final average = %f kcal mol-1" % system[MonitorName("average energy")] \
                                                       .accumulator().average()

print zmatmove
