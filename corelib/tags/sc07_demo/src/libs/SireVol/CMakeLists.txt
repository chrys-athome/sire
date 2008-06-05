########################################
#
# CMake file for library:SireVol
#
########################################

# Third Party dependencies of this module

# Other Sire libraries
include_directories(${CMAKE_SOURCE_DIR}/src/libs)

# Define the sources in SireVol
set ( SIREVOL_SOURCES
      aabox.cpp
      cartesian.cpp
      coordgroup.cpp
      mappingfunction.cpp
      periodicbox.cpp
      space.cpp
    )

add_library (SireVol ${SIREVOL_SOURCES})

set_target_properties (SireVol PROPERTIES
                       VERSION ${SIRE_VERSION}
                       SOVERSION ${SIRE_VERSION_MAJOR}
                       LINK_FLAGS ${SIRE_LINK_FLAGS}
                      )

# Link to other Sire libraries
target_link_libraries (SireVol
                       SireMaths
                       SireBase
                       SireStream
                       )

# installation
install( TARGETS SireVol
         RUNTIME DESTINATION ${SIRE_BIN}
         LIBRARY DESTINATION ${SIRE_LIBS}
         ARCHIVE DESTINATION ${SIRE_ARCHIVES}
       )
