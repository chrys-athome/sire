
from Sire.MM import *
from Sire.Vol import *
from Sire.IO import *
from Sire.Maths import *
from Sire.Units import *
from Sire.Qt import *

(mols, space) = Amber().readCrdTop("../io/waterbox.crd", "../io/waterbox.top")

cljatoms = CLJAtoms( mols.molecules() )

gridbox = AABox( Vector(0), Vector(5) )

coul_cutoff = 20 * angstrom
lj_cutoff = 10 * angstrom

def test_build_grid(verbose = False):

    cljfunc = CLJShiftFunction(coul_cutoff, lj_cutoff)

    grid = GridInfo(gridbox, 0.5 * angstrom)

    if verbose:
        print("\nTest grid equals %s" % grid)

    assert(cljfunc.supportsGridCalculation())

    gridpot = cljfunc.calculate(cljatoms, grid)

if __name__ == "__main__":
    test_build_grid(True)

