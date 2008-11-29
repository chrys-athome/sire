
from Sire.IO import *
from Sire.Mol import *

protodir = "/Users/chris/Work/ProtoMS"

print "Parameterising water"

water = PDB().readMolecule("test/io/water.pdb")

protoms = ProtoMS("%s/protoms2" % protodir)

protoms.addParameterFile( "%s/parameter/solvents.ff" % protodir )
protoms.addParameterFile( "%s/parameter/amber99.ff" % protodir )
protoms.addParameterFile( "%s/parameter/amber99-residues.ff" % protodir )
protoms.addParameterFile( "%s/parameter/gaff.ff" % protodir )

water = protoms.parameterise(water, ProtoMS.SOLVENT)

print "Parameterising ethane"

protoms.addParameterFile( "%s/tutorials/protoms-ethanemethanol/dual/in/ethane.par" % protodir )

ethane = PDB().readMolecule("%s/tutorials/protoms-ethanemethanol/dual/in/ethane.pdb" % protodir)

ethane = ethane.edit().rename("ethane").commit()

ethane = protoms.parameterise(ethane, ProtoMS.SOLUTE)

PDB().write(ethane, "test.pdb")
