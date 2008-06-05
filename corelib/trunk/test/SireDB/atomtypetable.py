
from Sire.Mol import *
from Sire.IO import *
from Sire.DB import *

mols = PDB().read("test/io/water.pdb")
mols = rigidAndMoleculeCutting(mols)

tip4p = mols[0]

table = AtomTypeTable(tip4p)

assert( table.nAtoms() == 4 )
assert( table.hasMissingParameters() )

assert( ("O00",1) in table )
assert( ("H01",1) in table )
assert( ("H02",1) in table )
assert( ("M03",1) in table )

OW = AtomType("OW","oxygen")
HW = AtomType("HW","hydrogen")
DM = AtomType("??")

assert( table.missingParameters().count() == 4 )

table[ ("O00",1) ] = OW

assert( table.missingParameters().count() == 3 )
assert( not ("O00",1) in table.missingParameters() )
assert( ("H01",1) in table.missingParameters() )
assert( ("H02",1) in table.missingParameters() )
assert( ("M03",1) in table.missingParameters() )

assert( table[ ("O00",1) ] == OW )

table[ ("H01",1) ] = HW
table[ ("H02",1) ] = HW

assert( table.missingParameters().count() == 1 )
assert( not ("O00",1) in table.missingParameters() )
assert( not ("H01",1) in table.missingParameters() )
assert( not ("H02",1) in table.missingParameters() )
assert( ("M03",1) in table.missingParameters() )

assert( table.hasMissingParameters() )
assert( table.hasMissingAtomTypes() )

assert( table.assignedAtomType( ("O00",1) ) )
assert( table.assignedAtomType( ("H01",1) ) )
assert( table.assignedParameter( ("H02",1) ) )
assert( not table.assignedAtomType( ("M03",1) ) )

assert( table[CGAtomID(0,0)] == OW )
assert( table[CGAtomID(0,1)] == HW )
assert( table[CGAtomID(0,2)] == HW )

assert( not table.assignedAtomType( CGAtomID(0,3) ) )

table.setAtomType( ResNumAtomID(1,3), DM )
assert( not table.hasMissingParameters() )

assert( table[ResNumAtomID(1,0)] == OW )
assert( table[ResNumAtomID(1,1)] == HW )
assert( table[ResNumAtomID(1,2)] == HW )
assert( table[CGAtomID(0,3)] == DM )

assert( table[ ("M03",1) ] == DM )

assert( table[ (CutGroupID(0),0) ] == OW )
assert( table[ (ResNum(1),1) ] == HW )


