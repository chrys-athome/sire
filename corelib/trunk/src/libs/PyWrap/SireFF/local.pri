
#use the QtCore library
QT += core

sire.depends *= SireFF SireVol SireMol SireDB

#say that this is a python wrapper plugin
python.wrapper = FF

LIBS += $$boost.libs
INCLUDEPATH += $$python.headers $$boost.headers

