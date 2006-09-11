########################################
#
# CMake file for application:spier
#
########################################

# Third Party dependencies of this module
include_directories(${QT_QTGUI_INCLUDE_DIR}
                    ${QT_QTOPENGL_INCLUDE_DIR}
		   )

# Other Sire libraries
include_directories(${CMAKE_SOURCE_DIR}/src/libs)

# Define the sources in siretest
set ( SPIER_SOURCES
      main.cpp
    )

# generate the application
add_executable ( spier ${SPIER_SOURCES} )

set_target_properties (spier PROPERTIES
                       VERSION ${SIRE_VERSION}
                       SOVERSION ${SIRE_VERSION_MAJOR}
                      )

# Link to other Sire libraries
target_link_libraries (spier
                       Spier
                      )

