
from Sire.IO import *
from Sire.MM import *
from Sire.Mol import *
from Sire.Move import *
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

zmat = ethane.property("zmatrix")

for line in zmat.lines():
    print line, line.bondDelta(), line.angleDelta().to(degrees), \
                                  line.dihedralDelta().to(degrees)

zmat = ZMatrixCoords(zmat, ethane)

for line in zmat.lines():
    print line

for i in range (0,180,10):
    zmat.setDihedral( AtomName("H02"), AtomName("C01"), AtomName("C05"), \
                      AtomName("H06"), i * degrees )

    ethane = ethane.edit().setProperty("coordinates", zmat.toCartesian() ).commit()

    PDB().write( ethane, "test%003d.pdb" % i )
