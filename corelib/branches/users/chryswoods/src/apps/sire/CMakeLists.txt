########################################
#
# CMake file for application:sire
#
########################################

# Third Party dependencies of this module
include_directories(${BOOST_INCLUDE_DIR})

# Other Sire libraries
include_directories(${CMAKE_SOURCE_DIR}/src/libs)

# Define the sources in sire
set ( SIRE_SOURCES
      main.cpp
    )

# generate the application
add_executable ( sire ${SIRE_SOURCES} )

set_target_properties (sire PROPERTIES
                       VERSION ${SIRE_VERSION}
                       SOVERSION ${SIRE_VERSION_MAJOR}
                      )

# Link to other Sire libraries
target_link_libraries (sire
                       SireIO
                       SireMM
                       SireMol
                      )

# installation
install( TARGETS sire
         RUNTIME DESTINATION ${SIRE_BIN}
         LIBRARY DESTINATION ${SIRE_LIBS}
         ARCHIVE DESTINATION ${SIRE_ARCHIVES}
       )
