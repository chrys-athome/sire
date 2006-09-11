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
include( FindPythonLibs )

message( STATUS "${PYTHON_LIBRARIES}" )

# Sire depends on >= boost 1.31
set (Boost_FIND_REQUIRED TRUE)
include ( ${CMAKE_MODULE_PATH}/FindBoost.cmake )
find_package(Boost REQUIRED)

# Sire depends on >= GSL 1.6
include ( ${CMAKE_MODULE_PATH}/FindGSL.cmake )
find_package(GSL REQUIRED)

# Recurse into the src/libs and src/apps subdirectories
add_subdirectory (src/libs)
add_subdirectory (src/apps)

