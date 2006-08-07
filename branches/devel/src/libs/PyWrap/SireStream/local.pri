
#use the QtCore library
QT += core

sire.depends *= SireStream

#say that this is a python wrapper plugin
python.wrapper = Stream

LIBS += $$boost.libs
INCLUDEPATH += $$python.headers $$boost.headers
