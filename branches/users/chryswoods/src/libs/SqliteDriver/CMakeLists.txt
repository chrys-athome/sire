########################################
#
# CMake file for library:SqliteDriver
#
########################################

# Third Party dependencies of this module
include_directories( ${CMAKE_SOURCE_DIR}/src/libs
                     ${QT_QTSQL_INCLUDE_DIR}
                   )

# Define the sources in SqliteDriver
set ( SQLITEDRIVER_SOURCES
      qsql_sqlite.cpp
    )

if (WIN32)
   #include windows-specific source files
   set( SQLITEDRIVER_OS_SOURCES os_win.c )
else (WIN32)
   #include unix-specific source files
   set( SQLITEDRIVER_OS_SOURCES os_unix.c )
endif (WIN32)

add_definitions( -DQT_NO_CAST_TO_ASCII -DQT_NO_CAST_FROM_ASCII )

add_library (SqliteDriver
                ${SQLITEDRIVER_SOURCES} ${SQLITEDRIVER_OS_SOURCES})

set_target_properties (SqliteDriver PROPERTIES
                       VERSION ${SIRE_VERSION}
                       SOVERSION ${SIRE_VERSION_MAJOR}
                      )

# Link to other libraries
target_link_libraries (SqliteDriver
                       ${QT_QTSQL_LIBRARY}
                       ${QT_QTCORE_LIBRARY}
                      )

# installation
install( TARGETS SqliteDriver
         RUNTIME DESTINATION ${SIRE_BIN}
         LIBRARY DESTINATION ${SIRE_LIBS}
         ARCHIVE DESTINATION ${SIRE_ARCHIVES}
       )
