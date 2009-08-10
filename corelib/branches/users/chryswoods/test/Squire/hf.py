
from Sire.Squire import *
from Sire.Maths import *

A = S_GTO(Vector(0,0,0), 0.5)
B = S_GTO(Vector(0,0,1.4), 0.5)
C = PointCharge(Vector(0,0,0), 1.0)

print "OVERLAP ",overlap_integral(A,B)
print "KINETIC ",kinetic_integral(A,B)
print "POTENTIAL ",potential_integral(C, A,B)

#basis
#0.0 0.0 0.0  0  0.5
#0.0 0.0 0.0  0  1.0
#
#1.4 0.0 0.0  0  0.5
#1.4 0.0 0.0  0  1.0
#
#charges
#0.0 0.0 0.0 1.0
#1.4 0.0 0.0 1.0

hf = HF()

hf.add( S_GTO(Vector(0,0,0), 0.5) )
hf.add( S_GTO(Vector(0,0,0), 1.0) )

hf.add( S_GTO(Vector(1.4,0,0), 0.5) )
hf.add( S_GTO(Vector(1.4,0,0), 1.0) )

hf.add( PointCharge(Vector(0,0,0), 1.0) )
hf.add( PointCharge(Vector(1.4,0,0), 1.0) )

hf.solve()

