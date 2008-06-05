
#use the QtCore, QtGui and QtOpenGL libraries
QT += core gui opengl

CONFIG *= moleculerep canvasui glcanvas spiercore

sire.depends *= SireMol ThirdPartyGUI

LIBS += 
INCLUDEPATH += $$boost.headers

