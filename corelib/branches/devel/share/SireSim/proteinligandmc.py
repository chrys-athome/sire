
from Sire.Sim import *

options = Options.fromXMLFile("share/SireSim/proteinligandmc.xml", ["mc"])

print "AS AN XML FILE"
print options.toXML()

print "\n\nAS A CONFIG FILE"
print options.toConfig(True)
