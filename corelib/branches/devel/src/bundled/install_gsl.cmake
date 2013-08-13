###
### CMake file to install the GNU Scientific Library
### in the Sire bundle
###

# First, try to find the GSL library in the "bundled" directory. If it exists,
# then we don't need to do anything
unset(GSL_LIBRARY CACHE)
unset(GSL_CBLAS_LIBRARY CACHE)
find_library( GSL_LIBRARY "gsl" PATHS ${BUNDLE_STAGEDIR}/lib NO_DEFAULT_PATH )
find_library( GSL_CBLAS_LIBRARY "gslcblas" PATHS ${BUNDLE_STAGEDIR}/lib NO_DEFAULT_PATH )

if ( GSL_LIBRARY AND GSL_CBLAS_LIBRARY )
  message( STATUS "Have already compiled a bundled version of GSL")

else()
  message( STATUS "Compiling and installing a bundled version of the GNU Scientific Library (GSL)" )

  set( GSL_ZIPFILE "${CMAKE_SOURCE_DIR}/src/bundled/gsl.tar.gz" )

  if (EXISTS "${GSL_ZIPFILE}")
    set( GSL_BUILD_DIR "${BUNDLE_BUILDDIR}/gsl" )

    if (NOT EXISTS "${GSL_BUILD_DIR}")
      message( STATUS "Unzipping ${GSL_ZIPFILE} to ${GSL_BUILD_DIR}" )
      execute_process(
          COMMAND ${CMAKE_COMMAND} -E tar xzf ${GSL_ZIPFILE}
          WORKING_DIRECTORY ${BUNDLE_BUILDDIR}
      )
    endif()

    set( GSL_OPTIONS "--enable-static=no;--enable-shared=yes;--prefix=${BUNDLE_STAGEDIR};CC=${CMAKE_C_COMPILER}" )
    message( STATUS "${GSL_OPTIONS}" )

    message( STATUS "Patience... Configuring GSL..." )
    execute_process( COMMAND ${GSL_BUILD_DIR}/configure ${GSL_OPTIONS}
                     WORKING_DIRECTORY ${GSL_BUILD_DIR} )

    message( STATUS "Patience... Compiling GSL..." )
    execute_process( COMMAND ${CMAKE_MAKE_PROGRAM}
                     WORKING_DIRECTORY ${GSL_BUILD_DIR} )

    message( STATUS "Patience... Installing GSL..." )
    execute_process( COMMAND ${CMAKE_MAKE_PROGRAM} install
                   WORKING_DIRECTORY ${GSL_BUILD_DIR} )

    unset(GSL_LIBRARY CACHE)
    unset(GSL_CBLAS_LIBRARY CACHE)
    find_library( GSL_LIBRARY "gsl" PATHS ${BUNDLE_STAGEDIR}/lib NO_DEFAULT_PATH )
    find_library( GSL_CBLAS_LIBRARY "gslcblas" PATHS ${BUNDLE_STAGEDIR}/lib NO_DEFAULT_PATH )

    if ( GSL_LIBRARY )
      # need to set the install name so that we can find the library when it is 
      # placed into the bundle directory
      if (APPLE)
        execute_process( COMMAND ${CMAKE_INSTALL_NAME_TOOL} -id "@rpath/libgsl.0.dylib" ${GSL_LIBRARY} )
        execute_process( COMMAND ${CMAKE_INSTALL_NAME_TOOL} -id "@rpath/libgslcblas.0.dylib" ${GSL_CBLAS_LIBRARY} )
      endif()
    endif()
  endif()
endif()

if ( GSL_LIBRARY AND GSL_CBLAS_LIBRARY )
  message( STATUS "Using GSL from ${GSL_LIBRARY} | ${GSL_CBLAS_LIBRARY}" )

  set( GSL_LIBRARIES "${GSL_LIBRARY};${GSL_CBLAS_LIBRARY}" )
  set( GSL_INCLUDE_DIR "${BUNDLED_STAGEDIR}/include")

  set( SIRE_FOUND_GSL TRUE )
else()
  message( STATUS "Strange? Cannot find the file containing GSL's source (${GSL_ZIPFILE}).
                   We cannot compile it, so will need to rely on the system version..." )
endif()
