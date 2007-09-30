########################################
#
# CMake file for library:SireFF
#
########################################

# Third Party dependencies of this module
include_directories(${QT_QTSQL_INCLUDE_DIR})

# Other Sire libraries
include_directories(${CMAKE_SOURCE_DIR}/src/libs)

# Define the headers in SireFF
set ( SIREFF_HEADERS
      errors.h
      ffbase.h
      ffcalculator.h
      ffcalculatorbase.h
      ffcomponent.h
      ffexpression.h
      ffgroupid.h
      ffprocessor.h
      ffthreadprocessor.h
      ffthreadworker.h
      ffworker.h
      ffworkerbase.h
      forcefield.h
      forcefieldid.h
      forcefields.h
      forcefieldsbase.h
      forcetable.h
      qhash_sireff.h
    )

# Define the sources in SireFF
set ( SIREFF_SOURCES
      errors.cpp
      ffbase.cpp
      ffcalculatorbase.cpp
      ffcalculator.cpp
      ffcomponent.cpp
      ffexpression.cpp
      ffgroupid.cpp
      ffprocessor.cpp
      ffthreadprocessor.cpp
      ffthreadworker.cpp
      ffworkerbase.cpp
      ffworker.cpp
      forcefield.cpp
      forcefieldsbase.cpp
      forcefields.cpp
      forcetable.cpp
    
      ${SIREFF_HEADERS}
    )

add_library (SireFF ${SIREFF_SOURCES})

set_target_properties (SireFF PROPERTIES
                       VERSION ${SIRE_VERSION}
                       SOVERSION ${SIRE_VERSION_MAJOR}
                      )

# Link to other Sire libraries
target_link_libraries (SireFF
                       SireDB
                       SireCluster
                       SireBase
                       ${QT_QTSQL_LIBRARY}
                       )

# installation
install( TARGETS SireFF
         RUNTIME DESTINATION ${SIRE_BIN}
         LIBRARY DESTINATION ${SIRE_LIBS}
         ARCHIVE DESTINATION ${SIRE_ARCHIVES}
       )

install( FILES ${SIREFF_HEADERS} 
         DESTINATION ${SIRE_INCLUDES}/SireFF )
