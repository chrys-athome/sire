
sire.depends *= Spier SireMaths SireMol SireBase \
                SireError SireStream SireCAS SireVol SireDB SireMM SireFF SireIO \
                SqliteDriver ThirdParty ThirdPartyGUI SireUnits

#Use the QtCore, QtGui and QtOpenGL libraries
QT += core gui opengl

LIBS += $$python.libs $$boost.libs
INCLUDEPATH += $$python.headers $$boost.headers

#once compiled, write the command file
cmdfile = $$writeCmdFile(spier,spier_bin)
sire.cmdfile.files = $$cmdfile
sire.cmdfile.path = $$sire.install.bin

INSTALLS += sire.cmdfile

