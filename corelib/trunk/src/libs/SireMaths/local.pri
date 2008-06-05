
#use the QtCore library
QT += core

sire.depends *= SireError SireStream

LIBS += $$gsl.libs
INCLUDEPATH += $$boost.headers $$gsl.headers
