########################################
#
# CMake file for library:Siren
#
########################################

if (NOT QT_QTXML_FOUND)
  message( FATAL "Siren requires that the QtXML module is available!" )
endif()

include_directories(${CMAKE_SOURCE_DIR}/src/libs/Siren 
                    ${CMAKE_SOURCE_DIR}/src/libs
                    ${QT_QTXML_INCLUDE_DIR})

set ( SIREN_PUBLIC_HEADERS

      sirenglobal.h
      tostring.h
      getbacktrace.h
      md5sum.h
  
      exception.h
      errors.h

      forages.h
      mutex.h
      semaphore.h
      thread.h
      waitcondition.h

      logger.h
      tester.h

      stream.h
      streamqt.h
      datastream.h
      xmlstream.h

      hanptr.hpp
      hanref.h
      objptr.hpp
      objref.h

      mutable.h

      class.h 
      handle.h
      interface.h     
      object.h
      primitive.h

    )

set ( SIREN_PRIVATE_HEADERS

      detail/sharestrings.h
      detail/globalsharedpointer.hpp
      detail/sharedpolypointer.hpp
      detail/sharedpolypointer_cast.hpp
    )

set ( SIREN_SOURCES

      ${SIREN_PUBLIC_HEADERS}

      getbacktrace.cpp
      md5sum.cpp
 
      exception.cpp
      errors.cpp

      forages.cpp
      mutex.cpp
      semaphore.cpp
      thread.cpp
      waitcondition.cpp

      logger.cpp
      tester.cpp

      stream.cpp
      streamqt.cpp
      datastream.cpp
      xmlstream.cpp

      hanptr.cpp
      hanref.cpp
      objptr.cpp
      objref.cpp

      mutable.cpp

      class.cpp
      handle.cpp
      interface.cpp
      object.cpp
      primitive.cpp

      ${SIREN_PRIVATE_HEADERS}

      detail/globalsharedpointer.cpp
      detail/sharedpolypointer.cpp
      detail/sharestrings.cpp
    
      third_party/md5.h
      third_party/md5.c
     
      third_party/encode.h
      third_party/decode.h
      third_party/cencode.h
      third_party/cdecode.h
      third_party/cencode.c
      third_party/cdecode.c
    )


add_library ( Siren ${SIREN_SOURCES} )

set_target_properties (Siren PROPERTIES
                       VERSION ${SIRE_VERSION}
                       SOVERSION ${SIRE_VERSION_MAJOR}
                      )

target_link_libraries ( Siren 
                        ${QT_QTXML_LIBRARY} 
                        ${QT_QTCORE_LIBRARY} )

# installation
install( TARGETS Siren  EXPORT SireLibraries
         RUNTIME DESTINATION ${SIRE_BIN}
         LIBRARY DESTINATION ${SIRE_LIBS}
         ARCHIVE DESTINATION ${SIRE_ARCHIVES}
       )

install( FILES ${SIREN_PUBLIC_HEADERS}
         DESTINATION ${SIRE_INCLUDES}/Siren )

install( FILES ${SIREN_PRIVATE_HEADERS}
         DESTINATION ${SIRE_INCLUDES}/Siren/detail )
