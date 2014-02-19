
from Sire.MM import *
from Sire.Vol import *
from Sire.IO import *
from Sire.Maths import *
from Sire.Units import *
from Sire.Mol import *
from Sire.Qt import *

from nose.tools import assert_almost_equal

import math

coul_cutoff = 25 * angstrom
lj_cutoff = 10 * angstrom

grid_spacing = 0.25 * angstrom

(mols, space) = Amber().readCrdTop("../io/waterbox.crd", "../io/waterbox.top")

cljatoms = CLJAtoms( mols.molecules() )

clusterbox = AABox( Vector(10), Vector(2) )
print(clusterbox)

cluster = MoleculeGroup("cluster")
waters = MoleculeGroup("waters")

for i in range(0, mols.nMolecules()):
    mol = mols[ MolIdx(i) ].molecule()

    if clusterbox.contains( mol.evaluate().center() ):
        cluster.add(mol)
    else:
        waters.add(mol)

wateratoms = CLJAtoms(waters)
clusteratoms = CLJAtoms(cluster)

waterboxes = CLJBoxes(wateratoms)
clusterboxes = CLJBoxes(clusteratoms)

def test_build_grid(verbose = False):

    cljfunc = CLJShiftFunction(coul_cutoff, lj_cutoff)

    grid = GridInfo(AABox(Vector(10),Vector(1)), 0.23*angstrom)

    if verbose:
        print("\nTest grid equals %s" % grid)

    assert(cljfunc.supportsGridCalculation())

    water = CLJAtoms( mols[MolIdx(0)].molecule() )

    gridpot = cljfunc.calculate(water, grid)

    reduce_fac = math.sqrt(one_over_four_pi_eps0)

    for i in range(0,grid.nPoints()):
        gridpoint = grid.point(i)
        atom = CLJAtoms( [CLJAtom(gridpoint, 1*mod_electron, LJParameter.dummy(), 1000)] )
        cnrg = cljfunc.coulomb(atom, water)

        if verbose:
            if (i % 100 == 0) or (abs(cnrg - gridpot[i] * reduce_fac) > 0.1):
                print("%s  %s   %s   %s   %s   %s" % (i, grid[i], gridpoint, gridpot[i], 
                                                      cnrg, gridpot[i] * reduce_fac))

        assert_almost_equal( cnrg, gridpot[i] * reduce_fac, 2 )


def test_energy(verbose = False):
    cljfunc = CLJShiftFunction(coul_cutoff, lj_cutoff)
    cljfunc.setSpace(Cartesian())

    t = QElapsedTimer()

    cljgrid = CLJGrid()
    cljgrid.setCLJFunction(cljfunc)
    cljgrid.setFixedAtoms(wateratoms)
    cljgrid.setGridDimensions(clusteratoms, grid_spacing, 2*angstrom)

    cljff = InterGroupCLJFF("cljff")
    cljff.setSpace(Cartesian())
    cljff.setSwitchingFunction( HarmonicSwitchingFunction(coul_cutoff,lj_cutoff) )
    cljff.setShiftElectrostatics(True)
    cljff.setCombiningRules("arithmetic")
    cljff.add(cluster, MGIdx(0))
    cljff.add(waters, MGIdx(1))

    if verbose:
        print("Grid: %s" % cljgrid.grid())

    if verbose:
        print("\nFirst grid calculation...")

    cljgrid.setUseGrid(True)
    t.start()
    (grid_cnrg, grid_ljnrg) = cljgrid.calculate(clusterboxes)
    grid_ns = t.nsecsElapsed()

    if verbose:
        print("\nSecond grid calculation...")

    t.start()
    (grid2_cnrg, grid2_ljnrg) = cljgrid.calculate(clusterboxes)
    grid2_ns = t.nsecsElapsed()

    if verbose:
        print("\nNo-grid calculation...")

    cljgrid.setUseGrid(False)
    t.start()
    (nogrid_cnrg, nogrid_ljnrg) = cljgrid.calculate(clusterboxes)
    nogrid_ns = t.nsecsElapsed()

    if verbose:
        print("\nExact calculation...")

    cljcalc = CLJCalculator()
    t.start()
    (cnrg, ljnrg) = cljcalc.calculate(cljfunc, clusterboxes, waterboxes)
    ns = t.nsecsElapsed()

    if verbose:
        print("\nOld exact calculation...")

    t.start()
    cljff.energies()
    old_ns = t.nsecsElapsed()
    old_cnrg = cljff.energy( cljff.components().coulomb() ).value()
    old_ljnrg = cljff.energy( cljff.components().lj() ).value()

    if coul_cutoff.value() != lj_cutoff.value():
        # the old forcefield has a broken implementation of the LJ
        # cutoff when it is not equal to the coulomb cutoff - we need
        # to recalculate the LJ energy...
        cljff.setSwitchingFunction( HarmonicSwitchingFunction(lj_cutoff) )
        old_ljnrg = cljff.energy( cljff.components().lj() ).value()

    if verbose:
        print("\nRESULTS")
        print("GRID  1:  %s  %s  %s  (%s ms)" % (grid_cnrg+grid_ljnrg,
                                                 grid_cnrg, grid_ljnrg,
                                                 (0.000001*grid_ns)) )
        print("GRID  2:  %s  %s  %s  (%s ms)" % (grid2_cnrg+grid2_ljnrg,
                                                 grid2_cnrg, grid2_ljnrg,
                                                 (0.000001*grid2_ns)) )
        print("NO GRID:  %s  %s  %s  (%s ms)" % (nogrid_cnrg+nogrid_ljnrg,
                                                 nogrid_cnrg, nogrid_ljnrg,
                                                 (0.000001*nogrid_ns)) )
        print("EXACT  :  %s  %s  %s  (%s ms)" % (cnrg+ljnrg,
                                                 cnrg, ljnrg,
                                                 (0.000001*ns)) )
        print("OLD    :  %s  %s  %s  (%s ms)" % (old_cnrg+old_ljnrg,
                                                 old_cnrg, old_ljnrg,
                                                 (0.000001*old_ns)) )

        assert_almost_equal( grid_cnrg, grid2_cnrg, 5 )
        assert_almost_equal( grid_ljnrg, grid_ljnrg, 5 )
        assert_almost_equal( grid_cnrg, nogrid_cnrg, 2 )
        assert_almost_equal( grid_ljnrg, nogrid_ljnrg, 5 )
        assert_almost_equal( grid_cnrg, cnrg, 2 )
        assert_almost_equal( nogrid_cnrg, cnrg, 5 )
        assert_almost_equal( grid_ljnrg, ljnrg, 5 )
        assert_almost_equal( grid_cnrg, old_cnrg, 2 )
        assert_almost_equal( grid_ljnrg, old_ljnrg, 3 )


if __name__ == "__main__":
    test_build_grid(True)
    test_energy(True)
