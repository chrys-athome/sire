
LIBS += $$QMAKE_LIBS_OPENGL
INCLUDEPATH += $$QMAKE_INCDIR_OPENGL

#disable symbol hiding as the third party code has not
#been written to support it
sire.no_symbol_hiding = true
