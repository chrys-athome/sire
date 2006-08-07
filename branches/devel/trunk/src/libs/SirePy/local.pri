
#use the QtCore library
QT += core

LIBS += $$python.libs
INCLUDEPATH += $$python.headers $$boost.headers

sire.depends  *= SireError
