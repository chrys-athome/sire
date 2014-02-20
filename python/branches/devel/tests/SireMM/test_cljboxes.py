
from Sire.MM import *
from Sire.IO import *
from Sire.Mol import *
from Sire.Units import *
from Sire.Qt import *

(waters,space) = Amber().readCrdTop("../io/waterbox.crd", "../io/waterbox.top")

cljwaters = []

for i in range(0,waters.nMolecules()):
    cljwaters.append( CLJAtoms( waters[MolIdx(i)] ) )


def test_boxing(verbose = False):
    water = cljwaters[0]

    boxes = CLJBoxes()

    if verbose:
        print("\nAdding this water to a box multiple times...")
        print(water)

    for i in range(0,10):
        idxs = boxes.add(water)

        if verbose:
            print("nAtoms() == %s, %s x %s" % (boxes.nAtoms(), i+1,
                                               water.nAtoms()))

        assert( boxes.nAtoms() == (i+1)*water.nAtoms() )

        for i in range(0, water.nAtoms()):
            atom = water[i]
            idx = idxs[i]
            boxatom = boxes.at(idx)

            if verbose:
                print("%s at %s. Equals %s" % (atom, idx, boxatom))

            assert( atom.coordinates() == boxatom.coordinates() )
            assert( atom.charge() == boxatom.charge() )
            assert( atom == boxatom )

if __name__ == "__main__":
    test_boxing(True)

