################################
#
#  CMake Project file for Sire
#
#   (C) Christopher Woods
#
################################

# require cmake >= 2.4.1
cmake_minimum_required(VERSION 2.4.1 FATAL_ERROR)

# use loose loop syntax in CMakeLists files
set( CMAKE_ALLOW_LOOSE_LOOP_CONSTRUCTS TRUE )

# Absolutely can't run cmake in the source directory!
if ( CMAKE_BINARY_DIR STREQUAL CMAKE_SOURCE_DIR )
  message( FATAL_ERROR "You must run CMake in a different directory to the source!" ) 
endif ( CMAKE_BINARY_DIR STREQUAL CMAKE_SOURCE_DIR )

# name the project
project (SirePython2)

# need FindSire
set (SIRE_INCLUDE_DIR "/Users/chris/local/include/Sire" )
set (SIRE_LIB_DIR "/Users/chris/local/lib" )
set (SIREERROR_LIBRARY "-L${SIRE_LIB_DIR} -lSireError" )
set (SIREMATHS_LIBRARY "-L${SIRE_LIB_DIR} -lSireMaths" )
set (SIREBASE_LIBRARY "-L${SIRE_LIB_DIR} -lSireBase" )
set (SIRECAS_LIBRARY "-L${SIRE_LIB_DIR} -lSireCAS" )
set (SIREID_LIBRARY "-L${SIRE_LIB_DIR} -lSireID" )
set (SIREMOL_LIBRARY "-L${SIRE_LIB_DIR} -lSireMol" )
set (SIRESTREAM_LIBRARY "-L${SIRE_LIB_DIR} -lSireStream" )
set (SIREVOL_LIBRARY "-L${SIRE_LIB_DIR} -lSireVol" )

# project version
set (SIRE_VERSION_MAJOR "0")
set (SIRE_VERSION_MINOR "0")
set (SIRE_VERSION_PATCH "1")

set (SIRE_VERSION "${SIRE_VERSION_MAJOR}.${SIRE_VERSION_MINOR}.${SIRE_VERSION_PATCH}")

# Ensure that the Sire's directories are searched for header files
# before the system directories - this prevents problems in case
# an older version of Sire is already installed on the system
INCLUDE_DIRECTORIES( BEFORE ${SIREPYTHON2_SOURCE_DIR} )

# Build shared libraries by default
set (BUILD_SHARED_LIBS ON)

# Target an i686 by default
set (SIRE_TARGET_OPTIONS "-march=i686" CACHE STRING
            "Compiler option used to specify the target processor.")

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

# Path to the project's extra cmake files
set (CMAKE_MODULE_PATH ${CMAKE_SOURCE_DIR}/build/cmake)

# Add option to turn off symbol hiding
option ( SIRE_SYMBOL_HIDING "Turn on library symbol hiding" ON )

# include the right Qt modules - what we need depends on 
# whether any GUI apps are compiled
set (QT_USE_QTSQL TRUE)

# Don't need GUI if aren't compiling Spier
if ( SIRE_BUILD_SPIER )
   set (QT_USE_QTGUI TRUE)
   set (QT_USE_OPENGL TRUE)
else()
   set (QT_DONT_USE_QTGUI TRUE)
endif()

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

if( NOT APPLE )
   # Use the python interpreter to get the link options, version and install
   # directory - simplify by using the python script in the build directory
   set (PYTHON_CONFIG ${CMAKE_SOURCE_DIR}/build/python-config)

   # get the link flags for the shared library
   exec_program ( ${PYTHON_EXECUTABLE}
                  ARGS "${PYTHON_CONFIG} --ldflags"
                  OUTPUT_VARIABLE PYTHON_LIBRARY
                )

   # get the include directories
   exec_program ( ${PYTHON_EXECUTABLE}
                  ARGS "${PYTHON_CONFIG} --includes"
                  OUTPUT_VARIABLE PYTHON_INCLUDE_DIR
                )

   # get the path to the python site files (excluding the prefix)
   set( PYTHON_SITE_DIR "lib/python${PYTHON_MAJOR_VERSION}.${PYTHON_MINOR_VERSION}/site-packages" )

else()

   include(FindPythonLibs)
   find_package(Python REQUIRED)

   # get the path to the python site files (excluding the prefix)
   set( PYTHON_SITE_DIR "lib/python${PYTHON_MAJOR_VERSION}.${PYTHON_MINOR_VERSION}/site-packages" )
   set( PYTHON_INCLUDE_DIR ${PYTHON_INCLUDE_PATH} )
   set (SIRE_PYTHON "${PYTHON_SITE_DIR}")

endif()

message( STATUS "Python paths ${PYTHON_LIBRARY} | ${PYTHON_INCLUDE_DIR} | ${PYTHON_SITE_DIR}" )

# set the location to install the python modules
set (SIRE_PYTHON "${PYTHON_SITE_DIR}")

# Sire depends on >= boost 1.31
set (BOOST_FIND_REQUIRED TRUE)
include ( FindBoost )

message( STATUS "Boost paths ${BOOST_LIBRARY_DIRS} | ${BOOST_INCLUDE_DIRS}" )

# Need to ensure that we are linking to boost_python shared library!
if (UNIX)
  if ( BOOST_PYTHON_LIBRARY MATCHES "(\\.a$)" OR
       BOOST_PYTHON_LIBRARY MATCHES "(\\.A$)" )

    message( FATAL_ERROR "Need to link to shared boost::python library (should be "
                         "boost_python.so or boost_python.dylib, "
                         "not ${BOOST_PYTHON_LIBRARY})" )

  endif()
endif()

message( STATUS "Boost python library = ${BOOST_PYTHON_LIBRARY}" )
message( STATUS "Boost unit test library = ${BOOST_UNIT_TEST_FRAMEWORK_LIBRARY}" )

# Sire depends on >= GSL 1.6
include ( FindGSL )
find_package(GSL REQUIRED)

message( STATUS "GSL paths ${GSL_LINK_DIRECTORIES} ${GSL_LIBRARIES} | ${GSL_INCLUDE_DIR}" )

# Add defaults for all parts of the program
add_definitions( -I${QT_INCLUDE_DIR} 
                 -I${QT_QTCORE_INCLUDE_DIR} 
                 -I${BOOST_INCLUDE_DIRS}
                 -I${GSL_INCLUDE_DIR}
               )

add_definitions( ${QT_DEFINITIONS} )

# Set compile flags for debug and release

set( SIRE_PLATFORM_OPTIONS "" )

#if(APPLE)
#   set( SIRE_PLATFORM_OPTIONS 
#       "-arch i386 -Wno-long-double -no-cpp-precomp -mno-fused-madd -fno-common" )
#endif()

set( SIRE_C_FLAGS_RELEASE "-Wall -O2 -fomit-frame-pointer ${SIRE_PLATFORM_OPTIONS}" )
set( SIRE_CXX_FLAGS_RELEASE ${SIRE_C_FLAGS_RELEASE} )

set( SIRE_C_FLAGS_DEBUG "-Wall -g -O2 ${SIRE_PLATFORM_OPTIONS}" )
set( SIRE_CXX_FLAGS_DEBUG ${SIRE_C_FLAGS_DEBUG} )

# Also set flags for size-optimised compile - this is used for
# all of the python wrappers, which can grow to enormous sizes 
# otherwise! (and aren't performance critical)
set( SIRE_C_FLAGS_SMALL "-Wall -Os ${SIRE_PLATFORM_OPTIONS}" )
set( SIRE_CXX_FLAGS_SMALL ${SIRE_C_FLAGS_SMALL} )

# Add machine definition (supplied by user?)
ADD_DEFINITIONS( ${SIRE_TARGET_OPTIONS} )

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
          message( "Turning on openMP parallelisation!")
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

# Recurse into the module subdirectories 
add_subdirectory (Qt)
add_subdirectory (Error)
add_subdirectory (Base)
add_subdirectory (CAS)
add_subdirectory (ID)
add_subdirectory (Maths)
add_subdirectory (Vol)

install( FILES __init__.py
         DESTINATION ${SIRE_PYTHON}/Sire 
       )

