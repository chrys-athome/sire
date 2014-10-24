###
### CMake file to install sqlite
### in the Sire bundle
###

# First, try to find the sqlite library in the "bundled" directory. If it exists,
# then we don't need to do anything
unset(SQLITE_LIBRARY CACHE)
find_library( SQLITE_LIBRARY "sqlite3" PATHS ${BUNDLE_STAGEDIR}/lib NO_DEFAULT_PATH )

if ( SQLITE_LIBRARY )
  message( STATUS "Have already compiled a bundled version of sqlite")
  set( SQLITE_INCLUDE_DIR "${BUNDLE_STAGEDIR}/include" )  
else()
  message( STATUS "Compiling and installing a bundled version of sqlite" )

  set( SQLITE_ZIPFILE "${CMAKE_SOURCE_DIR}/src/bundled/sqlite.tar.gz" )

  if (EXISTS "${SQLITE_ZIPFILE}")
    set( SQLITE_BUILD_DIR "${BUNDLE_BUILDDIR}/sqlite" )

    if (NOT EXISTS "${SQLITE_BUILD_DIR}")
      message( STATUS "Unzipping ${SQLITE_ZIPFILE} to ${SQLITE_BUILD_DIR}" )
      execute_process(
          COMMAND ${CMAKE_COMMAND} -E tar xzf ${SQLITE_ZIPFILE}
          WORKING_DIRECTORY ${BUNDLE_BUILDDIR}
      )
    endif()

    set( SQLITE_OPTIONS "--prefix=${BUNDLE_STAGEDIR}" )
    list( APPEND SQLITE_OPTIONS "--enable-shared" )
    set(ENV{CC} ${CMAKE_C_COMPILER})
    message( STATUS "${SQLITE_OPTIONS}" )

    message( STATUS "Patience... Configuring sqlite..." )
    execute_process( COMMAND ${SQLITE_BUILD_DIR}/configure ${SQLITE_OPTIONS}
                     WORKING_DIRECTORY ${SQLITE_BUILD_DIR} )

    message( STATUS "Patience... Compiling sqlite..." )
    execute_process( COMMAND ${CMAKE_MAKE_PROGRAM} -k -j ${NCORES}
                     WORKING_DIRECTORY ${SQLITE_BUILD_DIR} )

    message( STATUS "Patience... Installing sqlite..." )
    execute_process( COMMAND ${CMAKE_MAKE_PROGRAM} install
                   WORKING_DIRECTORY ${SQLITE_BUILD_DIR} )

    set( SQLITE_INCLUDE_DIR "${BUNDLE_STAGEDIR}/include" )

    if (APPLE)
      set( SQLITE_LIBRARY "${BUNDLE_STAGEDIR}/lib/libsqlite3.dylib" )
      execute_process( COMMAND ${CMAKE_INSTALL_NAME_TOOL} -id "@rpath/libsqlite3.0.dylib" ${SQLITE_LIBRARY} )
    else()
      set( SQLITE_LIBRARY "${BUNDLE_STAGEDIR}/lib/libsqlite3.so" )
    endif()
  endif()
endif()

if ( SQLITE_LIBRARY )
  message( STATUS "Using sqlite from ${SQLITE_LIBRARY}" )

  set( SIRE_FOUND_SQLITE TRUE )
else()
  message( STATUS "Strange? Cannot find the installed sqlite library. We cannot compile it, so will need to rely on the system version..." )
endif()
