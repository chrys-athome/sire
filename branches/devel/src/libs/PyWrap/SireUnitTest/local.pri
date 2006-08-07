
#use the QtCore library, and SireDB requires QtSql
QT += core sql

sire.depends *= SireUnitTest SireFF SireMol SireCAS SireMaths

#say that this is a python wrapper plugin
python.wrapper = UnitTest

LIBS += $$boost.libs
INCLUDEPATH += $$python.headers $$boost.headers

