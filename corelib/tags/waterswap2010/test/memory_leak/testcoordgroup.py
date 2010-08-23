#!/bin/env python

from Sire.Vol import *
from Sire.Maths import *

import copy
import time
import sys

c = CoordGroup(30000)

for i in range(0,10000):
  
  print >>sys.stderr, i
  
  print >>sys.stderr, "Copy..."
  c2 = copy.deepcopy(c)
  
  print >>sys.stderr, "Edit..."
  editor = c.edit()
  
  print >>sys.stderr, "Translate..."
  editor.translate( Vector(1,0,0) )
  
  print >>sys.stderr, "Commit..."
  c = editor.commit()
  
  #time.sleep(0.5)

