
from Sire.IO import *
from Sire.Mol import *
from Sire.CAS import *
from Sire.System import *
from Sire.Move import *
from Sire.MM import *
from Sire.FF import *
from Sire.Units import *
from Sire.Vol import *
from Sire.Maths import *
from Sire.Base import *

import os,re,sys
import shutil

from IPython.Debugger import Tracer; debug_here = Tracer()

top_file = "test/io/SYSTEM.top"
crd_file = "test/io/SYSTEM.crd"

amber = Amber()
mols = amber.readcrdtop(crd_file, top_file)

lig = mols.molecule(MolNum(1)).molecule()

print lig.properties()

at = lig.atoms()[0]

print at
for prop in at.propertyKeys():
    print prop, at.property(prop)

prot = mols.molecule(MolNum(2)).molecule()

wat = mols.molecule(MolNum(3)).molecule()
