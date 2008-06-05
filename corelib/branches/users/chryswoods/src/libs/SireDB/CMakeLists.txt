########################################
#
# CMake file for library:SireIO
#
########################################

# Third Party dependencies of this module
include_directories(${QT_QTSQL_INCLUDE_DIR})

# Other Sire libraries
include_directories(${CMAKE_SOURCE_DIR}/src/libs)

# Define the headers in SireDB
set ( SIREDB_HEADERS
      diskcache.h
      errors.h

      ThirdParty/sqlite3.h
    )

# Define the sources in SireDB
set ( SIREDB_SOURCES
      diskcache.cpp
      errors.cpp

      ThirdParty/sqlite3.c
    
      ${SIREDB_HEADERS}
    )

add_library (SireDB ${SIREDB_SOURCES})

set_target_properties (SireDB PROPERTIES
                       VERSION ${SIRE_VERSION}
                       SOVERSION ${SIRE_VERSION_MAJOR}
                      )

# Link to other Sire libraries
target_link_libraries (SireDB
                       SireBase
                       )

# installation
install( TARGETS SireDB
         RUNTIME DESTINATION ${SIRE_BIN}
         LIBRARY DESTINATION ${SIRE_LIBS}
         ARCHIVE DESTINATION ${SIRE_ARCHIVES}
       )

install( FILES ${SIREDB_HEADERS} 
         DESTINATION ${SIRE_INCLUDES}/SireDB )
