
#use the QtCore library
QT += core

sire.depends *= SireUnits

#say that this is a python wrapper plugin
python.wrapper = Units

LIBS += $$boost.libs
INCLUDEPATH += $$python.headers $$boost.headers
