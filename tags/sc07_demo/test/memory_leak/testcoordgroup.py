#!/bin/env python

from Sire.Vol import *

import copy
import time
import sys

c = CoordGroup(30000)

for i in range(0,100):
  
  print >>sys.stderr, i
  
  print >>sys.stderr, "Copy..."
  c2 = copy.deepcopy(c)
  
  print >>sys.stderr, "Edit..."
  editor = c.edit()
  
  print >>sys.stderr, "Translate..."
  editor.translate( (1,0,0) )
  
  print >>sys.stderr, "Commit..."
  c = editor.commit()
  
  #time.sleep(0.5)

