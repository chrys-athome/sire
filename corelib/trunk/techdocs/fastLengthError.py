#!/bin/env python

from ProtoMS3 import *

import time

v1 = Vector(1.0,2.0,3.0)
v2 = Vector(3.0,2.0,1.0)

qprint(v1.toString())
qprint(v2.toString())

print "Doing distance..."

timer = QTime()

timer.start()

f = file("test.data","w")

for i in range(0,1000,10):
  print i
  for j in range(0,1000,10):
    for k in range(0,1000,10):
      
      v = Vector(i*0.1,j*0.1,k*0.1)
      
      d1 = v.length()
      d2 = v.fastLength()
      
      if (d1 != 0.0):
          err = 100.0 * (d1-d2)/d1
      else:
          err = 0.0
      
      f.write("%f %f\n" % (d1,err))
      
f.close()
   
runtime = timer.elapsed()

print "Completed in %d ms" % runtime
