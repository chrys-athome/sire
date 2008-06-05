
from Sire.Mol import *
from Sire.IO import *
from Sire.DB import *
from Sire.MM import *
from Sire.Qt import *

mols = PDB().read("test/io/water.pdb")
mols = rigidAndMoleculeCutting(mols)

tip4p = mols[0]

table = ParameterTable(tip4p)

table.addTable("SireDB::AtomTypeTable")
table.addTable("SireMM::ChargeTable")

#cool dynamic casting :-)
print table.asA("SireDB::AtomTypeTable").nAtomTypes()
print table.asA("SireMM::ChargeTable").nCharges()

#can now even do this from the Python class itself...
print table.isA(ChargeTable)
print table.isA(AtomTypeTable)

#or from instances of the class
atomtypes = AtomTypeTable()

print table.isA(atomtypes)

table.addTable(LJTable)

print table.types()

print table.asA(LJTable).hasMissingLJs()

table.asA(LJTable)[("O00",1)] = LJParameter(3.15363, 0.1550)

ljtable = table.asA(LJTable)

ljtable[("H01",1)] = LJParameter.dummy()
ljtable[("H02",1)] = ljtable[("H01",1)]
ljtable[ ResNumAtomID(1,3) ] = LJParameter.dummy()

assert( not table.asA(LJTable).hasMissingParameters() )
print table.asA(LJTable).hasMissingParameters()

for atom in ljtable.atoms():
	print atom, ljtable[atom]

ctable = table.asA(ChargeTable)

ctable[ CGAtomID(0,0) ] = 0.000
ctable[ CGAtomID(0,1) ] = 0.520
ctable[ CGAtomID(0,2) ] = 0.520
ctable[ CGAtomID(0,3) ] = -1.040

assert( not table.asA(ChargeTable).hasMissingParameters() )

for atom in table.molecule().atoms():
	print atom, table.asA(ChargeTable)[atom]

atable = table.asA(AtomTypeTable)

atable.setParameter( ResNumAtomID(1,0), AtomType("OW","oxygen") )
atable.setParameter( ResNumAtomID(1,1), AtomType("HW","hydrogen") )
atable.setParameter( ResNumAtomID(1,2), atable.getParameter(ResNumAtomID(1,1)) )
atable.setParameter( ResNumAtomID(1,3), AtomType("??","dummy") )

assert( not table.asA(AtomTypeTable).hasMissingParameters() )

for atom in atable.atoms():
	print atom, table.asA(AtomTypeTable)[atom]

#now test the streaming operators
b = QByteArray()
d = QDataStream( b, QIODevice.OpenMode.WriteOnly )

#serialise the table
d << table

#deserialise the table
d = QDataStream(b)

table = ParameterTable()
print table.types()

d >> table

print table.types()

for atom in table.molecule().atoms():
	print atom, table.asA(AtomTypeTable)[atom], table.asA(ChargeTable)[atom], \
                    table.asA(LJTable)[atom]

print "Done!"

