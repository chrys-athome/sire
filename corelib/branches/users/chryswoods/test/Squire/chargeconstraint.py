
from Sire.IO import *
from Sire.Squire import *
from Sire.System import *
from Sire.Mol import *
from Sire.MM import *
from Sire.Maths import *
from Sire.Move import *

import os

mol = PDB().readMolecule("test/io/methanol.pdb")

mol = mol.edit().rename("methanol").commit()

protoms = ProtoMS("%s/Work/ProtoMS/protoms2" % os.getenv("HOME"))

protoms.addParameterFile("test/ff/methanol.par")
protoms.addParameterFile("test/ff/solvents.ff")

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

water = PDB().read("test/io/water.pdb")

tip4p = water[ MolIdx(0) ].molecule()

tip4p = protoms.parameterise(tip4p, ProtoMS.SOLVENT)

chgs = tip4p.property("charge")
ljs = tip4p.property("LJ")

for i in range(0, water.nMolecules()):
    tip4p = water[ MolIdx(i) ].molecule()
    tip4p = tip4p.edit().setProperty("charge", chgs) \
                        .setProperty("LJ", ljs) \
                 .commit()

    water.update(tip4p)

cljff = InterGroupCLJFF("solute-solvent")

cljff.add( mol, MGIdx(0) )
cljff.add( water, MGIdx(1) )

system.add(cljff)

waterff = InterCLJFF("solvent")
waterff.add( water )

system.add(waterff)

all = MoleculeGroup("all")
all.add(mol)
all.add(water)

moves = RigidBodyMC(all)

moves.move( system, 100 )

