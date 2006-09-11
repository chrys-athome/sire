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
      cartesian.cpp
      periodicbox.cpp
      simvolume.cpp
    )

add_library (SireVol SHARED ${SIREVOL_SOURCES})

set_target_properties (SireVol PROPERTIES
                       VERSION ${SIRE_VERSION}
                       SOVERSION ${SIRE_VERSION_MAJOR}
                      )

# Link to other Sire libraries
target_link_libraries (SireVol
                       SireMol
                       )
