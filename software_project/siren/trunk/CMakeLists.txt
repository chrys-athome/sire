########################################
#
# CMake file for library:Siren
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
project (SIREN CXX C)

# project version
set (SIREN_VERSION_MAJOR "0")
set (SIREN_VERSION_MINOR "0")
set (SIREN_VERSION_PATCH "1")

set (SIREN_VERSION "${SIREN_VERSION_MAJOR}.${SIREN_VERSION_MINOR}.${SIREN_VERSION_PATCH}")

math (EXPR SIREN_VERSION_NUMBER
      "${SIREN_VERSION_MAJOR}*100000 + ${SIREN_VERSION_MINOR}*100 + ${SIREN_VERSION_PATCH}")

set (SIREN_VERSION_STRING "${SIREN_VERSION_MAJOR}_${SIREN_VERSION_MINOR}_${SIREN_VERSION_PATCH}")

# Build shared libraries by default
set (BUILD_SHARED_LIBS ON)

# Siren depends on >= Qt 4.4
find_package( Qt4 4.4.0 COMPONENTS QtCore QtXml )

if ( ${QT_QTCORE_FOUND} )
   include (${QT_USE_FILE})
   message( STATUS "Using Qt version ${QT_VERSION_MAJOR}.${QT_VERSION_MINOR}.${QT_VERSION_PATCH} "
                   "(${QT_LIBRARIES})" )
else()
   message( FATAL_ERROR "Qt >= 4.4 is required by Sire" )
endif()

include_directories(${CMAKE_SOURCE_DIR} 
                    ${QT_QTCORE_INCLUDE_DIR})

set ( SIREN_PUBLIC_HEADERS

      Siren/bytearray.h
      Siren/class.h
      Siren/exception.h
      Siren/exceptions.h
      Siren/macros.h
      Siren/none.h
      Siren/number.h
      Siren/object.h
      Siren/obj.h
      Siren/obj.hpp
      Siren/schemas.h
      Siren/siren.h
      Siren/siren.hpp
      Siren/stream.h
      Siren/string.h
    )

set ( SIREN_PRIVATE_HEADERS

      Siren/detail/boostsupport.h
      Siren/detail/metatype.h
      Siren/detail/metatype.hpp
      Siren/detail/qt4support.h
    )

set ( SIREN_SOURCES

      ${SIREN_PUBLIC_HEADERS}

      bytearray.cpp
      class.cpp
      exception.cpp
      exceptions.cpp
      none.cpp
      number.cpp
      object.cpp
      obj.cpp
      schemas.cpp
      stream.cpp
      string.cpp

      ${SIREN_PRIVATE_HEADERS}

      detail/metatype.cpp
    )


add_library ( Siren ${SIREN_SOURCES} )

set_target_properties (Siren PROPERTIES
                       VERSION ${SIREN_VERSION_MAJOR}
                       SOVERSION ${SIREN_VERSION_MINOR}
                      )

target_link_libraries ( Siren 
                        ${QT_QTXML_LIBRARY} 
                        ${QT_QTCORE_LIBRARY} )

# installation
install( TARGETS Siren EXPORT Siren
         RUNTIME DESTINATION bin
         LIBRARY DESTINATION lib
         ARCHIVE DESTINATION lib/static
       )

install( FILES ${SIREN_PUBLIC_HEADERS}
         DESTINATION include/Siren )

install( FILES ${SIREN_PRIVATE_HEADERS}
         DESTINATION include/Siren/detail )

