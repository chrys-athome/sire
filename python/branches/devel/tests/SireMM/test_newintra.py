
import Sire.Stream

from Sire.MM import *

protein = Sire.Stream.load("../io/protein.s3")

def test_compare(verbose = False):
    old_intraff = IntraCLJFF("old")
    old_intraff.add(protein)

    cnrg = old_intraff.energy( old_intraff.components().coulomb() )
    ljnrg = old_intraff.energy( old_intraff.components().lj() )

    if verbose:
        print("\nOLD: %s  %s  %s" % (cnrg+ljnrg,cnrg,ljnrg))


if __name__ == "__main__":
    test_compare(True)

