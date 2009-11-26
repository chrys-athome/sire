
from Sire.IO import *
from Sire.MM import *
from Sire.Mol import *
from Sire.Move import *
from Sire.MM import *
from Sire.System import *
from Sire.Vol import *
from Sire.Maths import *
from Sire.Units import *

protodir = "/Users/chris/Work/ProtoMS"

print "Parameterising indole"

protoms = ProtoMS( "%s/protoms2" % protodir )

protoms.addParameterFile( "%s/parameter/gaff.ff" % protodir )
protoms.addParameterFile( "%s/tutorials/protoms-setup-ligand/in/indolefree.template" % protodir )

indole = PDB().readMolecule("test/io/indole.pdb")

indole = indole.edit().rename("indole").commit()

indole = protoms.parameterise(indole, ProtoMS.SOLUTE)

internalff = InternalFF("InternalFF")

internalff.add( indole )

print "Bond = %f" % internalff.energy( internalff.components().bond() ).to(kcal_per_mol)
print "Angle = %f" % internalff.energy( internalff.components().angle() ).to(kcal_per_mol)
print "Dihedral = %f" % internalff.energy( internalff.components().dihedral() ).to(kcal_per_mol)

intraclj = IntraCLJFF("IntraCLJFF")

intraclj.add( indole )

print "IntraCLJ = %f" % intraclj.energy().to(kcal_per_mol)
print "Intra Coul = %f" % intraclj.energy( intraclj.components().coulomb() ).to(kcal_per_mol)
print "Intra LJ   = %f" % intraclj.energy( intraclj.components().lj() ).to(kcal_per_mol)

print "Total = %f" % (internalff.energy().to(kcal_per_mol) + intraclj.energy().to(kcal_per_mol))

indole = indole.atom( AtomName("H3") ).move().translate( Vector(0.1,0,0) ).molecule()

intraclj.update(indole)

print "\nEnergy post-move"

print "IntraCLJ = %f" % intraclj.energy().to(kcal_per_mol)
print "Intra Coul = %f" % intraclj.energy( intraclj.components().coulomb() ).to(kcal_per_mol)
print "Intra LJ   = %f" % intraclj.energy( intraclj.components().lj() ).to(kcal_per_mol)

print "\nAre we sure?"

intraclj.mustNowRecalculateFromScratch()

print "IntraCLJ = %f" % intraclj.energy().to(kcal_per_mol)
print "Intra Coul = %f" % intraclj.energy( intraclj.components().coulomb() ).to(kcal_per_mol)
print "Intra LJ   = %f" % intraclj.energy( intraclj.components().lj() ).to(kcal_per_mol)

