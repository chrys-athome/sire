########################################
#
# CMake file for application:sire_browser
#
########################################

if ( ${QT_QTGUI_FOUND} AND ${QT_QTWEBKIT_FOUND} )

    # Third Party dependencies of this module
    include_directories(${BOOST_INCLUDE_DIR})

    # Other Sire libraries
    include_directories(${CMAKE_SOURCE_DIR}/src/libs)

    QT4_GENERATE_MOC( pluginfactory.h pluginfactory_moc.cpp )

    # Define the sources in sire_browser
    set ( SIRE_BROWSER_SOURCES
          main.cpp
          pluginfactory.cpp
          pluginfactory_moc.cpp
        )

    # generate the application
    add_executable ( sire_browser ${SIRE_BROWSER_SOURCES} )

    set_target_properties (sire_browser PROPERTIES
                           VERSION ${SIRE_VERSION}
                           SOVERSION ${SIRE_VERSION_MAJOR}
                          )

    # Link to other Sire libraries
    target_link_libraries (sire_browser 
                           Spier
                           ${QT_QTCORE_LIBRARY}
                           ${QT_QTGUI_LIBRARY}
                           ${QT_QTWEBKIT_LIBRARY})

    # installation
    install( TARGETS sire_browser
             RUNTIME DESTINATION ${SIRE_BIN}
             LIBRARY DESTINATION ${SIRE_LIBS}
             ARCHIVE DESTINATION ${SIRE_ARCHIVES}
           )

else()
    message( STATUS "Not compiling sire_browser as the QtGui or QtWebkit "
                    "libraries are not available." )
endif()
