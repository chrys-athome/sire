########################################
#
# CMake file for library:Squire
#
########################################

# Third Party dependencies of this module
#include_directories(${QT_QTNETWORK_INCLUDE_DIR})

# Other Sire libraries
include_directories(${CMAKE_SOURCE_DIR}/src/libs)

set ( SQUIRE_HEADERS

      qmff.h
      qmmmff.h
      qmmmpotential.h
      qmpotential.h
      qmprogram.h

      molpro.h
    )

set ( SQUIRE_SOURCES

      register_squire.cpp

      qmff.cpp
      qmmmff.cpp
      qmmmpotential.cpp
      qmpotential.cpp
      qmprogram.cpp

      molpro.cpp

      ${SQUIRE_HEADERS}
    )

#add the additional include files needed by Squire
include_directories( ${SQUIRE_INCLUDE_DIRS} )
  
add_library (Squire ${SQUIRE_SOURCES})

set_target_properties (Squire PROPERTIES
                         VERSION ${SIRE_VERSION}
                         SOVERSION ${SIRE_VERSION_MAJOR}
                         LINK_FLAGS ${SIRE_LINK_FLAGS}
                      )

# Link to other Sire libraries
target_link_libraries (Squire
                       SireMM
                       SireFF
                       SireMol
                       SireCAS
                       SireBase
                       SireStream
                       SireError
                       ${SQUIRE_EXTRA_LIBRARIES}
                      )

# installation
install( TARGETS Squire
         RUNTIME DESTINATION ${SIRE_BIN}
         LIBRARY DESTINATION ${SIRE_LIBS}
         ARCHIVE DESTINATION ${SIRE_ARCHIVES}
       )

install( FILES ${SQUIRE_HEADERS}
         DESTINATION ${SIRE_INCLUDES}/Squire )
