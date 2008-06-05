
#use the QtCore library
QT += core

sire.depends *= SireError

#say that this is a python wrapper plugin
python.wrapper = Error

LIBS += $$boost.libs
INCLUDEPATH += $$python.headers $$boost.headers
