########################################
#
# CMake file for library:SireSystem
#
########################################

# Third Party dependencies of this module

# Other Sire libraries
include_directories(${CMAKE_SOURCE_DIR}/src/libs)

# Define the headers in SireSystem
set ( SIRESYSTEM_HEADERS
      checkpoint.h
      errors.h
      localsimsystem.h
      monitorenergy.h
      move.h
      moves.h
      qhash_siresystem.h
      querysystem.h
      rdfmonitor.h
      simsystem.h
      system.h
      systemdata.h
      systemid.h
      systemmonitor.h
      systemmonitors.h
    )

# Define the sources in SireSystem
set ( SIRESYSTEM_SOURCES
      checkpoint.cpp
      errors.cpp
      localsimsystem.cpp
      monitorenergy.cpp
      move.cpp
      moves.cpp
      querysystem.cpp
      rdfmonitor.cpp
      simsystem.cpp
      system.cpp
      systemdata.cpp
      systemmonitor.cpp
      systemmonitors.cpp
    
      ${SIRESYSTEM_HEADERS}
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

install( FILES ${SIRESYSTEM_HEADERS}
         DESTINATION ${SIRE_INCLUDES}/SireSystem )
