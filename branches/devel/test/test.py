
from Sire.Mol import *
from Sire.IO import *
from Sire.DB import *
from Sire.MM import *
from Sire.CAS import *
from Sire.Units import *

mols = PDB().read("test/io/water.pdb")

#automatically generate the connectivity of the molecules
for mol in mols:
    mol.addAutoBonds()
#    mol.setName("Water [TIP4P]")

mols = flexibleAndMoleculeCutting(mols)

tip4p = mols[0]
print tip4p

#create the database and load into it the solvent parameters
db = ParameterDB()

ProtoMS().read("test/ff/solvents.ff", db)

#now assign the parameters for the water
params = db.assign(tip4p, [ assign_atoms(
                                 using_parameters(ChargeDB,LJDB,AtomTypeDB), 
                                 using_relationships(RelateMRADB) ),
                            assign_bonds(
                                 using_parameters(BondDB),
                                 using_relationships(RelateMRADB,
                                                     RelateAtomTypeDB) ),
                            assign_angles(
                                 using_parameters(AngleDB),
                                 using_relationships(RelateMRADB,
                                                     RelateAtomTypeDB) ),
                            assign_dihedrals(
                                 using_parameters(DihedralDB),
                                 using_relationships(RelateMRADB,
                                                     RelateAtomTypeDB) ),
                          ],
                   mol_name == "T4P" 
                  )
             
assert( params.isA(ChargeTable) )
assert( params.isA(LJTable) )
assert( params.isA(AtomTypeTable) )
assert( params.isA(BondTable) )
assert( params.isA(AngleTable) )

chgs = params.asA(ChargeTable)
ljs = params.asA(LJTable)
typs = params.asA(AtomTypeTable)
bonds = params.asA(BondTable)
angles = params.asA(AngleTable)

assert( not chgs.hasMissingParameters() )
assert( not ljs.hasMissingParameters() )
assert( not typs.hasMissingParameters() )
assert( not bonds.hasMissingParameters() )
assert( not angles.hasMissingParameters() )

for atom in params.molecule().atoms():
    print atom, chgs[atom], ljs[atom], typs[atom]
 
for bond in bonds.bonds():
    print bond, bonds[bond]
               
for angle in angles.angles():
    print angle, angles[angle]

#check the parameters are correct
assert( chgs[("O00",1)] == 0.0 )
assert( chgs[("H01",1)] == 0.52 )
assert( chgs[("H02",1)] == 0.52 )
assert( chgs[("M03",1)] == -1.04 )

assert( ljs[("O00",1)] == LJParameter(3.15363,0.155) )
assert( ljs[("H01",1)] == LJParameter.dummy() )
assert( ljs[("H02",1)] == LJParameter.dummy() )
assert( ljs[("M03",1)] == LJParameter.dummy() )

assert( typs[("O00",1)] == AtomType("OW","oxygen") )
assert( typs[("H01",1)] == AtomType("HW","hydrogen") )
assert( typs[("H02",1)] == AtomType("HW","hydrogen") )
assert( typs[("M03",1)] == AtomType("??",0) )

r = db.asA("SireMM::BondDB").r()
print db.getLog()

assert( bonds[ (("H01",1),("O00",1)) ] == 450*pow((r-0.9572),2) )
assert( bonds[ (("H02",1),("O00",1)) ] == 450*pow((r-0.9572),2) )
                
theta = db.asA("SireMM::AngleDB").theta()

assert( angles[ (("H01",1),("O00",1),("H02",1)) ] == 55*pow((theta - 104.52*degrees),2) )

print "Done!"
