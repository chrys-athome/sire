
from Sire.IO import *
from Sire.MM import *
from Sire.System import *
from Sire.Mol import *
from Sire.Maths import *
from Sire.FF import *
from Sire.Move import *
from Sire.Units import *
from Sire.Vol import *
from Sire.Qt import *

import os

try:
    run_explicit = int( os.environ["RUN_EXPLICIT"] )
except:
    run_explicit = False

amber = Amber()

(molecules, space) = amber.readCrdTop("test/io/waterbox.crd", "test/io/waterbox.top")

system = System()

swapwaters = MoleculeGroup("swapwaters")
waters = MoleculeGroup("waters")

molnums = molecules.molNums();

for molnum in molnums:
    water = molecules[molnum].molecule()

    if water.residue().number() == ResNum(2025):
        center_water = water

swapwaters.add(center_water)

center_point = center_water.evaluate().center()

for molnum in molnums:
    if molnum != center_water.number():
        water = molecules[molnum].molecule()

        if Vector.distance(center_point, water.evaluate().center()) < 7.5:
            water = water.residue().edit().setProperty("PDB-residue-name", "SWP").commit()
            swapwaters.add(water)
        else:
            waters.add(water)

system.add(swapwaters)
system.add(waters)

gridff = GridFF("gridff")
gridff.setBuffer(2 * angstrom)
gridff.setGridSpacing( 0.25 * angstrom )
gridff.setLJCutoff( 7.5 * angstrom )
gridff.setCoulombCutoff( 25 * angstrom )

gridff.add(swapwaters, MGIdx(0))
gridff.add(waters, MGIdx(1))
gridff.setSpace(space)
gridff.setShiftElectrostatics(True)

cljff = InterGroupCLJFF("cljff")
cljff.setSwitchingFunction( HarmonicSwitchingFunction(15*angstrom) )
cljff.add(swapwaters, MGIdx(0))
cljff.add(waters, MGIdx(1))
cljff.setSpace(space)
cljff.setShiftElectrostatics(True)

swap_swapff = InterCLJFF("swap-swap")
swap_swapff.setSpace(Cartesian())
swap_swapff.setSwitchingFunction(NoCutoff())
swap_swapff.add(swapwaters)
system.add(swap_swapff)

if run_explicit:
    system.add(cljff)
else:
    system.add(gridff)

print system.energies()

rbmc = RigidBodyMC(swapwaters)
rbmc.setReflectionSphere(center_point, 7.5*angstrom)

moves = SameMoves(rbmc)

PDB().write(system.molecules(), "test0000.pdb")

t = QTime()

for i in range(1,11):
    print "Moving the system..."
    t.start()
    system = moves.move(system, 1000, False)
    ms = t.elapsed()
    print "Moves complete! Took %d ms" % ms
    print system.energies()

    PDB().write(system.molecules(), "test%0004d.pdb" % i)

print "Done :-)"
