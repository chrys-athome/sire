
from SireFF import *
from SireMol import *
from SireIO import *
from SireSystem import *
from SireLogger import *

mmdb = MMDB()

ProtoMS().read("./test/input/oplsaa.clj", mmdb)
ProtoMS().read("./test/input/solvent.clj", mmdb)
ProtoMS().read("./test/input/oplsa.tmpl", mmdb)

mols = PDB().read("./test/input/protein.pdb")

protein = flexibleAndResidueCutting(mols[0])
protein.setName("MyProtein")

params = mmdb.parametise(protein)

print "***Protein residue charges***"
for resparams in params:
	print "%s : %f" % (resparams.residueID(), resparams.totalCharge())

print "***Parametisation Warnings***"
print params.warnings()

print "***Parametisation Errors***"
print params.errors()

params.setCharge( AtomIndex("HA",13), 0.05 )
params.setCharge( AtomIndex("CA",13), -0.05 )

waters = PDB().read("./test/input/waters.pdb")

for water in waters:
	water = rigidAndMoleculeCutting(water)
	water.setName("Water")
	
waterparams = mmdb.parametise(water[0])

print "***Water parameters***
for atomparam in waterparams[0]:
	print "%s : Chg = %f : Sig = %f : Eps = %f" % (atomparam.atom(),atomparam.charge() \
	                                               atomparam.sigma(),atomparam.epsilon())
												   
system = SimSystem("Protein in Water")

cljff = InterCLJFF()
cljff.add(protein,params)
cljff.add(waters,waterparams)

intraff = OPLSIntraFF()
intraff.add(protein,params)

system.forcefield().add( "Protein-Water", cljff )
system.forcefield().add( "Protein Intra", intraff )

sim = Simulation(system)

moves = MCMove(protein) + 1000 * MCMove(waters)

simstatus = sim.run(moves)

errorlogger = Logger("Simulation Errors", simstatus, ["WARNING","FATAL"])
output = Logger("Simulation Output", simstatus, ["INFO", "MOVE", "ENERGY", "HEADER"])

errorlogger.log("./output/errors.txt")
output.log("./output/output.txt")

print "Thankyou and goodnight :-)"


#####################
#
# For FEP/FDTI...

ligand = Mol2().read(....)

ligandparams = mmdb.parametise(ligand)

ligandclj = InterGroupCLJFF()
ligandclj.addToA(ligand, ligandparams)

system.forcefield().add("Ligand-System", ligandclj)

system.forcefield().setTotal( cljff + intraff + lmbda*ligandclj )


