
from Sire.IO import *

tinker = Tinker()

tinker.loadParameters("test/io/charmm27.prm")

mols = tinker.read("test/io/geom500.xyz")

print mols
print mols.nMolecules()

