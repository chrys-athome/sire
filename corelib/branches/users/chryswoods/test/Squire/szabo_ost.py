
from Sire.Squire import *
from Sire.Maths import *

s0 = S_GTO( Vector(0,0,0), 0.27095*2.0925*2.0925 )
s1 = S_GTO( Vector(1.4632,0,0), 0.27095*1.24*1.24 )

print s0.alpha(), s0.scale(), s1.alpha(), s1.scale(), Vector.distance2(s0.center(), s1.center())

n0 = PointCharge( Vector(0,0,0), 2 )
n1 = PointCharge( Vector(1.4632,0,0), 1 )

print overlap_integral(s0,s0)
print overlap_integral(s0,s1)
print overlap_integral(s1,s1)

hf = HF()

hf.add(s0)
hf.add(s1)
hf.add(n0)
hf.add(n1)

hf.solve()

