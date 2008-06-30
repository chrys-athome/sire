from Sire.Mol import *
from Sire.MM import *
from Sire.FF import *
from Sire.IO import *
from Sire.Maths import *
from Sire.Units import *

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

print "\nTesting an internal forcefield...\n"

internalff = InternalFF()

bondfuncs = TwoAtomFunctions(tip4p)

r = internalff.symbols().bond().r()

bondfuncs.set( O00.index(), H01.index(), 5 * ( 5.0 - r )**2 )
bondfuncs.set( O00.index(), H02.index(), 5 * ( 5.0 - r )**2 )

anglefuncs = ThreeAtomFunctions(tip4p)

theta = internalff.symbols().angle().theta()

anglefuncs.set( H01.index(), O00.index(), H02.index(), 3 * ( (90*degrees).value() - theta )**2 )

tip4p = tip4p.edit().setProperty( "bond", bondfuncs ) \
                    .setProperty( "angle", anglefuncs ) \
             .commit()

internalff.add( tip4p )

print internalff.energy()
print internalff.energy( internalff.components().bond() )
print internalff.energy( internalff.components().angle() )
