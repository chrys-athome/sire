#############################
##
## The SireStream module
##
## (C) Christopher Woods
##

from Sire.Stream._Stream import *

import sys

_pvt_load = load

_pvt_modules = { "SireBase"   : "Sire.Base", 
                 "SireCAS"    : "Sire.CAS",
                 "SireDB"     : "Sire.DB",
                 "SireError"  : "Sire.Error",
                 "SireFF"     : "Sire.FF",
                 "SireID"     : "Sire.ID",
                 "SireIO"     : "Sire.IO",
                 "SireMM"     : "Sire.MM",
                 "SireMaths"  : "Sire.Maths",
                 "SireMol"    : "Sire.Mol",
                 "SireMove"   : "Sire.Move",
                 "SireSystem" : "Sire.System",
                 "SireUnits"  : "Sire.Units",
                 "SireVol"    : "Sire.Vol",
                 "Squire"     : "Sire.Squire" }

def _pvt_loadLibrary(lib):

    lib = str(lib)

    if lib in _pvt_modules:
        __import__( _pvt_modules[lib] ) 
        sys.stdout.write(".")

def load(data):
    header = getDataHeader(data)

    sys.stdout.write("Loading required Sire Python modules.")

    for lib in header.requiredLibraries():
        _pvt_loadLibrary(lib)

    print "Done!"

    return _pvt_load(data)

