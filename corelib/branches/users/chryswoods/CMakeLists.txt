################################
#
#  CMake Project file for Sire
#
#   (C) Christopher Woods
#
################################

# require cmake >= 2.6.0
cmake_minimum_required(VERSION 2.6.0 FATAL_ERROR)

# use loose loop syntax in CMakeLists files
set( CMAKE_ALLOW_LOOSE_LOOP_CONSTRUCTS TRUE )

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

math (EXPR SIRE_VERSION_NUMBER 
      "${SIRE_VERSION_MAJOR}*100000 + ${SIRE_VERSION_MINOR}*100 + ${SIRE_VERSION_PATCH}")

set (SIRE_VERSION_STRING "${SIRE_VERSION_MAJOR}_${SIRE_VERSION_MINOR}_${SIRE_VERSION_PATCH}")

# Whether or not to use manual SSE (will probably be removed
# in favour of compile-time detection)
set (SIRE_USE_SSE ON)

# Ensure that the Sire's directories are searched for header files
# before the system directories - this prevents problems in case
# an older version of Sire is already installed on the system
INCLUDE_DIRECTORIES( BEFORE ${SIRE_SOURCE_DIR} )

# Turn off FLOP counting (timing) by default - this adds a small runtime penalty
option (SIRE_TIME_FLOPS "Turn on floating point timing" OFF)

# Build shared libraries by default
set (BUILD_SHARED_LIBS ON)

# This is where you set your own compiler flags
set (SIRE_USER_CFLAGS "-mtune=native -O3 -fomit-frame-pointer")
set (SIRE_USER_CPPFLAGS "-mtune=native -O3 -fomit-frame-pointer")

# Name of executable install dir
set (SIRE_BIN "bin")
# Shared library install dir
set (SIRE_LIBS "lib")
# Static library install dir
set (SIRE_ARCHIVES "lib/static")
# Include (header) files install dir
set (SIRE_INCLUDES "include/Sire")
# CMake install files install dir
set (SIRE_CMAKEFILES "include/Sire/cmake")

# Uncomment for more verbose compiling/linking
# (or run 'make VERBOSE=1')
# set (CMAKE_VERBOSE_MAKEFILE ON)

# Path to the project's extra cmake files
set (CMAKE_MODULE_PATH ${CMAKE_SOURCE_DIR}/build/cmake)

# Add option to turn off symbol hiding
option ( SIRE_SYMBOL_HIDING "Turn on library symbol hiding" ON )

#find_package(SIRE COMPONENTS SireBase SireCAS Squire REQUIRED)

#now create a file that will hold a list of all of the 
#extra include directories used by the Sire modules
set (SIRE_EXTRA_INCLUDES_FILE "${CMAKE_BINARY_DIR}/SireExtraIncludes.cmake")
file ( REMOVE ${SIRE_EXTRA_INCLUDES_FILE} )

# This macro is used to record extra include files in SireExtraIncludes.cmake
macro (SAVE_SIRE_EXTRA_INCLUDE _name)
  file(APPEND ${SIRE_EXTRA_INCLUDES_FILE}
        "list (APPEND SIRE_EXTRA_INCLUDES \"${_name}\")\n")
endmacro (SAVE_SIRE_EXTRA_INCLUDE)

# Sire depends on >= Qt 4.2
set( QT_DONT_USE_QTGUI "TRUE" )
set( QT_USE_QTSQL "TRUE" )
find_package( Qt4 4.2.0 REQUIRED QtCore QtSQL )

if (QT4_FOUND)
   include (${QT_USE_FILE})
   message( STATUS "Using Qt version ${QT_VERSION_MAJOR}.${QT_VERSION_MINOR}.${QT_VERSION_PATCH} "
                   "(${QT_LIBRARIES})")
else()
   message( FATAL_ERROR "Qt >= 4.2 is requires by Sire" )
endif()

# Need also to find the boost libraries
FIND_PACKAGE ( Boost  )

if (Boost_FOUND)
  message(STATUS "Boost paths ${Boost_LIBRARY_DIRS} | ${Boost_INCLUDE_DIR}" )
  set ( BOOST_INCLUDE_DIRS "${Boost_INCLUDE_DIR}" )
  include_directories( ${Boost_INCLUDE_DIR} )

  #add this include path to the list of extra include
  #paths required for Sire
  save_sire_extra_include(${Boost_INCLUDE_DIR})
else()
  message(FATAL_ERROR "Cannot find the boost libraries.")
endif()  

# Set compile flags for debug and release

if (SIRE_USE_SSE)
  set (SIRE_PLATFORM_OPTIONS "${SIRE_PLATFORM_OPTIONS} -msse2 -mfpmath=sse -ffast-math")
  ADD_DEFINITIONS( -DSIRE_USE_SSE )
endif()

if (SIRE_TIME_FLOPS)
  ADD_DEFINITIONS( -DSIRE_TIME_ROUTINES )
endif()

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
    set( SIRE_PLATFORM_OPTIONS "${SIRE_PLATFORM_OPTIONS} -pipe" )
        
    # Now gcc 4 specific options
    if ( GCC_MAJOR_VERSION GREATER 3 )
      # remove warnings about strict aliasing (lots from Qt4
      # when compiling with GCC 4, and they are annoying!)
      set( SIRE_PLATFORM_OPTIONS "${SIRE_PLATFORM_OPTIONS} -Wno-strict-aliasing" )

      # gcc-4.2 has -march=native, that autodetects best compiler
      # for the current machine
      #if ( GCC_MINOR_VERSION GREATER 1 )
      #   message( "Adding -march=native to compiler options. This is a bad idea "
      #            "if this is not the machine that you will be running on..." )
      #
      #   ADD_DEFINITIONS( -march=native )
      #endif()

      # gcc-4.2 and above has openMP - turn it on (except for Mac as libgomp is broken!)
      if ( GCC_MINOR_VERSION GREATER 1 AND NOT APPLE )
          set( SIRE_PLATFORM_OPTIONS "${SIRE_PLATFORM_OPTIONS} -fopenmp" )
          set( SIRE_LINK_FLAGS "${SIRE_LINK_FLAGS} -fopenmp" )
      endif()

      # add symbol hiding if the compiler supports it and it is
      # requested
      if ( SIRE_SYMBOL_HIDING )

        ADD_DEFINITIONS( -DSIRE_VISIBILITY_AVAILABLE )
        set( SIRE_PLATFORM_OPTIONS "${SIRE_PLATFORM_OPTIONS} -fvisibility=hidden -fvisibility-inlines-hidden" )

      else()
  
        ADD_DEFINITIONS( -DSIRE_NO_VISIBILITY_AVAILABLE )

      endif()
    
    else()
    
      ADD_DEFINITIONS( -DSIRE_NO_VISIBILITY_AVAILABLE )

    endif()
    
endif()

set( SIRE_C_FLAGS_RELEASE "-Wall ${SIRE_USER_CFLAGS} ${SIRE_PLATFORM_OPTIONS}" )
set( SIRE_CXX_FLAGS_RELEASE "-Wall ${SIRE_USER_CPPFLAGS} ${SIRE_PLATFORM_OPTIONS}" )

set( SIRE_C_FLAGS_DEBUG "-Wall -g ${SIRE_USER_CFLAGS} -O0 ${SIRE_PLAFORM_OPTIONS}" )
set( SIRE_CXX_FLAGS_DEBUG "-Wall -g ${SIRE_USER_CPPFLAGS} -O0 ${SIRE_PLATFORM_OPTIONS}" )

# Also set flags for size-optimised compile - this is used for
# all of the python wrappers, which can grow to enormous sizes
# otherwise! (and aren't performance critical)
set( SIRE_C_FLAGS_SMALL "-Wall -Os ${SIRE_PLATFORM_OPTIONS}" )
set( SIRE_CXX_FLAGS_SMALL ${SIRE_C_FLAGS_SMALL} )

# Copy the Sire compile flags to CMake
set( CMAKE_C_FLAGS ${SIRE_C_FLAGS_RELEASE} )
set( CMAKE_CXX_FLAGS ${SIRE_CXX_FLAGS_RELEASE} )

message( STATUS "Compiler flags = ${SIRE_CXX_FLAGS_RELEASE}" )

# Ensure that the libraries don't contain any undefined symbols
if(APPLE)
  set( SIRE_LINK_FLAGS "${SIRE_LINK_FLAGS} -rdynamic -Wl" )
else(UNIX)
  set( SIRE_LINK_FLAGS "${SIRE_LINK_FLAGS} -rdynamic -Wl,--no-undefined" )
endif()

# Add this to all shared libraries
set( CMAKE_SHARED_LINKER_FLAGS "${CMAKE_SHARED_LINKER_FLAGS} ${SIRE_LINK_FLAGS}" )
set( CMAKE_EXE_LINKER_FLAGS    "${CMAKE_EXE_LINKER_FLAGS} ${SIRE_LINK_FLAGS}" )

message( STATUS "Linker flags = ${CMAKE_SHARED_LINKER_FLAGS}" )

# Now run some platform tests - the results will be placed in config.h
include (CheckIncludeFiles)
include (CheckSymbolExists)
include (CheckFunctionExists)

check_include_files( unistd.h HAVE_UNISTD_H )
check_function_exists( lseek64 HAVE_LSEEK64 )

#create config.h
configure_file(${CMAKE_CURRENT_SOURCE_DIR}/build/cmake/sire_config.h.in 
               ${CMAKE_CURRENT_BINARY_DIR}/sire_config.h)

#include the build directory so that we can get sire_config.h and sire_version.h
include_directories(${CMAKE_BINARY_DIR})

# Add the compiler test directory - this allows us to run some compiler
# tests to make sure that Sire will compile properly
add_subdirectory (build/test_compiler)

# Recurse into the src/libs and src/apps subdirectories
add_subdirectory (src/libs)
add_subdirectory (src/apps)

# Finally, write the version header file and finish off the component dependencies file
include( GetSvnVersion )

# install the extra includes file in the Sire/cmake directory
install (FILES ${SIRE_EXTRA_INCLUDES_FILE} 
         DESTINATION ${SIRE_CMAKEFILES}
        )

install (FILES ${CMAKE_CURRENT_BINARY_DIR}/sire_config.h
         DESTINATION ${SIRE_INCLUDES})
