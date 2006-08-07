
#use the QtCore and QtSql libraries
QT += core sql

sire.depends *= SireMol SireCAS SqliteDriver

LIBS += 
INCLUDEPATH += $$boost.headers

