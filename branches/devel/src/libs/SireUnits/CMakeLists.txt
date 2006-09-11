########################################
#
# CMake file for library:SireUnits
#
########################################

# Third Party dependencies of this module

# Other Sire libraries
include_directories(${CMAKE_SOURCE_DIR}/src/libs)

# Define the sources in SireUnits
set ( SIREUNITS_SOURCES
      convert.cpp            
    )

add_library (SireUnits SHARED ${SIREUNITS_SOURCES})

set_target_properties (SireUnits PROPERTIES
                       VERSION ${SIRE_VERSION}
                       SOVERSION ${SIRE_VERSION_MAJOR}
                      )
