
#use the QtCore library
QT += core

sire.depends *= SireMaths SireCAS SireMol \
                SireVol SireDB SireIO SireFF SireMM SirePy SireTest \
                SqliteDriver SireBase SireUnits SireError \
                SireStream ThirdParty

#once compiled, write the command file
cmdfile = $$writeCmdFile(sire,sire_bin)
sire.cmdfile.files = $$cmdfile
sire.cmdfile.path = $$sire.install.bin

INSTALLS += sire.cmdfile

LIBS += $$python.libs $$boost.libs
INCLUDEPATH += $$python.headers $$boost.headers

