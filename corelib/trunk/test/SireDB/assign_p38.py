
from Sire.Mol import *
from Sire.IO import *
from Sire.DB import *
from Sire.MM import *

from Sire.Qt import *

mols = PDB().read("test/io/p38.pdb")
mols = flexibleAndResidueCutting(mols)

p38 = mols[0]

#create the database and load into it the solvent parameters
db = ParameterDB()

print "Reading in ProtoMS parameter files..."
ProtoMS().read("test/ff/opls96.ff", db)
ProtoMS().read("test/ff/opls96-residues.ff", db)
ProtoMS().read("test/ff/solvents.ff", db)

#now assign the parameters for the water
t = QTime()

print "Assigning the parameters..."
t.start()
params = db.assign(p38, assign_atoms(
                                using_parameters(ChargeDB,LJDB,AtomTypeDB), 
                                using_relationships(RelateMRADB,RelateAtomTypeDB) )
                  )

ms = t.elapsed()
print "...assigning parameters took %d ms" % ms
	 
assert( params.isA(ChargeTable) )
assert( params.isA(LJTable) )
assert( params.isA(AtomTypeTable) )

chgs = params.asA(ChargeTable)
ljs = params.asA(LJTable)
typs = params.asA(AtomTypeTable)

#assert( not chgs.hasMissingParameters() )
#assert( not ljs.hasMissingParameters() )
#assert( not typs.hasMissingParameters() )

for atom in params.molecule().atoms():
    if (chgs.assignedParameter(atom) and ljs.assignedParameter(atom) and
               type.assignedParameter(atom)):
	       
	 print atom, chgs.getCharge(atom), ljs.getLJ(atom), \
        	        typs.getParameter(atom)
                
print "Done!"
