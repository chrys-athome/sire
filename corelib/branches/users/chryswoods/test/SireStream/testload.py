
import Sire.Stream

filename = "test/SireStream/tmp_testdata.sire"

header = Sire.Stream.getDataHeader(filename)
print header.dataType()
print header.requiredLibraries()

print header.createdBy()
print header.createdWhen().toString()
print header.createdWhere()

print header.requiredMemory()
print header.compressionRatio()
print header.digest()
print header.repository()
print header.buildVersion()
print header.systemInfo()

system = Sire.Stream.load(filename)

print system.energies()

