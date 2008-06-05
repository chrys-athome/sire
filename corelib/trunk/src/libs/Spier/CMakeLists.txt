########################################
#
# CMake file for library:Spier
#
########################################

# Third Party dependencies of this module
include_directories(${QT_QTGUI_INCLUDE_DIR}
                    ${QT_QTOPENGL_INCLUDE_DIR})

# Other Sire libraries
include_directories(${CMAKE_SOURCE_DIR}/src/libs)

# Define the sources in Spier
set ( SPIER_SOURCES
      button.cpp
      camera.cpp
      canvasui.cpp
      color.cpp
      cpkrep.cpp
      glcanvas.cpp
      glcylinder.cpp
      glcylindermesh.cpp
      glmaterial.cpp
      glmatrix.cpp
      glshader.cpp
      glsphere.cpp
      glspheremesh.cpp
      gltexture.cpp
      glwidget.cpp
      inputinterpreter.cpp
      mainwindow.cpp
      material.cpp
      materialmgr.cpp
      materialstate.cpp
      meshmgr.cpp
      moldisplaydata.cpp
      moleculerep.cpp
      molselectid.cpp
      mouseclickhelper.cpp
      piemenu.cpp
      piemenubutton.cpp
      selectid.cpp
      textlabel.cpp
      vdwrep.cpp
      viewer.cpp
      viewmode.cpp
      viewobj.cpp
      widgetanimator.cpp
      wrapper.cpp
    )

# Add the moc files
set ( SPIER_MOCS
      button.h
      camera.h
      canvasui.h
      glcanvas.h
      glwidgetbase.h
      inputinterpreter.h
      mainwindow.h
      textlabel.h
      viewer.h
      viewmode.h
    )

# generate rules for building source files that moc generates
qt4_wrap_cpp(SPIER_MOC_SRCS ${SPIER_MOCS})

# generate the library
add_library (Spier
                ${SPIER_SOURCES}
                ${SPIER_MOC_SRCS}
            )

set_target_properties (Spier PROPERTIES
                       VERSION ${SIRE_VERSION}
                       SOVERSION ${SIRE_VERSION_MAJOR}
                      )

# Link to other Sire libraries
target_link_libraries (Spier
                       SireIO
                       SireMol
                       SireMaths
                       ThirdPartyGUI
                       ${QT_QTGUI_LIBRARY}
                       ${QT_QTOPENGL_LIBRARY}
                       )

# installation
install( TARGETS Spier
         RUNTIME DESTINATION ${SIRE_BIN}
         LIBRARY DESTINATION ${SIRE_LIBS}
         ARCHIVE DESTINATION ${SIRE_ARCHIVES}
       )
