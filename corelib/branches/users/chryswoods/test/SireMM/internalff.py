from Sire.Mol import *
from Sire.MM import *
from Sire.FF import *
from Sire.IO import *
from Sire.Maths import *

tip4p = PDB().read("test/io/water.pdb").moleculeAt(0).molecule()

bonds = Connectivity(tip4p)

O00 = tip4p.atom( AtomName("O00") )
H01 = tip4p.atom( AtomName("H01") )
H02 = tip4p.atom( AtomName("H02") )
M03 = tip4p.atom( AtomName("M03") )

for i in range(0,4):
    print i, tip4p.atom( AtomIdx(i) ).name(), bonds.connectionsTo( AtomIdx(i) )

print Vector.distance( O00.property("coordinates"), M03.property("coordinates") )
print Vector.distance( H01.property("coordinates"), M03.property("coordinates") )
print Vector.distance( O00.property("coordinates"), H01.property("coordinates") )

def testSplit(atom0, atom1):
    print "\nSplitting by %s and %s" % (atom0.name(), atom1.name())

    group0, group1 = bonds.split( atom0.index(), atom1.index() )

    print group0.selectedAtoms()
    print group1.selectedAtoms()

testSplit(O00, H01)
testSplit(H01, O00)
testSplit(M03, O00)
testSplit(H02, O00)

internalff = InternalFF()

bondfuncs = TwoAtomFunctions(tip4p)

r = internalff.symbols().bond().r()

bondfuncs.set( O00, H01, 5 * pow( 1.0 - r, 2 ))
