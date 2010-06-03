
from Sire.IO import *
from Sire.Mol import *
from Sire.MM import *
from Sire.System import *
from Sire.Move import *
from Sire.Base import *
from Sire.FF import *
from Sire.Maths import *
from Sire.Units import *

import os

ethane = PDB().readMolecule("test/io/ethane.pdb")

protoms_dir = "%s/Work/ProtoMS" % os.getenv("HOME")

protoms = ProtoMS("%s/protoms2" % protoms_dir)
protoms.addParameterFile("test/io/ethane.ff")

ethane = ethane.edit().rename("ethane").commit()
ethane = protoms.parameterise(ethane, ProtoMS.SOLUTE)

intraff = InternalFF("intraff")
intraclj = IntraCLJFF("intraclj")

intraff.add(ethane)
intraclj.add(ethane)

solute = MoleculeGroup("solute", ethane)
solute.add(ethane)

system = System()
system.add(intraff)
#system.add(intraclj)
system.add(solute)

md = MolecularDynamics(solute, VelocityVerlet(),
                       {"velocity generator" : MaxwellBoltzmann(25*celsius)})
md.setTimeStep(1*femtosecond)

PDB().write(system.molecules(), "test0000.pdb")

for i in range(1,11):
    md.move(system, 10)
    PDB().write(system.molecules(), "test%0004d.pdb" % i)
