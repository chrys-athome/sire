###
### CMake file to install the Python 3 interpreter
### in the Sire bundle
###

# Need to set the version of Python bundled with Sire
set( PYTHON_VERSION "3.3" )
set( PYTHON_ABIFLAGS "m" )

# First, try to find the Python library in the "bundled" directory. If it exists,
# then we don't need to do anything
unset(PYTHON_LIBRARY CACHE)

find_library( PYTHON_LIBRARY 
              NAMES python${PYTHON_VERSION}${PYTHON_ABIFLAGS}
              PATHS ${BUNDLE_STAGEDIR}/lib NO_DEFAULT_PATH )

if ( PYTHON_LIBRARY )
  message( STATUS "Have already compiled a bundled version of Python 3")
else()
  message( STATUS "Compiling and installing a bundled version of Python 3" )

  set( PYTHON_ZIPFILE "${CMAKE_SOURCE_DIR}/bundled/python3.tar.gz" )

  if (EXISTS "${PYTHON_ZIPFILE}")
    set( PYTHON_BUILD_DIR "${BUNDLE_BUILDDIR}/python3" )

    if (NOT EXISTS "${PYTHON_BUILD_DIR}")
      message( STATUS "Unzipping ${PYTHON_ZIPFILE} to ${PYTHON_BUILD_DIR}" )
      execute_process(
          COMMAND ${CMAKE_COMMAND} -E tar xzf ${PYTHON_ZIPFILE}
          WORKING_DIRECTORY ${BUNDLE_BUILDDIR}
      )
    endif()

    set( PYTHON_OPTIONS "--enable-shared;--prefix=${BUNDLE_STAGEDIR};CC=${CMAKE_C_COMPILER};CXX=${CMAKE_CXX_COMPILER}" )
    message( STATUS "${PYTHON_OPTIONS}" )

    message( STATUS "Patience... Configuring Python..." )
    execute_process( COMMAND ${PYTHON_BUILD_DIR}/configure ${PYTHON_OPTIONS}
                     WORKING_DIRECTORY ${PYTHON_BUILD_DIR} )

    message( STATUS "Patience... Compiling Python..." )
    execute_process( COMMAND ${CMAKE_MAKE_PROGRAM}
                     WORKING_DIRECTORY ${PYTHON_BUILD_DIR} )

    message( STATUS "Patience... Installing Python..." )
    execute_process( COMMAND ${CMAKE_MAKE_PROGRAM} install
                   WORKING_DIRECTORY ${PYTHON_BUILD_DIR} )

    if (APPLE)
      set( PYTHON_LIBRARY "${BUNDLE_STAGEDIR}/lib/libpython${PYTHON_VERSION}${PYTHON_ABIFLAGS}.dylib" )
      execute_process( COMMAND ${CMAKE_INSTALL_NAME_TOOL} -id "@rpath/libpython${PYTHON_VERSION}${PYTHON_ABIFLAGS}.dylib" ${PYTHON_LIBRARY} )
    else()
      set( PYTHON_LIBRARY "${BUNDLE_STAGEDIR}/lib/libpython${PYTHON_VERSION}${PYTHON_ABIFLAGS}.so" )
    endif()
  endif()
endif()

if ( PYTHON_LIBRARY )
  set( PYTHON_LIBRARIES "${PYTHON_LIBRARY}" )
  set( PYTHON_INCLUDE_DIR "${BUNDLE_STAGEDIR}/include/python${PYTHON_VERSION}${PYTHON_ABIFLAGS}")
  set( PYTHON_SITE_DIR "${SIRE_BUNDLED_DIR}/lib/python${PYTHON_VERSION}/site-packages" )
  set( PYTHON_MODULE_EXTENSION ".so" )

  if (APPLE)
    execute_process( COMMAND chmod u+w ${PYTHON_LIBRARY} )
    execute_process( COMMAND ${CMAKE_INSTALL_NAME_TOOL} -id "@rpath/libpython${PYTHON_VERSION}${PYTHON_ABIFLAGS}.dylib" ${PYTHON_LIBRARY} )
  endif()

  message( STATUS "Using bundled python in ${PYTHON_LIBRARIES} | ${PYTHON_INCLUDE_DIR}" )
  message( STATUS "Python modules will be installed to ${PYTHON_SITE_DIR}" )

  set( SIRE_FOUND_PYTHON TRUE )
else()
  message( STATUS "Strange? Cannot find the installed Python library. We cannot compile it, so will need to rely on the system version..." )
endif()
