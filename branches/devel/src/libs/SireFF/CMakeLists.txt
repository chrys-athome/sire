########################################
#
# CMake file for library:SireFF
#
########################################

# Third Party dependencies of this module
include_directories(${QT_QTSQL_INCLUDE_DIR})

# Other Sire libraries
include_directories(${CMAKE_SOURCE_DIR}/src/libs)

# Define the sources in SireFF
set ( SIREFF_SOURCES
      energy.cpp
      forcefield.cpp
    )

add_library (SireFF SHARED ${SIREFF_SOURCES})

set_target_properties (SireFF PROPERTIES
                       VERSION ${SIRE_VERSION}
                       SOVERSION ${SIRE_VERSION_MAJOR}
                      )

# Link to other Sire libraries
target_link_libraries (SireFF
                       SireDB
                       ${QT_QTSQL_LIBRARY}
                       )

