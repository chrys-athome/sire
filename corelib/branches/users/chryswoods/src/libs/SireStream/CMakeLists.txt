########################################
#
# CMake file for library:SireStream
#
########################################

include_directories(${CMAKE_SOURCE_DIR}/src/libs)

#also include the build directory so that we can get sire_version.h
include_directories(${CMAKE_BINARY_DIR})

set ( SIRESTREAM_HEADERS
      datastream.h
      magic_error.h
      shareddatastream.h
      streamdata.hpp
      version_error.h
      versionid.h
    )

set ( SIRESTREAM_SOURCES

      datastream.cpp
      getmagic.cpp
      magic_error.cpp
      shareddatastream.cpp
      streamdata.cpp
      version_error.cpp
    
      ${SIRESTREAM_HEADERS}
    )


add_library ( SireStream ${SIRESTREAM_SOURCES} )

target_link_libraries (SireStream
                       ThirdParty
                       SireError
                       ${QT_QTCORE_LIBRARY}
                      )

# installation
install( TARGETS SireStream
         RUNTIME DESTINATION ${SIRE_BIN}
         LIBRARY DESTINATION ${SIRE_LIBS}
         ARCHIVE DESTINATION ${SIRE_ARCHIVES}
       )

install( FILES ${SIRESTREAM_HEADERS}
         DESTINATION ${SIRE_INCLUDES}/SireStream )
