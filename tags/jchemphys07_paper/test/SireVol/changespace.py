
from Sire.Mol import *
from Sire.IO import *
from Sire.Vol import *
from Sire.FF import *
from Sire.MM import *
from Sire.CAS import *
from Sire.Maths import *
from Sire.Qt import *
from Sire.Units import *
from Sire.System import *
from Sire.Move import *
from Sire.Base import *

import sys

timer = QTime()

mols = PDB().read("test/io/water.pdb")

#specify the space in which the molecules are placed
box0 = PeriodicBox( (-18.3854,-18.66855,-18.4445),
                    ( 18.3854, 18.66855, 18.4445) )

box1 = PeriodicBox( box0.minCoords() + (50,0,0),
                    box0.maxCoords() + (50,0,0) )

PDB().write(mols, "test0.pdb")
                    
def changeSpace(mols, old_box, new_box):
    new_mols = []
    
    for mol in mols:
        new_mol = mol.move().setCoordinates(
                      new_box.mapFromSelf(mol.coordGroups(), old_box) )
                            
        new_mols.append( Molecule(new_mol) )
        
    return new_mols

print "Box1 %s, %s, %s" % (box1.minCoords(), box1.center(), box1.maxCoords())

timer.start()
mols = changeSpace(mols,box0,box1)
    
ms = timer.elapsed()
print "Took %d ms" % ms
    
PDB().write(mols, "test1.pdb")

box2 = PeriodicBox( box1.center() - (1,1,1),
                    box1.center() + (1,1,1) )
                    
print "Box2 %s, %s, %s" % (box2.minCoords(), box2.center(), box2.maxCoords())

timer.start()
mols = changeSpace(mols,box1,box2)
    
ms = timer.elapsed()
print "Took %d ms" % ms

PDB().write(mols, "test2.pdb")

box3 = PeriodicBox( box0.center() - (100,10,10),
                    box0.center() + (100,10,10) )
                    
print "Box3 %s, %s, %s" % (box3.minCoords(), box3.center(), box3.maxCoords())

timer.start()
mols = changeSpace(mols,box2,box3)
    
ms = timer.elapsed()
print "Took %d ms" % ms

PDB().write(mols, "test3.pdb")

timer.start()
mols = changeSpace(mols,box3,box0)
    
ms = timer.elapsed()
print "Took %d ms" % ms

PDB().write(mols, "test4.pdb")

