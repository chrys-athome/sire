
from Sire.IO import *
from Sire.MM import *
from Sire.Mol import *
from Sire.Move import *
from Sire.MM import *
from Sire.System import *
from Sire.Vol import *
from Sire.Units import *

protodir = "/Users/chris/Work/ProtoMS"

print "Parameterising indole"

protoms = ProtoMS( "%s/protoms2" % protodir )

protoms.addParameterFile( "%s/parameter/gaff.ff" % protodir )
protoms.addParameterFile( "%s/tutorials/protoms-setup-ligand/in/indolefree.template" % protodir )

indole = PDB().readMolecule("%s/tutorials/protoms-setup-ligand/in/indoleh.pdb" % protodir)

indole = indole.edit().rename("indole").commit()

indole = protoms.parameterise(indole, ProtoMS.SOLUTE)

print indole.property("charge").array()
print indole.property("LJ").array()

zmat = indole.property("z-matrix")

for line in zmat.lines():
    print line, line.bondDelta(), line.angleDelta().to(degrees), \
                                  line.dihedralDelta().to(degrees)

internalff = InternalFF("InternalFF")

internalff.add( indole )

print "Total = %f" % internalff.energy().to(kcal_per_mol)
print "Bond = %f" % internalff.energy( internalff.components().bond() ).to(kcal_per_mol)
print "Angle = %f" % internalff.energy( internalff.components().angle() ).to(kcal_per_mol)
print "Dihedral = %f" % internalff.energy( internalff.components().dihedral() ).to(kcal_per_mol)

intraclj = IntraCLJFF("IntraCLJFF")

intraclj.add( indole )

print "IntraCLJ = %f" % intraclj.energy().to(kcal_per_mol)
print "Intra Coul = %f" % intraclj.energy( intraclj.components().coulomb() ).to(kcal_per_mol)
print "Intra LJ   = %f" % intraclj.energy( intraclj.components().lj() ).to(kcal_per_mol)

system = System()

system.add( internalff )
system.add( intraclj )

system.setProperty( "space", Cartesian() )
system.setProperty( "switchingFunction", NoCutoff() )

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
