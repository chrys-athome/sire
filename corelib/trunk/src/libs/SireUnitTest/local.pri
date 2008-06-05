
#use the QtCore library, and SireDB requires QtSql
QT += core sql

sire.depends *= SireFF SireMol SireCAS SireMaths

LIBS += $$boost.libs
INCLUDEPATH += $$boost.headers

RESOURCES += resources.qrc
