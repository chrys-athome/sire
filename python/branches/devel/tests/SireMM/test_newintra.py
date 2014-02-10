
import Sire.Stream

from Sire.MM import *
from Sire.Mol import *
from Sire.Vol import *
from Sire.Qt import *
from Sire.Units import *
from Sire.IO import *

realcompare = False

if realcompare:
    amber = Amber();
    amber.set14Factors(0, 0)

    (molecules, space) = amber.readCrdTop("../io/proteinbox.crd", "../io/proteinbox.top")

    protein = molecules[ MolNum(1) ].molecule()

    print("Protein has %d atoms" % protein.nAtoms())
else:
    protein = Sire.Stream.load("../io/protein.s3")

coul_cutoff = 15 * angstrom
lj_cutoff = 15 * angstrom

def test_compare(verbose = False):
    old_intraff = IntraCLJFF("old")
    old_intraff.setSpace( Cartesian() )
    old_intraff.setSwitchingFunction( HarmonicSwitchingFunction(coul_cutoff,lj_cutoff) )
    old_intraff.setShiftElectrostatics( True )

    old_intraff.add(protein)

    if verbose:
        print("\nCalculating IntraCLJFF energy...")

    t = QTime()
    t.start()
    cnrg = old_intraff.energy( old_intraff.components().coulomb() ).value()
    old_ms1 = t.elapsed()
    t.start()
    ljnrg = old_intraff.energy( old_intraff.components().lj() ).value()
    old_ms2 = t.elapsed()

    if verbose:
        print("Loading protein into CLJIntraShiftFunction...")

    intrafunc = CLJIntraShiftFunction()
    intrafunc.setConnectivity(protein)
    intrafunc.setSpace( Cartesian() )
    intrafunc.setCutoff(coul_cutoff, lj_cutoff)

    cljatoms = CLJAtoms(protein, CLJAtoms.USE_ATOMIDX)

    if verbose:
        print("Calculating CLJIntraShiftFunction energies...")

    t.start()
    new_cnrg = intrafunc.coulomb(cljatoms)
    new_ms1 = t.elapsed()
    t.start()
    new_ljnrg = intrafunc.lj(cljatoms)
    new_ms2 = t.elapsed()

    if verbose:
        print("\nOLD: %s  %s  %s (took %s|%s ms)" % (cnrg+ljnrg,cnrg,ljnrg,old_ms1,old_ms2))
        print("NEW: %s  %s  %s (took %s|%s ms)" % (new_cnrg+new_ljnrg,new_cnrg,new_ljnrg,new_ms1,new_ms2))

    cljboxes = CLJBoxes(cljatoms, 7.5*angstrom)
    cljcalc = CLJCalculator()

    t.start()
    (cnrg, ljnrg) = cljcalc.calculate(intrafunc, cljboxes)
    ms = t.elapsed()

    if verbose:
        print("BOX: %s  %s  %s (took %s ms)" % (cnrg+ljnrg,cnrg,ljnrg,ms))
   


if __name__ == "__main__":
    test_compare(True)

