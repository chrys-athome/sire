
from Sire.MM import *
from Sire.System import *
from Sire.Move import *
from Sire.IO import *
from Sire.Mol import *
from Sire.Maths import *
from Sire.Vol import *
from Sire.Units import *
from Sire.Qt import *

from nose.tools import assert_almost_equal

(mols, space) = Amber().readCrdTop("../io/waterbox.crd", "../io/waterbox.top")

#space = Cartesian()

coul_cutoff = 15 * angstrom
lj_cutoff = 15 * angstrom

oldff = InterCLJFF("oldff")
oldff.setSwitchingFunction( HarmonicSwitchingFunction(coul_cutoff,lj_cutoff) )
oldff.setSpace(space)
oldff.setShiftElectrostatics(True)

newff = InterFF("newff")
newff.setProperty("switchingFunction", HarmonicSwitchingFunction(coul_cutoff,lj_cutoff))
newff.setProperty("space",space)

t = QElapsedTimer()
t.start()
cljmols = CLJBoxes( CLJAtoms(mols.molecules()) )
cljns = t.nsecsElapsed()

t.start()
oldff.add(mols)
oldns = t.nsecsElapsed()

t.start()
newff.add(mols)
newns = t.nsecsElapsed()

print("Setup times: %s ms vs. %s ms (%s ms)" % (0.000001*oldns,0.000001*newns,
                                                0.000001*cljns))

def test_energy(verbose = False):
    t = QElapsedTimer()

    cljfunc = CLJShiftFunction(coul_cutoff, lj_cutoff)
    cljfunc.setSpace(space)
    cljcalc = CLJCalculator()
    t.start()
    (cnrg,ljnrg) = cljcalc.calculate(cljfunc,cljmols)
    ns = t.nsecsElapsed()

    t.start()
    oldnrgs = oldff.energies()
    oldns = t.nsecsElapsed()
    oldcnrg = oldff.energy(oldff.components().coulomb()).value()
    oldljnrg = oldff.energy(oldff.components().lj()).value()

    t.start()
    newnrgs = newff.energies()
    newns = t.nsecsElapsed()
    newcnrg = newff.energy(newff.components().coulomb()).value()
    newljnrg = newff.energy(newff.components().lj()).value()

    if verbose:
        print("\nTotal energy")
        print("CLJFUNC:  %s  %s  %s  : %s ms" % (cnrg+ljnrg,cnrg,ljnrg,
                                                 0.000001*ns))
        print("OLD FF :  %s  %s  %s  : %s ms" % (oldcnrg+oldljnrg,oldcnrg,oldljnrg,
                                                 0.000001*oldns))
        print("NEW FF :  %s  %s  %s  : %s ms" % (newcnrg+newljnrg,newcnrg,newljnrg,
                                                 0.000001*newns))

    assert_almost_equal( cnrg, newcnrg, 6 )
    assert_almost_equal( ljnrg, newljnrg, 6 )
    assert_almost_equal( oldcnrg, newcnrg, 2 )
    assert_almost_equal( oldljnrg, newljnrg, 2 )

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
    oldcnrg = oldff.energy( oldff.components().coulomb() ).value()
    oldljnrg = oldff.energy( oldff.components().lj() ).value()

    t.start()
    newnrgs = newff.energies()
    newns = t.nsecsElapsed()
    newcnrg = newff.energy( newff.components().coulomb() ).value()
    newljnrg = newff.energy( newff.components().lj() ).value()

    if verbose:
        print("\nChanged energy")
        print("OLD FF :  %s  %s  %s  : %s ms" % (oldcnrg+oldljnrg,oldcnrg,oldljnrg,
                                                 0.000001*oldns))
        print("NEW FF :  %s  %s  %s  : %s ms" % (newcnrg+newljnrg,newcnrg,newljnrg,
                                                 0.000001*newns))

    assert_almost_equal( oldcnrg, newcnrg, 1 )
    assert_almost_equal( oldljnrg, newljnrg, 1 )

    oldff.mustNowRecalculateFromScratch()
    newff.mustNowRecalculateFromScratch()

    t.start()
    oldnrgs = oldff.energies()
    oldns = t.nsecsElapsed()

    r_oldcnrg = oldff.energy( oldff.components().coulomb() ).value()
    r_oldljnrg = oldff.energy( oldff.components().lj() ).value()

    t.start()
    newnrgs = newff.energies()
    newns = t.nsecsElapsed()

    r_newcnrg = newff.energy( newff.components().coulomb() ).value()
    r_newljnrg = newff.energy( newff.components().lj() ).value()

    if verbose:
        print("\nRecalculated energy")
        print("OLD FF :  %s  %s  %s  : %s ms" % (r_oldcnrg+r_oldljnrg,r_oldcnrg,r_oldljnrg,
                                                 0.000001*oldns))
        print("NEW FF :  %s  %s  %s  : %s ms" % (r_newcnrg+r_newljnrg,r_newcnrg,r_newljnrg,
                                                 0.000001*newns))

    assert_almost_equal( oldcnrg, r_oldcnrg, 6 )
    assert_almost_equal( oldljnrg, r_oldljnrg, 6 )
    assert_almost_equal( newcnrg, r_newcnrg, 6 )
    assert_almost_equal( newljnrg, r_newljnrg, 6 )

def test_sim(verbose = False):

    oldsys = System()
    newsys = System()

    #oldsys.add(mols)
    #newsys.add(mols)

    oldsys.add(oldff)
    newsys.add(newff)

    t = QElapsedTimer()

    oldsys.mustNowRecalculateFromScratch()
    newsys.mustNowRecalculateFromScratch()

    t.start()
    nrgs = oldsys.energies()
    oldns = t.nsecsElapsed()

    t.start()
    nrgs = newsys.energies()
    newns = t.nsecsElapsed()

    oldcnrg = oldsys.energy( oldff.components().coulomb() ).value()
    oldljnrg = oldsys.energy( oldff.components().lj() ).value()

    newcnrg = newsys.energy( newff.components().coulomb() ).value()
    newljnrg = newsys.energy( newff.components().lj() ).value()

    if verbose:
        print("\nStarting energy")
        print("OLD SYS:  %s  %s  %s  : %s ms" % (oldcnrg+oldljnrg,oldcnrg,oldljnrg,
                                                 0.000001*oldns))
        print("NEW SYS:  %s  %s  %s  : %s ms" % (newcnrg+newljnrg,newcnrg,newljnrg,
                                                 0.000001*newns))

    moves = RigidBodyMC(mols)
    moves.setGenerator( RanGenerator( 42 ) )

    t.start()
    moves.move(oldsys, 1000, False)
    move_oldns = t.nsecsElapsed()

    moves.setGenerator( RanGenerator( 42 ) )

    t.start()
    moves.move(newsys, 1000, False)
    move_newns = t.nsecsElapsed()

    t.start()
    nrgs = oldsys.energies()
    oldns = t.nsecsElapsed()
    
    t.start()
    nrgs = newsys.energies()
    newns = t.nsecsElapsed()

    oldcnrg = oldsys.energy( oldff.components().coulomb() ).value()
    oldljnrg = oldsys.energy( oldff.components().lj() ).value()
    
    newcnrg = newsys.energy( newff.components().coulomb() ).value()
    newljnrg = newsys.energy( newff.components().lj() ).value()

    if verbose:
        print("\nMoves: %s ms vs. %s ms" % (0.000001*move_oldns, 0.000001*move_newns))
        print("OLD SYS:  %s  %s  %s  : %s ms" % (oldcnrg+oldljnrg,oldcnrg,oldljnrg,
                                                 0.000001*oldns))
        print("NEW SYS:  %s  %s  %s  : %s ms" % (newcnrg+newljnrg,newcnrg,newljnrg,
                                                 0.000001*newns))
    
    oldsys.mustNowRecalculateFromScratch()
    newsys.mustNowRecalculateFromScratch()
    
    t.start()
    nrgs = oldsys.energies()
    oldns = t.nsecsElapsed()
    
    t.start()
    nrgs = newsys.energies()
    newns = t.nsecsElapsed()

    r_oldcnrg = oldsys.energy( oldff.components().coulomb() ).value()
    r_oldljnrg = oldsys.energy( oldff.components().lj() ).value()

    r_newcnrg = newsys.energy( newff.components().coulomb() ).value()
    r_newljnrg = newsys.energy( newff.components().lj() ).value()

    if verbose:
        print("\nRecalculated energy")
        print("OLD SYS:  %s  %s  %s  : %s ms" % (r_oldcnrg+r_oldljnrg,r_oldcnrg,r_oldljnrg,
                                                 0.000001*oldns))
        print("NEW SYS:  %s  %s  %s  : %s ms" % (r_newcnrg+r_newljnrg,r_newcnrg,r_newljnrg,
                                                 0.000001*newns))
    

if __name__ == "__main__":
    test_energy(True)
    test_sim(True)
