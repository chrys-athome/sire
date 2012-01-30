
from Sire.Sim import *

options = Options.fromXMLFile("share/SireSim/proteinligandmc.xml", ["mc"])

print options.toXML()

options = options.setValue("solute.name", "sb2")
print options.toConfig()

solute = options.getOption("solute")
print solute.getValue("name")
print solute.toConfig()

options = options.setValue("preferential_sampling.molecule", 1)

print options.toConfig()

print solute.getOption("solute")
