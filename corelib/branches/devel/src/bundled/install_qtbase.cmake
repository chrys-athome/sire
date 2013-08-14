###
### CMake file to install QtCore from Qt 5
### in the Sire bundle
###

# First, try to find the QtCore library in the "bundled" directory. If it exists,
# then we don't need to do anything
set( Qt5Core_DIR "${BUNDLE_STAGEDIR}/lib/cmake/Qt5Core" )
find_package( Qt5Core QUIET )

if ( Qt5Core_FOUND )
  message( STATUS "Have already compiled a bundled version of Qt5Core")

else()
  message( STATUS "Compiling and installing a bundled version of QtCore from Qt5" )
  message( STATUS "THIS WILL TAKE A VERY LONG TIME" )

  set( QT_ZIPFILE "${CMAKE_SOURCE_DIR}/src/bundled/qtbase.tar.gz" )

  if (EXISTS "${QT_ZIPFILE}")
    set( QT_BUILD_DIR "${BUNDLE_BUILDDIR}/qtbase" )

    if (NOT EXISTS "${QT_BUILD_DIR}")
      message( STATUS "Unzipping ${QT_ZIPFILE} to ${QT_BUILD_DIR}" )
      execute_process(
          COMMAND ${CMAKE_COMMAND} -E tar xzf ${QT_ZIPFILE}
          WORKING_DIRECTORY ${BUNDLE_BUILDDIR}
      )
    endif()

    set( QT_OPTIONS "-no-javascript-jit;-no-glib;-no-framework;-no-opengl;-no-kms;-no-dbus;-no-pch;-no-icu;-no-iconv;-no-cups;-no-nis;-no-widgets;-no-gui;-no-rpath;-no-linuxfb;-no-directfb;-no-eglfs;-no-xcb;-qt-pcre;-no-openssl;-no-gif;-qt-zlib;-no-pkg-config;-c++11;-confirm-license;-opensource;-prefix;${BUNDLE_STAGEDIR}" )
    message( STATUS "${QT_OPTIONS}" )

    message( STATUS "Patience... Configuring QtCore..." )
    execute_process( COMMAND ${QT_BUILD_DIR}/configure ${QT_OPTIONS}
                     WORKING_DIRECTORY ${QT_BUILD_DIR} )

    message( STATUS "Patience... Compiling QtCore..." )
    execute_process( COMMAND ${CMAKE_MAKE_PROGRAM}
                     WORKING_DIRECTORY ${QT_BUILD_DIR} )

    message( STATUS "Patience... Installing QtCore..." )
    execute_process( COMMAND ${CMAKE_MAKE_PROGRAM} install
                   WORKING_DIRECTORY ${QT_BUILD_DIR} )

    set( Qt5Core_DIR "${BUNDLE_STAGEDIR}/lib/cmake/Qt5Core" )
    find_package( Qt5Core )

    if ( Qt5Core_FOUND )
      # need to set the install name so that we can find the library when it is 
      # placed into the bundle directory
      if (APPLE)
        execute_process( COMMAND ${CMAKE_INSTALL_NAME_TOOL} -id "@rpath/libQt5Core.dylib" ${BUNDLE_STAGEDIR}/lib/libQt5Core.dylib )
        execute_process( COMMAND ${CMAKE_INSTALL_NAME_TOOL} -id "@rpath/libQt5Core_debug.dylib" ${BUNDLE_STAGEDIR}/lib/libQt5Core_debug.dylib )
      endif()
    endif()
  endif()
endif()

if ( Qt5Core_FOUND )
  message( STATUS "Using Qt5Core from ${BUNDLE_STAGEDIR}" )
  get_target_property(QtCore_location Qt5::Core LOCATION)
  message( STATUS "Library: ${QtCore_location}" )
  set( SIRE_FOUND_QT TRUE )
else()
  message( STATUS "Strange? Cannot find the file containing the cmake for Qt5Core.
                   We cannot compile it, so will need to rely on the system version..." )
endif()
