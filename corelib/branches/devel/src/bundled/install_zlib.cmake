###
### CMake file to install zlib
### in the Sire bundle
###

# First, try to find the zlib library in the "bundled" directory. If it exists,
# then we don't need to do anything
unset(ZLIB_LIBRARY CACHE)
find_library( ZLIB_LIBRARY "z" PATHS ${BUNDLE_STAGEDIR}/lib NO_DEFAULT_PATH )

if ( ZLIB_LIBRARY )
  message( STATUS "Have already compiled a bundled version of zlib")
  set( ZLIB_INCLUDE_DIR "${BUNDLE_STAGEDIR}/include" )  
else()
  message( STATUS "Compiling and installing a bundled version of zlib" )

  set( ZLIB_ZIPFILE "${CMAKE_SOURCE_DIR}/src/bundled/zlib.tar.gz" )

  if (EXISTS "${ZLIB_ZIPFILE}")
    set( ZLIB_BUILD_DIR "${BUNDLE_BUILDDIR}/zlib" )

    if (NOT EXISTS "${ZLIB_BUILD_DIR}")
      message( STATUS "Unzipping ${ZLIB_ZIPFILE} to ${ZLIB_BUILD_DIR}" )
      execute_process(
          COMMAND ${CMAKE_COMMAND} -E tar xzf ${ZLIB_ZIPFILE}
          WORKING_DIRECTORY ${BUNDLE_BUILDDIR}
      )
    endif()

    set( ZLIB_OPTIONS "--prefix=${BUNDLE_STAGEDIR}" )
    set(ENV{CC} ${CMAKE_C_COMPILER})
    message( STATUS "${ZLIB_OPTIONS}" )

    message( STATUS "Patience... Configuring zlib..." )
    execute_process( COMMAND ${ZLIB_BUILD_DIR}/configure ${ZLIB_OPTIONS}
                     WORKING_DIRECTORY ${ZLIB_BUILD_DIR} )

    message( STATUS "Patience... Compiling zlib..." )
    execute_process( COMMAND ${CMAKE_MAKE_PROGRAM} -k -j ${NCORES}
                     WORKING_DIRECTORY ${ZLIB_BUILD_DIR} )

    message( STATUS "Patience... Installing zlib..." )
    execute_process( COMMAND ${CMAKE_MAKE_PROGRAM} install
                   WORKING_DIRECTORY ${ZLIB_BUILD_DIR} )

    set( ZLIB_INCLUDE_DIR "${BUNDLE_STAGEDIR}/include" )

    if (APPLE)
      # create a '.dylib' link to the library (uses .so by default)
      set( ZLIB_LIBRARY "${BUNDLE_STAGEDIR}/lib/libz.dylib" )
      execute_process( COMMAND ln -s libz.so libz.dylib
                       WORKING_DIRECTORY ${BUNDLE_STAGEDIR}/lib )
      execute_process( COMMAND ${CMAKE_INSTALL_NAME_TOOL} -id "@rpath/libz.1.dylib" ${ZLIB_LIBRARY} )
    else()
      set( ZLIB_LIBRARY "${BUNDLE_STAGEDIR}/lib/libz.so" )
    endif()
  endif()
endif()

find_package( ZLIB REQUIRED )

if ( ZLIB_LIBRARY )
  message( STATUS "Using zlib from ${ZLIB_LIBRARY}" )

  set( SIRE_FOUND_ZLIB TRUE )
else()
  message( STATUS "Strange? Cannot find the installed zlib library. We cannot compile it, so will need to rely on the system version..." )
endif()
