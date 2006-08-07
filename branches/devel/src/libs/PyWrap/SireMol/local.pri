
#use the QtCore library
QT += core

sire.depends *= SireMol SireBase SireMaths SireUnits

#say that this is a python wrapper plugin
python.wrapper = Mol

LIBS += $$boost.libs
INCLUDEPATH += $$python.headers $$boost.headers
