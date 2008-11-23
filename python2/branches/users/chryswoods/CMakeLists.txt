################################
#
#  CMake Project file for Sire
#
#   (C) Christopher Woods
#
################################

# require cmake >= 2.6.0
cmake_minimum_required(VERSION 2.6.0 FATAL_ERROR)

#use loose loop syntax in CMakeLists files
set( CMAKE_ALLOW_LOOSE_LOOP_CONSTRUCTS TRUE )

# Absolutely can't run cmake in the source directory!
if ( CMAKE_BINARY_DIR STREQUAL CMAKE_SOURCE_DIR )
  message( FATAL_ERROR "You must run CMake in a different directory to the source!" ) 
endif ( CMAKE_BINARY_DIR STREQUAL CMAKE_SOURCE_DIR )

# name the project
project (SirePython2)

# Path to the project's extra cmake files
set (CMAKE_MODULE_PATH ${CMAKE_SOURCE_DIR}/build/cmake)

# Find the Sire libraries
find_package (Sire 0.0.1 REQUIRED)

if (NOT SIRE_FOUND)
   message (FATAL_ERROR "The required version of Sire was not found.")
endif()

# Now add the paths to the header files of the dependencies of Sire
include_directories (BEFORE SYSTEM ${SIRE_EXTRA_INCLUDES})
include_directories (BEFORE ${SIRE_INCLUDE_DIR})

# Sire depends on >= Qt 4.2
set( QT_DONT_USE_QTGUI "TRUE" )
find_package( Qt4 4.2.0 REQUIRED QtCore QtSQL )

if (QT4_FOUND)
   include (${QT_USE_FILE})
   message( STATUS "Using Qt version ${QT_VERSION_MAJOR}.${QT_VERSION_MINOR}.${QT_VERSION_PATCH} "
                   "(${QT_LIBRARIES})")
else()
   message( FATAL_ERROR "Qt >= 4.2 is requires by Sire" )
endif()

# Ensure that the Sire's directories are searched for header files
# before the system directories - this prevents problems in case
# an older version of Sire is already installed on the system
INCLUDE_DIRECTORIES( BEFORE ${SIREPYTHON2_SOURCE_DIR} )

# Build shared libraries by default
set (BUILD_SHARED_LIBS ON)

# Name of executable install dir
set (SIRE_BIN "bin")
# Shared library install dir
set (SIRE_LIBS "lib")
# Static library install dir
set (SIRE_ARCHIVES "lib/static")
# Include (header) files install dir
set (SIRE_INCLUDES "include/Sire")

# Uncomment for more verbose compiling/linking
# (or run 'make VERBOSE=1')
# set (CMAKE_VERBOSE_MAKEFILE ON)

# Add option to turn off symbol hiding
option ( SIRE_SYMBOL_HIDING "Turn on library symbol hiding" ON )

# Sire depends on >= Python 2.2
set (PYTHON_MIN_VERSION "2.2")
include( FindPython )

if (NOT PYTHON_LIBS_FOUND)
    
    if (PYTHON_INSTALLED_VERSION_TOO_OLD)
        message( FATAL_ERROR "Installed version of Python (${PYTHON_VERSION}) is too old."
                             "Sire requires at least Python ${PYTHON_MIN_VERSION}" )
    else()
        message( FATAL_ERROR "The python libraries were not found. These are necessary to build "
                             "the python bindings." )
    endif()
endif()

message( STATUS "Python paths ${PYTHON_LIBRARIES} | ${PYTHON_INCLUDE_DIR} | ${PYTHON_SITE_DIR}" )

include_directories( ${PYTHON_INCLUDE_DIR} )

# set the location to install the python modules
set (SIRE_PYTHON "${PYTHON_SITE_DIR}")

# Sire depends on >= boost 1.31 - all libraries must be dynamically linked
set (BOOST_ALL_DYN_LINK "YES")
FIND_PACKAGE ( Boost 1.31 COMPONENTS python )

if (Boost_FOUND)
  message(STATUS "Boost paths ${Boost_LIBRARY_DIRS} | ${Boost_INCLUDE_DIR}" )

  set ( BOOST_INCLUDE_DIRS "${Boost_INCLUDE_DIR}" )
  
  include_directories( ${Boost_INCLUDE_DIR} )

  if (Boost_PYTHON_FOUND)
    message(STATUS "Using boost::python library ${Boost_PYTHON_LIBRARY}")
    set ( BOOST_PYTHON_LIBRARY "${Boost_PYTHON_LIBRARY}" )
  else()
    message(FATAL_ERROR "Cannot find the boost::python library, even though "
                        "boost has been found.")
  endif()
else()
  message(FATAL_ERROR "Cannot find the boost libraries.")
endif()  

# Need to ensure that we are linking to boost_python shared library!
if (UNIX)
  if ( BOOST_PYTHON_LIBRARY MATCHES "(\\.a$)" OR
       BOOST_PYTHON_LIBRARY MATCHES "(\\.A$)" )

    message( FATAL_ERROR "Need to link to shared boost::python library (should be "
                         "boost_python.so or boost_python.dylib, "
                         "not ${BOOST_PYTHON_LIBRARY})" )

  endif()
endif()

# Set compile flags for debug and release

set( SIRE_PLATFORM_OPTIONS "" )

set( SIRE_C_FLAGS_RELEASE "-Wall -Os -fomit-frame-pointer ${SIRE_PLATFORM_OPTIONS}" )
set( SIRE_CXX_FLAGS_RELEASE ${SIRE_C_FLAGS_RELEASE} )

set( SIRE_C_FLAGS_DEBUG "-Wall -Os -g ${SIRE_PLATFORM_OPTIONS}" )
set( SIRE_CXX_FLAGS_DEBUG ${SIRE_C_FLAGS_DEBUG} )

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
    endif()

    # Add options common to all gcc version
    ADD_DEFINITIONS( -pipe )
        
    # Now gcc 4 specific options
    if ( GCC_MAJOR_VERSION GREATER 3 )
      # remove warnings about strict aliasing (lots from Qt4
      # when compiling with GCC 4, and they are annoying!)
      ADD_DEFINITIONS( -Wno-strict-aliasing )

      # gcc-4.2 has -march=native, that autodetects best compiler
      # for the current machine
      #if ( GCC_MINOR_VERSION GREATER 1 )
      #   message( "Adding -march=native to compiler options. This is a bad idea "
      #            "if this is not the machine that you will be running on..." )
      #
      #   ADD_DEFINITIONS( -march=native )
      #endif()

      # gcc-4.2 and above has openMP - turn it on
      if ( GCC_MINOR_VERSION GREATER 1 )
          message( STATUS "Turning on openMP parallelisation!")
          ADD_DEFINITIONS( -fopenmp )
          set( SIRE_LINK_FLAGS "${SIRE_LINK_FLAGS} -fopenmp" )
      endif()

      # add symbol hiding if the compiler supports it and it is
      # requested
      if ( SIRE_SYMBOL_HIDING )

        ADD_DEFINITIONS( -DSIRE_VISIBILITY_AVAILABLE )
        ADD_DEFINITIONS( -fvisibility=hidden -fvisibility-inlines-hidden )

      else()
  
        ADD_DEFINITIONS( -DSIRE_NO_VISIBILITY_AVAILABLE )

      endif()
    
    else()
    
      ADD_DEFINITIONS( -DSIRE_NO_VISIBILITY_AVAILABLE )

    endif()
    
endif()

# Copy the Sire compile flags to CMake
set( CMAKE_C_FLAGS ${SIRE_C_FLAGS_RELEASE} )
set( CMAKE_CXX_FLAGS ${SIRE_CXX_FLAGS_RELEASE} )

message( STATUS "Compiler flags = ${SIRE_CXX_FLAGS_RELEASE}" )

# Ensure that the libraries don't contain any undefined symbols
if(APPLE)
  set( SIRE_LINK_FLAGS "${SIRE_LINK_FLAGS} -Wl" )
else(UNIX)
  set( SIRE_LINK_FLAGS "${SIRE_LINK_FLAGS} -Wl,--no-undefined" )
endif()

# Add this to all shared libraries
set( CMAKE_SHARED_LINKER_FLAGS "${CMAKE_SHARED_LINKER_FLAGS} ${SIRE_LINK_FLAGS}" )
set( CMAKE_EXE_LINKER_FLAGS    "${CMAKE_EXE_LINKER_FLAGS} ${SIRE_LINK_FLAGS}" )

message( STATUS "Linker flags = ${CMAKE_SHARED_LINKER_FLAGS}" )

# Add the helper library
add_subdirectory (Helpers)

# Recurse into the module subdirectories 
add_subdirectory (Qt)
add_subdirectory (Error)
add_subdirectory (Base)
add_subdirectory (CAS)
add_subdirectory (Cluster)
add_subdirectory (DB)
add_subdirectory (FF)
add_subdirectory (ID)
add_subdirectory (IO)
add_subdirectory (Maths)
add_subdirectory (MM)
add_subdirectory (Mol)
add_subdirectory (Move)
add_subdirectory (Stream)
add_subdirectory (Squire)
add_subdirectory (System)
add_subdirectory (Units)
add_subdirectory (Vol)

#also compile the MPI-aware python executable
add_subdirectory (sire_python)

install( FILES __init__.py
         DESTINATION ${SIRE_PYTHON}/Sire 
       )

