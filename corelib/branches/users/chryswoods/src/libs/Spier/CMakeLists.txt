########################################
#
# CMake file for library:Spier
#
########################################

if ( QT_QTGUI_FOUND AND QT_QTOPENGL_FOUND )

    # Third Party dependencies of this module
    find_package( OpenGL REQUIRED )

    include_directories( ${OPENGL_INCLUDE_DIR} )

    # Other Sire libraries
    include_directories(${CMAKE_SOURCE_DIR}/src/libs)

    # Define the files on which moc must be run
    set ( SPIER_MOC_HEADERS
          renderview.h
          spierview.h
        )

    foreach ( mocfile ${SPIER_MOC_HEADERS} )
        string(REGEX REPLACE "\\.h$" "_moc.cpp" mocsource ${mocfile} )
        QT4_GENERATE_MOC( ${mocfile} ${mocsource} )
        list(APPEND mocsources ${mocsource})
    endforeach ( mocfile )

    # Define the header files for Spier
    set ( SPIER_HEADERS

          camera.h
          color.h

          glbackground.h
          glcanvas.h
          gldisplaylist.h
          glmatrix.h
          glmesh.h
          glsphere.h

          ${SPIER_MOC_HEADERS}
        )

    # Define the sources in Spier
    set ( SPIER_SOURCES

          register_spier.cpp

          camera.cpp
          color.cpp

          glbackground.cpp
          glcanvas.cpp
          gldisplaylist.cpp
          glmatrix.cpp
          glmesh.cpp
          glsphere.cpp

          renderview.cpp
          spierview.cpp

          ${SPIER_HEADERS}
          ${mocsources}
        )

    add_library (Spier ${SPIER_SOURCES})

    set_target_properties (Spier PROPERTIES
                                 VERSION ${SIRE_VERSION}
                                 SOVERSION ${SIRE_VERSION_MAJOR}
                          )

    # Link to other Sire libraries
    target_link_libraries (Spier
                           SireStream
                           SireMaths
                           ${QT_QTGUI_LIBRARY}
                           ${QT_QTOPENGL_LIBRARY}
                           ${OPENGL_gl_LIBRARY}
                           ${OPENGL_glu_LIBRARY}
                           )

    # installation
    install( TARGETS Spier EXPORT SireLibraries
             RUNTIME DESTINATION ${SIRE_BIN}
             LIBRARY DESTINATION ${SIRE_LIBS}
             ARCHIVE DESTINATION ${SIRE_ARCHIVES}
           )

    install( FILES ${SPIER_HEADERS} 
             DESTINATION ${SIRE_INCLUDES}/Spier )

else()
    message( STATUS "Not building the Spier library as QtGui or QtOpenGL are not available." )
endif()
