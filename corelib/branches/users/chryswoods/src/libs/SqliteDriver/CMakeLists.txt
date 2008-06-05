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
      sqlite3.c
    )

set ( SQLITEDRIVER_MOCS
      qsql_sqlite.h
    )

# generate rules for building the source files that moc generates
qt4_wrap_cpp( SQLITEDRIVER_MOC_SRCS ${SQLITEDRIVER_MOCS} )

add_definitions( -DQT_NO_CAST_TO_ASCII -DQT_NO_CAST_FROM_ASCII )

add_library (SqliteDriver ${SQLITEDRIVER_SOURCES} ${SQLITEDRIVER_MOC_SRCS} )

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
