
#use the QtCore and QtSql libraries
QT += core sql

sire.depends *= SireMM SireVol SireMol SireDB SireFF

#say that this is a python wrapper plugin
python.wrapper = MM

LIBS += $$boost.libs
INCLUDEPATH += $$python.headers $$boost.headers

