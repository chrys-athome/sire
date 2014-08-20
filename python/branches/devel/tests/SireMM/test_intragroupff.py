
from Sire.IO import *
from Sire.Mol import *
from Sire.MM import *
from Sire.System import *
from Sire.Move import *
from Sire.Maths import *
from Sire.Units import *
from Sire.Vol import *
from Sire.Qt import *

from Sire.Tools import AmberLoader

import Sire.Stream

import os

coul_cutoff = 15 * angstrom
lj_cutoff = 10 * angstrom

nmoves = 1000

switchfunc = HarmonicSwitchingFunction(coul_cutoff,coul_cutoff,
                                       lj_cutoff,lj_cutoff)

# Need to load with 0 1-4 scale factors because
# the new IntraFF forcefield does not account for 1-4
# terms
amber = Amber()
amber.set14Factors(0,0)

print("Loading protein box with zero 1-4 factors...")

if os.path.exists("../io/proteinbox_zero14.s3"):
    (mols, space) = Sire.Stream.load("../io/proteinbox_zero14.s3")
else:
    (mols,space) = amber.readCrdTop("../io/proteinbox.crd", "../io/proteinbox.top")
    Sire.Stream.save( (mols,space), "../io/proteinbox_zero14.s3" )

#space = Cartesian()

protein = mols[ MolWithResID("ALA") ].molecule()
ligand = mols[ MolWithResID("OSE") ].molecule()

flex = AmberLoader.generateFlexibility(ligand)
ligand = ligand.edit().setProperty("flexibility", flex).commit()

all = protein.selection()
part0 = protein.selection()
part1 = protein.selection()
part0 = part0.selectNone()
part1 = part1.selectNone()

for i in range(0, int(protein.nResidues()/2)):
    part0 = part0.select( ResIdx(i) )

for i in range(int(protein.nResidues()/2), protein.nResidues()):
    part1 = part1.select( ResIdx(i) )

protein0 = PartialMolecule(protein, part0)
protein1 = PartialMolecule(protein, part1)

print("Protein: Part 0 contains %d atoms, part 1 contains %d atoms, all contains %d atoms" \
            % (part0.nSelected(), part1.nSelected(), all.nSelected()))

all = ligand.selection()
part0 = ligand.selection()
part0 = part0.selectNone()
part1 = ligand.selection()
part1 = part1.selectNone()

for i in range(0, int(ligand.nAtoms()/2)):
    part0 = part0.select( AtomIdx(i) )

for i in range(int(ligand.nAtoms()/2),ligand.nAtoms()):
    part1 = part1.select( AtomIdx(i) )

ligand0 = PartialMolecule(ligand, part0)
ligand1 = PartialMolecule(ligand, part1)

print("Ligand: Part 0 contains %d atoms, part 1 contains %d atoms, all contains %d atoms" \
            % (part0.nSelected(), part1.nSelected(), all.nSelected()))

oldff = IntraGroupCLJFF("oldff")
oldff.setSwitchingFunction(switchfunc)
oldff.setSpace(space)
oldff.setShiftElectrostatics(True)

totalff = IntraCLJFF("totalff")
totalff.setSwitchingFunction(switchfunc)
totalff.setSpace(space)
totalff.setShiftElectrostatics(True)

newff = IntraGroupFF("newff")
newff.setProperty("cljFunction", CLJIntraShiftFunction())
newff.setProperty("switchingFunction",switchfunc)
newff.setProperty("space",space)
newff.disableParallelCalculation()

new0ff = IntraFF("new0ff")
new0ff.setProperty("cljFunction", CLJIntraShiftFunction())
new0ff.setProperty("switchingFunction",switchfunc)
new0ff.setProperty("space",space)
new0ff.disableParallelCalculation()

new1ff = IntraFF("new0ff")
new1ff.setProperty("cljFunction", CLJIntraShiftFunction())
new1ff.setProperty("switchingFunction",switchfunc)
new1ff.setProperty("space",space)
new1ff.disableParallelCalculation()

parff = IntraGroupFF("parff")
parff.setProperty("cljFunction", CLJIntraShiftFunction())
parff.setProperty("switchingFunction",switchfunc)
parff.setProperty("space",space)
parff.enableParallelCalculation()

print("Adding molecules to forcefields...")
oldff.add(ligand0, MGIdx(0))
oldff.add(ligand1, MGIdx(1))
oldff.add(protein0, MGIdx(0))
oldff.add(protein1, MGIdx(1))
newff.add(ligand0, MGIdx(0))
newff.add(ligand1, MGIdx(1))
newff.add(protein0, MGIdx(0))
newff.add(protein1, MGIdx(1))
parff.add(ligand0, MGIdx(0))
parff.add(ligand1, MGIdx(1))
parff.add(protein0, MGIdx(0))
parff.add(protein1, MGIdx(1))

totalff.add(ligand)
totalff.add(protein)
new0ff.add(ligand0)
new0ff.add(protein0)
new1ff.add(ligand1)
new1ff.add(protein1)

print("Performing tests...\n")
t = QElapsedTimer()

def assert_almost_equal( x, y, delta = 0.5 ):
    if abs(x-y) > delta:
        print("ERROR: %s is not equal to %s within a delta of %s" % (x,y,delta))
        assert(False)

def assert_equal( x, y ):
    if not (x == y):
        print("ERROR: %s is not equal to %s" % (x, y))
        assert(False)

def _checkEnergies(oldsys, newsys, parsys, verbose):
    t.start()
    nrgs = oldsys.energies()
    oldns = t.nsecsElapsed()
    
    oldcnrg = oldsys.energy( oldff.components().coulomb() ).value()
    oldljnrg = oldsys.energy( oldff.components().lj() ).value()
    
    t.start()
    nrgs = newsys.energies()
    newns = t.nsecsElapsed()
    
    newcnrg = newsys.energy( newff.components().coulomb() ).value()
    newljnrg = newsys.energy( newff.components().lj() ).value()
      
    t.start()
    nrgs = parsys.energies()
    parns = t.nsecsElapsed()
    
    parcnrg = parsys.energy( parff.components().coulomb() ).value()
    parljnrg = parsys.energy( parff.components().lj() ).value()

    if verbose:
        print("OLD SYS :  %s  %s  %s  : %s ms" % (oldcnrg+oldljnrg,oldcnrg,oldljnrg,
                                                  0.000001*oldns))
        print("NEW SYS :  %s  %s  %s  : %s ms" % (newcnrg+newljnrg,newcnrg,newljnrg,
                                                  0.000001*newns))
        print("PAR SYS :  %s  %s  %s  : %s ms" % (parcnrg+parljnrg,parcnrg,parljnrg,
                                                  0.000001*parns))

    #assert_almost_equal( oldcnrg, newcnrg, 0.5 )
    #assert_almost_equal( oldljnrg, newljnrg, 0.5 )
    #assert_almost_equal( parcnrg, newcnrg, 0.5 )
    #assert_almost_equal( parljnrg, newljnrg, 0.5 )

def test_energy(verbose = False):
    oldff.mustNowRecalculateFromScratch()
    newff.mustNowRecalculateFromScratch()
    parff.mustNowRecalculateFromScratch()

    t.start()
    nrgs = oldff.energies()
    oldns = t.nsecsElapsed()

    oldcnrg = oldff.energy( oldff.components().coulomb() ).value()
    oldljnrg = oldff.energy( oldff.components().lj() ).value()

    t.start()
    nrgs = newff.energies()
    newns = t.nsecsElapsed()

    newcnrg = newff.energy( newff.components().coulomb() ).value()
    newljnrg = newff.energy( newff.components().lj() ).value()

    t.start()
    nrgs = parff.energies()
    parns = t.nsecsElapsed()

    parcnrg = parff.energy( parff.components().coulomb() ).value()
    parljnrg = parff.energy( parff.components().lj() ).value()

    if verbose:
        print("\nTotal energy")
        print("OLD FF :  %s  %s  %s  : %s ms" % (oldcnrg+oldljnrg,oldcnrg,oldljnrg,
                                                 0.000001*oldns))
        print("NEW FF :  %s  %s  %s  : %s ms" % (newcnrg+newljnrg,newcnrg,newljnrg,
                                                 0.000001*newns))
        print("PAR FF :  %s  %s  %s  : %s ms" % (parcnrg+parljnrg,parcnrg,parljnrg,
                                                 0.000001*parns))

    assert_almost_equal( oldcnrg, newcnrg, 0.5 )
    assert_almost_equal( oldljnrg, newljnrg, 0.5 )
    assert_almost_equal( parcnrg, newcnrg, 0.5 )
    assert_almost_equal( parljnrg, newljnrg, 0.5 )

    # now calculate what the total total energy should be
    totalff.update( newff.molecules() )
    new0ff.update( newff.molecules() )
    new1ff.update( newff.molecules() )
    total_cnrg = totalff.energy( totalff.components().coulomb() ).value()
    total_ljnrg = totalff.energy( totalff.components().lj() ).value()

    new0_cnrg = new0ff.energy( new0ff.components().coulomb() ).value()
    new0_ljnrg = new0ff.energy( new0ff.components().lj() ).value()

    new1_cnrg = new1ff.energy( new1ff.components().coulomb() ).value()
    new1_ljnrg = new1ff.energy( new1ff.components().lj() ).value()

    new_total_cnrg = new0_cnrg + new1_cnrg + newcnrg
    new_total_ljnrg = new0_ljnrg + new1_ljnrg + newljnrg

    if verbose:
        print("\nChecked total")
        print("TOTAL FF : %s  %s  %s" % (total_cnrg+total_ljnrg, total_cnrg, total_ljnrg))
        print("NEW FF   : %s  %s  %s" % (new_total_cnrg+new_total_ljnrg, \
                                         new_total_cnrg,new_total_ljnrg))

    assert_almost_equal( total_cnrg, new_total_cnrg )
    assert_almost_equal( total_ljnrg, new_total_ljnrg )


def test_ligand_moves(verbose = False):
    mols = MoleculeGroup("mols")
    mols.add(ligand)

    oldsys = System()
    newsys = System()
    parsys = System()

    oldsys.add(oldff)
    oldsys.add(mols)

    newsys.add(newff)
    newsys.add(mols)

    parsys.add(parff)
    parsys.add(mols)

    moves = InternalMove(mols)
    moves.enableOptimisedMoves()

    oldsys.mustNowRecalculateFromScratch()
    newsys.mustNowRecalculateFromScratch()
    parsys.mustNowRecalculateFromScratch()

    if verbose:
        print("\nChecking total system energies...")

    _checkEnergies(oldsys, newsys, parsys, verbose)

    if verbose:
        print("\nChecking system simulation...")

    moves.setGenerator( RanGenerator(42) )
    moves.clearStatistics()

    t.start()
    moves.move(oldsys, nmoves, False)
    oldns = t.nsecsElapsed()
    old_naccept = moves.nAccepted()
    old_nreject = moves.nRejected()

    moves.setGenerator( RanGenerator(42) )
    moves.clearStatistics()

    t.start()
    moves.move(newsys, nmoves, False)
    newns = t.nsecsElapsed()
    new_naccept = moves.nAccepted()
    new_nreject = moves.nRejected()

    moves.setGenerator( RanGenerator(42) )
    moves.clearStatistics()

    t.start()
    moves.move(parsys, nmoves, False)
    parns = t.nsecsElapsed()
    par_naccept = moves.nAccepted()
    par_nreject = moves.nRejected()
       
    if verbose:
        print("OLD: accept = %s, reject = %s, time = %s ms" % \
                   (old_naccept, old_nreject, oldns*0.000001))
        print("NEW: accept = %s, reject = %s, time = %s ms" % \
                   (new_naccept, new_nreject, newns*0.000001))
        print("PAR: accept = %s, reject = %s, time = %s ms" % \
                   (par_naccept, par_nreject, parns*0.000001))

        print("\nChecking final total energies...")

    _checkEnergies(oldsys, newsys, parsys, verbose)

    if verbose:
        print("\nRechecking total energies...")

    oldsys.mustNowRecalculateFromScratch()
    newsys.mustNowRecalculateFromScratch()
    parsys.mustNowRecalculateFromScratch()

    _checkEnergies(oldsys, newsys, parsys, verbose)

    # now calculate what the total total energy should be
    totalff.update( newff.molecules() )
    new0ff.update( newff.molecules() )
    new1ff.update( newff.molecules() )
    total_cnrg = totalff.energy( totalff.components().coulomb() ).value()
    total_ljnrg = totalff.energy( totalff.components().lj() ).value()

    new0_cnrg = new0ff.energy( new0ff.components().coulomb() ).value()
    new0_ljnrg = new0ff.energy( new0ff.components().lj() ).value()

    new1_cnrg = new1ff.energy( new1ff.components().coulomb() ).value()
    new1_ljnrg = new1ff.energy( new1ff.components().lj() ).value()

    newcnrg = newff.energy( newff.components().coulomb() ).value()
    newljnrg = newff.energy( newff.components().lj() ).value()

    new_total_cnrg = new0_cnrg + new1_cnrg + newcnrg
    new_total_ljnrg = new0_ljnrg + new1_ljnrg + newljnrg

    if verbose:
        print("\nChecked total")
        print("TOTAL FF : %s  %s  %s" % (total_cnrg+total_ljnrg, total_cnrg, total_ljnrg))
        print("NEW FF   : %s  %s  %s" % (new_total_cnrg+new_total_ljnrg, \
                                         new_total_cnrg,new_total_ljnrg))

    assert_almost_equal( total_cnrg, new_total_cnrg )
    assert_almost_equal( total_ljnrg, new_total_ljnrg )


def test_protein_moves(verbose = False):
    mols = MoleculeGroup("mols")
    
    for residue in protein.residues():
        mols.add(residue)

    oldsys = System()
    newsys = System()
    parsys = System()

    oldsys.add(oldff)
    oldsys.add(mols)

    newsys.add(newff)
    newsys.add(mols)

    parsys.add(parff)
    parsys.add(mols)

    moves = RigidBodyMC(mols)
    moves.setMaximumTranslation(0.25 * angstrom)
    moves.setMaximumRotation(1 * degrees)
    moves.enableOptimisedMoves()

    oldsys.mustNowRecalculateFromScratch()
    newsys.mustNowRecalculateFromScratch()
    parsys.mustNowRecalculateFromScratch()

    if verbose:
        print("\nChecking total system energies...")

    _checkEnergies(oldsys, newsys, parsys, verbose)

    if verbose:
        print("\nChecking system simulation...")

    moves.setGenerator( RanGenerator(42) )
    moves.clearStatistics()

    t.start()
    moves.move(oldsys, nmoves, False)
    oldns = t.nsecsElapsed()
    old_naccept = moves.nAccepted()
    old_nreject = moves.nRejected()

    moves.setGenerator( RanGenerator(42) )
    moves.clearStatistics()

    t.start()
    moves.move(newsys, nmoves, False)
    newns = t.nsecsElapsed()
    new_naccept = moves.nAccepted()
    new_nreject = moves.nRejected()

    moves.setGenerator( RanGenerator(42) )
    moves.clearStatistics()

    t.start()
    moves.move(parsys, nmoves, False)
    parns = t.nsecsElapsed()
    par_naccept = moves.nAccepted()
    par_nreject = moves.nRejected()
       
    if verbose:
        print("OLD: accept = %s, reject = %s, time = %s ms" % \
                   (old_naccept, old_nreject, oldns*0.000001))
        print("NEW: accept = %s, reject = %s, time = %s ms" % \
                   (new_naccept, new_nreject, newns*0.000001))
        print("PAR: accept = %s, reject = %s, time = %s ms" % \
                   (par_naccept, par_nreject, parns*0.000001))

        print("\nChecking final total energies...")

    _checkEnergies(oldsys, newsys, parsys, verbose)

    if verbose:
        print("\nRechecking total energies...")

    oldsys.mustNowRecalculateFromScratch()
    newsys.mustNowRecalculateFromScratch()
    parsys.mustNowRecalculateFromScratch()

    _checkEnergies(oldsys, newsys, parsys, verbose)

    # now calculate what the total total energy should be
    totalff.update( newff.molecules() )
    new0ff.update( newff.molecules() )
    new1ff.update( newff.molecules() )
    total_cnrg = totalff.energy( totalff.components().coulomb() ).value()
    total_ljnrg = totalff.energy( totalff.components().lj() ).value()

    new0_cnrg = new0ff.energy( new0ff.components().coulomb() ).value()
    new0_ljnrg = new0ff.energy( new0ff.components().lj() ).value()

    new1_cnrg = new1ff.energy( new1ff.components().coulomb() ).value()
    new1_ljnrg = new1ff.energy( new1ff.components().lj() ).value()

    newcnrg = newff.energy( newff.components().coulomb() ).value()
    newljnrg = newff.energy( newff.components().lj() ).value()

    new_total_cnrg = new0_cnrg + new1_cnrg + newcnrg
    new_total_ljnrg = new0_ljnrg + new1_ljnrg + newljnrg

    if verbose:
        print("\nChecked total")
        print("TOTAL FF : %s  %s  %s" % (total_cnrg+total_ljnrg, total_cnrg, total_ljnrg))
        print("NEW FF   : %s  %s  %s" % (new_total_cnrg+new_total_ljnrg, \
                                         new_total_cnrg,new_total_ljnrg))

    assert_almost_equal( total_cnrg, new_total_cnrg )
    assert_almost_equal( total_ljnrg, new_total_ljnrg )


if __name__ == "__main__":
    test_energy(True)
    test_ligand_moves(True)
    test_protein_moves(True)
