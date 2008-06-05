
#use the QtCore and QtSql libraries
QT += core sql

sire.depends *= SireDB SireMol

#say that this is a python wrapper plugin
python.wrapper = DB

LIBS += $$boost.libs
INCLUDEPATH += $$python.headers $$boost.headers

