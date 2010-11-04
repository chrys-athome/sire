########################################
#
# CMake file for library:SireDB
#
########################################

# Other Sire libraries
include_directories(${CMAKE_SOURCE_DIR}/src/libs)

# Set the compile options for Sqlite3
set ( SQLITE3_OPTIONS "-DSQLITE_THREADSAFE=0;-DSQLITE_OMIT_LOAD_EXTENSION" )

# Define the headers in SireDB
set ( SIREDB_HEADERS
      database.h
      diskcache.h
      errors.h
      table.h

      ThirdParty/sqlite3.h
    )

# Define the sources in SireDB
set ( SIREDB_SOURCES

      register_siredb.cpp

      database.cpp
      diskcache.cpp
      errors.cpp
      table.cpp

      ThirdParty/sqlite3.c
    
      ${SIREDB_HEADERS}
    )

# sqlite3 cannot be compiled with GCC -ffast-math
string( REGEX REPLACE "-ffast-math" " " CMAKE_C_FLAGS ${CMAKE_C_FLAGS} )
add_definitions( ${SQLITE3_OPTIONS} )

add_library (SireDB ${SIREDB_SOURCES})

set_target_properties (SireDB PROPERTIES
                       VERSION ${SIRE_VERSION}
                       SOVERSION ${SIRE_VERSION_MAJOR}
                      )

# Link to other Sire libraries
target_link_libraries (SireDB
                       SireBase
                       SireStream
                      )

# installation
install( TARGETS SireDB EXPORT SireLibraries
         RUNTIME DESTINATION ${SIRE_BIN}
         LIBRARY DESTINATION ${SIRE_LIBS}
         ARCHIVE DESTINATION ${SIRE_ARCHIVES}
       )

install( FILES ${SIREDB_HEADERS} 
         DESTINATION ${SIRE_INCLUDES}/SireDB )
