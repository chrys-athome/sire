from Sire.Sim import *

o1 = Option("name", "The name of the simulation", StringValue("my sim"))
o2 = Option("input_dir", "The input directory", StringValue("input"), True)
o3 = Option("output_dir", "The output directory", StringValue("output"))
o4 = Option("temperature", "The simulation temperature", StringValue("25"), True)
o5_1 = Option("name", "The name of the solute", StringValue("ligand"), True)
o5_2 = Option("molecule", "The identity of the solute molecule", StringValue("file:1,molecule:2"))
o5 = Option("solute", "Configure solutes in the system", Options([o5_1,o5_2]), True, True)

options = Options( [o1, o2, o3, o4, o5] )

print "Initial, default configuration file"
print options.toConfig(True)

print "\nAfter setting some options..."
new_opts = options.setValue("solute[2].name", "osel")
print new_opts.toConfig(True)

print "\nNow trying to load up the same configuration file..."

new_opts = options.fromConfig( new_opts.toConfig() )
print new_opts.toConfig()

print "\nNow parsing an XML configuration file..."
options = Options.fromXMLFile("test/SireSim/testoptions.xml")
print options.toConfig()

print "\n\nHere is the XML"
print new_opts.toXML()
print options.toXML()
