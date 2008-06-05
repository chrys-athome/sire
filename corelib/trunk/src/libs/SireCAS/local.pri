
#use the QtCore library
QT += core

sire.depends *= SireMaths SireError SireStream

LIBS += $$gsl.libs
INCLUDEPATH += $$boost.headers $$gsl.headers
