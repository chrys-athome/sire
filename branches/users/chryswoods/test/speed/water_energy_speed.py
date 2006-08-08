
from Sire.Base import *
from Sire.IO import *
from Sire.FF import *
from Sire.Maths import *
from Sire.Mol import *
from Sire.Vol import *
from Sire.Qt import *

t = QTime()

db = MMDB()

dirname = "/home/chris/ProtoMC/trunk/parameter"

ProtoMS().read("test/io/water.ff", db)

cljff = InterCLJFF()

intercljff = InterGroupCLJFF()
cljffa = InterCLJFF()
cljffb = InterCLJFF()

mincoords = Vector(-18.3854, -18.66855, -18.4445)
maxcoords = Vector( 18.3854,  18.66855,  18.4445)

#mincoords = Vector(-8000.0, -8000.0, -8000.0)
#maxcoords = Vector(8000.0,8000.0,8000.0)

vol = PeriodicBox(mincoords, maxcoords)

cljff.setVolume(vol)
intercljff.setVolume(vol)
cljffa.setVolume(vol)
cljffb.setVolume(vol)

def setCutoff(dist):
    cljff.setCutoff(dist)
    intercljff.setCutoff(dist)
    cljffa.setCutoff(dist)
    cljffb.setCutoff(dist)
    
setCutoff(15.0)

#mols = PDB().read("%s/../example/p38/sb2.pdb" % dirname)
mols = PDB().read("test/io/water.pdb")

print "Read in %d molecules!" % mols.count()

i = 0

for mol in mols:
    mol.setName("T4P")
    tip4pclj = db.getMMAtomParameters(mol, LamPosition.LAM_0)

    mol_mol = Molecule.create(mol, SINGLE_CUTGROUP + RIGID_MOLECULE)
    
    cljff.add(mol_mol, tip4pclj)
    
    i = i + 1
    if ( i % 2 == 0):
        intercljff.addToA(mol_mol, tip4pclj)
        cljffa.add(mol_mol, tip4pclj)
    else:
        intercljff.addToB(mol_mol, tip4pclj)
        cljffb.add(mol_mol, tip4pclj)

t.start()
nrg = cljff.energy()
ms = t.elapsed()
print nrg
print "Took %d ms" % ms

t.start()
nrg_pair = intercljff.energy() 
nrg_a = cljffa.energy() 
nrg_b = cljffb.energy()
ms = t.elapsed()
print nrg_pair
print nrg_a
print nrg_b
print nrg_pair + nrg_a + nrg_b
print "Took %d ms" % ms

ffgroup = FFGroup()
ffgroup.add("groupA", cljffa)
ffgroup.add("groupB", cljffb)
ffgroup.add("groupA-groupB", intercljff)

t.start()
nrg = ffgroup.energy()
ms = t.elapsed()

print nrg
print "Took %d ms" % ms

ffgroup.setEquilibration(False)
ffgroup.testSpeed(intercljff.cutGroups(), 1000)
ffgroup.testSpeed(intercljff.cutGroups(), 1000)
ffgroup.testSpeed(intercljff.cutGroups(), 1000)

ffgroup.setEquilibration(True)
ffgroup.testSpeed(intercljff.cutGroups(), 1000)
ffgroup.testSpeed(intercljff.cutGroups(), 1000)
ffgroup.testSpeed(intercljff.cutGroups(), 1000)

ffgroup2 = FFGroup()
ffgroup2.add("all", cljff)

ffgroup2.setEquilibration(False)
ffgroup2.testSpeed(intercljff.cutGroups(), 1000)
ffgroup2.testSpeed(intercljff.cutGroups(), 1000)
ffgroup2.testSpeed(intercljff.cutGroups(), 1000)

ffgroup2.setEquilibration(True)
ffgroup2.testSpeed(intercljff.cutGroups(), 1000)
ffgroup2.testSpeed(intercljff.cutGroups(), 1000)
ffgroup2.testSpeed(intercljff.cutGroups(), 1000)


# print "\n*********\nStart test\n********"
# 
# def testSpeed():
#     t.start()
#     nrg = cljff.energy()
#     ms = t.elapsed()
#     
#     print nrg
#     print "Took %d ms" % ms
# 
# 
# def runTest(cutoff):
#     print "%f A cutoff" % cutoff
#     cljff.setCutoff(cutoff)
# 
#     for i in range(0,5):    
#         testSpeed()
# 
# runTest(5.0)
# runTest(10.0)
# runTest(15.0)
# runTest(20.0)
# runTest(40.0)
# runTest(80.0)


print "Done!"
