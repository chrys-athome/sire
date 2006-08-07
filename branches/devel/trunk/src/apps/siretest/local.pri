
#use the QtCore library
QT += core

sire.depends *= SireMaths SireCAS SireMol \
                SireVol SireDB SireFF SireMM SireIO SirePy SireTest \
                SqliteDriver SireBase SireUnits SireError \
                SireStream SireUnitTest ThirdParty

LIBS += -lboost_unit_test_framework $$python.libs $$boost.libs
INCLUDEPATH += $$python.headers $$boost.headers

#once compiled, write the command file
cmdfile = $$writeCmdFile(siretest,siretest_bin)
sire.cmdfile.files = $$cmdfile
sire.cmdfile.path = $$sire.install.bin

INSTALLS += sire.cmdfile

