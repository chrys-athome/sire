
from Sire.Mol import *
from Sire.Move import *
from Sire.FF import *
from Sire.MM import *
from Sire.System import *
from Sire.Units import *
from Sire.Maths import *
from Sire.Vol import *
from Sire.CAS import *
from Sire.IO import *
from Sire.Qt import *

timer = QTime()

# create a regular lattice of 125 LJ atoms (5 x 5 x 5)

mols = []

ljs = AtomicLJs( [ LJParameter(2.0*angstrom, 2.0*kcal_per_mol) ] )

for i in range(0,5):
   for j in range(0,5):
       for k in range(0,5):
       
           editmol = EditMol("LJ")
           editmol.add( ResNum(1), "LJ" )
           editmol.add( Atom( ("LJ",ResNum(1)), "H", Vector(2*i,2*j,2*k) ) )
           
           mol = Molecule(editmol)
           mol.setProperty("ljs", ljs)
           
           mols.append(mol)
           
space = PeriodicBox( (0,0,0), (10,10,10) )

switchfunc = HarmonicSwitchingFunction( 4.5, 4.0 )

ljff = InterLJFF(space, switchfunc)
ljff.add(mols)

ffields = ForceFields()
ffields.add(ljff)

all_mols = MoleculeGroup("all", mols)

system = System(all_mols, ffields)
system.setSpace(space)

mc = RigidBodyMC( UniformSampler(all_mols) )
mc.setMaximumTranslation( 0.1 * angstrom )

volmc = VolumeMove( MapAsMolecules(all_mols) )
volmc.setVolumeChangingFunction( UniformVolumeChange(50 * angstrom3) )

moves = WeightedMoves()

moves.add(mc, 125)
moves.add(volmc, 1)

#equilibration
for i in range(1,100):
    print "Running block %d" % i
    timer.start()
    moves = system.run(moves, 1000)
    
    print "Took %d ms" % timer.elapsed()
    print "Energy = %f | Volume = %f" % (system.forceFields().energy(),    
                                         system.info().space().volume())
                                         
    print "MC Accept %d Reject %d" % (moves.moves()[0].clone().nAccepted(),
                                      moves.moves()[0].clone().nRejected())
    
    print "VMC Accept %d Reject %d" % (moves.moves()[1].clone().nAccepted(),
                                       moves.moves()[1].clone().nRejected())

mols = system.info().groups().molecules()

#now add an RDF monitor
solute = MoleculeGroup("solute", mols[0])
solvent = MoleculeGroup("solvent", mols[1:])

groups = system.info().groups()
groups.add(solute)
groups.add(solvent)

monitors = SystemMonitors()

rdfmon = RDFMonitor(mols[0], solvent)
rdfmon.addRDF( [("LJ",ResNum(1))], [("LJ",ResNum(1))],
               RDF(0,5,20) )

monitors.set(Symbol("RDF"), rdfmon)

space = system.info().space()
system = System( groups, system.forceFields(), monitors )
system.setSpace(space)

for i in range(1,200):
    print "Running block %d" % i
    timer.start()
    moves = system.run(moves, 1000)
    
    print "Took %d ms" % timer.elapsed()
    print "Energy = %f | Volume = %f" % (system.forceFields().energy(),    
                                         system.info().space().volume())
                                         
    print "MC Accept %d Reject %d" % (moves.moves()[0].clone().nAccepted(),
                                      moves.moves()[0].clone().nRejected())
    
    print "VMC Accept %d Reject %d" % (moves.moves()[1].clone().nAccepted(),
                                       moves.moves()[1].clone().nRejected())

rdfmon = system.monitors().monitor(Symbol("RDF"))

rdf = rdfmon.getRDF( [("LJ",ResNum(1))], [("LJ",ResNum(1))] )

rdf = rdf.normalise()

import os
xmgrace = os.popen("xmgrace -pipe", "w")

for point in rdf:
    print >>xmgrace,"%f  %f" % point


