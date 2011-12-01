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
set (SIREN_VERSION_MAJOR "1")
set (SIREN_VERSION_MINOR "0")
set (SIREN_VERSION_PATCH "1")

set (SIREN_VERSION "${SIREN_VERSION_MAJOR}.${SIREN_VERSION_MINOR}.${SIREN_VERSION_PATCH}")

math (EXPR SIREN_VERSION_NUMBER
      "${SIREN_VERSION_MAJOR}*100000 + ${SIREN_VERSION_MINOR}*100 + ${SIREN_VERSION_PATCH}")

set (SIREN_VERSION_STRING "${SIREN_VERSION_MAJOR}_${SIREN_VERSION_MINOR}_${SIREN_VERSION_PATCH}")

# Build shared libraries by default
set (BUILD_SHARED_LIBS ON)

# Make sure that the Siren 'Siren' include directory is searched first
# when looking for header files
include_directories( BEFORE SYSTEM ${CMAKE_SOURCE_DIR}/Siren/third_party )
include_directories( BEFORE SYSTEM ${CMAKE_SOURCE_DIR} )

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
      Siren/block.h
      Siren/block.hpp
      Siren/bytearray.h
      Siren/class.h
      Siren/date.h
      Siren/datetime.h
      Siren/editor.h
      Siren/editor.hpp
      Siren/exception.h
      Siren/exceptions.h
      Siren/forages.h
      Siren/interfaces.h
      Siren/macros.h
      Siren/mutable.h
      Siren/mutable.hpp
      Siren/mutex.h
      Siren/none.h
      Siren/number.h
      Siren/object.h
      Siren/object.hpp
      Siren/obj.h
      Siren/obj.hpp
      Siren/percentage.h
      Siren/promise.h
      Siren/readwritelock.h
      Siren/semaphore.h
      Siren/schemas.h
      Siren/siren.h
      Siren/siren.hpp
      Siren/static.h
      Siren/stream.h
      Siren/string.h
      Siren/stringlist.h
      Siren/systemsemaphore.h
      Siren/testreport.h
      Siren/thread.h
      Siren/threadstorage.h
      Siren/time.h
      Siren/timer.h
      Siren/tostring.h
      Siren/tostring.hpp
      Siren/waitcondition.h
      Siren/worklog.h
      Siren/workmonitor.h
      Siren/workpacket.h
      Siren/workqueue.h
      Siren/workqueueitem.h
      Siren/workscheduler.h
      Siren/workspace.h
      Siren/workspace.hpp
    )

set ( SIREN_PRIVATE_HEADERS

      Siren/detail/blockdata.h
      Siren/detail/promisedata.h
      Siren/detail/workqueuedata.h
      Siren/detail/workqueueitemdata.h

      Siren/detail/boostsupport.h
      Siren/detail/metatype.h
      Siren/detail/metatype.hpp
      Siren/detail/qt4support.h
    )

set ( SIREN_SOURCES

      ${SIREN_PUBLIC_HEADERS}

      ${SIREN_THIRD_PARTY_SOURCES}

      assert.cpp
      block.cpp
      bytearray.cpp
      class.cpp
      date.cpp
      datetime.cpp
      editor.cpp
      exception.cpp
      exceptions.cpp
      forages.cpp
      interfaces.cpp
      mutable.cpp
      mutex.cpp
      none.cpp
      number.cpp
      object.cpp
      obj.cpp
      percentage.cpp
      promise.cpp
      readwritelock.cpp
      semaphore.cpp
      schemas.cpp
      siren.cpp
      static.cpp
      stream.cpp
      string.cpp
      stringlist.cpp
      systemsemaphore.cpp
      testreport.cpp
      thread.cpp
      threadstorage.cpp
      time.cpp
      timer.cpp
      waitcondition.cpp
      worklog.cpp
      workmonitor.cpp
      workpacket.cpp
      workqueue.cpp
      workqueueitem.cpp
      workscheduler.cpp
      workspace.cpp

      ${SIREN_PRIVATE_HEADERS}

      detail/metatype.cpp
    )


add_library ( Siren ${SIREN_SOURCES} )
add_executable ( sirentest sirentest.cpp )

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
