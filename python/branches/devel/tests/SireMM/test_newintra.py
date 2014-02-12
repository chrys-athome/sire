
import Sire.Stream

from Sire.MM import *
from Sire.Mol import *
from Sire.Vol import *
from Sire.Qt import *
from Sire.Maths import *
from Sire.Units import *
from Sire.IO import *

from nose.tools import assert_almost_equal

import sys

realcompare = False

try:
    realcompare = int(sys.argv[1])
except:
    pass

if realcompare:
    amber = Amber();
    amber.set14Factors(0, 0)

    (molecules, space) = amber.readCrdTop("../io/proteinbox.crd", "../io/proteinbox.top")

    protein = molecules[ MolNum(1) ].molecule()

    print("Protein has %d atoms" % protein.nAtoms())
else:
    protein = Sire.Stream.load("../io/protein.s3")
    space = PeriodicBox( Vector(20,20,20) )

    cnrg_vacuum = -16406.191630892663  
    ljnrg_vacuum = -3171.065732415715

    cnrg_box = -16406.191630892663  
    ljnrg_box = -3171.065732415715

    cnrg_space = cnrg_vacuum
    ljnrg_space = ljnrg_vacuum

coul_cutoff = 15 * angstrom
lj_cutoff = 15 * angstrom

intraff = IntraCLJFF("old")
intraff.setSpace( Cartesian() )
intraff.setSwitchingFunction( HarmonicSwitchingFunction(coul_cutoff,lj_cutoff) )
intraff.setShiftElectrostatics( True )

intraff.add(protein)

intrafunc = CLJIntraShiftFunction(coul_cutoff, lj_cutoff)
intrafunc.setConnectivity(protein)
intrafunc.setSpace( Cartesian() )

cljatoms = CLJAtoms(protein, CLJAtoms.USE_ATOMIDX)
cljboxes = CLJBoxes(cljatoms, 7.5*angstrom)

def pvt_compare(verbose):

    t = QElapsedTimer()
    t.start()
    intraff.energies()
    old_ns = t.nsecsElapsed()

    old_cnrg = intraff.energy( intraff.components().coulomb() ).value()
    old_ljnrg = intraff.energy( intraff.components().lj() ).value()

    if not realcompare:
        # here is the energy of the protein without the 1-4 terms
        # (the new forcefield doesn't calculate 1-4 terms)
        old_cnrg = cnrg_space  
        old_ljnrg = ljnrg_space

    t.start()
    (new_cnrg, new_ljnrg) = intrafunc.calculate(cljatoms)
    new_ns = t.nsecsElapsed()

    cljcalculator = CLJCalculator()
    t.start()
    (box_cnrg, box_ljnrg) = cljcalculator.calculate(intrafunc, cljboxes)
    box_ns = t.nsecsElapsed()

    if verbose:
        print("OLD: %s  %s  %s  %s ms" % (old_cnrg+old_ljnrg, old_cnrg, old_ljnrg, 0.000001*old_ns))
        print("NEW: %s  %s  %s  %s ms" % (new_cnrg+new_ljnrg, new_cnrg, new_ljnrg, 0.000001*new_ns))
        print("BOX: %s  %s  %s  %s ms" % (box_cnrg+box_ljnrg, box_cnrg, box_ljnrg, 0.000001*box_ns))
    
    assert_almost_equal( new_cnrg, old_cnrg, 2 )
    assert_almost_equal( new_ljnrg, old_ljnrg, 2 )
    assert_almost_equal( box_cnrg, old_cnrg, 2 )
    assert_almost_equal( box_ljnrg, old_ljnrg, 2 )


def test_compare_vacuum(verbose = False):
    intraff.setSpace( Cartesian() )
    intrafunc.setSpace( Cartesian() )

    if not realcompare:
        cnrg_space = cnrg_vacuum
        ljnrg_space = ljnrg_vacuum

    pvt_compare(verbose)


#def test_compare_box(verbose = False):
#    intraff.setSpace( space )
#    intrafunc.setSpace( space )
#
#    if not realcompare:
#        cnrg_space = cnrg_box
#        ljnrg_space = ljnrg_box
#
#    pvt_compare(verbose)

if __name__ == "__main__":
    print("\nIntramolecular vacuum energy")
    test_compare_vacuum(True)

    #print("\nIntramolecular periodic boundaries energy")
    #test_compare_box(True)
