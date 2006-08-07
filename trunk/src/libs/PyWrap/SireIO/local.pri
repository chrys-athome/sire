
#use the QtCore library, and SireDB depends on QtSql
QT += core sql

sire.depends *= SireIO SireMol SireMM

#say that this is a python wrapper plugin
python.wrapper = IO

LIBS += $$boost.libs
INCLUDEPATH += $$python.headers $$boost.headers

