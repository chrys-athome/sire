#############################
##
## The Squire library.
##
## This contains all of the classes that are used to 
## provide Quantum Mechanics (QM) functionality to 
## Sire (normally by providing links to existing
## QM programs)
##

import Sire.FF
import Sire.Mol
import Sire.DB

# Import all of the classes and functions from the C++ library
from Sire.Squire._Squire import *

# Now define some pure Python functions and classes that are part of
# the module
