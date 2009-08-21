
from Sire.Squire import *
from Sire.Maths import *
from Sire.Units import *

p0 = Vector(0,0,0)
s0 = S_GTO( 0.27095*2.0925*2.0925 )

p1 = Vector(1.4632,0,0)
s1 = S_GTO( 0.27095*1.24*1.24 )

print s0.alpha(), s0.scale(), s1.alpha(), s1.scale(), Vector.distance2(p0,p1)

n0 = PointCharge( Vector(0,0,0), 2*mod_electron )
n1 = PointCharge( Vector(1.4632,0,0), 1*mod_electron )

print overlap_integral( SS_GTO(p0,s0, p0,s0) )
print overlap_integral( SS_GTO(p0,s0, p1,s1) )
print overlap_integral( SS_GTO(p1,s1, p1,s1) )

hf = HF()

hf.add(p0, s0)
hf.add(p1, s1)
hf.add(n0.center(), n0.charge()*mod_electron)
hf.add(n1.center(), n1.charge()*mod_electron)

hf.solve()

