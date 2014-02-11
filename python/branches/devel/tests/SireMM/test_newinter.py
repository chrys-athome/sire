
from Sire.IO import *
from Sire.Mol import *
from Sire.MM import *
from Sire.Units import *
from Sire.Vol import *
from Sire.Qt import *

coul_cutoff = 1.75 * angstrom
lj_cutoff = 1.75 * angstrom

compare_energies = False

if compare_energies:
    (waters, space) = Amber().readCrdTop("../io/waterbox.crd", "../io/waterbox.top", "peratom")
else:
    (waters, space) = Amber().readCrdTop("../io/waterbox.crd", "../io/waterbox.top")

group0 = MoleculeGroup("group0")
group1 = MoleculeGroup("group1")

for i in range(0,10):
    group0.add( waters[MolIdx(i)] )

for i in range(10,waters.nMolecules()):
    group1.add( waters[MolIdx(i)] )

cljatoms = CLJAtoms(waters.molecules())
cljatoms0 = CLJAtoms(group0.molecules())
cljatoms1 = CLJAtoms(group1.molecules())

boxes = CLJBoxes(cljatoms, 20*angstrom)
boxes0 = CLJBoxes(cljatoms0)
boxes1 = CLJBoxes(cljatoms1)

cljff = InterCLJFF("all")
cljff.setSwitchingFunction( HarmonicSwitchingFunction(coul_cutoff,coul_cutoff,lj_cutoff,lj_cutoff) )
cljff.add(waters)
cljff.setShiftElectrostatics(True)

cljff01 = InterGroupCLJFF("pair")
cljff01.setSwitchingFunction( HarmonicSwitchingFunction(coul_cutoff,coul_cutoff,lj_cutoff,lj_cutoff) )
cljff01.add(group0, MGIdx(0))
cljff01.add(group1, MGIdx(1))

cljfunc = CLJShiftFunction(coul_cutoff, lj_cutoff)

def test_compare(verbose):

    cljcalc = CLJCalculator()

    t = QElapsedTimer()
    t.start()
    cljff.energies()
    old_ns = t.nsecsElapsed()

    old_cnrg = cljff.energy(cljff.components().coulomb()).value()
    old_ljnrg = cljff.energy(cljff.components().lj()).value()

    t.start()
    new_cnrg = 1.0
    new_ljnrg = 1.0
    (new_cnrg, new_ljnrg) = cljfunc.calculate(cljatoms)
    new_ns = t.nsecsElapsed()

    t.start()
    (box_cnrg, box_ljnrg) = cljcalc.calculate(cljfunc, boxes)
    box_ns = t.nsecsElapsed()

    if verbose:
        print("OLD: %s  %s  %s  %s ms" % (old_cnrg+old_ljnrg, old_cnrg, old_ljnrg, 0.000001*old_ns))
        print("NEW: %s  %s  %s  %s ms" % (new_cnrg+new_ljnrg, new_cnrg, new_ljnrg, 0.000001*new_ns))
        print("BOX: %s  %s  %s  %s ms" % (box_cnrg+box_ljnrg, box_cnrg, box_ljnrg, 0.000001*box_ns))

def test_compare_vacuum(verbose = True):
    cljff.setSpace(Cartesian())
    cljff01.setSpace(Cartesian())
    cljfunc.setSpace(Cartesian())
    test_compare(verbose)

def test_compare_box(verbose = True):
    cljff.setSpace(space)
    cljff01.setSpace(space)
    cljfunc.setSpace(space)
    test_compare(verbose)
    
if __name__ == "__main__":
    print("nCutGroups() = %s" % waters[MolIdx(0)].molecule().nCutGroups())
    print("space = %s" % space)

    print("\nVacuum boundary conditions")
    test_compare_vacuum(True)

    print("\nPeriodic box boundary conditions")
    test_compare_box(True)
