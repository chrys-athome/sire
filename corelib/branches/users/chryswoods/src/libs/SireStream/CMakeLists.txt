########################################
#
# CMake file for library:SireStream
#
########################################

include_directories(${CMAKE_SOURCE_DIR}/src/libs)

set ( SIRESTREAM_HEADERS
      datastream.h
      errors.h
      magic_error.h
      md5sum.h
      shareddatastream.h
      streamdata.hpp
      streampolypointer.hpp
      version_error.h
      versionid.h
    )

set ( SIRESTREAM_SOURCES

      datastream.cpp
      errors.cpp
      getmagic.cpp
      magic_error.cpp
      md5sum.cpp
      shareddatastream.cpp
      streamdata.cpp
      version_error.cpp

      ThirdParty/md5.c
    
      ${SIRESTREAM_HEADERS}
    )


add_library ( SireStream ${SIRESTREAM_SOURCES} )

target_link_libraries (SireStream
                       SireError
                      )

# installation
install( TARGETS SireStream  EXPORT SireLibraries
         RUNTIME DESTINATION ${SIRE_BIN}
         LIBRARY DESTINATION ${SIRE_LIBS}
         ARCHIVE DESTINATION ${SIRE_ARCHIVES}
       )

install( FILES ${SIRESTREAM_HEADERS}
         DESTINATION ${SIRE_INCLUDES}/SireStream )

