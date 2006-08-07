
#use the QtCore library
QT += core

#say that this is a python wrapper plugin
python.wrapper = Base

sire.depends *= SireBase SireError SireStream ThirdParty

LIBS += $$boost.libs
INCLUDEPATH += $$python.headers $$boost.headers
