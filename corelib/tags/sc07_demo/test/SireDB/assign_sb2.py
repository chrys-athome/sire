
from Sire.Mol import *
from Sire.IO import *
from Sire.DB import *
from Sire.MM import *

from Sire.Qt import *

mols = PDB().read("test/io/sb2.pdb")

for mol in mols:
	mol.addAutoBonds()

mols = flexibleAndResidueCutting(mols)

sb2 = mols[0]

print sb2.toString()

#create the database and load into it the solvent parameters
db = ParameterDB()

print "Reading in ProtoMS parameter files..."
ProtoMS().read("test/ff/sb2.ff", db)

#now assign the parameters for the water
t = QTime()

print "Assigning the reference parameters..."
t.start()
params = db.assign(sb2, [assign_atoms(using_parameters(ChargeDB,LJDB,AtomTypeDB)),
			 assign_bonds(using_parameters(BondDB)),
			 assign_angles(using_parameters(AngleDB)),
			 assign_dihedrals(using_parameters(DihedralDB)),
			 using_relationships(RelateMRADB,RelateAtomTypeDB)],
			(mol_name == "SB2") & (mol_state == "reference")
                  )

ms = t.elapsed()
print "...assigning parameters took %d ms" % ms
	 
assert( params.isA(ChargeTable) )
assert( params.isA(LJTable) )
assert( params.isA(AtomTypeTable) )
assert( params.isA(BondTable) )
assert( params.isA(AngleTable) )
assert( params.isA(DihedralTable) )

chgs = params.asA(ChargeTable)
ljs = params.asA(LJTable)
typs = params.asA(AtomTypeTable)
bonds = params.asA(BondTable)
angles = params.asA(AngleTable)
dihedrals = params.asA(DihedralTable)

print bonds.nBonds(), bonds.nBondParameters()
print angles.nAngles(), angles.nAngleParameters()
print dihedrals.nDihedrals(), dihedrals.nDihedralParameters()

assert( not chgs.hasMissingParameters() )
assert( not ljs.hasMissingParameters() )
assert( not typs.hasMissingParameters() )

totalchg = 0.0

for atom in params.molecule().atoms():
	print atom, chgs.getCharge(atom), ljs.getLJ(atom), \
        	        typs.getParameter(atom)

	totalchg += chgs.getCharge(atom).charge()
                
print "Total charge on molecule == %s" % totalchg
	
for bond in bonds.bonds():
	if (bonds.assignedParameter(bond)):
		print bond, bonds[bond]
	
for angle in angles.angles():
	if (angles.assignedParameter(angle)):
		print angle, angles[angle]

for dihedral in dihedrals.dihedrals():
	if (dihedrals.assignedParameter(dihedral)):
		print dihedral, dihedrals[dihedral]
	
print "Assigning the perturbed parameters..."
t.start()
pert_params = db.assign(sb2, assign_atoms(
                                     using_parameters(ChargeDB,LJDB,AtomTypeDB),
				     using_relationships(RelateMRADB,RelateAtomTypeDB) ),
		             (mol_name == "SB2") & (mol_state == "perturbed")
		       )

ms = t.elapsed()
print "...assigning parameters took %d ms" % ms

assert( pert_params.isA(ChargeTable) )
assert( pert_params.isA(LJTable) )
assert( pert_params.isA(AtomTypeTable) )

chgs = pert_params.asA(ChargeTable)
ljs = pert_params.asA(LJTable)
typs = pert_params.asA(AtomTypeTable)

assert( not chgs.hasMissingParameters() )
assert( not ljs.hasMissingParameters() )
assert( not typs.hasMissingParameters() )

totalchg = 0.0

for atom in params.molecule().atoms():
        print atom, chgs.getCharge(atom), ljs.getLJ(atom), \
	                        typs.getParameter(atom)

	totalchg += chgs.getCharge(atom).charge()

print "Total charge on molecule == %s" % totalchg

