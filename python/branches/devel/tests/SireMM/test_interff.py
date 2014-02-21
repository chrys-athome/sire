
from Sire.MM import *
from Sire.IO import *
from Sire.Mol import *
from Sire.Maths import *
from Sire.Units import *
from Sire.Qt import *

(mols, space) = Amber().readCrdTop("../io/waterbox.crd", "../io/waterbox.top")

coul_cutoff = 20 * angstrom
lj_cutoff = 10 * angstrom

oldff = InterCLJFF("oldff")
oldff.setSwitchingFunction( HarmonicSwitchingFunction(coul_cutoff,lj_cutoff) )
oldff.setSpace(space)
oldff.setShiftElectrostatics(True)

newff = InterFF("newff")
newff.setProperty("switchingFunction", HarmonicSwitchingFunction(coul_cutoff,lj_cutoff))
newff.setProperty("space",space)

t = QElapsedTimer()
t.start()
oldff.add(mols)
oldns = t.nsecsElapsed()

t.start()
newff.add(mols)
newns = t.nsecsElapsed()

print("Setup times: %s ms vs. %s ms" % (0.000001*oldns,0.000001*newns))

def test_energy(verbose = False):
    t = QElapsedTimer()

    t.start()
    oldnrgs = oldff.energies()
    oldns = t.nsecsElapsed()

    t.start()
    newnrgs = newff.energies()
    newns = t.nsecsElapsed()

    if verbose:
        print(oldnrgs)
        print(newnrgs)
        print("%s ms vs. %s ms" % (oldns * 0.000001, newns * 0.000001))

    water = mols[ MolIdx(0) ].molecule()

    water = water.move().translate( Vector(1,0,0) ).commit()

    t.start()
    oldff.update(water)
    old_change = t.nsecsElapsed()

    t.start()
    newff.update(water)
    new_change = t.nsecsElapsed()

    if verbose:
        print("ff.update(): %s ms vs. %s ms" % (old_change*0.000001,
                                                new_change*0.000001))

    t.start()
    oldnrgs = oldff.energies()
    oldns = t.nsecsElapsed()

    t.start()
    newnrgs = newff.energies()
    newns = t.nsecsElapsed()

    if verbose:
        print(oldnrgs)
        print(newnrgs)
        print("%s ms vs. %s ms" % (oldns * 0.000001, newns * 0.000001))


if __name__ == "__main__":
    test_energy(True)

