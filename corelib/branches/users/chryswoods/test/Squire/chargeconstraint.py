
from Sire.IO import *
from Sire.Squire import *
from Sire.System import *
from Sire.Mol import *
from Sire.MM import *
from Sire.Maths import *
from Sire.Move import *

import os

mol = PDB().readMolecule("test/io/sb2.pdb")

mol = mol.edit().rename("sb2").commit()

protoms = ProtoMS("%s/Work/ProtoMS/protoms2" % os.getenv("HOME"))

protoms.addParameterFile("test/ff/sb2.ff")

mol = protoms.parameterise(mol, ProtoMS.SOLUTE)

print mol.property("charge").array()

qmff = QMFF("MopacFF")
qmff.setQuantumProgram( Mopac() )

qmff.add(mol)

intraff = InternalFF("IntraFF")
intraff.add(mol)

intraclj = IntraCLJFF("IntraCLJ")
intraclj.add(mol)

print intraff.energy()
print intraclj.energy()

print intraff.energy() + intraclj.energy()

print qmff.energy()

solute = MoleculeGroup("solute")
solute.add(mol)

system = System()

system.add(qmff)
system.add(intraff)
system.add(intraclj)

system.add(solute)

print system.energy()

chg_constraint = QMChargeConstraint(solute)
chg_constraint.setChargeCalculator( AM1BCC() )

print chg_constraint.isSatisfied(system)

mols = chg_constraint.update(system)

system.update(mols)

print chg_constraint.isSatisfied(system)

system.add(chg_constraint)

print system.energy()

print system.constraintsSatisfied()

mol = system[ MGIdx(0) ][ mol.number() ].molecule()

print mol.property("charge").array()

