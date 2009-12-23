
from Sire.IO import *
from Sire.Mol import *

import os

protomsdir = "%s/Work/ProtoMS" % os.getenv("HOME")

protoms = ProtoMS( "%s/protoms2" % protomsdir )

protoms.addParameterFile( "%s/parameter/amber99.ff" % protomsdir )
protoms.addParameterFile( "%s/parameter/gaff.ff" % protomsdir )
protoms.addParameterFile( "test/ff/sb2.ff" )

mol = PDB().readMolecule("test/io/sb2.pdb")

mol = mol.edit().rename("SB2").commit()

mol = protoms.parameterise(mol, ProtoMS.SOLUTE)

print mol.property("connectivity")
print mol.property("perturbations")

