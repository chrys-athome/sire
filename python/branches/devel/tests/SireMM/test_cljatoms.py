
from Sire.IO import *
from Sire.MM import *

(mols, space) = Amber().readCrdTop("../io/waterbox.crd", "../io/waterbox.top")

def test_ids():
    water = mols.first().molecule()
    molnum = water.number().value()

    clj = CLJAtoms(water)

    print(molnum)
    print(clj.IDs())

    assert( not clj.isDummy(0) )
    assert( not clj.isDummy(1) )
    assert( not clj.isDummy(2) )
    assert( clj.isDummy(3) )

    assert( clj.IDs()[0] == molnum )
    assert( clj.IDs()[1] == molnum )
    assert( clj.IDs()[2] == molnum )
    assert( clj.IDs()[3] == 0 )
    

if __name__ == "__main__":
    test_ids()

