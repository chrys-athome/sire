########################################
#
# CMake file for library:SireSystem
#
########################################

# Third Party dependencies of this module

# Other Sire libraries
include_directories(${CMAKE_SOURCE_DIR}/src/libs)

# Define the sources in SireSystem
set ( SIRESYSTEM_SOURCES
      localsimsystem.cpp
      move.cpp
      moves.cpp
      simsystem.cpp
      system.cpp
      systemdata.cpp
    )

add_library (SireSystem ${SIRESYSTEM_SOURCES})

set_target_properties (SireSystem PROPERTIES
                       VERSION ${SIRE_VERSION}
                       SOVERSION ${SIRE_VERSION_MAJOR}
                      )

# Link to other Sire libraries
target_link_libraries (SireSystem
                       SireFF
                       SireMol
                       SireCAS
                       SireBase
                       )

# installation
install( TARGETS SireSystem
         RUNTIME DESTINATION ${SIRE_BIN}
         LIBRARY DESTINATION ${SIRE_LIBS}
         ARCHIVE DESTINATION ${SIRE_ARCHIVES}
       )
