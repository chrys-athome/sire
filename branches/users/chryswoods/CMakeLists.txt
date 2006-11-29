################################
#
#  CMake Project file for Sire
#
#   (C) Christopher Woods
#
################################

# require cmake >= 2.4.1
cmake_minimum_required(VERSION 2.4.1 FATAL_ERROR)

# Absolutely can't run cmake in the source directory!
if ( CMAKE_BINARY_DIR STREQUAL CMAKE_SOURCE_DIR )
  message( FATAL_ERROR "You must run CMake in a different directory to the source!" ) 
endif ( CMAKE_BINARY_DIR STREQUAL CMAKE_SOURCE_DIR )

# name the project
project (SIRE)

# project version
set (SIRE_VERSION_MAJOR "0")
set (SIRE_VERSION_MINOR "0")
set (SIRE_VERSION_PATCH "1")

set (SIRE_VERSION "${SIRE_VERSION_MAJOR}.${SIRE_VERSION_MINOR}.${SIRE_VERSION_PATCH}")

# Ensure that the Sire's directories are searched for header files
# before the system directories - this prevents problems in case
# an older version of Sire is already installed on the system
INCLUDE_DIRECTORIES( BEFORE ${SIRE_SOURCE_DIR} )

# Build shared libraries by default
set (BUILD_SHARED_LIBS ON)

# Name of executable install dir
set (SIRE_BIN "bin")
# Shared library install dir
set (SIRE_LIBS "lib")
# Static library install dir
set (SIRE_ARCHIVES "lib/static")

# Uncomment for more verbose compiling/linking
# (or run 'make VERBOSE=1')
# set (CMAKE_VERBOSE_MAKEFILE ON)

# Path to the project's extra cmake files
set (CMAKE_MODULE_PATH ${CMAKE_SOURCE_DIR}/build/cmake)

# Add option to disable compilation of Spier (removes openGL dependency)
option ( SIRE_BUILD_SPIER "Build Spier molecular viewer (requires openGL)" ON )

# Add option to turn off symbol hiding
option ( SIRE_SYMBOL_HIDING "Turn on library symbol hiding" ON )

# include the right Qt modules - what we need depends on 
# whether any GUI apps are compiled
set (QT_USE_QTSQL TRUE)

# Don't need GUI if aren't compiling Spier
if ( SIRE_BUILD_SPIER )
   set (QT_USE_QTGUI TRUE)
   set (QT_USE_OPENGL TRUE)
else ( SIRE_BUILD_SPIER )
   set (QT_DONT_USE_QTGUI TRUE)
endif ( SIRE_BUILD_SPIER )

# Sire depends on >= Qt 4.1
set (QT_MIN_VERSION "4.1.0")
include( FindQt4 )

# I only link to individual Qt libraries, as this way I can ensure
# that Spier links to QtGUI, while the Sire libraries don't (as there
# is no need for Sire to be linked to any GUI libraries).
# However, this means that I also need to pull in the libraries on which
# the various Qt components depend - I will do this by adding them
# onto the end of the actual Qt libraries
set( QT_QTCORE_LIBRARY ${QT_QTCORE_LIBRARY} ${QT_CORE_LIB_DEPENDENCIES} )
set( QT_QTGUI_LIBRARY ${QT_QTGUI_LIBRARY} ${QT_GUI_LIB_DEPENDENCIES} )

# Sire depends on >= Python 2.2
set( PYTHON_REQUIRE_MAJOR 2 )
set( PYTHON_REQUIRE_MINOR 2 )
include( FindPython )

# Use the python interpreter to get the link options, version and install
# directory - simplify by using the python script in the build directory
set (PYTHON_CONFIG ${CMAKE_SOURCE_DIR}/build/python-config)

# get the link flags for the shared library
exec_program ( ${PYTHON_EXECUTABLE}
               ARGS "${PYTHON_CONFIG} --lflags"
	       OUTPUT_VARIABLE PYTHON_LIBRARY
	     )

# get the include directories
exec_program ( ${PYTHON_EXECUTABLE}
               ARGS "${PYTHON_CONFIG} --cflags"
	       OUTPUT_VARIABLE PYTHON_INCLUDE_DIR
	     )

# get the path to the python site files (excluding the prefix)
set( PYTHON_SITE_DIR "lib/python${PYTHON_MAJOR_VERSION}.${PYTHON_MINOR_VERSION}/site-packages" )

message( STATUS "Python paths ${PYTHON_LIBRARY} | ${PYTHON_INCLUDE_DIR} | ${PYTHON_SITE_DIR}" )

# set the location to install the python modules
set (SIRE_PYTHON "${PYTHON_SITE_DIR}")

# Sire depends on >= boost 1.31
set (BOOST_FIND_REQUIRED TRUE)
include ( FindBoost )

message( STATUS "Boost paths ${BOOST_LIBRARY_DIRS} | ${BOOST_INCLUDE_DIRS}" )

# Sire depends on >= boost_python 1.31
find_library( BOOST_PYTHON_LIBRARY
              NAMES
                boost_python
              PATHS
                ${BOOST_LIBRARY_DIRS}
                /usr/lib
                /usr/local/lib
                ENV LD_LIBRARY_PATH
              DOC
                "Path where the Boost Python Library can be found"
            )

# Need to ensure that we are linking to boost_python shared library!
if (UNIX)
  if ( BOOST_PYTHON_LIBRARY MATCHES "(\\.so$)" OR BOOST_PYTHON_LIBRARY MATCHES "(\\.SO$)" )
  else ( BOOST_PYTHON_LIBRARY MATCHES "(\\.so$)" OR BOOST_PYTHON_LIBRARY MATCHES "(\\.SO$)" )
    message( FATAL_ERROR "Need to link to shared boost::python library (should be "
                         "boost_python.so, "
                         "not ${BOOST_PYTHON_LIBRARY})" )
  endif ( BOOST_PYTHON_LIBRARY MATCHES "(\\.so$)" OR BOOST_PYTHON_LIBRARY MATCHES "(\\.SO$)" )
endif (UNIX)

message( STATUS "Boost python library = ${BOOST_PYTHON_LIBRARY}" )

find_library( BOOST_UNITTEST_LIBRARY
              NAMES
                boost_unit_test_framework
              PATHS
                ${BOOST_LIBRARY_DIRS}
                /usr/lib
                /usr/local/lib
                ENV LD_LIBRARY_PATH
              DOC
                "Path where the Boost UnitTest Library can be found"
            )

message( STATUS "Boost unit test library = ${BOOST_UNITTEST_LIBRARY}" )

# Sire depends on >= GSL 1.6
include ( FindGSL )
find_package(GSL REQUIRED)

message( STATUS "GSL paths ${GSL_LINK_DIRECTORIES} ${GSL_LIBRARIES} | ${GSL_INCLUDE_DIR}" )

# Add defaults for all parts of the program
include_directories( ${QT_INCLUDE_DIR} 
                     ${QT_QTCORE_INCLUDE_DIR} 
                     ${BOOST_INCLUDE_DIRS}
                   )

add_definitions( ${QT_DEFINITIONS} )

# Set compile flags for debug and release
set( SIRE_C_FLAGS_RELEASE "-Wall -O2" )
set( SIRE_CXX_FLAGS_RELEASE ${SIRE_C_FLAGS_RELEASE} )

set( SIRE_C_FLAGS_DEBUG "-Wall -g -O2" )
set( SIRE_CXX_FLAGS_DEBUG ${SIRE_C_FLAGS_DEBUG} )

# Also set flags for size-optimised compile - this is used for
# all of the python wrappers, which can grow to enormous sizes 
# otherwise! (and aren't performance critical)
set( SIRE_C_FLAGS_SMALL "-Wall -Os" )
set( SIRE_CXX_FLAGS_SMALL ${SIRE_C_FLAGS_SMALL} )

# Add compiler specific flags
if ( CMAKE_COMPILER_IS_GNUCC )

    # get the compiler version (via gcc -dumpversion)
    # (I'll assume that we are using the same version
    # of compiler for C and C++...)
    EXEC_PROGRAM( ${CMAKE_CXX_COMPILER}
                  ARGS -dumpversion
                  OUTPUT_VARIABLE exe_output )

    # These two regexps are copied from the Chicken CMake
    # files (which provide a very useful cmake tutorial - thanks :-)                  
    SET(VERSION_BUILD_REGEX "^([0-9]+)[.]([0-9]+).*$")
    STRING(REGEX REPLACE ${VERSION_BUILD_REGEX} "\\1"
           GCC_MAJOR_VERSION ${exe_output})
    STRING(REGEX REPLACE ${VERSION_BUILD_REGEX} "\\2"
           GCC_MINOR_VERSION ${exe_output})
        
    message( STATUS "Using GCC version "
                    "${GCC_MAJOR_VERSION}.${GCC_MINOR_VERSION}" )

    # First, we need at least GCC 3.0, as I don't think that
    # GCC 2.x can handle the template code that I use
    if ( GCC_MAJOR_VERSION LESS 3 )
       message( FATAL_ERROR "Sire requires GCC >= 3.0. Please upgrade "
                      "your version of GCC." )
    endif ( GCC_MAJOR_VERSION LESS 3 )

    # Add options common to all gcc version
    ADD_DEFINITIONS( -pipe )
        
    # Now gcc 4 specific options
    if ( GCC_MAJOR_VERSION GREATER 3 )
      # remove warnings about strict aliasing (lots from Qt4
      # when compiling with GCC 4, and they are annoying!)
      ADD_DEFINITIONS( -Wno-strict-aliasing )

      # add symbol hiding if the compiler supports it and it is
      # requested
      if ( SIRE_SYMBOL_HIDING )

        ADD_DEFINITIONS( -DSIRE_VISIBILITY_AVAILABLE )
        ADD_DEFINITIONS( -fvisibility=hidden -fvisibility-inlines-hidden )

      else ( SIRE_SYMBOL_HIDING )
  
        ADD_DEFINITIONS( -DSIRE_NO_VISIBILITY_AVAILABLE )

      endif ( SIRE_SYMBOL_HIDING )
    
    else ( GCC_MAJOR_VERSION GREATER 3 )
    
      ADD_DEFINITIONS( -DSIRE_NO_VISIBILITY_AVAILABLE )

    endif ( GCC_MAJOR_VERSION GREATER 3 )
    
endif ( CMAKE_COMPILER_IS_GNUCC )

# Copy the Sire compile flags to CMake
set( CMAKE_C_FLAGS ${SIRE_C_FLAGS_RELEASE} )
set( CMAKE_CXX_FLAGS ${SIRE_CXX_FLAGS_RELEASE} )

# Ensure that the libraries don't contain any undefined symbols
set( SIRE_LINK_FLAGS "-Wl,--no-undefined" )

# Add this to all shared libraries
set( CMAKE_SHARED_LINKER_FLAGS "${CMAKE_SHARED_LINKER_FLAGS} ${SIRE_LINK_FLAGS}" )

# Recurse into the src/libs and src/apps subdirectories
add_subdirectory (src/libs)
add_subdirectory (src/apps)

