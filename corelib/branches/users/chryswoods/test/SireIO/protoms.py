
from Sire.IO import *
from Sire.MM import *
from Sire.Mol import *
from Sire.Move import *
from Sire.MM import *
from Sire.System import *
from Sire.Vol import *
from Sire.Units import *

protodir = "/Users/chris/Work/ProtoMS"

print "Parameterising water"

water = PDB().readMolecule("test/io/water.pdb")

protoms = ProtoMS("%s/protoms2" % protodir)

protoms.addParameterFile( "%s/parameter/solvents.ff" % protodir )
protoms.addParameterFile( "%s/parameter/amber99.ff" % protodir )
protoms.addParameterFile( "%s/parameter/amber99-residues.ff" % protodir )
protoms.addParameterFile( "%s/parameter/gaff.ff" % protodir )

water = protoms.parameterise(water, ProtoMS.SOLVENT)

print water.property("charge").array()
print water.property("LJ").array()

print "Parameterising ethane"

protoms.addParameterFile( "%s/tutorials/protoms-ethanemethanol/dual/in/ethane.par" % protodir )

ethane = PDB().readMolecule("%s/tutorials/protoms-ethanemethanol/dual/in/ethane.pdb" % protodir)

ethane = ethane.edit().rename("ethane").commit()

ethane = protoms.parameterise(ethane, ProtoMS.SOLUTE)

print ethane.property("charge").array()
print ethane.property("LJ").array()

zmat = ethane.property("z-matrix")

for line in zmat.lines():
    print line, line.bondDelta(), line.angleDelta().to(degrees), \
                                  line.dihedralDelta().to(degrees)

internalff = InternalFF("InternalFF")

internalff.add( ethane )

print "Total = %f" % internalff.energy().to(kcal_per_mol)
print "Bond = %f" % internalff.energy( internalff.components().bond() ).to(kcal_per_mol)
print "Angle = %f" % internalff.energy( internalff.components().angle() ).to(kcal_per_mol)
print "Dihedral = %f" % internalff.energy( internalff.components().dihedral() ).to(kcal_per_mol)

system = System()
system.setProperty("space", Cartesian())

system.add( internalff )

zmatmove = ZMatMove( internalff.groups()[0] )

PDB().write( system.molecules(), "test000.pdb" )

print "Running a simulation - initial energy = %f kcal mol-1" % system.energy().to(kcal_per_mol)

for i in range(0,100):

    sim = Simulation.run( system, zmatmove, 1000 )
    
    system = sim.system()
    zmatmove = sim.moves()[0]

    PDB().write( system.molecules(), "test%003d.pdb" % (i+1) )

    print "%d : Energy = %f kcal mol-1" % ( (i+1)*1000, \
                                            system.energy().to(kcal_per_mol) )

print "Finished - final energy = %f kcal mol-1" % system.energy().to(kcal_per_mol)
print zmatmove
