################################
#
#  CMake Project file for Sire
#    Python 2 wrappers
#
#   (C) Christopher Woods
#
################################

# Uncomment for more verbose compiling/linking
# (or run 'make VERBOSE=1')
# set (CMAKE_VERBOSE_MAKEFILE ON)

# require cmake >= 2.6.0
cmake_minimum_required(VERSION 2.6.0 FATAL_ERROR)

#use loose loop syntax in CMakeLists files
set( CMAKE_ALLOW_LOOSE_LOOP_CONSTRUCTS TRUE )

macro( FATAL_MESSAGE )
  set( m "" )
  foreach ( ARG ${ARGV} )
    set( m "${m} ${ARG}" )
  endforeach()

  message( FATAL_ERROR "\n *** ERROR *** \n ${m} \n *************\n" )
endmacro( FATAL_MESSAGE )

macro( WARNING_MESSAGE )
  set( m "" )
  foreach ( ARG ${ARGV} )
    set( m "${m} ${ARG}" )
  endforeach()

  message( STATUS "\n *** WARNING *** \n ${m} \n ***************\n" )
endmacro( WARNING_MESSAGE )

# Absolutely can't run cmake in the source directory!
if ( CMAKE_BINARY_DIR STREQUAL CMAKE_SOURCE_DIR )
  fatal_message( "You must run CMake in a different directory to the source!" ) 
endif ( CMAKE_BINARY_DIR STREQUAL CMAKE_SOURCE_DIR )

# name the project
project (SirePython2)

# Path to the project's extra cmake files
set (CMAKE_MODULE_PATH ${CMAKE_SOURCE_DIR}/build/cmake)

# Find the Sire libraries
find_package (Sire 0.0.1 REQUIRED)

if (NOT SIRE_FOUND)
   fatal_message("The required version of Sire was not found.")
endif()

message( STATUS "Found Sire version ${SIRE_VERSION}" )
message( STATUS "Sire root = ${SIRE_INSTALL_PREFIX}" )
message( STATUS "Header files in ${SIRE_INCLUDE_DIR}" )
message( STATUS "Libraries in ${SIRE_INSTALL_PREFIX}/${SIRE_LIBS}" )

set ( SIRE_STRIP_COMMAND "${CMAKE_STRIP} ${SIRE_STRIP_OPTIONS}" )

configure_file(
  "${CMAKE_CURRENT_SOURCE_DIR}/build/cmake/cmake_strip.in"
  "${CMAKE_CURRENT_BINARY_DIR}/cmake_strip"
  IMMEDIATE @ONLY)

set ( CMAKE_STRIP "${CMAKE_CURRENT_BINARY_DIR}/cmake_strip" )

# Make sure that we are using the same compiler as that used to compile Sire
option (SIRE_CHECK_COMPILER "Turn off to skip the compiler check" ON)

macro( compare_strings RESULT val0 val1)
  if (${val0} STREQUAL ${val1})
    set( ${RESULT} TRUE )
  else()
    set( ${RESULT} FALSE )
  endif()
endmacro()

macro( compare_values RESULT val0 val1)
  if (${val0} EQUAL ${val1})
    set( ${RESULT} TRUE )
  else()
    set( ${RESULT} FALSE )
  endif()
endmacro()

if ( SIRE_CHECK_COMPILER )

  compare_strings( SAME_C_COMPILER ${SIRE_C_COMPILER} ${CMAKE_C_COMPILER} )
  compare_strings( SAME_CXX_COMPILER ${SIRE_CXX_COMPILER} ${CMAKE_CXX_COMPILER} )

  if ( NOT SAME_C_COMPILER OR NOT SAME_CXX_COMPILER ) 

    if ( NOT SAME_C_COMPILER )
      message( STATUS "Different C compilers! ${SIRE_C_COMPILER} vs. ${CMAKE_C_COMPILER}" )
    endif()

    if ( NOT SAME_CXX_COMPILER )
      message( STATUS "Different C++ compilers! ${SIRE_CXX_COMPILER} vs. ${CMAKE_CXX_COMPILER}" )
    endif()

    message( STATUS "Using different compilers is really not recommended. If you want to "
                    "skip this check, then set SIRE_CHECK_COMPILER to \"OFF\" using ccmake" )

    fatal_message( "You cannot compile the python wrappers with different compilers"
                   "(${CMAKE_C_COMPILER}, ${CMAKE_CXX_COMPILER}) than those used to"
                   "compile the Sire libraries"
                   "(${SIRE_C_COMPILER}, ${SIRE_CXX_COMPILER})."
                   "You need to remove CMakeCache.txt and rerun cmake using the command;\n\n"
                   "CC=${SIRE_C_COMPILER} CXX=${SIRE_CXX_COMPILER} cmake ${CMAKE_SOURCE_DIR}\n" )
  endif()
endif()

# Now add the paths to the header files of the dependencies of Sire
include_directories (BEFORE ${SIRE_INCLUDE_DIR})

# Sire depends on >= Qt 4.2
find_package( Qt4 4.2.0 COMPONENTS QtCore QtXml QtSQL QtGui QtOpenGL QtWebKit )

if ( ${QT_QTCORE_FOUND} AND ${QT_QTSQL_FOUND} )
   include (${QT_USE_FILE})
   message( STATUS "Using Qt version ${QT_VERSION_MAJOR}.${QT_VERSION_MINOR}.${QT_VERSION_PATCH} "
                   "(${QT_LIBRARIES})" )

   compare_values( SAME_QT_MAJOR ${QT_VERSION_MAJOR} ${SIRE_QT_VERSION_MAJOR} )
   compare_values( SAME_QT_MINOR ${QT_VERSION_MINOR} ${SIRE_QT_VERSION_MINOR} )
   compare_values( SAME_QT_PATCH ${QT_VERSION_PATCH} ${SIRE_QT_VERSION_PATCH} )

   if ( NOT SAME_QT_MAJOR )
      fatal_message( "Compiling the python wrappers against a different major"
                     "version of Qt (${QT_VERSION_MAJOR}) than Sire (${SIRE_QT_VERSION_MAJOR})"
                     "is not supported." )
   
   elseif ( NOT SAME_QT_MINOR AND SAME_QT_PATCH )

      warning_message( "Compiling the python wrappers with a version of Qt"
                       "(${QT_VERSION_MAJOR}.${QT_VERSION_MINOR}.${QT_VERSION_PATCH})"
                       "which is different to the Sire Qt version" 
                       "(${SIRE_QT_VERSION_MAJOR}.${SIRE_QT_VERSION_MINOR}.${SIRE_QT_VERSION_PATCH})"
                       "is not recommended!!!" )
   endif()

else()                                          
   fatal_message( "Qt >= 4.2 is required by Sire" )
endif()

# Build shared libraries by default
option (BUILD_SHARED_LIBS "Build shared python modules" ON)

# Sire depends on >= Python 2.2
set (PYTHON_MIN_VERSION "2.2")
find_package( Python )

if (NOT PYTHON_LIBS_FOUND)
    if (PYTHON_INSTALLED_VERSION_TOO_OLD)
        fatal_message( "Installed version of Python (${PYTHON_VERSION}) is too old."
                       "Sire requires at least Python ${PYTHON_MIN_VERSION}" )
    else()
        fatal_message( "The python libraries were not found. These are necessary to build" 
                       "the python bindings." )
    endif()
endif()

if ( NOT ${PYTHON_MAJOR_VERSION} EQUAL 2 )
   fatal_message( "The python 2 wrappers can only be built for a python 2 interpreter."  
                  "The found python intepreter (${PYTHON_EXECUTABLE}) is version"
                  "${PYTHON_MAJOR_VERSION}.")
endif() 

message( STATUS "Python paths ${PYTHON_LIBRARIES} | ${PYTHON_INCLUDE_DIR} | ${PYTHON_SITE_DIR}" )
include_directories( ${PYTHON_INCLUDE_DIR} )

# set the location to install the python modules
set (SIRE_PYTHON "${PYTHON_SITE_DIR}")

# Sire depends on >= boost 1.31 - all libraries must be dynamically linked
# (else we could end up with multiple boost::python libraries linked, which
#  causes problems for RTTI and exceptions)
set (BOOST_ALL_DYN_LINK "YES")
FIND_PACKAGE ( Boost 1.31 COMPONENTS python )

if (Boost_FOUND)
  message(STATUS "Boost paths ${Boost_LIBRARY_DIRS} | ${Boost_INCLUDE_DIR}" )

  compare_strings( SAME_BOOST_INCLUDE ${SIRE_Boost_INCLUDE_DIR} ${Boost_INCLUDE_DIR} )

  if ( NOT SAME_BOOST_INCLUDE )
    warning_message( "It is not a good idea to compile the python wrappers against"
                     "boost libraries with headers in ${Boost_INCLUDE_DIR} when"
                     "Sire was built using different boost headers in ${SIRE_Boost_INCLUDE_DIR}." )
  endif()

  set ( BOOST_INCLUDE_DIRS "${Boost_INCLUDE_DIR}" )
  
  include_directories( ${Boost_INCLUDE_DIR} )

  if (Boost_PYTHON_FOUND)
    message(STATUS "Using boost::python library ${Boost_PYTHON_LIBRARY}")
    set ( BOOST_PYTHON_LIBRARY "${Boost_PYTHON_LIBRARY}" )
  else()
    fatal_message( "Cannot find the boost::python library, even though"
                   "boost has been found.")
  endif()
else()
  fatal_message("Cannot find the boost libraries.")
endif()  

# We will use the small compiler flags (as the wrappers are too big and don't need optimising)
message( STATUS "Compiling the python 2 wrappers using a SMALL build." )
set( CMAKE_C_FLAGS ${SIRE_C_FLAGS_SMALL} )
set( CMAKE_CXX_FLAGS ${SIRE_CXX_FLAGS_SMALL} )

set( CMAKE_SHARED_LINKER_FLAGS "${CMAKE_SHARED_LINKER_FLAGS} ${SIRE_SHARE_LINK_FLAGS}" )
set( CMAKE_STATIC_LINKER_FLAGS "${CMAKE_STATIC_LINKER_FLAGS} ${SIRE_STATIC_LINK_FLAGS}" )
set( CMAKE_EXE_LINKER_FLAGS    "${CMAKE_EXE_LINKER_FLAGS} ${SIRE_EXE_LINK_FLAGS}" )

message( STATUS  "CMAKE_SYSTEM_NAME      == ${CMAKE_SYSTEM_NAME}" )
message( STATUS  "CMAKE_C_COMPILER       == ${CMAKE_C_COMPILER}" )
message( STATUS  "CMAKE_CXX_COMPILER     == ${CMAKE_CXX_COMPILER}" )
message( STATUS  "C compiler flags       == ${CMAKE_C_FLAGS}" )
message( STATUS  "C++ compiler flags     == ${CMAKE_CXX_FLAGS}" )
message( STATUS  "Shared library flags   == ${CMAKE_SHARED_LINKER_FLAGS}" )
message( STATUS  "Static library flags   == ${CMAKE_STATIC_LINKER_FLAGS}" )
message( STATUS  "Executable link flags  == ${CMAKE_EXE_LINKER_FLAGS}" )

# Recurse into the module subdirectories 
add_subdirectory (Qt)
add_subdirectory (Base)
add_subdirectory (CAS)
add_subdirectory (Cluster)
#add_subdirectory (DB)
#add_subdirectory (FF)
add_subdirectory (ID)
#add_subdirectory (IO)
add_subdirectory (Maths)
#add_subdirectory (MM)
#add_subdirectory (Mol)
#add_subdirectory (Move)
add_subdirectory (Siren)
#add_subdirectory (Spier)
#add_subdirectory (Squire)
#add_subdirectory (System)
add_subdirectory (Units)
add_subdirectory (Vol)

#also compile the sire_python executable
#add_subdirectory (sire_python)

#and the sire_ipython executable
#add_subdirectory (sire_ipython)

install( FILES __init__.py
         DESTINATION ${SIRE_PYTHON}/Sire 
       )

# These commands are used to create an 'uninstall' target
# (this is copied from the vtk.org wiki)
configure_file(
  "${CMAKE_CURRENT_SOURCE_DIR}/build/cmake/cmake_uninstall.cmake.in"
  "${CMAKE_CURRENT_BINARY_DIR}/cmake_uninstall.cmake"
  IMMEDIATE @ONLY)

add_custom_target(uninstall
  "${CMAKE_COMMAND}" -P "${CMAKE_CURRENT_BINARY_DIR}/cmake_uninstall.cmake")

