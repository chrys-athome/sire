
#use the QtCore library
QT += core

#say that this is a python wrapper plugin
python.wrapper = CAS

sire.depends *= SireCAS SireMaths SireError SireStream

LIBS += $$boost.libs $$gsl.libs
INCLUDEPATH += $$python.headers $$boost.headers $$gsl.headers
