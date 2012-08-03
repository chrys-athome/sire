
from Sire.IO import *
from Sire.MM import *
from Sire.System import *
from Sire.Mol import *
from Sire.Maths import *
from Sire.Move import *
from Sire.Units import *

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

system.setProperty("space", space)

gridff = GridFF("gridff")
gridff.setGridSpacing( 2 * angstrom )

gridff.add(swapwaters, MGIdx(0))
gridff.add(waters, MGIdx(1))

gridff.setSpace(space)

system.add(gridff)

print system.energies()

rbmc = RigidBodyMC(swapwaters)
rbmc.setReflectionSphere(center_point, 7.5*angstrom)

moves = SameMoves(rbmc)

PDB().write(system.molecules(), "test0000.pdb")

for i in range(1,11):
    print "Moving the system..."
    system = moves.move(system, 100, False)
    print "Moves complete!"
    print system.energies()

    PDB().write(system.molecules(), "test%0004d.pdb" % i)

print "Done :-)"
