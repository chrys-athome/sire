########################################
#
# CMake file for application:sire
#
########################################

# Third Party dependencies of this module
include_directories(${BOOST_INCLUDE_DIR})

# Other Sire libraries
include_directories(${CMAKE_SOURCE_DIR}/src/libs)

if ( SIRE_USE_MPI )
  message( STATUS " ** Adding MPI support to sire ** " )
  message( STATUS "MPI headers: ${MPI_INCLUDE_PATH}" )
  message( STATUS "MPI libraries: ${MPI_LIBRARY} ${MPI_EXTRA_LIBRARY}" )

  include_directories( BEFORE SYSTEM ${MPI_INCLUDE_PATH} )

  add_definitions( -DSIRE_USE_MPI )
endif()

# Define the sources in sire
set ( SIRE_HEADERS
      restartpacket.h
    )

set ( SIRE_SOURCES

      restartpacket.cpp

      main.cpp

      ${SIRE_HEADERS}
    )

# generate the application
add_executable ( sire ${SIRE_SOURCES} )

set_target_properties (sire PROPERTIES
                       VERSION ${SIRE_VERSION}
                       SOVERSION ${SIRE_VERSION_MAJOR}
                      )

# Link to other Sire libraries
target_link_libraries (sire
                       SireIO
                       Squire
                       SireMove
                       SireMol
                       SireCluster
                       SireBase
                       SireStream
                      )

# installation
install( TARGETS sire
         RUNTIME DESTINATION ${SIRE_BIN}
         LIBRARY DESTINATION ${SIRE_LIBS}
         ARCHIVE DESTINATION ${SIRE_ARCHIVES}
       )
