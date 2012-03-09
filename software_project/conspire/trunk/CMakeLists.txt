########################################
#
# CMake file for library:Conspire
#
########################################

# require cmake >= 2.8.0
cmake_minimum_required(VERSION 2.8.0 FATAL_ERROR)

# use loose loop syntax in CMakeLists files
set( CMAKE_ALLOW_LOOSE_LOOP_CONSTRUCTS TRUE )

# Absolutely can't run cmake in the source directory!
if ( CMAKE_BINARY_DIR STREQUAL CMAKE_SOURCE_DIR )
  message( FATAL_ERROR "You must run CMake in a different directory to the source!" )
endif ( CMAKE_BINARY_DIR STREQUAL CMAKE_SOURCE_DIR )

# name the project
project (CONSPIRE CXX C)

# project version
set (CONSPIRE_VERSION_MAJOR "1")
set (CONSPIRE_VERSION_MINOR "0")
set (CONSPIRE_VERSION_PATCH "1")

set (CONSPIRE_VERSION "${CONSPIRE_VERSION_MAJOR}.${CONSPIRE_VERSION_MINOR}.${CONSPIRE_VERSION_PATCH}")

math (EXPR CONSPIRE_VERSION_NUMBER
      "${CONSPIRE_VERSION_MAJOR}*100000 + ${CONSPIRE_VERSION_MINOR}*100 + ${CONSPIRE_VERSION_PATCH}")

set (CONSPIRE_VERSION_STRING "${CONSPIRE_VERSION_MAJOR}_${CONSPIRE_VERSION_MINOR}_${CONSPIRE_VERSION_PATCH}")

# Build shared libraries by default
set (BUILD_SHARED_LIBS ON)

# Make sure that the Conspire 'Conspire' include directory is searched first
# when looking for header files
include_directories( BEFORE SYSTEM ${CMAKE_SOURCE_DIR}/SirenCore )
include_directories( BEFORE SYSTEM ${CMAKE_SOURCE_DIR}/Conspire/third_party )
include_directories( BEFORE SYSTEM ${CMAKE_SOURCE_DIR} )

# Siren depends on >= Qt 4.4
find_package( Qt4 4.4.0 COMPONENTS QtCore QtXml )

if ( ${QT_QTCORE_FOUND} )
   include (${QT_USE_FILE})
   message( STATUS "Using Qt version ${QT_VERSION_MAJOR}.${QT_VERSION_MINOR}.${QT_VERSION_PATCH} "
                   "(${QT_LIBRARIES})" )
else()
   message( FATAL_ERROR "Qt >= 4.4 is required by Sire" )
endif()

add_subdirectory (SirenCore)

include_directories(${CMAKE_SOURCE_DIR}
                    ${CMAKE_SOURCE_DIR}/SireCore/Siren
                    ${QT_QTCORE_INCLUDE_DIR}
                    ${QT_QTXML_INCLUDE_DIR})

set ( CONSPIRE_THIRD_PARTY_SOURCES

    )

set ( CONSPIRE_PUBLIC_HEADERS

      Conspire/conspire.h
      Conspire/conspire.hpp

      Conspire/dimensions.h
      Conspire/dimensions.hpp
      Conspire/exceptions.h
      Conspire/indexvalue.h
      Conspire/option.h
      Conspire/qtsupport.h
      Conspire/tempvalue.h
      Conspire/value.h
      Conspire/values.h

    )

set ( CONSPIRE_PRIVATE_HEADERS

    )

set ( CONSPIRE_SOURCES

      cpp/conspire.cpp
      cpp/dimensions.cpp
      cpp/exceptions.cpp
      cpp/indexvalue.cpp
      cpp/option.cpp
      cpp/tempvalue.cpp
      cpp/value.cpp
      cpp/values.cpp

      ${CONSPIRE_PUBLIC_HEADERS}

      ${CONSPIRE_THIRD_PARTY_SOURCES}

      ${CONSPIRE_PRIVATE_HEADERS}

    )


add_library ( Conspire ${CONSPIRE_SOURCES} )
add_executable ( conspire_test SirenCore/cpp/sirentest.cpp )

set_target_properties (Conspire PROPERTIES
                       VERSION ${CONSPIRE_VERSION_MAJOR}
                       SOVERSION ${CONSPIRE_VERSION_MAJOR}
                      )

target_link_libraries ( Conspire
                        Siren
                        ${QT_QTCORE_LIBRARY}
                        ${QT_QTXML_LIBRARY} )

target_link_libraries ( conspire_test Conspire Siren )

add_subdirectory ( GUI )

# installation
install( TARGETS Conspire EXPORT Conspire
         RUNTIME DESTINATION bin
         LIBRARY DESTINATION lib
         ARCHIVE DESTINATION lib/static
       )

install( FILES ${CONSPIRE_PUBLIC_HEADERS}
         DESTINATION include/Conspire )

install( DIRECTORY ${SOURCE_DIR}/Conspire/third_party
         DESTINATION include/Conspire
         PATTERN .svn EXCLUDE
       )

install( FILES ${CONSPIRE_PRIVATE_HEADERS}
         DESTINATION include/Conspire/detail )
