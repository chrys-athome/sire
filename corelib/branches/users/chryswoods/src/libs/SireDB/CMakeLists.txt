########################################
#
# CMake file for library:SireDB
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

      register_siredb.cpp

      diskcache.cpp
      errors.cpp

      ThirdParty/sqlite3.c
    
      ${SIREDB_HEADERS}
    )

add_library (SireDB ${SIREDB_SOURCES})

find_library (DL_LIBRARY dl DOC "System dynamic linking library (contains dlopen)")
find_library (PTHREAD_LIBRARY pthread DOC "System pthread library")

set (DL_FOUND "NO")
set (PTHREAD_FOUND "NO")

if ( DL_LIBRARY )
  set (DL_FOUND "YES")
  message ( "Found DL library : ${DL_LIBRARY}" )
else()
  message (FATAL_ERROR "The dl (dynamic linking) library is required" )
endif()

if ( PTHREAD_LIBRARY )
  set (PTHREAD_FOUND "YES")
  message ( "Found pthread library : ${PTHREAD_LIBRARY}" )
else()
  message (FATAL_ERROR "The pthread library is required" )
endif()

set_target_properties (SireDB PROPERTIES
                       VERSION ${SIRE_VERSION}
                       SOVERSION ${SIRE_VERSION_MAJOR}
                      )

# Link to other Sire libraries
target_link_libraries (SireDB
                       SireBase
                       SireStream
                       ${DL_LIBRARY}
                       ${PTHREAD_LIBRARY}
                       ${QT_QTSQL_LIBRARY}
                      )

# installation
install( TARGETS SireDB EXPORT SireLibraries
         RUNTIME DESTINATION ${SIRE_BIN}
         LIBRARY DESTINATION ${SIRE_LIBS}
         ARCHIVE DESTINATION ${SIRE_ARCHIVES}
       )

install( FILES ${SIREDB_HEADERS} 
         DESTINATION ${SIRE_INCLUDES}/SireDB )
