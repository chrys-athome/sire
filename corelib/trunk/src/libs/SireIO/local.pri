
#use the QtCore library, and SireDB depends on QtSql
QT += core sql

sire.depends *= SireMol SireMM

LIBS += 
INCLUDEPATH += $$boost.headers

