
#use the QtCore library
QT += core

#say that this is a python wrapper plugin
python.wrapper = Qt

LIBS += $$boost.libs
INCLUDEPATH += $$python.headers $$boost.headers
