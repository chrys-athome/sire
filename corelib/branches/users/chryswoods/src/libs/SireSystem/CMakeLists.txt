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
      constraint.h
      constraints.h
      checkpoint.h
      errors.h
      identityconstraint.h
      moleculeconstraint.h
      monitorcomponent.h
      monitorcomponents.h
      monitorid.h
      monitoridentifier.h
      monitoridx.h
      monitormonitor.h
      monitorname.h
      rdfmonitor.h
      spacewrapper.h
      sysid.h
      sysidentifier.h
      sysidx.h
      sysname.h
      system.h
      systemmonitor.h
      systemmonitors.h
    )

# Define the sources in SireSystem
set ( SIRESYSTEM_SOURCES

      register_siresystem.cpp

      constraint.cpp
      constraints.cpp
      checkpoint.cpp
      errors.cpp
      identityconstraint.cpp
      moleculeconstraint.cpp
      monitorcomponent.cpp
      monitorcomponents.cpp
      monitorid.cpp
      monitoridentifier.cpp
      monitormonitor.cpp
      rdfmonitor.cpp
      spacewrapper.cpp
      sysid.cpp
      sysidentifier.cpp
      system.cpp
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
                       SireStream
                       )

# installation
install( TARGETS SireSystem EXPORT SireLibraries
         RUNTIME DESTINATION ${SIRE_BIN}
         LIBRARY DESTINATION ${SIRE_LIBS}
         ARCHIVE DESTINATION ${SIRE_ARCHIVES}
       )

install( FILES ${SIRESYSTEM_HEADERS}
         DESTINATION ${SIRE_INCLUDES}/SireSystem )

