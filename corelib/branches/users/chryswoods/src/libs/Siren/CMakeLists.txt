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
  
      datastream.h
      errors.h
      exception.h
      getbacktrace.h
      logger.h
      md5sum.h
      magic_error.h
      version_error.h
      streamdata.hpp
      tester.h
      xmlstream.h

      sirenglobal.h
      tostring.h

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
      stream.h

    )

set ( SIREN_PRIVATE_HEADERS

      detail/sharestrings.h
      detail/globalsharedpointer.hpp
      detail/sharedpolypointer.hpp
      detail/sharedpolypointer_cast.hpp
      detail/streampolypointer.hpp
    )

set ( SIREN_SOURCES

      ${SIREN_PUBLIC_HEADERS}

      datastream.cpp
      errors.cpp
      exception.cpp
      getbacktrace.cpp
      logger.cpp
      md5sum.cpp
      magic_error.cpp
      version_error.cpp
      streamdata.cpp
      tester.cpp
      xmlstream.cpp

      hanptr.cpp
      hanref.cpp
      objptr.cpp
      objref.cpp

      class.cpp
      handle.cpp
      interface.cpp
      object.cpp
      primitive.cpp
      stream.cpp

      ${SIREN_PRIVATE_HEADERS}

      detail/globalsharedpointer.cpp
      detail/sharedpolypointer.cpp
      detail/sharestrings.cpp
    
      third_party/md5.h
      third_party/md5.c
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
