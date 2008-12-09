
from Sire.IO import *
from Sire.MM import *
from Sire.Mol import *
from Sire.Move import *
from Sire.MM import *
from Sire.System import *
from Sire.Vol import *
from Sire.Units import *

import sys

protodir = "/Users/chris/Work/ProtoMS"

print "Parameterising COX2"

protoms = ProtoMS( "%s/protoms2" % protodir )

protoms.addParameterFile( "%s/parameter/amber99.ff" % protodir )
protoms.addParameterFile( "%s/parameter/amber99-residues.ff" % protodir )
protoms.addParameterFile( "%s/parameter/gaff.ff" % protodir )

cox2 = PDB().readMolecule("test/io/cox2.pdb")

cox2 = cox2.edit().rename("COX2").commit()

cox2 = protoms.parameterise(cox2, ProtoMS.PROTEIN)

zmat = cox2.property("z-matrix")

for line in zmat.lines():
    print line, line.bondDelta(), line.angleDelta().to(degrees), \
                                  line.dihedralDelta().to(degrees)

internalff = InternalFF("InternalFF")

internalff.add( cox2 )

print "Total = %f" % internalff.energy().to(kcal_per_mol)
print "Bond = %f" % internalff.energy( internalff.components().bond() ).to(kcal_per_mol)
print "Angle = %f" % internalff.energy( internalff.components().angle() ).to(kcal_per_mol)
print "Dihedral = %f" % internalff.energy( internalff.components().dihedral() ).to(kcal_per_mol)

intraclj = IntraCLJFF("IntraCLJFF")

intraclj.add( cox2 )

print "IntraCLJ = %f" % intraclj.energy().to(kcal_per_mol)
print "Intra Coul = %f" % intraclj.energy( intraclj.components().coulomb() ).to(kcal_per_mol)
print "Intra LJ   = %f" % intraclj.energy( intraclj.components().lj() ).to(kcal_per_mol)

system = System()

system.add( internalff )
system.add( intraclj )

system.setProperty( "space", Cartesian() )
system.setProperty( "switchingFunction", NoCutoff() )

residues = MoleculeGroup("residues")

cox2_residues = cox2.residues()

for i in range(0, cox2_residues.count()):
    residues.add( cox2_residues[i] )

system.add( residues )

zmatmove = ZMatMove(residues)

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
