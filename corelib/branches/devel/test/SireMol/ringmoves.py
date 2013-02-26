
from Sire.Mol import *
from Sire.IO import *
from Sire.Units import *

import Sire.Stream

osel = Sire.Stream.load("test/io/osel.s3")

PDB().write(osel, "test0000.pdb")

map = { "weight function" : RelFromMass() }

bond = BondID( AtomName("C2"), AtomName("C9") )
angle = AngleID( AtomName("C2"), AtomName("C9"), AtomName("C8") )
dihedral = DihedralID( AtomName("C2"), AtomName("C9"), AtomName("C8"), AtomName("C5") )

osel = osel.move().change(bond, 0.3*angstrom, map).commit()

PDB().write(osel, "test0001.pdb")

osel = osel.move().change(bond, -0.3*angstrom, map).commit()

PDB().write(osel, "test0002.pdb")

osel = osel.move().change(angle, 15*degrees, map).commit()

PDB().write(osel, "test0003.pdb")

osel = osel.move().change(angle, -15*degrees, map).commit()

PDB().write(osel, "test0004.pdb")

osel = osel.move().change(dihedral, 30*degrees, map).commit()

PDB().write(osel, "test0005.pdb")

osel = osel.move().change(dihedral, -30*degrees, map).commit()

PDB().write(osel, "test0006.pdb")

