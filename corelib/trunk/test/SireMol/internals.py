# Tests for the classes wrapped by internals.cpp

from Sire.Mol import *

b1 = Bond( ("C",1), ("H",1) )
b2 = Bond( ("H",1), ("C",1) )

assert( b1 == b2 )
assert( not b1 != b2 )
assert( not b1 < b2 )
assert( b1 <= b2 )
assert( not b1 > b2 )
assert( b1 >= b2 )

assert( b1.count() == 2 )
assert( b2.size() == 2 )

assert( "C" in b1 )
assert( ("C",1) in b1 )
assert( 1 in b1 )

assert( not "S" in b1 )
assert( not ("S",3) in b1 )
assert( not 3 in b1 )

assert( b1.intraResidue() )
assert( not b1.interResidue() )

a1 = Angle( ("C",1), ("N",1), ("O",2) )
a2 = Angle( ("O",2), ("N",1), ("C",1) )

assert( a1 == a2 )
assert( not a1 != a2 )
assert( not a1 < a2 )
assert( a1 <= a2 )
assert( not a1 > a2 )
assert( a1 >= a2 )

assert( a1.count() == 3 )
assert( a2.size() == 3 )

assert( "C" in a1 )
assert( ("C",1) in a1 )
assert( 1 in a1 )

assert( not "S" in a1 )
assert( not ("S",3) in a1 )
assert( not 3 in a1 )

assert( not a1.intraResidue() )
assert( a1.interResidue() )

d1 = Dihedral( ("C",1), ("N",1), ("O",2), ("C",2) )
d2 = Dihedral( ("C",2), ("O",2), ("N",1), ("C",1) )

assert( d1 == d2 )
assert( not d1 != d2 )
assert( not d1 < d2 )
assert( d1 <= d2 )
assert( not d1 > d2 )
assert( d1 >= d2 )

assert( d1.count() == 4 )
assert( d2.size() == 4 )

assert( "C" in d1 )
assert( ("C",1) in d1 )
assert( 1 in d1 )

assert( not "S" in d1 )
assert( not ("S",3) in d1 )
assert( not 3 in d1 )

assert( not d1.intraResidue() )
assert( d1.interResidue() )

i1 = Improper( ("CA",1), ("O",1), ("N",1), ("C",1) )
i2 = Improper( ("CA",1), ("O",1), ("N",1), ("C",1) )

assert( i1 == i2 )
assert( not i1 != i2 )
assert( not i1 < i2 )
assert( i1 <= i2 )
assert( not i1 > i2 )
assert( i1 >= i2 )

assert( i1.size() == 4 )
assert( i2.count() == 4 )

assert( "C" in i1 )
assert( ("C",1) in i1 )
assert( 1 in i1 )

assert( not "S" in i1 )
assert( not ("S",3) in i1 )
assert( not 3 in i1 )

assert( i1.intraResidue() )
assert( not i1.interResidue() )

