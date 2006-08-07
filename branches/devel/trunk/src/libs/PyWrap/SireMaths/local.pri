
#use the QtCore library
QT += core

sire.depends *= SireMaths SireError SireStream

#say that this is a python wrapper plugin
python.wrapper = Maths

LIBS += $$boost.libs $$gsl.libs
INCLUDEPATH += $$python.headers $$boost.headers $$gsl.headers
