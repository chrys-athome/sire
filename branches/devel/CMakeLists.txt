################################
#
#  CMake Project file for Sire
#
#   (C) Christopher Woods
#
################################

# require cmake >= 2.4.1
cmake_minimum_required(VERSION 2.4.1 FATAL_ERROR)

# name the project
project (SIRE)

# project version
set (SIRE_VERSION_MAJOR "0")
set (SIRE_VERSION_MINOR "0")
set (SIRE_VERSION_PATCH "1")

set (SIRE_VERSION "${SIRE_VERSION_MAJOR}.${SIRE_VERSION_MINOR}.${SIRE_VERSION_PATCH}")

# Build shared libraries by default
set (BUILD_SHARED_LIBS ON)

# Name of executable install dir
set (SIRE_BIN "bin")
# Shared library install dir
set (SIRE_LIBS "lib")
# Static library install dir
set (SIRE_ARCHIVES "lib/static")

# Uncomment for more verbose compiling/linking
# set (CMAKE_VERBOSE_MAKEFILE ON)

# Path to the project's extra cmake files
set (CMAKE_MODULE_PATH ${CMAKE_SOURCE_DIR}/build/cmake)

# Add option to disable compilation of Spier (removes openGL dependency)
option ( BUILD_SPIER "Build Spier molecular viewer (requires openGL)" ON )

# include the right Qt modules - what we need depends on 
# whether any GUI apps are compiled
set (QT_USE_QTSQL TRUE)

# Don't need GUI if aren't compiling Spier
if ( BUILD_SPIER )
   set (QT_USE_QTGUI TRUE)
   set (QT_USE_OPENGL TRUE)
else ( BUILD_SPIER )
   set (QT_DONT_USE_QTGUI TRUE)
endif ( BUILD_SPIER )

# Sire depends on >= Qt 4.1
set (QT_MIN_VERSION "4.1.0")
include( FindQt4 )

include_directories( ${QT_INCLUDE_DIR} ${QT_QTCORE_INCLUDE_DIR} )
add_definitions( ${QT_DEFINITIONS} )

# Sire depends on >= Python 2.3
find_program( Python REQUIRED )

# Get the version number of this python interpreter
exec_program ( ${PYTHON_EXECUTABLE}
               ARGS "-c \"import sys; print sys.version[0:1]\""
	       OUTPUT_VARIABLE PYTHON_MAJOR_VERSION
	     )

exec_program ( ${PYTHON_EXECUTABLE}
               ARGS "-c \"import sys; print sys.version[2:3]\""
	       OUTPUT_VARIABLE PYTHON_MINOR_VERSION
	     )

set( PYTHON_REQUIRE_MAJOR 2 )
set( PYTHON_REQUIRE_MINOR 4 )

set( PYTHON_VERSION_FOUND False )

if ( PYTHON_MAJOR_VERSION EQUAL PYTHON_REQUIRE_MAJOR )
   if (PYTHON_MINOR_VERSION EQUAL PYTHON_REQUIRE_MINOR )
      set (PYTHON_VERSION_FOUND True)
   endif (PYTHON_MINOR_VERSION EQUAL PYTHON_REQUIRE_MINOR)

   if (PYTHON_MINOR_VERSION GREATER PYTHON_REQUIRE_MINOR)
      set (PYTHON_VERSION_FOUND True)
   endif (PYTHON_MINOR_VERSION GREATER PYTHON_REQUIRE_MINOR)
endif (PYTHON_MAJOR_VERSION EQUAL PYTHON_REQUIRE_MAJOR)

if ( PYTHON_MAJOR_VERSION GREATER PYTHON_REQUIRE_MAJOR )
   set (PYTHON_VERSION_FOUND True)
endif (PYTHON_MAJOR_VERSION GREATER PYTHON_REQUIRE_MAJOR )

if ( NOT PYTHON_VERSION_FOUND )
   message( FATAL_ERROR  "Require Python version >= "
                         "${PYTHON_REQUIRE_MAJOR}.${PYTHON_REQUIRE_MINOR}, while have found "
                         "Python-Version ${PYTHON_MAJOR_VERSION}.${PYTHON_MINOR_VERSION}" )
endif ( NOT PYTHON_VERSION_FOUND )

message( STATUS "Found Python-Version ${PYTHON_MAJOR_VERSION}.${PYTHON_MINOR_VERSION}" )

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
set (Boost_FIND_REQUIRED TRUE)
include ( ${CMAKE_MODULE_PATH}/FindBoost.cmake )
find_package(Boost REQUIRED)

# Sire depends on >= boost_python 1.31
set ( BOOST_VERSION "1_32" CACHE STRING "Version of Boost Library to use")

find_library( BOOST_PYTHON_LIBRARY
              NAMES
                boost_python-${BOOST_VERSION}
                boost_python
              PATHS
                /usr/lib
                /usr/local/lib
              DOC 
                "Path where the Boost Python Library can be found"
            )

message( STATUS "Boost python library = ${BOOST_PYTHON_LIBRARY}" )

find_library( BOOST_UNITTEST_LIBRARY
              NAMES
	        boost_unit_test_framework-${BOOST_VERSION}
	        boost_unit_test_framework
	      PATHS
	        /usr/lib
		/usr/local/lib
	      DOC
	        "Path where the Boost Unit Test Library can be found"
            )

message( STATUS "Boost unit test library = ${BOOST_UNITTEST_LIBRARY}" )

find_library( UTIL_LIBRARY
              NAMES
	        util
	      PATHS
	        /usr/lib
		/usr/local/lib
	      DOC
	        "Path where the util library can be found (libutil.so)"
	    )

if (UNIX)
   set( PYTHON_LIBRARY ${PYTHON_LIBRARY} ${UTIL_LIBRARY} )
endif (UNIX)

# Sire depends on >= GSL 1.6
include ( ${CMAKE_MODULE_PATH}/FindGSL.cmake )
find_package(GSL REQUIRED)

# Recurse into the src/libs and src/apps subdirectories
add_subdirectory (src/libs)
add_subdirectory (src/apps)

