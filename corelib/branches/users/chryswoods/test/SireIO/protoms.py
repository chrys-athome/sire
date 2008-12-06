
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

switchfunc = HarmonicSwitchingFunction( 10 * angstrom, 9.5 * angstrom )
box = PeriodicBox( Vector(-12.5,-12.5,-12.5), Vector(12.5,12.5,12.5) )

print "Parameterising water"

water = PDB().readMolecule("test/io/water.pdb")

protoms = ProtoMS("%s/protoms2" % protodir)

protoms.addParameterFile( "%s/parameter/solvents.ff" % protodir )
protoms.addParameterFile( "%s/parameter/amber99.ff" % protodir )
protoms.addParameterFile( "%s/parameter/amber99-residues.ff" % protodir )
protoms.addParameterFile( "%s/parameter/gaff.ff" % protodir )

water = protoms.parameterise(water, ProtoMS.SOLVENT)

print water.property("charge").array()
print water.property("LJ").array()

waters = PDB().read("%s/tutorials/protoms-ethanemethanol/dual/in/boxT4P.pdb" % protodir )

waterff = InterCLJFF("waterff")
solute_waterff = InterGroupCLJFF("solute-waterff")

waterff.setProperty("space", box)
waterff.setProperty("switchingFunction", switchfunc )

solute_waterff.setProperty("space", box)
solute_waterff.setProperty("switchingFunction", switchfunc)

for i in range(0,waters.molecules().count()):
   mol = waters[MolIdx(i)].molecule().edit().rename("T4P") \
                          .setProperty( "charge", water.property("charge") ) \
                          .setProperty( "LJ", water.property("LJ") ) \
                          .commit()

   waterff.add(mol)
   solute_waterff.add( mol, MGIdx(1) )

print "Parameterising ethane"

protoms.addParameterFile( "%s/tutorials/protoms-ethanemethanol/dual/in/ethane.par" % protodir )

ethane = PDB().readMolecule("%s/tutorials/protoms-ethanemethanol/dual/in/ethane.pdb" % protodir)

ethane = ethane.edit().rename("ethane").commit()

ethane = protoms.parameterise(ethane, ProtoMS.SOLUTE)

print ethane.property("charge").array()
print ethane.property("LJ").array()

zmat = ethane.property("z-matrix")

for line in zmat.lines():
    print line, line.bondDelta(), line.angleDelta().to(degrees), \
                                  line.dihedralDelta().to(degrees)

internalff = InternalFF("InternalFF")

internalff.add( ethane )
solute_waterff.add( ethane, MGIdx(0) )

print "Total = %f" % internalff.energy().to(kcal_per_mol)
print "Bond = %f" % internalff.energy( internalff.components().bond() ).to(kcal_per_mol)
print "Angle = %f" % internalff.energy( internalff.components().angle() ).to(kcal_per_mol)
print "Dihedral = %f" % internalff.energy( internalff.components().dihedral() ).to(kcal_per_mol)

intraclj = IntraCLJFF("IntraCLJFF")

intraclj.add( ethane )

print "IntraCLJ = %f" % intraclj.energy().to(kcal_per_mol)
print "Intra Coul = %f" % intraclj.energy( intraclj.components().coulomb() ).to(kcal_per_mol)
print "Intra LJ   = %f" % intraclj.energy( intraclj.components().lj() ).to(kcal_per_mol)

print "Solute-Solvent = %f" % solute_waterff.energy().to(kcal_per_mol)
print "Solute-Solvent Coul = %f" % solute_waterff.energy( solute_waterff.components().coulomb() ) \
                                                 .to(kcal_per_mol)

print "Solute-Solvent  LJ  = %f" % solute_waterff.energy( solute_waterff.components().lj() ) \
                                                 .to(kcal_per_mol)

system = System()
system.add( waterff )
system.add( solute_waterff )
system.add( internalff )
system.add( intraclj )

system.setProperty( "space", box )
system.setProperty( "switchingFunction", switchfunc )

solute = internalff.groups()[0]
solvent = waterff.groups()[0]

zmatmove = ZMatMove( solute )
solventmove = RigidBodyMC( PrefSampler(ethane, solvent) )
solventmove.setMaximumTranslation( 1 * angstrom )
solventmove.setMaximumRotation( 5 * degrees )

solutemove = RigidBodyMC( solute )
solutemove.setMaximumTranslation( 1 * angstrom )
solutemove.setMaximumRotation( 5 * degrees )

volumemove = VolumeMove()

moves = WeightedMoves()

moves.add( zmatmove, 10 )
moves.add( solutemove, 10 )
moves.add( solventmove, 100 )
moves.add( volumemove, 1 )

PDB().write( system.molecules(), "test000.pdb" )

print "Running a simulation - initial energy = %f kcal mol-1" % system.energy().to(kcal_per_mol)

for i in range(0,100):

    sim = Simulation.run( system, moves, 1000 )
    
    system = sim.system()
    moves = sim.moves()

    PDB().write( system.molecules(), "test%003d.pdb" % (i+1) )

    print "%d : Energy = %f kcal mol-1" % ( (i+1)*1000, \
                                            system.energy().to(kcal_per_mol) )

print "Finished - final energy = %f kcal mol-1" % system.energy().to(kcal_per_mol)

for i in range(0, moves.count()):
    print moves[i]

