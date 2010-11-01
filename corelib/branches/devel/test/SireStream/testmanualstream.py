
from Sire.Mol import *
from Sire.IO import *
from Sire.Vol import *
from Sire.Maths import *
from Sire.Stream import *
from Sire.Qt import *

mol = PDB().readMolecule("test/io/water.pdb")
space = PeriodicBox( Vector(1,2,3), Vector(4,5,6) )

print mol
print space

f = QByteArray()

ds = QDataStream()

ds.setDevice(f)

ds << mol
ds << space

