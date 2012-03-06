########################################
#
# CMake file for library:SirenCore
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
project (SIRENCORE CXX C)

# project version
set (SIRENCORE_VERSION_MAJOR "1")
set (SIRENCORE_VERSION_MINOR "0")
set (SIRENCORE_VERSION_PATCH "1")

set (SIRENCORE_VERSION "${SIRENCORE_VERSION_MAJOR}.${SIRENCORE_VERSION_MINOR}.${SIRENCORE_VERSION_PATCH}")

math (EXPR SIRENCORE_VERSION_NUMBER
      "${SIRENCORE_VERSION_MAJOR}*100000 + ${SIRENCORE_VERSION_MINOR}*100 + ${SIRENCORE_VERSION_PATCH}")

set (SIRENCORE_VERSION_STRING "${SIRENCORE_VERSION_MAJOR}_${SIRENCORE_VERSION_MINOR}_${SIRENCORE_VERSION_PATCH}")

# Build shared libraries by default
set (BUILD_SHARED_LIBS ON)

# Make sure that the Siren 'Siren' include directory is searched first
# when looking for header files
include_directories( BEFORE SYSTEM ${CMAKE_SOURCE_DIR}/SirenCore/Siren/third_party )
include_directories( BEFORE SYSTEM ${CMAKE_SOURCE_DIR}/SirenCore )

# Siren depends on >= Qt 4.4
find_package( Qt4 4.4.0 COMPONENTS QtCore )

if ( ${QT_QTCORE_FOUND} )
   include (${QT_USE_FILE})
   message( STATUS "Using Qt version ${QT_VERSION_MAJOR}.${QT_VERSION_MINOR}.${QT_VERSION_PATCH} "
                   "(${QT_LIBRARIES})" )
else()
   message( FATAL_ERROR "Qt >= 4.4 is required by Sire" )
endif()

include_directories(${CMAKE_SOURCE_DIR} 
                    ${QT_QTCORE_INCLUDE_DIR})

set ( SIREN_THIRD_PARTY_SOURCES

      third_party/fcmp.h
      third_party/fcmp.cpp
    )

set ( SIREN_PUBLIC_HEADERS

      Siren/assert.h
      Siren/boostsupport.h
      Siren/class.h
      Siren/editor.h
      Siren/editor.hpp
      Siren/exception.h
      Siren/exceptions.h
      Siren/interfaces.h
      Siren/macros.h
      Siren/mutable.h
      Siren/mutable.hpp
      Siren/none.h
      Siren/number.h
      Siren/object.h
      Siren/object.hpp
      Siren/obj.h
      Siren/obj.hpp
      Siren/qtsupport.h
      Siren/schemas.h
      Siren/siren.h
      Siren/siren.hpp
      Siren/static.h
      Siren/stream.h
      Siren/testreport.h
      Siren/tostring.h
      Siren/tostring.hpp
    )

set ( SIREN_PRIVATE_HEADERS

      Siren/detail/metatype.h
      Siren/detail/metatype.hpp
    )

set ( SIREN_SOURCES

      ${SIREN_PUBLIC_HEADERS}

      ${SIREN_THIRD_PARTY_SOURCES}

      cpp/assert.cpp
      cpp/class.cpp
      cpp/editor.cpp
      cpp/exception.cpp
      cpp/exceptions.cpp
      cpp/interfaces.cpp
      cpp/mutable.cpp
      cpp/none.cpp
      cpp/number.cpp
      cpp/object.cpp
      cpp/obj.cpp
      cpp/schemas.cpp
      cpp/siren.cpp
      cpp/static.cpp
      cpp/stream.cpp
      cpp/testreport.cpp

      ${SIREN_PRIVATE_HEADERS}

      cpp/metatype.cpp
    )


add_library ( Siren ${SIREN_SOURCES} )
add_executable ( sirentest cpp/sirentest.cpp )

set_target_properties (Siren PROPERTIES
                       VERSION ${SIREN_VERSION_MAJOR}
                       SOVERSION ${SIREN_VERSION_MAJOR}
                      )

target_link_libraries ( Siren  
                        ${QT_QTCORE_LIBRARY} )

target_link_libraries ( sirentest Siren )

# installation
install( TARGETS Siren EXPORT Siren
         RUNTIME DESTINATION bin
         LIBRARY DESTINATION lib
         ARCHIVE DESTINATION lib/static
       )

install( FILES ${SIREN_PUBLIC_HEADERS}
         DESTINATION include/Siren )

install( DIRECTORY ${SOURCE_DIR}/Siren/third_party
         DESTINATION include/Siren
         PATTERN .svn EXCLUDE
       )

install( FILES ${SIREN_PRIVATE_HEADERS}
         DESTINATION include/Siren/detail )
