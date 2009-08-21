
from Sire.Squire import *
from Sire.Maths import *
from Sire.Units import *

A = S_GTO(0.5, 1)
B = S_GTO(0.5, 1)
C = PointCharge( Vector(0,0,0), 1*mod_electron )

AB = SS_GTO( Vector(0,0,0), A, Vector(0,0,1.4), B )

print "OVERLAP ",overlap_integral(AB)
print "KINETIC ",kinetic_integral(AB)
print "POTENTIAL ",potential_integral(C, AB)
print "ELECTRON ",electron_integral(AB, AB)

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

#hf = HF()

#hf.add( S_GTO(Vector(0,0,0), 0.5) )
#hf.add( S_GTO(Vector(0,0,0), 1.0) )

#hf.add( S_GTO(Vector(1.4,0,0), 0.5) )
#hf.add( S_GTO(Vector(1.4,0,0), 1.0) )

#hf.add( PointCharge(Vector(0,0,0), 1.0) )
#hf.add( PointCharge(Vector(1.4,0,0), 1.0) )

#hf.solve()
