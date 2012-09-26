
from Sire.MM import *
from Sire.Mol import *
from Sire.Maths import *
from Sire.Units import *
from Sire.FF import *
from Sire.System import *

mol = Molecule("intra")
mol = mol.edit().add( CGName("1") ) \
                .add( AtomName("A") ).cutGroup() \
                .add( AtomName("B") ).cutGroup() \
         .molecule().commit()

mol = mol.edit().atom( AtomName("A") ) \
                .setProperty("charge", 0.5 * mod_electron) \
                .setProperty("LJ", LJParameter(3*angstrom,0.5*kcal_per_mol)) \
                .setProperty("coordinates", Vector(0,0,0)) \
                .molecule().atom( AtomName("B") ) \
                .setProperty("charge", -0.5 * mod_electron) \
                .setProperty("LJ", LJParameter(3*angstrom,0.5*kcal_per_mol)) \
                .setProperty("coordinates", Vector(6,0,0)) \
                .molecule().commit()

sclpairs = CLJNBPairs(mol)
sclpairs.set( AtomName("A"), AtomName("A"), CLJScaleFactor(0,0) )
sclpairs.set( AtomName("B"), AtomName("B"), CLJScaleFactor(0,0) )
sclpairs.set( AtomName("A"), AtomName("B"), CLJScaleFactor(1.0,1.0) )

mol = mol.edit().setProperty("intrascale", sclpairs) \
                .setProperty("lj_intrascale", LJNBPairs(sclpairs)) \
                .setProperty("coul_intrascale", CoulombNBPairs(sclpairs)) \
                .commit()

mol_a = mol.edit().renumber().remove(AtomName("B")).commit()
mol_b = mol.edit().renumber().remove(AtomName("A")).commit()

intraclj = IntraCLJFF("intraclj")
intraclj.add(mol)

intralj = IntraLJFF("intralj")
intralj.add(mol, {"intrascale" : "lj_intrascale"})

intracoul = IntraCoulombFF("intracoul")
intracoul.add(mol, {"intrascale" : "coul_intrascale"})

interclj = InterCLJFF("interclj")
interclj.add(mol_a)
interclj.add(mol_b)

interlj = InterLJFF("interlj")
interlj.add(mol_a)
interlj.add(mol_b)

intercoul = InterCoulombFF("intercoul")
intercoul.add(mol_a)
intercoul.add(mol_b)

intra_group = MoleculeGroup("intra")
intra_group.add(mol)

inter_group = MoleculeGroup("inter")
inter_group.add(mol_a)
inter_group.add(mol_b)

intrasys = System()
intrasys.add(intraclj)
intrasys.add(intra_group)

intersys = System()
intersys.add(interclj)
intersys.add(inter_group)

intraljsys = System()
intraljsys.add(intralj)
intraljsys.add(intra_group)

intracoulsys = System()
intracoulsys.add(intracoul)
intracoulsys.add(intra_group)

interljsys = System()
interljsys.add(interlj)
interljsys.add(inter_group)

intercoulsys = System()
intercoulsys.add(intercoul)
intercoulsys.add(inter_group)

inters = [intersys, interljsys, intercoulsys]
intras = [intrasys, intraljsys, intracoulsys]

for intra in intras:
    print intra.energies()

for inter in inters:
    print inter.energies()

intra_forces = ForceTable(intra_group)
inter_forces = ForceTable(inter_group)

intrasys.force(intra_forces)
intersys.force(inter_forces)

print intra_forces.getTable(mol.number()).toVector()
print inter_forces.getTable(mol_a.number()).toVector(), \
      inter_forces.getTable(mol_b.number()).toVector()

intra_forces.initialiseTables()
inter_forces.initialiseTables()

intrasys.force(intra_forces, intraclj.components().coulomb())
intersys.force(inter_forces, interclj.components().coulomb())
                
print intra_forces.getTable(mol.number()).toVector()
print inter_forces.getTable(mol_a.number()).toVector(), \
      inter_forces.getTable(mol_b.number()).toVector()

intra_forces.initialiseTables()
inter_forces.initialiseTables()

intrasys.force(intra_forces, intraclj.components().lj())
intersys.force(inter_forces, interclj.components().lj())
                                                
print intra_forces.getTable(mol.number()).toVector()
print inter_forces.getTable(mol_a.number()).toVector(), \
      inter_forces.getTable(mol_b.number()).toVector()

