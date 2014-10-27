###
### CMake file to install openssl
### in the Sire bundle
###

# First, try to find the ssl library in the "bundled" directory. If it exists,
# then we don't need to do anything
unset(SSL_LIBRARY CACHE)
find_library( SSL_LIBRARY "ssl" PATHS ${BUNDLE_STAGEDIR}/lib NO_DEFAULT_PATH )
find_library( CRYPTO_LIBRARY "crypto" PATHS ${BUNDLE_STAGEDIR}/lib NO_DEFAULT_PATH )

if ( SSL_LIBRARY AND CRYPTO_LIBRARY )
  set( OPENSSL_INCLUDE_DIR "${BUNDLE_STAGEDIR}/include" )
  message( STATUS "Have already compiled a bundled version of ssl")
else()
  message( STATUS "Compiling and installing a bundled version of ssl" )

  set( SSL_ZIPFILE "${CMAKE_SOURCE_DIR}/src/bundled/openssl.tar.gz" )

  if (EXISTS "${SSL_ZIPFILE}")
    set( SSL_BUILD_DIR "${BUNDLE_BUILDDIR}/openssl" )

    if (NOT EXISTS "${SSL_BUILD_DIR}")
      message( STATUS "Unzipping ${SSL_ZIPFILE} to ${SSL_BUILD_DIR}" )
      execute_process(
          COMMAND ${CMAKE_COMMAND} -E tar xzf ${SSL_ZIPFILE}
          WORKING_DIRECTORY ${BUNDLE_BUILDDIR}
      )
    endif()

    set( SSL_OPTIONS "--prefix=${BUNDLE_STAGEDIR}" )
    list( APPEND SSL_OPTIONS "shared" )
    list( APPEND SSL_OPTIONS "zlib-dynamic" )
    set(ENV{CC} ${CMAKE_C_COMPILER})
    set(ENV{CXX} ${CMAKE_CXX_COMPILER})

    # The below option is used to tell openssl which platform to
    # compile for - this will eventually need to more detailed to
    # allow cmake to pass in the correct option (currently only
    # supporting 64bit OS X or Linux
    if (APPLE)
      list( APPEND SSL_OPTIONS "darwin64-x86_64-cc" )      
    else()
      list( APPEND SSL_OPTIONS "linux-x86_64" )
    endif()

    message( STATUS "${SSL_OPTIONS}" )

    message( STATUS "Patience... Configuring openssl..." )
    execute_process( COMMAND ${SSL_BUILD_DIR}/Configure ${SSL_OPTIONS}
                     WORKING_DIRECTORY ${SSL_BUILD_DIR} )

    message( STATUS "Patience... Compiling openssl..." )
    # OpenSSL make file is not safe to compile in parallel
    execute_process( COMMAND ${CMAKE_MAKE_PROGRAM}
                     WORKING_DIRECTORY ${SSL_BUILD_DIR} )

    # Now, if this is on OS X, we have to set the name of the 
    # openssl library before installing. This is because installing
    # also compiles all of the engines, which need to know the new
    # @RPATH supported name
    if (APPLE)
      set( SSL_LIBRARY "${BUNDLE_STAGEDIR}/lib/libssl.dylib" )
      execute_process( COMMAND chmod u+w ${SSL_LIBRARY} )
      execute_process( COMMAND ${CMAKE_INSTALL_NAME_TOOL} -id "@rpath/libssl.1.0.0.dylib" ${SSL_LIBRARY} )
      execute_process( COMMAND chmod u-w ${SSL_LIBRARY} )
      set( CRYPTO_LIBRARY "${BUNDLE_STAGEDIR}/lib/libcrypto.dylib" )
      execute_process( COMMAND chmod u+w ${CRYPTO_LIBRARY} )
      execute_process( COMMAND ${CMAKE_INSTALL_NAME_TOOL} -id "@rpath/libcrypto.1.0.0.dylib" ${CRYPTO_LIBRARY} )
      execute_process( COMMAND chmod u-w ${CRYPTO_LIBRARY} )
    endif()

    message( STATUS "Patience... Installing openssl..." )
    execute_process( COMMAND ${CMAKE_MAKE_PROGRAM} install
                     WORKING_DIRECTORY ${SSL_BUILD_DIR} )

    set( OPENSSL_INCLUDE_DIR "${BUNDLE_STAGEDIR}/include" )

    if (APPLE)
      set( SSL_LIBRARY "${BUNDLE_STAGEDIR}/lib/libssl.dylib" )
      execute_process( COMMAND chmod u+w ${SSL_LIBRARY} )
      execute_process( COMMAND ${CMAKE_INSTALL_NAME_TOOL} -id "@rpath/libssl.1.0.0.dylib" ${SSL_LIBRARY} )
      execute_process( COMMAND chmod u-w ${SSL_LIBRARY} )
      set( CRYPTO_LIBRARY "${BUNDLE_STAGEDIR}/lib/libcrypto.dylib" )
      execute_process( COMMAND chmod u+w ${CRYPTO_LIBRARY} )
      execute_process( COMMAND ${CMAKE_INSTALL_NAME_TOOL} -id "@rpath/libcrypto.1.0.0.dylib" ${CRYPTO_LIBRARY} )
      execute_process( COMMAND chmod u-w ${CRYPTO_LIBRARY} )
    else()
      set( SSL_LIBRARY "${BUNDLE_STAGEDIR}/lib/libssl.so" )
      set( CRYPTO_LIBRARY "${BUNDLE_STAGEDIR}/lib/libcryto.so" )
    endif()

  endif()
endif()

find_package( OpenSSL REQUIRED )

if ( SSL_LIBRARY )
  message( STATUS "Using openssl from ${SSL_LIBRARY}|${CRYPTO_LIBRARY}" )
  set( SIRE_FOUND_SSL TRUE )
else()
  message( STATUS "Strange? Cannot find the installed openssl library. We cannot compile it, so will need to rely on the system version..." )
endif()

