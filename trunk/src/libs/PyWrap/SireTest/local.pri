
#use the QtCore library
QT += core

sire.depends *= SireTest SireDB

#say that this is a python wrapper plugin
python.wrapper = Test

LIBS += $$boost.libs
INCLUDEPATH += $$python.headers $$boost.headers
