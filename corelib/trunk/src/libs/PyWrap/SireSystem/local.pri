CONFIG += wrappers

sire.depends *= SireSystem SireError SireStream SireMol SireFF

#say that this is a python wrapper plugin
python.wrapper = System

LIBS += $$python.libs $$boost.libs
INCLUDEPATH += $$python.headers $$boost.headers
