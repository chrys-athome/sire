
#use the QtCore library
QT += core

sire.depends *= SireVol SireMol SireBase SireMaths SireUnits

#say that this is a python wrapper plugin
python.wrapper = Vol

LIBS += $$boost.libs
INCLUDEPATH += $$python.headers $$boost.headers

