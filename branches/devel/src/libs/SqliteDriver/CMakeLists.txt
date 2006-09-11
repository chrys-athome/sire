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
      alter.c
      analyze.c
      attach.c
      auth.c
      btree.c
      build.c
      callback.c
      date.c
      delete.c
      expr.c
      func.c
      hash.c
      insert.c
      legacy.c
      main.c
      opcodes.c
      pager.c
      parse.c
      pragma.c
      prepare.c
      printf.c
      qsql_sqlite.cpp
      random.c
      select.c
      table.c
      tokenize.c
      trigger.c
      update.c
      utf.c
      util.c
      vacuum.c
      vdbe.c
      vdbeapi.c
      vdbeaux.c
      vdbefifo.c
      vdbemem.c
      where.c
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
                      )

# installation
install( TARGETS SqliteDriver
         RUNTIME DESTINATION ${SIRE_BIN}
         LIBRARY DESTINATION ${SIRE_LIBS}
         ARCHIVE DESTINATION ${SIRE_ARCHIVES}
       )
