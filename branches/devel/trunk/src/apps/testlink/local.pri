
#use the QtCore library
QT += core sql opengl gui

LIBS += $$python.libs $$boost.libs -L$${sire.root}/.wrappers
LIBS += -lboost_unit_test_framework
INCLUDEPATH += $$python.headers $$boost.headers

#### A python script will append the names 
#### of all sire libraries below, so that they
#### are all linked together, thus helping in 
#### the discovery of any missing symbols

LIBS += -lSireCAS -lSireUnits -lSireMol -lSireError -lSireVol -lSireDB -lSireFF -lSireIO -lSireMM -lThirdParty -lSirePy -lSireTest -lSireMaths -lSpier -lSireStream -lSqliteDriver -lSireUnitTest -lThirdPartyGUI -lSireBase -l_Stream -l_Maths -l_CAS -l_Test -l_Mol -l_Error -l_Vol -l_DB -l_FF -l_IO -l_MM -l_Qt -l_Base
